#include "Common.h"
#include "Figure.h"

//----------------------------------------------------
//						CSimple2DEdge
//----------------------------------------------------
bool CSimple2DEdge::CheckCrossEdges(CSimple2DEdge *pEdge1, CSimple2DEdge *pEdge2)
{
	//x座標によるチェック
	if (pEdge1->m_Vertex[0].x >= pEdge1->m_Vertex[1].x)
	{
		if ((pEdge1->m_Vertex[0].x < pEdge2->m_Vertex[0].x && pEdge1->m_Vertex[0].x < pEdge2->m_Vertex[1].x)
		  ||(pEdge1->m_Vertex[1].x > pEdge2->m_Vertex[0].x && pEdge1->m_Vertex[1].x > pEdge2->m_Vertex[1].x))
			return false;
	}
	else
        if ((pEdge1->m_Vertex[1].x < pEdge2->m_Vertex[0].x && pEdge1->m_Vertex[1].x < pEdge2->m_Vertex[1].x) 
		  || (pEdge1->m_Vertex[0].x > pEdge2->m_Vertex[0].x && pEdge1->m_Vertex[0].x > pEdge2->m_Vertex[1].x))
			return false;
    //y座標によるチェック
    if (pEdge1->m_Vertex[0].y >= pEdge1->m_Vertex[1].y)
	{
        if ((pEdge1->m_Vertex[0].y < pEdge2->m_Vertex[0].y && pEdge1->m_Vertex[0].y < pEdge2->m_Vertex[1].y) 
		 || (pEdge1->m_Vertex[1].y > pEdge2->m_Vertex[0].y && pEdge1->m_Vertex[1].y > pEdge2->m_Vertex[1].y))
			return false;
	}	
	else
        if ((pEdge1->m_Vertex[1].y < pEdge2->m_Vertex[0].y && pEdge1->m_Vertex[1].y < pEdge2->m_Vertex[1].y) 
		 || (pEdge1->m_Vertex[0].y > pEdge2->m_Vertex[0].y && pEdge1->m_Vertex[0].y > pEdge2->m_Vertex[1].y))
			return false;

    if (((pEdge1->m_Vertex[0].x - pEdge1->m_Vertex[1].x) * (pEdge2->m_Vertex[0].y - pEdge1->m_Vertex[0].y) + (pEdge1->m_Vertex[0].y - pEdge1->m_Vertex[1].y) * (pEdge1->m_Vertex[0].x - pEdge2->m_Vertex[0].x)) *
        ((pEdge1->m_Vertex[0].x - pEdge1->m_Vertex[1].x) * (pEdge2->m_Vertex[1].y - pEdge1->m_Vertex[0].y) + (pEdge1->m_Vertex[0].y - pEdge1->m_Vertex[1].y) * (pEdge1->m_Vertex[0].x - pEdge2->m_Vertex[1].x)) > 0)
        	return false;

    if (((pEdge2->m_Vertex[0].x - pEdge2->m_Vertex[1].x) * (pEdge1->m_Vertex[0].y - pEdge2->m_Vertex[0].y) + (pEdge2->m_Vertex[0].y - pEdge2->m_Vertex[1].y) * (pEdge2->m_Vertex[0].x - pEdge1->m_Vertex[0].x)) *
        ((pEdge2->m_Vertex[0].x - pEdge2->m_Vertex[1].x) * (pEdge1->m_Vertex[1].y - pEdge2->m_Vertex[0].y) + (pEdge2->m_Vertex[0].y - pEdge2->m_Vertex[1].y) * (pEdge2->m_Vertex[0].x - pEdge1->m_Vertex[1].x)) > 0)
			return false;


	return true;
}

//---------------------------------------------------------------
//						２つの線分の交点を求める
//---------------------------------------------------------------
D3DXVECTOR2 * CSimple2DEdge::GetIntersectionPoint(CSimple2DEdge * pEdge1,CSimple2DEdge * pEdge2)
{
	D3DXVECTOR2 * pVec;
	D3DXVECTOR2 Vec1,Vec2,Vec3;
	D3DXVec2Subtract(&Vec1,&pEdge1->m_Vertex[1],&pEdge1->m_Vertex[0]);
	D3DXVec2Subtract(&Vec2,&pEdge2->m_Vertex[0],&pEdge1->m_Vertex[0]);
	D3DXVec2Subtract(&Vec3,&pEdge2->m_Vertex[1],&pEdge2->m_Vertex[0]);
	float cross1 = Vec3.x*Vec1.y - Vec3.y*Vec1.x;
	float cross2 = Vec3.x*Vec2.y - Vec3.y*Vec2.x;

	Vec1.x = Vec1.x*cross2/cross1 + pEdge1->m_Vertex[0].x;
	Vec1.y = Vec1.y*cross2/cross1 + pEdge1->m_Vertex[0].y;
	pVec = &Vec1;
	return pVec;
}

//----------------------------------------------------
//						CSimpleTriangle
//----------------------------------------------------
float CSimpleTriangle::GetPointHeight(float x,float z)
{
	D3DXVECTOR3 pV1 = D3DXVECTOR3(m_Vertex[0].x - m_Vertex[1].x,m_Vertex[0].y - m_Vertex[1].y,m_Vertex[0].z - m_Vertex[1].z);
	D3DXVECTOR3 pV2 = D3DXVECTOR3(m_Vertex[1].x - m_Vertex[2].x,m_Vertex[1].y - m_Vertex[2].y,m_Vertex[1].z - m_Vertex[2].z);
	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal,&pV1,&pV2);

	if(normal.y < 0)
	{
		normal.x = -normal.x;
		normal.y = -normal.y;
		normal.z = -normal.z;
	}

	return(m_Vertex[0].y - 1/normal.y*(normal.x*(x - m_Vertex[0].x) + normal.z*(z - m_Vertex[0].z) ) );
}



void D3DXVECTORROTATE(D3DXVECTOR3 * vec,D3DXVECTOR3 * rot)
{
	float x,y,z;

	x = vec->x;
	y = vec->y;
	vec->x = (float)(x*cos(rot->z/180.0f*D3DX_PI)-y*sin(rot->z/180*D3DX_PI));
    vec->y = (float)(x*sin(rot->z/180.0f*D3DX_PI)+y*cos(rot->z/180*D3DX_PI));

	y = vec->y;
	z = vec->z;
	vec->y = (float)(y*cos(rot->x/180.0f*D3DX_PI)-z*sin(rot->x/180*D3DX_PI));
    vec->z = (float)(y*sin(rot->x/180.0f*D3DX_PI)+z*cos(rot->x/180*D3DX_PI));

	x = vec->x;
	z = vec->z;
	vec->z = (float)(z*cos(rot->y/180.0f*D3DX_PI)-x*sin(rot->y/180*D3DX_PI));
    vec->x = (float)(z*sin(rot->y/180.0f*D3DX_PI)+x*cos(rot->y/180*D3DX_PI));
}

static int GetSignedArea(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3)
{
	return (int)((p2.x - p1.x) * (p3.z - p1.z) 
		 - (p3.x - p1.x) * (p2.z - p1.z));
}

bool CheckContained(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR3 p)
{
    int a = GetSignedArea(p1, p2, p3);
    if(a == 0) return false;//3点が直線に並んでる

	if(CheckPointonEdge(p1,p2,p)||CheckPointonEdge(p2,p3,p)||CheckPointonEdge(p1,p3,p))
		return true;

	if(
		(GetSignedArea(p1, p2, p) >= 0&&GetSignedArea(p2, p3, p) >= 0&&GetSignedArea(p3, p1, p) >= 0)
		||
		(GetSignedArea(p1, p2, p) <= 0&&GetSignedArea(p2, p3, p) <= 0&&GetSignedArea(p3, p1, p) <= 0)
		)
		return true;
	return false;
}

bool CheckPointonEdge(D3DXVECTOR3 p1, D3DXVECTOR3 p2,D3DXVECTOR3 p)
{
	if(p1.x - p2.x < 0.00001f&&p1.x - p2.x > -0.00001f
 	&& p1.x - p.x > 0.00001f&&p1.x - p.x < -0.00001f)
		return true;

	if((p1.y - p.y)/(p1.x - p.x) - (p1.y - p2.y)/(p1.x - p2.x) > -0.00001f
	&& (p1.y - p.y)/(p1.x - p.x) - (p1.y - p2.y)/(p1.x - p2.x) < 0.00001f )
	return true;

	return false;
}