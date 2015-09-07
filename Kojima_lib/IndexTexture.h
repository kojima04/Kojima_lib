#pragma once
#include "DrawObject.h"
#include "Figure.h"

class CIndexTexture 
	:	public CDrawObject
{
public:
	CIndexTexture();
	~CIndexTexture();

	HRESULT SetUp();
	void Draw();

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	CTextureVertex * m_Vertex;
	WORD		   * m_Index;

	std::list<CSimpleTriangle *> m_TriangleList;//�O�p�`�̃��X�g
	D3DXVECTOR3 * GetMostFarShapeVertex();

	D3DXVECTOR3 * m_ShapeVertex;	
	int m_ShapeVertexNum;
	float m_ShapeScale;
	float m_TextureScale;
	void CheckStripeCross();

	void SetVertexColor(int index,int r,int g,int b);

	int m_Alfa;

	LPDIRECT3DTEXTURE9 * GetpTexture(){return &m_pTexture;};
	HRESULT SetpTexturefromFile(std::string);

	float GetHeight(float x);
	bool CheckoverGround(float x);


protected:
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̒�`
	const DWORD MY_VERTEX_FVF;

	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;

protected:
	int m_MapWidth;
	int m_MapHeight;
	int m_MapSize;
	float m_MaptipLength;

	float * m_SlopeMap;
	float * m_HeightMap;

};