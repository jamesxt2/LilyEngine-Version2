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

	Ptr<T>& operator=(T* asset)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = asset;

		if (m_Asset != nullptr)
			m_Asset->AddRef();

		return *this;
	}

	Ptr<T>& operator=(const Ptr<T>& other)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = other.m_Asset;

		if (m_Asset != nullptr)
			m_Asset->AddRef();

		return *this;
	}

	T* operator->()
	{
		return m_Asset;
	}

	T* Get() const { return m_Asset; }
	T** GetAddressOf() const { return &m_Asset; }

	bool operator==(T* asset) const
	{
		return m_Asset == asset;
	}

	bool operator!=(T* asset) const
	{
		return m_Asset != asset;
	}

	bool operator==(const Ptr<T>& asset) const
	{
		return m_Asset == asset.m_Asset;
	}

	bool operator!=(const Ptr<T>& asset) const
	{
		return m_Asset != asset.m_Asset;
	}

private:
	T* m_Asset;
};

template<typename T>
bool operator==(void* asset, const Ptr<T>& pAsset)
{
	return asset == pAsset.Get();
}

template<typename T>
bool operator!=(void* asset, const Ptr<T>& pAsset)
{
	return asset != pAsset.Get();
}