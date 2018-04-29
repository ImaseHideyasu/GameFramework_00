//--------------------------------------------------------------------------------------
// File: TextureLoader.cpp
//
// テクスチャ読み込みの管理クラス
//
// Date: 2018.4.26
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TextureLoader.h"

using namespace DirectX;

TextureLoader::~TextureLoader()
{
	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		delete it->second;
	}
	m_map.clear();
}

void TextureLoader::Reset()
{
	// テクスチャリソースのみ解放する
	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		it->second->ReleaseTexture();
	}
}

void TextureLoader::Initialize(ID3D11Device * device)
{
	m_device = device;

	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		// テクスチャをもう一度リロードする
		ID3D11ShaderResourceView* p;
		CreateWICTextureFromFile(m_device, it->first.c_str(), nullptr, &p);
		if (m_map[it->first]->ReloadTexture(p) != true)
		{
			// 登録できなかったのでリリースする
			p->Release();
		}
	}
}

ID3D11ShaderResourceView * TextureLoader::LoadTexture(const wchar_t * szFileName)
{
	std::wstring str(szFileName);

	// ロード済みかチェックする
	auto it = m_map.find(str);
	if (it != m_map.end())
	{
		// 参照カウンターを増やす
		it->second->IncrementReferenceCounter();
		return it->second->GetTextureHandle();
	}
	else
	{
		// テクスチャをロードする
		ID3D11ShaderResourceView* p;
		CreateWICTextureFromFile(m_device, szFileName, nullptr, &p);
		m_map[str] = new TextureInfo(p);
		return p;
	}
}

bool TextureLoader::ReleaseTexture(const wchar_t * szFileName)
{
	std::wstring str(szFileName);
	auto it = m_map.find(str);
	if (it != m_map.end())
	{
		// 参照カンターを減算する
		if (it->second->DecrementReferenceCounter() == false)
		{
			// テクスチャ情報を解放する
			delete it->second;
			m_map.erase(it);
			return true;
		}
	}
	return false;
}

ID3D11ShaderResourceView * TextureLoader::GetTextureHandle(const wchar_t * szFileName)
{
	auto it = m_map.find(szFileName);
	if (it != m_map.end())
	{
		return it->second->GetTextureHandle();
	}
	return nullptr;
}

TextureLoader::TextureInfo::~TextureInfo()
{
	ReleaseTexture();
}

bool TextureLoader::TextureInfo::DecrementReferenceCounter()
{
	m_referenceCounter--;
	if (m_referenceCounter == 0)
	{
		return false;
	}
	return true;
}

bool TextureLoader::TextureInfo::ReloadTexture(ID3D11ShaderResourceView * texture)
{
	// 解放されているか一応チェックする
	if (m_texture == nullptr)
	{
		m_texture = texture;
		return true;
	}
	return false;
}

void TextureLoader::TextureInfo::ReleaseTexture()
{
	// 参照カウントはそのままなのでデバイスのロストなどの対処など以外には使用しないこと！
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}
