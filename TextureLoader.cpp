//--------------------------------------------------------------------------------------
// File: TextureLoader.cpp
//
// �e�N�X�`���ǂݍ��݂̊Ǘ��N���X
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
	// �e�N�X�`�����\�[�X�̂݉������
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
		// �e�N�X�`����������x�����[�h����
		ID3D11ShaderResourceView* p;
		CreateWICTextureFromFile(m_device, it->first.c_str(), nullptr, &p);
		if (m_map[it->first]->ReloadTexture(p) != true)
		{
			// �o�^�ł��Ȃ������̂Ń����[�X����
			p->Release();
		}
	}
}

ID3D11ShaderResourceView * TextureLoader::LoadTexture(const wchar_t * szFileName)
{
	std::wstring str(szFileName);

	// ���[�h�ς݂��`�F�b�N����
	auto it = m_map.find(str);
	if (it != m_map.end())
	{
		// �Q�ƃJ�E���^�[�𑝂₷
		it->second->IncrementReferenceCounter();
		return it->second->GetTextureHandle();
	}
	else
	{
		// �e�N�X�`�������[�h����
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
		// �Q�ƃJ���^�[�����Z����
		if (it->second->DecrementReferenceCounter() == false)
		{
			// �e�N�X�`�������������
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
	// �������Ă��邩�ꉞ�`�F�b�N����
	if (m_texture == nullptr)
	{
		m_texture = texture;
		return true;
	}
	return false;
}

void TextureLoader::TextureInfo::ReleaseTexture()
{
	// �Q�ƃJ�E���g�͂��̂܂܂Ȃ̂Ńf�o�C�X�̃��X�g�Ȃǂ̑Ώ��ȂǈȊO�ɂ͎g�p���Ȃ����ƁI
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}
