#pragma once
#include "CShader.h"

class CComputeShader : public CShader
{
public:
	CComputeShader(UINT threadPerGroupX, UINT threadPerGroupY, UINT threadPerGroupZ);
	virtual ~CComputeShader();

	virtual int Bind() = 0;
	virtual void Clear() = 0;
	virtual void CalculateGroupNum() = 0;

	int CreateComputeShader(const std::wstring& filePath, const std::string& funcName);

	int Execute();

private:
	ComPtr<ID3DBlob>						m_CSBlob;
	ComPtr<ID3D11ComputeShader>				m_CS;

protected:
	UINT									m_ThreadPerGroupX;
	UINT									m_ThreadPerGroupY;
	UINT									m_ThreadPerGroupZ;

	UINT									m_GroupX;
	UINT									m_GroupY;
	UINT									m_GroupZ;

	TMaterialConst							m_Const;
};

