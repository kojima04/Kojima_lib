#include "Common.h"
#include "Model.h"

CModel::CModel()
{
	m_pMesh = NULL;
	m_pMaterial = NULL;
	m_pTexture = NULL;
	m_NumMaterial = 0;
	SetScale(1,1,1);
	SetPos(0,0,0);
	SetRot(0,0,0);
}

CModel::~CModel()
{
	Destroy();
}

void CModel::Destroy()
{
	if(m_pMesh)
	SAFE_RELEASE(m_pMesh);
	if(m_pTexture)
	SAFE_DELETEARRAY(m_pTexture);
	if(m_pMaterial)
	SAFE_DELETEARRAY(m_pMaterial);
}

bool CModel::LoadModelFromFile(std::string FileName)
{
	LPD3DXBUFFER lpD3DBuf;

	if(FAILED(D3DXLoadMeshFromX(
		FileName.c_str(),		//ファイル名
		D3DXMESH_SYSTEMMEM,
		GetDevice(),
		NULL,
		&lpD3DBuf,
		NULL,
		&m_NumMaterial,	//マテリアルの数
		&m_pMesh)))			//メッシュへのpp
	{
		return false;
	}

	//各種インスタンス取得
	D3DXMATERIAL *d3dxMatrs = (D3DXMATERIAL *)lpD3DBuf->GetBufferPointer();

	m_pTexture = new LPDIRECT3DTEXTURE9[m_NumMaterial];
	m_pMaterial = new D3DMATERIAL9[m_NumMaterial];

	if(m_pTexture == NULL || m_pMaterial == NULL)
	{
		SAFE_RELEASE(lpD3DBuf);
		return false;
	}

	for(DWORD i = 0; i < m_NumMaterial; ++i)
	{
		m_pMaterial[i] = d3dxMatrs[i].MatD3D;	//材質のコピー
		m_pMaterial[i].Ambient = m_pMaterial[i].Diffuse;
		D3DXCreateTextureFromFile(
			GetDevice(),
			d3dxMatrs[i].pTextureFilename,
			(&m_pTexture[i]));
	}

	SAFE_RELEASE(lpD3DBuf);

	LPD3DXMESH pMesh = NULL;
	if(m_pMesh->GetFVF()!=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1))
	{
		//メッシュに法線がない場合書き込む
		m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(),
				D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
			GetDevice(),&pMesh);
		SAFE_RELEASE(m_pMesh);
		m_pMesh = pMesh;
	}


	return true;
}

void CModel::Draw()
{
	Transform();
	m_stpD3DDevice->SetTransform(D3DTS_WORLD, &m_World);
	for(DWORD i = 0; i < m_NumMaterial; ++i)
	{
		GetDevice()->SetMaterial(&m_pMaterial[i]);
		GetDevice()->SetTexture(0, m_pTexture[i]);

		m_pMesh->DrawSubset(i);
	}
}

void CModel::SetpTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture[0] = tex;
}

HRESULT CModel::SetpTexturefromFile(int i,std::string FileName)
{
	HRESULT hr = 0;

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
        &m_pTexture[i]                   // テクスチャ名
    );

	
    if( FAILED(hr))
	{
        return E_FAIL;
	}

	return S_OK;
}