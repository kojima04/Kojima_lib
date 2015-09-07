#pragma once

class CRegressEquation3
{
public:
	CRegressEquation3();
	~CRegressEquation3();
	void SetUpNode(int index);
	void SetNodePara(int index,D3DXVECTOR3 * vec3)
	{
		m_Node[index] = *vec3;
	}
	void SetRegressEquation();

	float B0,B1,B2;

	D3DXVECTOR3 * m_Node;
	//D3DXVECTOR3 m_Node[10];
	int m_NodeNum;

	float GetRegressSolution(float x,float y);
	float SumPrdctDeviation(int i,int j);
	float SumSqrDeviation(int i);

	float m_VL_SumPrdctDv[3][3];
	float m_VL_SumSqrDv[3];
	float m_VL_Ave[3];
	float m_VL_Sum[3];
	float m_VL_SumSqr[3];
	float m_VL_SumPrdct[3][3];

	void SetUpVL();
};