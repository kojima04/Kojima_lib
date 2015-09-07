#pragma once

#include "DrawObject.h"


//=============================================================
// Texture
//=============================================================
class CTexture :
	public CDXObject
{
public:
	CTexture();
	~CTexture();

	void (CTexture::*pFuncTransform)(); 

	int  GetAlfa(){return m_Alfa;}
	void SetAlfa(int A);
	void SetVertexColor(int index,int r,int g,int b);
	int	GetVertexColor(int idx,int idx2){return m_VertexColor[idx][idx2];}

protected:

	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	// 頂点バッファの構造体
	struct MY_VERTEX
	{
		D3DXVECTOR3	p;		// 位置
		DWORD		color;	// 色
		D3DXVECTOR2	t;		// テクスチャ座標
	};

	int m_Alfa;//透過率
	int m_VertexColor[4][3];

protected:
	int RenderType;
	
	// 頂点バッファのフォーマットの定義
	const DWORD MY_VERTEX_FVF;

	// テクスチャへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;
	bool					m_isStatic;

	CTextureVertex			m_Vertex[4];

public:
	//テクスチャのファイル名
	std::string FileName;
	
	bool m_isZbuf;
	HRESULT Create();
	void SetUpVertex();

	void Draw(D3DXMATRIX * world);
	void SetpTexture(LPDIRECT3DTEXTURE9);
	LPDIRECT3DTEXTURE9 * GetpTexture(){return &m_pTexture;};
	HRESULT SetpTexturefromFile(std::string FileName);

	static LPDIRECT3DTEXTURE9 * GetpTexturefromFile(LPDIRECT3DTEXTURE9 * pTex,std::string FileName);

	void SetTextureVertex(int index,float x,float y);
	D3DXVECTOR2 * GetTextureVertex(int index){return &m_Vertex[index].t;};

	void SetVerTex(int index,D3DXVECTOR3 * vec);

	void SetpFuncTransform(void (CTexture::*pfunc)()){pFuncTransform = pfunc;};

	void SetTextureLenge(float left,float right);
	void SetTextureLengeVirtical(float top,float buttom);
};

