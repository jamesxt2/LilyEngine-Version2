#pragma once

template<typename T>
class Ptr
{
public:
	Ptr()
		: m_Asset(nullptr)
	{
	}

	Ptr(T* pAsset)
		: m_Asset(pAsset)
	{
		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& other)
		: m_Asset(other.m_Asset)
	{
		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	~Ptr()
	{
		if (m_Asset != nullptr)
			m_Asset->Release();
	}

	void operator=(T* asset)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = asset;

		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	void operator=(const Ptr<T>& other)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = other.m_Asset;

		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	T* operator->()
	{
		return m_Asset;
	}

	T* Get() { return m_Asset; }
	T** GetAddressOf() { return &m_Asset; }

private:
	T* m_Asset;
};