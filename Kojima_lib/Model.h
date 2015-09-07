#pragma once
#include "DrawObject.h"
#include "Texture.h"

class CModel :
	public CDrawObject
{
	public:
	CModel();
	~CModel();
	bool		LoadModelFromFile(std::string FileName);
	void		Draw();	
	void		Destroy();

	LPD3DXMESH	GetMesh(){return m_pMesh;}

protected:
	HRESULT		ComputeBoundingSphere();
protected:
	LPD3DXMESH		m_pMesh;		//	メッシュ
	D3DMATERIAL9 *	m_pMaterial;	//	マテリアル
	DWORD			m_NumMaterial;	//	マテリアルの数
	LPDIRECT3DTEXTURE9 *	m_pTexture;	//	テクスチャ
public:
	DWORD			GetNumMaterial(){return m_NumMaterial;}
	void SetpTexture(LPDIRECT3DTEXTURE9);

	LPDIRECT3DTEXTURE9 * GetTexture(int i){return &m_pTexture[i];}
	D3DMATERIAL9 * getMaterial(){return m_pMaterial;}
	HRESULT SetpTexturefromFile(int i,std::string);
};