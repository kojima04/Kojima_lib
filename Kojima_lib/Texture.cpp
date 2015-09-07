#include "Common.h"
#include "Texture.h"

//----------------------------------------------------
//					CTexture
//------------------------------------------------------


CTexture::CTexture(): m_pVB(0), m_pTexture(0), MY_VERTEX_FVF(D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
{
	m_Vertex[0].p.x = -0.5f; m_Vertex[0].p.y =  0.5f; m_Vertex[0].p.z = 0.0f; 
	m_Vertex[1].p.x =  0.5f; m_Vertex[1].p.y =  0.5f; m_Vertex[1].p.z = 0.0f; 
	m_Vertex[2].p.x = -0.5f; m_Vertex[2].p.y = -0.5f; m_Vertex[2].p.z = 0.0f;
	m_Vertex[3].p.x =  0.5f; m_Vertex[3].p.y = -0.5f; m_Vertex[3].p.z = 0.0f;

	m_Vertex[0].t.x =  0.0f; m_Vertex[0].t.y =  0.0f;
	m_Vertex[1].t.x =  1.0f; m_Vertex[1].t.y =  0.0f;
	m_Vertex[2].t.x =  0.0f; m_Vertex[2].t.y =  1.0f;
	m_Vertex[3].t.x =  1.0f; m_Vertex[3].t.y =  1.0f;

	m_isZbuf = false;

	m_Alfa = 255;

	m_Vertex[0].color = m_Vertex[1].color = m_Vertex[2].color = m_Vertex[3].color = D3DCOLOR_RGBA(255,255,255,m_Alfa);

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 3;++j)
			m_VertexColor[i][j] = 255;

	m_pTexture = NULL;
	m_isStatic = true;
	Create();
}

void CTexture::SetVerTex(int index,D3DXVECTOR3 * vec)
{
	m_Vertex[index].p = *vec;
}

void CTexture::SetTextureLenge(float left,float right)
{
	m_Vertex[0].t.x =  left; m_Vertex[0].t.y =  0.0f;
	m_Vertex[1].t.x =  right; m_Vertex[1].t.y =  0.0f;
	m_Vertex[2].t.x =  left; m_Vertex[2].t.y =  1.0f;
	m_Vertex[3].t.x =  right; m_Vertex[3].t.y =  1.0f;
}

void CTexture::SetTextureLengeVirtical(float top,float buttom)
{
	m_Vertex[0].t.x =  0.0f; m_Vertex[0].t.y =  buttom;
	m_Vertex[1].t.x =  1.0f; m_Vertex[1].t.y =  buttom;
	m_Vertex[2].t.x =  0.0f; m_Vertex[2].t.y =  top;
	m_Vertex[3].t.x =  1.0f; m_Vertex[3].t.y =  top;
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
CTexture::~CTexture()
{
	// �e�N�X�`���̉��
	if(m_pTexture != NULL)
	{
	//	m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̉��
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}
void CTexture::SetAlfa(int A)
{
	m_Alfa = A;
	for(int i = 0;i < 4;++i)
		SetVertexColor(i,m_VertexColor[i][0],m_VertexColor[i][1],m_VertexColor[i][2]);
	SetUpVertex();
}

HRESULT CTexture::Create()
{
	// �e�N�X�`���̏�����
	if(m_pTexture)
	{
		if(!m_isStatic)
			m_pTexture->Release();
		m_pTexture = NULL;
	}
	// ���_�o�b�t�@�̏�����
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	// �f�o�C�X�̃`�F�b�N
	if(m_stpD3DDevice == 0)
	{
		ERROR_MSG("CTexture::Create �f�o�C�X�ւ̃|�C���^������");
		return E_FAIL;	// �f�o�C�X�ւ̃|�C���^������
	}

	HRESULT hr = 0;

	hr = m_stpD3DDevice->CreateVertexBuffer(
										4 * sizeof(MY_VERTEX),
										D3DUSAGE_WRITEONLY,
										MY_VERTEX_FVF,
										D3DPOOL_MANAGED, &m_pVB, NULL
										);
    if( FAILED(hr))
	{
		ERROR_MSG("CTexture::Create:CreateVertexBuffer");
        return E_FAIL;
	}

	SetUpVertex();//���_���̐ݒ�

	
	return S_OK;
}



//-------------------------------------------------------------
// �e�N�X�`���t���l�p�`��`��
// ����
//		G_Device : IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^
//-------------------------------------------------------------
void CTexture::Draw(D3DXMATRIX * world)
{
	if(m_isZbuf)
		m_stpD3DDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0, 0, 0),1.0f,0);
	// �f�o�C�X�̃`�F�b�N
	if(m_stpD3DDevice == 0)
	{
		return;	// �f�o�C�X�ւ̃|�C���^������
	}

	// ���_�o�b�t�@�̃`�F�b�N
	if(m_pVB == 0)
	{
		return;	// ���_�o�b�t�@�ւ̃|�C���^������
	}

	// �s��̏�����
	//D3DXMatrixIdentity(world);

	//(this->*pFuncTransform)();
   
	// �����_�����O�p�C�v���C���Ƀ}�g���b�N�X��ݒ�
	m_stpD3DDevice->SetTransform(D3DTS_WORLD, world);
	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	m_stpD3DDevice->SetTexture(0, m_pTexture);
	// �e�N�X�`���t���l�p�`�̕`�揈��
    m_stpD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));
	// ���_�o�b�t�@�̃t�H�[�}�b�g�̐ݒ�
    m_stpD3DDevice->SetFVF(MY_VERTEX_FVF);
	// ���_�o�b�t�@�̕`��
    m_stpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

LPDIRECT3DTEXTURE9 * CTexture::GetpTexturefromFile(LPDIRECT3DTEXTURE9 * pTex,std::string FileName)
{
	HRESULT hr = 0;

	// �e�N�X�`���摜���t�@�C������ǂݍ���
	hr = D3DXCreateTextureFromFileEx(
        (LPDIRECT3DDEVICE9)m_stpD3DDevice,
        FileName.c_str(),              // �t�@�C����
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // �F�������\��
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
		pTex                   // �e�N�X�`����
    );

	
    if( FAILED(hr))
	{
        return NULL;
	}

	return pTex;
}

HRESULT CTexture::SetpTexturefromFile(std::string FileName)
{
	
	HRESULT hr = 0;

	// �e�N�X�`���摜���t�@�C������ǂݍ���
	hr = D3DXCreateTextureFromFileEx(
        (LPDIRECT3DDEVICE9)m_stpD3DDevice,
        FileName.c_str(),              // �t�@�C����
        0,
        0,
        0,
        0,
        D3DFMT_A8B8G8R8,                // �F�������\��
        D3DPOOL_MANAGED,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        D3DCOLOR_ARGB(255, 0, 255, 255), 
        NULL,
        NULL,
		&m_pTexture                   // �e�N�X�`����
    );

	m_isStatic = true;
	
    if( FAILED(hr))
	{
        return E_FAIL;
	}

	return S_OK;
}

void CTexture::SetpTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

void CTexture::SetUpVertex()
{
	//VertexBuffer�̒��g�𖄂߂�
    MY_VERTEX* v;
	// �f�[�^�������ݒ��ɃV�X�e�������������ړ����Ȃ��悤�Ƀ��b�N����
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // �e���_�̈ʒu
	v[0].p = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);	// ���_�@
    v[1].p = D3DXVECTOR3(0.5f,  0.5f, 0.0f );	// ���_�A
    v[2].p = D3DXVECTOR3(-0.5f,-0.5f, 0.0f );	// ���_�B
    v[3].p = D3DXVECTOR3(0.5f, -0.5f, 0.0f );	// ���_�C
	// �e�N�X�`�����W�̐ݒ�
	for(int i = 0; i < 4;i++)
		v[i].t = D3DXVECTOR2(m_Vertex[i].t.x,m_Vertex[i].t.y);

	// �e���_�̐F(��)
	for(int i = 0; i < 4;i++)
		//v[i].color = m_Vertex[i].color;
		v[i].color = m_Vertex[i].color = D3DCOLOR_RGBA(m_VertexColor[i][0],m_VertexColor[i][1],m_VertexColor[i][2],m_Alfa);
	// ���b�N���O��
    m_pVB->Unlock();
}

void CTexture::SetVertexColor(int index,int r,int g,int b)
{
//	m_Vertex[index].color = D3DCOLOR_RGBA(r,g,b,m_Alfa);
	m_VertexColor[index][0] = r;
	m_VertexColor[index][1] = g;
	m_VertexColor[index][2] = b;
}


void CTexture::SetTextureVertex(int index,float x,float y)
{
	m_Vertex[index].t.x = x;
	m_Vertex[index].t.y = y;
}

