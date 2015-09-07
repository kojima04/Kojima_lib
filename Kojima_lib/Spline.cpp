#include "Common.h"
#include "Spline.h"

CSplineCurve::CSplineCurve()
{
	m_Node = NULL;
	m_Not = NULL;
}

CSplineCurve::~CSplineCurve()
{
	delete[]  m_Node;
	m_Node = NULL;

	delete[]  m_Not;
	m_Not = NULL;
}

void CSplineCurve::SetUp(int i)
{
	m_Node = new D3DXVECTOR2[i];
	m_Not = new int[i + 4];
	m_NodeNum = i;
}

float CSplineCurve::SplnMetaFnc(int i,int j,float t)
{
	if(j == 1)
	{
			if(m_Not[i] <= t && t <= m_Not[i + 1])
			{
				return 1;
			}
			return 0;
	}

	float temp1 = SplnMetaFnc(i,j - 1,t);
	float temp2 = SplnMetaFnc(i + 1,j - 1,t);
	return(
		(t - m_Not[i])/(m_Not[i + j - 1] - m_Not[i])*SplnMetaFnc(i,j - 1,t)
		+
		(m_Not[i + j] - t)/(m_Not[i + j] - m_Not[i + 1])*SplnMetaFnc(i + 1,j - 1,t)
		);
}

float CSplineCurve::GetSplnX(float t)
{
	float temp = 0;
	for(int i = 0;i < m_NodeNum;++i)
	{
		temp += SplnMetaFnc(i,4,t)*m_Node[i].x;
	}
	return temp;
}

float CSplineCurve::GetSplnY(float t)
{
	float temp = 0;
	for(int i = 0;i < m_NodeNum;++i)
	{
		temp += SplnMetaFnc(i,4,t)*m_Node[i].y;
	}
	return temp;
}



CSplineFace::CSplineFace()
{
	m_Node = NULL;
	m_NotX = m_NotY = NULL;
}

CSplineFace::~CSplineFace()
{
	for(int i = 0;i < m_NodeNum.x;i++)
	{
		delete[]  m_Node[i];
		m_Node[i] = NULL;
	}
	delete[]  m_Node;
	m_Node = NULL;

	if(m_NotX != NULL)
	delete[] m_NotX;

	if(m_NotY != NULL)
	delete[] m_NotY;
}

void CSplineFace::SetNode(int u, int v)
{
	m_NodeNum.x = u;
	m_NodeNum.y = v;
	m_Node = new D3DXVECTOR3 *[u];
	for(int i = 0;i < u;i++)
		m_Node[i] = new D3DXVECTOR3 [v];

	m_NotX = new int[u + 4];
	m_NotY = new int[v + 4];
}



float CSplineFace::GetSplnZ(float u,float v)
{
	float temp = 0;
	for(int i = 0;i < m_NodeNum.x;++i)
		for(int j = 0;j < m_NodeNum.y;++j)
		{
			temp += SplnMetaFnc(i,4,u,1)*SplnMetaFnc(j,4,v,0)*m_Node[i][j].z;
		}
	return temp;
}

float CSplineFace::SplnMetaFnc(int i,int j,float t,bool isX)
{
	int * m_Not = m_NotY;
	if(isX)
	{
		m_Not = m_NotX;		
	}
	if(j == 1)
	{
			if(m_Not[i] <= t && t <= m_Not[i + 1])
			{
				return 1;
			}
			return 0;
	}

	float temp1 = SplnMetaFnc(i,j - 1,t,isX);
	float temp2 = SplnMetaFnc(i + 1,j - 1,t,isX);
	return(
		(t - m_Not[i])/(m_Not[i + j - 1] - m_Not[i])*SplnMetaFnc(i,j - 1,t,isX)
		+
		(m_Not[i + j] - t)/(m_Not[i + j] - m_Not[i + 1])*SplnMetaFnc(i + 1,j - 1,t,isX)
		);
}