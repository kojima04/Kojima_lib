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
// デストラクタ
//-------------------------------------------------------------
CTexture::~CTexture()
{
	// テクスチャの解放
	if(m_pTexture != NULL)
	{
	//	m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの解放
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
	// テクスチャの初期化
	if(m_pTexture)
	{
		if(!m_isStatic)
			m_pTexture->Release();
		m_pTexture = NULL;
	}
	// 頂点バッファの初期化
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	// デバイスのチェック
	if(m_stpD3DDevice == 0)
	{
		ERROR_MSG("CTexture::Create デバイスへのポインタが無い");
		return E_FAIL;	// デバイスへのポインタが無い
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

	SetUpVertex();//頂点情報の設定

	
	return S_OK;
}



//-------------------------------------------------------------
// テクスチャ付き四角形を描画
// 引数
//		G_Device : IDirect3DDevice9 インターフェイスへのポインタ
//-------------------------------------------------------------
void CTexture::Draw(D3DXMATRIX * world)
{
	if(m_isZbuf)
		m_stpD3DDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0, 0, 0),1.0f,0);
	// デバイスのチェック
	if(m_stpD3DDevice == 0)
	{
		return;	// デバイスへのポインタが無い
	}

	// 頂点バッファのチェック
	if(m_pVB == 0)
	{
		return;	// 頂点バッファへのポインタが無い
	}

	// 行列の初期化
	//D3DXMatrixIdentity(world);

	//(this->*pFuncTransform)();
   
	// レンダリングパイプラインにマトリックスを設定
	m_stpD3DDevice->SetTransform(D3DTS_WORLD, world);
	// テクスチャをパイプラインにセット
	m_stpD3DDevice->SetTexture(0, m_pTexture);
	// テクスチャ付き四角形の描画処理
    m_stpD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(MY_VERTEX));
	// 頂点バッファのフォーマットの設定
    m_stpD3DDevice->SetFVF(MY_VERTEX_FVF);
	// 頂点バッファの描画
    m_stpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

LPDIRECT3DTEXTURE9 * CTexture::GetpTexturefromFile(LPDIRECT3DTEXTURE9 * pTex,std::string FileName)
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
		pTex                   // テクスチャ名
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
		&m_pTexture                   // テクスチャ名
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
	//VertexBufferの中身を埋める
    MY_VERTEX* v;
	// データ書き込み中にシステムがメモリを移動しないようにロックする
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // 各頂点の位置
	v[0].p = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);	// 頂点①
    v[1].p = D3DXVECTOR3(0.5f,  0.5f, 0.0f );	// 頂点②
    v[2].p = D3DXVECTOR3(-0.5f,-0.5f, 0.0f );	// 頂点③
    v[3].p = D3DXVECTOR3(0.5f, -0.5f, 0.0f );	// 頂点④
	// テクスチャ座標の設定
	for(int i = 0; i < 4;i++)
		v[i].t = D3DXVECTOR2(m_Vertex[i].t.x,m_Vertex[i].t.y);

	// 各頂点の色(白)
	for(int i = 0; i < 4;i++)
		//v[i].color = m_Vertex[i].color;
		v[i].color = m_Vertex[i].color = D3DCOLOR_RGBA(m_VertexColor[i][0],m_VertexColor[i][1],m_VertexColor[i][2],m_Alfa);
	// ロックを外す
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

