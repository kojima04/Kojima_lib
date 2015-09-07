#pragma once

class CSplineCurve
{
public:
	CSplineCurve();
	~CSplineCurve();
	D3DXVECTOR2 * m_Node;
	int * m_Not;
	int m_NodeNum;
	void SetUp(int i);
	float SplnMetaFnc(int i,int j,float t);
	float GetSplnX(float);
	float GetSplnY(float);
};

class CSplineFace
{
public:
	CSplineFace();
	~CSplineFace();
	
	D3DXVECTOR3 ** m_Node;
	int * m_NotX;
	int * m_NotY;

	POINT m_NodeNum;
	void SetNode(int u,int v);
	float SplnMetaFnc(int i,int j,float t,bool isX4);
	float GetSplnZ(float u,float v);
};