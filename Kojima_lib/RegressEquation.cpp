#include "Common.h"
#include "RegressEquation.h"

CRegressEquation3::CRegressEquation3()
{
	m_Node = NULL;
}

CRegressEquation3::~CRegressEquation3()
{
	delete[]  m_Node;
	m_Node = NULL;
}

void CRegressEquation3::SetUpNode(int index)
{
	m_NodeNum = index;	
	m_Node = new D3DXVECTOR3[index];
}

float CRegressEquation3::GetRegressSolution(float x,float y)
{
	return (B0 + B1*x + B2*y);
}

void CRegressEquation3::SetRegressEquation()
{
	float temp = m_VL_SumSqrDv[0]*m_VL_SumSqrDv[1] - sqr(m_VL_SumPrdctDv[0][1]);
	B1 = ( m_VL_SumSqrDv[1]*m_VL_SumPrdctDv[0][2] - m_VL_SumPrdctDv[0][1]*m_VL_SumPrdctDv[1][2])/temp;
	B2 = (-m_VL_SumPrdctDv[0][1]*m_VL_SumPrdctDv[0][2] + m_VL_SumSqrDv[0]*m_VL_SumPrdctDv[1][2])/temp;
	B0 = m_VL_Ave[2] - B1*m_VL_Ave[0] - B2*m_VL_Ave[1];
}

void CRegressEquation3::SetUpVL()
{
	float ftemp[6] = {0};
	for(int i = 0;i < m_NodeNum;++i)
	{
		ftemp[0] += m_Node[i].x;
		ftemp[1] += m_Node[i].y;
		ftemp[2] += m_Node[i].z;
		ftemp[3] += sqr(m_Node[i].x);
		ftemp[4] += sqr(m_Node[i].y);
		ftemp[5] += sqr(m_Node[i].z);
	}
	for(int i = 0;i < 3;++i)
	{
		m_VL_Sum[i] = ftemp[i];
		m_VL_Ave[i] = ftemp[i]/m_NodeNum;
		m_VL_SumSqr[i] = ftemp[i + 3];
	}
	
	for(int i = 0;i < 3;++i)
	{
		m_VL_SumSqrDv[i] = m_VL_SumSqr[i] - sqr(m_VL_Sum[i])/m_NodeNum;
	}

	for(int i = 0;i < 6;++i)
		ftemp[i] = 0;

	for(int i = 0;i < m_NodeNum;++i)
	{
		ftemp[0] += m_Node[i].x * m_Node[i].y;
		ftemp[1] += m_Node[i].y * m_Node[i].z;
		ftemp[2] += m_Node[i].z * m_Node[i].x;
	}

	m_VL_SumPrdct[0][1] = ftemp[0];
	m_VL_SumPrdct[1][2] = ftemp[1];
	m_VL_SumPrdct[0][2] = ftemp[2];

	for(int i = 0;i < 3;++i)
		for(int j = 0;j < 3;++j)
		{
			m_VL_SumPrdctDv[i][j] = m_VL_SumPrdct[i][j] - m_VL_Sum[i]*m_VL_Sum[j]/m_NodeNum;
		}

	for(int i = 0;i < 3;++i)
	{
		m_VL_SumPrdctDv[i][i] = m_VL_SumSqrDv[i];
	}

	m_VL_SumPrdctDv[1][0] = m_VL_SumPrdctDv[0][1];
	m_VL_SumPrdctDv[2][1] = m_VL_SumPrdctDv[1][2];
	m_VL_SumPrdctDv[2][0] = m_VL_SumPrdctDv[0][2];

}