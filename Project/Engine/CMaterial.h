#pragma once
#include "CAsset.h"

class CGraphicsShader;
class CTexture;

class CMaterial : public CAsset
{
public:
	CMaterial();
	~CMaterial();
	CLONE(CMaterial)

	void Bind();

	virtual int Load(const std::wstring& filePath) override { return S_OK; }
	virtual int Save(const std::wstring& filePath) override { return S_OK; }

private:
	TMaterialConst				m_Const;
	Ptr<CGraphicsShader>		m_Shader;
	Ptr<CTexture>				m_arrTex[TEX_PARAM::END];

	bool						m_Dynamic;

public:
	inline void SetShader(Ptr<CGraphicsShader> shader) { m_Shader = shader; }
	inline Ptr<CGraphicsShader> GetShader() const { return m_Shader; }

	template<typename T>
	void SetScalarParam(SCALAR_PARAM param, const T& value);

	inline void SetTexParam(TEX_PARAM param, Ptr<CTexture> texture) { m_arrTex[param] = texture; }

	inline bool IsDynamic() const { return m_Dynamic; }

	Ptr<CMaterial> GetDynamicMaterial();
};

template<typename T>
inline void CMaterial::SetScalarParam(SCALAR_PARAM param, const T& value)
{
	if constexpr (std::is_same_v<T, int>)
		m_Const.iArr[param - INT_0] = value;
	if constexpr (std::is_same_v<T, UINT>)
		m_Const.iArr[param - INT_0] = value;
	if constexpr (std::is_same_v<T, float>)
		m_Const.fArr[param - FLOAT_0] = value;
	if constexpr (std::is_same_v<T, Vec2>)
		m_Const.v2Arr[param - VEC2_0] = value;
	if constexpr (std::is_same_v<T, Vec4>)
		m_Const.v4Arr[param - VEC4_0] = value;
	if constexpr (std::is_same_v<T, Matrix>)
		m_Const.matArr[param - MAT_0] = value;
}
