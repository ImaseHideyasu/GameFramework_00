//--------------------------------------------------------------------------------------
// File: TextureLoader.h
//
// �e�N�X�`���ǂݍ��݂̊Ǘ��N���X
//
// Date: 2018.4.26
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <unordered_map>
#include "Singleton.h"

class TextureLoader final : public Singleton<TextureLoader>
{
	class TextureInfo
	{
		/// <summary>
		///	�e�N�X�`���n���h��
		/// </summary>
		ID3D11ShaderResourceView* m_texture;
		/// <summary>
		/// �Q�ƃJ�E���^�[
		/// </summary>
		int m_referenceCounter;
	public:
		TextureInfo(ID3D11ShaderResourceView* texture) : m_texture(texture), m_referenceCounter(1) {}
		~TextureInfo();
		ID3D11ShaderResourceView* GetTextureHandle() { return m_texture; }
		int GetReferenceCounter() { return m_referenceCounter; }
		void IncrementReferenceCounter() { m_referenceCounter++; }
		bool DecrementReferenceCounter();
		bool ReloadTexture(ID3D11ShaderResourceView* texture);
		void ReleaseTexture();
	};

	ID3D11Device* m_device;

	/// <summary>
	/// ���[�h�ς݃e�N�X�`�����
	/// </summary>
	std::unordered_map<std::wstring, TextureInfo*> m_map;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TextureLoader();

	/// <summary>
	/// �������֐��i�f�o�C�X���X�g���ɍēx�Ăяo�����ƃe�N�X�`�����ă��[�h����j
	/// </summary>
	/// <param name="device">DirectX�̃f�o�C�X</param>
	void Initialize(ID3D11Device* device);

	/// <summary>
	/// �f�o�C�X���X�g���̃��Z�b�g�֐��i�f�o�C�X���X�g���ȊO�g�p���Ȃ����ƁI�j
	/// </summary>
	void Reset();

	/// <summary>
	/// �e�N�X�`�������[�h����֐�
	/// </summary>
	/// <param name="szFileName">�e�N�X�`�����i�p�X���܂ށj</param>
	/// <returns>�e�N�X�`���n���h��</returns>
	ID3D11ShaderResourceView* LoadTexture(const wchar_t* szFileName);

	/// <summary>
	/// �e�N�X�`�����������֐�
	/// </summary>
	/// <param name="szFileName">�e�N�X�`�����i�p�X���܂ށj</param>
	/// <returns>true�̏ꍇ�A����ɉ�����ꂽ</returns>
	bool ReleaseTexture(const wchar_t* szFileName);

	/// <summary>
	/// �e�N�X�`���n���h���̎擾�֐�
	/// </summary>
	/// <param name="szFileName">�e�N�X�`�����i�p�X���܂ށj</param>
	/// <returns>�e�N�X�`���n���h��</returns>
	ID3D11ShaderResourceView* GetTextureHandle(const wchar_t* szFileName);
};
