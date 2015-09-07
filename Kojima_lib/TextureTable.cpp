#include "Common.h"

#include "TextureTable.h"

CTextureTable::CTextureTable()
{

}

CTextureTable::~CTextureTable()
{
	Destroy();
}

bool CTextureTable::Init(LPDIRECT3DDEVICE9 device)
{
	m_stpD3DDevice = device;
	return true;	
}

void CTextureTable::Destroy()
{
	std::map<std::string,LPDIRECT3DTEXTURE9 *>::iterator iter = m_TextureBook.begin();

	while(iter != m_TextureBook.end())
	{
		SAFE_DELETE((*iter).second);
		++iter;
	}
	m_TextureBook.clear();
}

HRESULT CTextureTable::AddTexturefromFile(std::string FilePath,std::string FileName)
{
	
	HRESULT hr = 0;

	LPDIRECT3DTEXTURE9 * temp;
	temp = new LPDIRECT3DTEXTURE9;

	// テクスチャ画像をファイルから読み込む
	hr = D3DXCreateTextureFromFileEx(
        (LPDIRECT3DDEVICE9)m_stpD3DDevice,
        FileName.c_str(),              // ファイル名
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // 色抜きを可能に
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
        temp                   // テクスチャ名
    );

	
    if( FAILED(hr))
	{
        return E_FAIL;
	}

	m_TextureBook.insert(std::pair<std::string,LPDIRECT3DTEXTURE9 *>(FilePath,temp));

	return S_OK;
}

LPDIRECT3DTEXTURE9 CTextureTable::RequestTexture(std::string FilePath)
{
	std::map<std::string,LPDIRECT3DTEXTURE9 *>::iterator iter;

	iter = m_TextureBook.find(FilePath);
	if(iter == m_TextureBook.end())
	{
		ERROR_MSG("CTextureTable::RequestTexture　そんなTEXTUREねぇ！");
		return NULL;
	}
	return *((*iter).second);
}