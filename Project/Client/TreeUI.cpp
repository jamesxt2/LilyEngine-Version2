#include "pch.h"
#include "TreeUI.h"

/****************************TreeNode**************************************/
TreeNode::TreeNode()
	: m_Owner(nullptr), m_Parent(nullptr), m_Data(0), m_bFrame(false), m_bSelected(false)
{
}

TreeNode::TreeNode(const std::string& name, DWORD_PTR data)
	: m_Owner(nullptr), m_Parent(nullptr), m_Name(name), m_Data(data), m_bFrame(false), m_bSelected(false)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vector(m_vecChildNode);
}

void TreeNode::Render_Tick()
{
	std::string name = m_Name;

	UINT flag = 0;
	if (m_bFrame)
		flag |= ImGuiTreeNodeFlags_Framed;
	if (m_bSelected)
		flag |= ImGuiTreeNodeFlags_Selected;

	if (m_Owner->m_bShowFileName)
	{
		char buffer[256] = {};
		_splitpath_s(name.c_str(), 0, 0, 0, 0, buffer, 256, 0, 0);
		name = buffer;
	}

	if (m_vecChildNode.empty())
	{
		flag |= ImGuiTreeNodeFlags_Leaf;
		if (m_bFrame)
			name = "   " + name;
	}

	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		if (!m_bFrame && ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			m_Owner->SetSelectedNode(this);

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
			m_vecChildNode[i]->Render_Tick();

		ImGui::TreePop();
	}
}
/****************************TreeNode**************************************/



/****************************TreeUI****************************************/
TreeUI::TreeUI(const std::string& name)
	: EditorUI(name, "##Tree"), m_RootNode(nullptr), m_SelectedNode(nullptr),
	m_bShowRoot(true), m_bShowFileName(false),
	m_SelectedCallback(nullptr), m_SelectedInst(nullptr), m_SelectedDelegate(nullptr)
{
}

TreeUI::~TreeUI()
{
	if (m_RootNode != nullptr)
		delete m_RootNode;
}

void TreeUI::Render_Tick()
{
	if (m_RootNode == nullptr) return;

	if (m_bShowRoot)
		m_RootNode->Render_Tick();
	else
	{
		const std::vector<TreeNode*>& childNode = m_RootNode->GetChildNode();
		for (size_t i = 0; i < childNode.size(); ++i)
			childNode[i]->Render_Tick();
	}
}

TreeNode* TreeUI::AddTreeNode(TreeNode* parent, const std::string& nodeName, DWORD_PTR data)
{
	TreeNode* pNewNode = new TreeNode(nodeName, data);
	pNewNode->m_Owner = this;

	if (parent == nullptr)
	{
		assert(!m_RootNode);
		m_RootNode = pNewNode;
	}
	else
	{
		parent->AddChild(pNewNode);
	}

	return pNewNode;
}

void TreeUI::Clear()
{
	if (m_RootNode != nullptr)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}
void TreeUI::SetSelectedNode(TreeNode* node)
{
	if (m_SelectedNode)
		m_SelectedNode->m_bSelected = false;
	m_SelectedNode = node;
	if (m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = true;
		if (m_SelectedCallback)
			m_SelectedCallback();
		if (m_SelectedInst && m_SelectedDelegate)
			(m_SelectedInst->*m_SelectedDelegate)((DWORD_PTR)m_SelectedNode);
	}
}
/****************************TreeUI****************************************/
