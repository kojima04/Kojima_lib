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

	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	// ���_�o�b�t�@�̍\����
	struct MY_VERTEX
	{
		D3DXVECTOR3	p;		// �ʒu
		DWORD		color;	// �F
		D3DXVECTOR2	t;		// �e�N�X�`�����W
	};

	int m_Alfa;//���ߗ�
	int m_VertexColor[4][3];

protected:
	int RenderType;
	
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̒�`
	const DWORD MY_VERTEX_FVF;

	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;
	bool					m_isStatic;

	CTextureVertex			m_Vertex[4];

public:
	//�e�N�X�`���̃t�@�C����
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

