#include "Common.h"
#include "Camera.h"

CCamera::CCamera()
{
	m_Pos	= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Facus= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Head	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_NtoF = 45.0f;
	m_Nzone = 1.00f;
	m_Fzone = 10000.0f;
}


CCamera::~CCamera()
{
}

void CCamera::Transform()
{
	m_stpCameraPos = &m_Pos;
	m_stpCameraFocs = &m_Facus;

	D3DXMatrixLookAtLH(&m_View,&m_Pos,&m_Facus,&m_Head);

	/*D3DXMATRIX tmp;

	D3DXMatrixRotationYawPitchRoll(&tmp, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_View,&m_View,&tmp);*/

	float aspect = (float)GetD3DPP()->BackBufferWidth/(float)GetD3DPP()->BackBufferHeight;

	//float aspect = (float)GetD3DPP()->BackBufferHeight/(float)GetD3DPP()->BackBufferWidth;

	D3DXMatrixIdentity(&m_Proj);

	D3DXMatrixPerspectiveFovLH(&m_Proj, D3DXToRadian(m_NtoF), aspect, m_Nzone, m_Fzone);

	GetDevice()->SetTransform(D3DTS_PROJECTION,&m_Proj);

	GetDevice()->SetTransform(D3DTS_VIEW,&m_View);

	SetTransformProj(&m_Proj);

	SetTransformView(&m_View);

}

void CCamera::SetPos(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}

void CCamera::SetFacus(float x, float y, float z)
{
	m_Facus.x = x;
	m_Facus.y = y;
	m_Facus.z = z;
}

void CCamera::SetHead(float x, float y, float z)
{
	m_Head.x = x;
	m_Head.y = y;
	m_Head.z = z;
}



CMoveCamera::CMoveCamera()
{
	m_Gaze = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TryPod = 0;
	SetZf(10000);
	m_YAxis = *GetHead();
	m_ZAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_XAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

CMoveCamera::~CMoveCamera()
{
	
}

void CMoveCamera::SetRotation()
{

	D3DXMATRIX tmp;
	m_ZAxis = m_Gaze;

	D3DXMatrixRotationAxis(&tmp,&m_YAxis,m_Rot.y);
	D3DXVec3TransformCoord(&m_Gaze,&m_Gaze,&tmp);

	D3DXVec3Cross(&m_XAxis,&m_YAxis,&m_ZAxis);
	D3DXMatrixRotationAxis(&tmp,&m_XAxis,m_Rot.x);
	D3DXVec3TransformCoord(&m_Gaze,&m_Gaze,&tmp);

	D3DXVECTOR3 tmpX = m_XAxis,tmpY = m_YAxis,tmpZ = m_ZAxis;

	//D3DXMatrixRotationAxis(&tmp,&tmpX,m_Rot.x);
	//D3DXVec3TransformCoord(&m_YAxis,&m_YAxis,&tmp);

	//D3DXMatrixRotationAxis(&tmp,&tmpZ,m_Rot.z);
	//D3DXVec3TransformCoord(&m_YAxis,&m_YAxis,&tmp);

	//SetRot(0,0,0);

	//D3DXVECTORROTATE(&v,&m_Rot);
}
void CMoveCamera::SetRotation2()
{
	//D3DXVECTORROTATE(&m_Gaze,&m_Rot);
}

void CMoveCamera::SetGaze(float x, float y, float z)
{
	m_Gaze.x = x;
	m_Gaze.y = y;
	m_Gaze.z = z;
}

void CMoveCamera::SetRot(float x, float y, float z)
{
	if(x > 180)
	x = x - 180*2;
	else
		if(x < -180)
			x = x + 180*2;

	if(y > 180)
		y = y - 180*2;
	else
		if(y < -180)
			y = y + 180*2;

	if(z > 180)
		z = z - 180*2;
	else
		if(z < -180)
			z = z + 180*2;
	m_Rot.x = x;
	m_Rot.y = y;
	m_Rot.z = z;
}

void CMoveCamera::MoveFront(float d)
{
	D3DXVECTOR3 v;
	v = m_Gaze;

	D3DXVec3Normalize(&v,&v);
	v.x *= d;
	v.y *= 0;
	v.z *= d;
	D3DXVec3Add(&m_Pos,&m_Pos,&v); 
}	

void CMoveCamera::MoveFront2(float d)
{
	D3DXVECTOR3 v;
	v = m_Gaze;

	D3DXVec3Normalize(&v,&v);
	v.x *= d;
	v.y *= 0;
	v.z *= d;
	D3DXVec3Add(&m_Pos,&m_Pos,&v); 
}	

void CMoveCamera::MoveSide2(float d)
{
	D3DXVECTOR3 v;
	v = m_Gaze;

	D3DXVec3Normalize(&v,&v);
	D3DXVec3Cross(&v,&m_Head,&v);
	D3DXVec3Normalize(&v,&v);
	v.x *= d;
	v.y *= d;
	v.z *= d;
	D3DXVec3Add(&m_Pos,&m_Pos,&v); 
}

void CMoveCamera::MoveSide(float d)
{
	D3DXVECTOR3 v;
	v = m_Gaze;

	//D3DXVECTORROTATE(&v,&m_Rot);
	D3DXVec3Normalize(&v,&v);
	D3DXVec3Cross(&v,&m_Head,&v);
	D3DXVec3Normalize(&v,&v);
	v.x *= d;
	v.y *= 0;
	v.z *= d;
	D3DXVec3Add(&m_Pos,&m_Pos,&v); 
}

CSpringCamera::CSpringCamera()
{
	m_SprngConst = 0.05f;
	m_BaseDist = 20;
}

CSpringCamera::~CSpringCamera()
{
	
}

void CSpringCamera::Move()
{
	D3DXVECTOR3 v1 = m_Facus - m_Pos;	
	float d = D3DXVec3Length(&v1);
	D3DXVec3Normalize(&v1,&v1);

	v1.x *= m_SprngConst*(d - m_BaseDist);
	v1.y *= m_SprngConst*(d - m_BaseDist);
	v1.z *= m_SprngConst*(d - m_BaseDist);

	m_Pos += v1;
}


