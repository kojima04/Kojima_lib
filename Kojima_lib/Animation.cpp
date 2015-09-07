#include "Common.h"
#include "Animation.h"

//-----------------------------------------------------------------
//							CAnimation
//-----------------------------------------------------------------
CAnimation::CAnimation()
{
	m_KeyFlameCount = 0;
	m_FlameCount = 0;
	m_isActive = true;
	m_KeyFlameArray = NULL;
	m_isLoop = true;
	SetFeedOutCount(0);
	m_AnimFlameAll = 0;
	m_isTexReverse = false;
	m_KeyFlameNum = 1;
}

CAnimation::~CAnimation()
{
	Destroy();
}

void CAnimation::Destroy()
{
	if(m_KeyFlameArray != NULL)
		delete m_KeyFlameArray;
}

void CAnimation::SetAlfa(int A)
{
	m_fAlfa = (float)A;
	CTexture::SetAlfa(A);
}

void CAnimation::SetAnimPara(int num, int time)
{
	for(int i = 0;i < num;i++)
	{
		if(time != 0)
			m_KeyFlameArray[i] = time * (i + 1)/num;
		else
		{
			m_KeyFlameArray[i] = 0;
		}
	}
	if(time == 0)
		m_AnimFlameAll = 0;
	else
		m_AnimFlameAll = m_KeyFlameArray[num - 1];
	m_KeyFlameCount = 0;
	m_FlameCount = 0;
}

void CAnimation::SetKeyFlameNum(int num)
{
	if(m_KeyFlameNum != num||m_KeyFlameArray == NULL)
	{
		Destroy();
		m_KeyFlameNum = num;
		m_KeyFlameArray = new int[num];
	}
}

void CAnimation::SetUpVertex()
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
	{
		if(m_KeyFlameNum > 1)
			v[i].t = D3DXVECTOR2((float)m_KeyFlameCount/(float)m_KeyFlameNum + m_Vertex[i].t.x/(float)m_KeyFlameNum,m_Vertex[i].t.y);
		else
		v[i].t = D3DXVECTOR2(m_Vertex[i].t.x,m_Vertex[i].t.y);
	}


	// 各頂点の色(白)
	for(int i = 0; i < 4;i++)
		//v[i].color = m_Vertex[i].color;
		v[i].color = m_Vertex[i].color = D3DCOLOR_RGBA(m_VertexColor[i][0],m_VertexColor[i][1],m_VertexColor[i][2],m_Alfa);
	// ロックを外す
    m_pVB->Unlock();
}

void CAnimation::Animation()
{
	//VertexBufferの中身を埋める
    MY_VERTEX* v;
	// データ書き込み中にシステムがメモリを移動しないようにロックする
    m_pVB->Lock( 0, 0, (void**)&v, 0 );
    // 各頂点の位置puna
	v[0].p = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);	// 頂点①
    v[1].p = D3DXVECTOR3(0.5f,  0.5f, 0.0f);	// 頂点②
    v[2].p = D3DXVECTOR3(-0.5f,-0.5f, 0.0f);	// 頂点③
    v[3].p = D3DXVECTOR3(0.5f, -0.5f, 0.0f);	// 頂点④
	// テクスチャ座標の設定
	for(int i = 0; i < 4;i++)
		v[i].t = D3DXVECTOR2((float)m_KeyFlameCount/(float)m_KeyFlameNum + m_Vertex[i].t.x/(float)m_KeyFlameNum,m_Vertex[i].t.y);
	if(m_isTexReverse)
	{
		MY_VERTEX vtmp;
		vtmp.t.x = v[0].t.x;	vtmp.t.y = v[0].t.y;
		v[0].t.x = v[1].t.x;	v[0].t.y = v[1].t.y;
		v[1].t.x = vtmp.t.x;	v[1].t.y = vtmp.t.y;

		vtmp.t.x = v[2].t.x;	vtmp.t.y = v[2].t.y;
		v[2].t.x = v[3].t.x;	v[2].t.y = v[3].t.y;
		v[3].t.x = vtmp.t.x;	v[2].t.y = vtmp.t.y;
	}
	// 各頂点の色(白)
	for(int i = 0; i < 4;i++)
		v[i].color = m_Vertex[i].color;
	// ロックを外す
    m_pVB->Unlock();

	if(m_KeyFlameNum > 1)
	if(m_isActive)
	{
	
		if(m_KeyFlameNum != 1)
		{
			float t = (float)CFPSManager::GetFlmAngl();
			m_fFlameCount += (float)CFPSManager::GetFlmAngl();
			m_FlameCount = (int)m_fFlameCount;
		}

		if(m_fFlameCount > m_KeyFlameArray[m_KeyFlameCount])
		{
			m_KeyFlameCount++;
			if(m_KeyFlameCount >= m_KeyFlameNum)
			{
				if(!m_isLoop)
					m_isActive = false;

				m_KeyFlameCount = 0;
				m_fFlameCount = 0;
			}
		}
	}

}