#pragma once

class CSimple2DEdge
{
public:
	CSimple2DEdge(){};
	~CSimple2DEdge(){};
	D3DXVECTOR2 m_Vertex[2];
	void SetVertex(D3DXVECTOR2 V1,D3DXVECTOR2 V2){m_Vertex[0] = V1;m_Vertex[1] = V2;}
	static bool CheckCrossEdges(CSimple2DEdge * pEdge1,CSimple2DEdge * pEdge2);
	static D3DXVECTOR2 * GetIntersectionPoint(CSimple2DEdge * pEdge1,CSimple2DEdge * pEdge2);
	static D3DXVECTOR2 * GetVector(CSimple2DEdge * pEdge1,CSimple2DEdge * pEdge2);	
	static D3DXVECTOR2 * GetPalarelVector(D3DXVECTOR2 * pVec2,CSimple2DEdge * pEdge);
};

class CSimpleTriangle
{
public:
	CSimpleTriangle(){};
	~CSimpleTriangle(){};
	D3DXVECTOR3 m_Vertex[3];
	float GetPointHeight(float x,float y);
};

class CSimpleSquare
{
public:
	CSimpleSquare(){};
	~CSimpleSquare(){};
	CSimpleTriangle m_TriangleVertex[2];
};

class CSimpleSphere
{
public:
	CSimpleSphere(){};
	~CSimpleSphere(){};
	D3DXVECTOR3 m_Center;
	float m_HalfAcross;//”¼Œa
};

void D3DXVECTORROTATE(D3DXVECTOR3 * vec,D3DXVECTOR3 * rot);
static int GetSignedArea(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3);

bool CheckContained(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR3 p);

bool CheckPointonEdge(D3DXVECTOR3 p1, D3DXVECTOR3 p2,D3DXVECTOR3 p);



