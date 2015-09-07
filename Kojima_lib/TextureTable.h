#pragma once
#include "Texture.h"

class CTextureTable
{
public:
	CTextureTable();
	~CTextureTable();
	bool Init(LPDIRECT3DDEVICE9 device);
	void Destroy();
	LPDIRECT3DTEXTURE9 RequestTexture(std::string FilePath);
	HRESULT AddTexturefromFile(std::string FilePath,std::string FileName);
protected:
	std::map<std::string,LPDIRECT3DTEXTURE9 *> m_TextureBook;
	LPDIRECT3DDEVICE9 m_stpD3DDevice;
};