//--------------------------------------------------------------------------------------
// File: TextureLoader.h
//
// テクスチャ読み込みの管理クラス
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
		///	テクスチャハンドル
		/// </summary>
		ID3D11ShaderResourceView* m_texture;
		/// <summary>
		/// 参照カウンター
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
	/// ロード済みテクスチャ情報
	/// </summary>
	std::unordered_map<std::wstring, TextureInfo*> m_map;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureLoader();

	/// <summary>
	/// 初期化関数（デバイスロスト時に再度呼び出されるとテクスチャを再ロードする）
	/// </summary>
	/// <param name="device">DirectXのデバイス</param>
	void Initialize(ID3D11Device* device);

	/// <summary>
	/// デバイスロスト時のリセット関数（デバイスロスト時以外使用しないこと！）
	/// </summary>
	void Reset();

	/// <summary>
	/// テクスチャをロードする関数
	/// </summary>
	/// <param name="szFileName">テクスチャ名（パスを含む）</param>
	/// <returns>テクスチャハンドル</returns>
	ID3D11ShaderResourceView* LoadTexture(const wchar_t* szFileName);

	/// <summary>
	/// テクスチャを解放する関数
	/// </summary>
	/// <param name="szFileName">テクスチャ名（パスを含む）</param>
	/// <returns>trueの場合、正常に解放された</returns>
	bool ReleaseTexture(const wchar_t* szFileName);

	/// <summary>
	/// テクスチャハンドルの取得関数
	/// </summary>
	/// <param name="szFileName">テクスチャ名（パスを含む）</param>
	/// <returns>テクスチャハンドル</returns>
	ID3D11ShaderResourceView* GetTextureHandle(const wchar_t* szFileName);
};
