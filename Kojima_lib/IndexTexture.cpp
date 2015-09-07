#include"Common.h"
#include"IndexTexture.h"

//コンストラクタ
CIndexTexture::CIndexTexture():MY_VERTEX_FVF(D3DFVF_XYZ| D3DFVF_DIFFUSE| D3DFVF_TEX1)
{
	m_pIndexBuffer = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pTexture = NULL;
	SetScale(1,1,1);
	SetRot(0,0,0);
	SetPos(0,0,0);

	m_Alfa = 255;
}

//デストラクタ
CIndexTexture::~CIndexTexture()
{
	delete[] m_SlopeMap;
	m_SlopeMap = NULL;
	delete[] m_HeightMap;
	m_HeightMap = NULL;
	delete[] m_Vertex;
	m_Vertex = NULL;
	delete[] m_Index;
	m_Index = NULL;
	delete[] m_ShapeVertex;
	m_ShapeScale = NULL;
}

//セットアップ
HRESULT CIndexTexture::SetUp()
{
	// テクスチャの初期化
	if(m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// 頂点バッファの初期化
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}
	if(m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer = NULL;
	}
	// デバイスのチェック
	if(m_stpD3DDevice == 0)
	{
		ERROR_MSG("CTexture::Create デバイスへのポインタが無い");
		return E_FAIL;	// デバイスへのポインタが無い
	}
	m_ShapeVertex = new D3DXVECTOR3[5];
	m_ShapeVertex[0] = D3DXVECTOR3(-0.5f,0,0);
	m_ShapeVertex[1] = D3DXVECTOR3(0.5f,0,0);
	m_ShapeVertex[2] = D3DXVECTOR3(0,0.5f,0);
	m_ShapeVertex[3] = D3DXVECTOR3(-0.3f,-0.5f,0);
	m_ShapeVertex[4] = D3DXVECTOR3(0.3f,-0.5f,0);
	m_ShapeVertexNum = 5;

	m_TextureScale = 64;
	m_ShapeScale = 100;



	m_Vertex = new CTextureVertex[m_MapSize*4];
	m_Index =  new WORD[m_MapSize*6];


	for(int i = 0;i < m_ShapeVertexNum;++i)
	{
		
	}
	
	//一列目だけ
	for(int i = 0;i < m_MapHeight;++i)
	{
		D3DXVECTOR3 p[4];
		p[0] = m_Vertex[i * 4].p	 = D3DXVECTOR3((0.0f)*m_TextureScale/2,( 0.0f - i)*m_TextureScale,0);
		p[1] = m_Vertex[i * 4 + 1].p = D3DXVECTOR3((1.0f)*m_TextureScale/2,( 0.0f - i)*m_TextureScale,0);
		p[2] = m_Vertex[i * 4 + 2].p = D3DXVECTOR3((0.0f)*m_TextureScale/2,(-1.0f - i)*m_TextureScale,0);
		p[3] = m_Vertex[i * 4 + 3].p = D3DXVECTOR3((1.0f)*m_TextureScale/2,(-1.0f - i)*m_TextureScale,0);

		m_Vertex[i*4].t.x	  =  0.0f;
		m_Vertex[i*4].t.y	  =  0.0f;
		m_Vertex[i*4 + 1].t.x =  1.0f;
		m_Vertex[i*4 + 1].t.y =  0.0f;
		m_Vertex[i*4 + 2].t.x =  0.0f; 
		m_Vertex[i*4 + 2].t.y =  1.0f;
		m_Vertex[i*4 + 3].t.x =  1.0f; 
		m_Vertex[i*4 + 3].t.y =  1.0f;

		m_Vertex[i * 4].color = m_Vertex[i * 4 + 1].color = m_Vertex[i * 4 + 2].color = m_Vertex[i * 4 + 3].color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		for(int j = 0;j < 3;j++)
		m_Index[i * 6 + j] = j + i * 4;

		for(int j = 5;j > 2;j--)
		m_Index[i * 6 + j] = 6 - j + i * 4;
	}

	//二列目以降
	for(int k = 1;k < m_MapWidth + 1;++k)
	{
		int i = k * m_MapHeight;

		D3DXVECTOR3 p[4];
		p[0].x = m_Vertex[i * 4].p.x	 = (-0.0f + (float)k)*m_MaptipLength/2;
		p[1].x = m_Vertex[i * 4 + 1].p.x = ( 1.0f + (float)k)*m_MaptipLength/2;
		p[2].x = m_Vertex[i * 4 + 2].p.x = (-0.0f + (float)k)*m_MaptipLength/2;
		p[3].x = m_Vertex[i * 4 + 3].p.x = ( 1.0f + (float)k)*m_MaptipLength/2;

		m_Vertex[i * 4].p.z = m_Vertex[i * 4 + 1].p.z = m_Vertex[i * 4 + 2].p.z = m_Vertex[i * 4 + 3].p.z = 0;

		if(m_HeightMap[k] - m_HeightMap[k - 1] > 0)//角度が正の場合
		{
			m_Vertex[i*4].t.x	  =  0.0f;
			m_Vertex[i*4].t.y	  =  0.0f;
			if(i > 1)
				if(m_SlopeMap[k - 2] == 0.5f&&m_Vertex[((k - 1) * m_MapHeight)*4].t.y == 0.0f)
			{
				m_Vertex[i*4].t.y = 0.5f;
			}
			float a;
			float o = m_SlopeMap[k - 1];
			m_Vertex[i*4 + 1].t.x =  1.0f;
			a = tan(D3DXToRadian(m_SlopeMap[k - 1]));
			m_Vertex[i*4 + 1].t.y =  m_SlopeMap[k - 1] + m_Vertex[i*4].t.y;
			m_Vertex[i*4 + 2].t.x =  0.0f;
			m_Vertex[i*4 + 2].t.y =  0.0f;
			m_Vertex[i*4 + 3].t.x =  1.0f; 
			m_Vertex[i*4 + 3].t.y =  0.0f;

			p[0].y = m_Vertex[i * 4].p.y	 = m_HeightMap[k - 1]*m_MaptipLength;
			p[1].y = m_Vertex[i * 4 + 1].p.y = m_HeightMap[k]*m_MaptipLength;
			p[2].y = m_Vertex[i * 4 + 2].p.y =m_Vertex[i * 4].p.y - (m_Vertex[i*4].t.y)*m_MaptipLength;
			p[3].y = m_Vertex[i * 4 + 3].p.y =m_Vertex[i * 4 + 1].p.y - (m_Vertex[i*4 + 1].t.y)*m_MaptipLength;
		}
		else//角度が負の場合
		{
			m_Vertex[i*4].t.x	  =  0.0f;
			m_Vertex[i*4].t.y	  =  1.0f;
			if(k > 1)
			if(m_SlopeMap[k - 2] == -0.5f&&m_Vertex[((k - 1) * m_MapHeight)*4].t.y == 1.0f)
				m_Vertex[i*4].t.y = 0.5f;
			m_Vertex[i*4 + 1].t.x =  1.0f;
			m_Vertex[i*4 + 1].t.y =  m_SlopeMap[k - 1] + m_Vertex[i*4].t.y;
			m_Vertex[i*4 + 2].t.x =  0.0f; 
			m_Vertex[i*4 + 2].t.y =  0.0f;
			m_Vertex[i*4 + 3].t.x =  1.0f; 
			m_Vertex[i*4 + 3].t.y =  0.0f;

			p[0].y = m_Vertex[i * 4].p.y		= m_HeightMap[k - 1]*m_MaptipLength;
			p[1].y = m_Vertex[i * 4 + 1].p.y = m_HeightMap[k]*m_MaptipLength;
			p[2].y = m_Vertex[i * 4 + 2].p.y = m_Vertex[i * 4].p.y	 - (m_Vertex[i*4].t.y)*m_MaptipLength;
			p[3].y = m_Vertex[i * 4 + 3].p.y = m_Vertex[i * 4 + 1].p.y - (m_Vertex[i*4 + 1].t.y)*m_MaptipLength;
		}	

		m_Vertex[i * 4].color = m_Vertex[i * 4 + 1].color = m_Vertex[i * 4 + 2].color = m_Vertex[i * 4 + 3].color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		for(int j = 0;j < 3;j++)
			m_Index[(i) * 6 + j] = j + (i) * 4;

		for(int j = 5;j > 2;j--)
			m_Index[(i) * 6 + j] = 6 - j + (i) * 4;

		for(int j = 1;j < m_MapHeight;++j)//二段目以降
		{
			m_Vertex[(i + j)* 4].p.x	 = (-0.0f + (float)k)*m_MaptipLength/2;
			m_Vertex[(i + j)* 4 + 1].p.x = ( 1.0f + (float)k)*m_MaptipLength/2;
			m_Vertex[(i + j)* 4 + 2].p.x = (-0.0f + (float)k)*m_MaptipLength/2;
			m_Vertex[(i + j)* 4 + 3].p.x = ( 1.0f + (float)k)*m_MaptipLength/2;

			m_Vertex[(i + j)* 4].p.y	 = m_Vertex[i * 4 + 2].p.y - (float)(j - 1)*m_MaptipLength;
			m_Vertex[(i + j)* 4 + 1].p.y = m_Vertex[i * 4 + 2].p.y - (float)(j - 1)*m_MaptipLength;
			m_Vertex[(i + j)* 4 + 2].p.y = m_Vertex[i * 4 + 2].p.y - (float)(j)*m_MaptipLength;
			m_Vertex[(i + j)* 4 + 3].p.y = m_Vertex[i * 4 + 2].p.y - (float)(j)*m_MaptipLength;

			m_Vertex[(i + j)* 4].p.z = m_Vertex[(i + j)* 4 + 1].p.z = m_Vertex[(i + j)* 4 + 2].p.z = m_Vertex[(i + j)* 4 + 3].p.z = 0;

			m_Vertex[(i + j)*4].t.x	  =  0.0f;
			m_Vertex[(i + j)*4].t.y	  =  0.0f;
			m_Vertex[(i + j)*4 + 1].t.x =  1.0f;
			m_Vertex[(i + j)*4 + 1].t.y =  0.0f;
			m_Vertex[(i + j)*4 + 2].t.x =  0.0f; 
			m_Vertex[(i + j)*4 + 2].t.y =  1.0f;
			m_Vertex[(i + j)*4 + 3].t.x =  1.0f; 
			m_Vertex[(i + j)*4 + 3].t.y =  1.0f;

			m_Vertex[(i + j)* 4].color = m_Vertex[(i + j)* 4 + 1].color = m_Vertex[(i + j)* 4 + 2].color = m_Vertex[(i + j)* 4 + 3].color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

			for(int p = 0;p < 3;p++)
				m_Index[(i + j) * 6 + p] = p + (i + j) * 4;

			for(int p = 5;p > 2;p--)
				m_Index[(i + j) * 6 + p] = 6 - p + (i + j) * 4;
		}
		
	}


	return S_OK;
}


//描画関数
void CIndexTexture::Draw()
{
	Transform();

	//// レンダリングパイプラインにマトリックスを設定
	m_stpD3DDevice->SetTransform(D3DTS_WORLD, &m_World);

	// テクスチャをパイプラインにセット
	//for(int i = 0;i < m_MapSize;++i)
	m_stpD3DDevice->SetTexture(0, m_pTexture);
	
	m_stpD3DDevice->SetFVF(MY_VERTEX_FVF);

	m_stpD3DDevice->DrawIndexedPrimitiveUP
	(
	D3DPT_TRIANGLELIST,
	0,
	m_MapSize*4,
	m_MapSize*2,
	m_Index,
	D3DFMT_INDEX16,
	m_Vertex,
	sizeof(CTextureVertex)
	);

}

//頂点カラーを決める.
void CIndexTexture::SetVertexColor(int index,int r,int g,int b)
{
	m_Vertex[index].color = D3DCOLOR_RGBA(r,g,b,255);
}

//ファイルからテクスチャをロードする.
HRESULT CIndexTexture::SetpTexturefromFile(std::string FileName)
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

	
    if( FAILED(hr))
	{
        return E_FAIL;
	}

	return S_OK;
}