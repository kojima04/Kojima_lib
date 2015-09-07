#pragma once

#include "DXObject.h"
#include "Figure.h"

#define CAMERABASE_Z -724

class CCamera : public CDXObject
{
public:
	CCamera();
	~CCamera();

	void	Transform();

	void	SetFovY	(const float f){m_NtoF = f;}
	void	SetZn	(const float f){m_Nzone = f;}
	void	SetZf	(const float f){m_Fzone = f;}

	float	GetFovY()	const{return m_NtoF;}
	float	GetZn()		const{return m_Nzone;}
	float	GetZf()		const{return m_Fzone;}
	
	void	SetPos(float x, float y, float z);
	void	SetFacus(float x, float y, float z);
	void	SetHead(float x, float y, float z);

	void	SetPos(D3DXVECTOR3 *p){m_Pos = *p;}
	void	SetFacus(D3DXVECTOR3 *p){m_Facus = *p;}
	void	SetHead(D3DXVECTOR3 *p){m_Head = *p;}

	D3DXVECTOR3 * GetPos(){return &m_Pos;}
	D3DXVECTOR3 * GetFacus(){return &m_Facus;}
	D3DXVECTOR3 * GetHead(){return &m_Head;}

	D3DXMATRIX * GetView(){return &m_View;}
	D3DXMATRIX * GetProj(){return &m_Proj;}
	D3DXMATRIX * GetScreen(){return &m_Screen;}

	D3DXVECTOR3 m_Rot;

protected:

	D3DXVECTOR3	m_Pos;		
	D3DXVECTOR3	m_Facus;
	D3DXVECTOR3	m_Head;		

	D3DXMATRIX	m_Proj;		//	射影行列
	D3DXMATRIX	m_View;		//	ビュー行列
	D3DXMATRIX	m_Screen;	//	スクリーン行列

	float	m_NtoF;		//	視野
	float	m_Nzone;	//	ニアプレーン
	float	m_Fzone;	//	ファープレーン

};

class CMoveCamera : public CCamera
{
public:
	CMoveCamera();
	~CMoveCamera();
	D3DXVECTOR3 m_Gaze;
	//D3DXVECTOR3 m_Rot;
	float m_TryPod;
	void SetRotation();
	void SetRotation2();

	void SetRot(D3DXVECTOR3 *rot){m_Rot = *rot;};
	void SetRot(float x, float y, float z);
	void SetTryPod(float f){m_TryPod = f;};
	void SetGaze(D3DXVECTOR3 *g){m_Gaze = *g;};
	void SetGaze(float x,float y,float z);
	D3DXVECTOR3 * GetGaze(){return &m_Gaze;}
	void MoveFront(float d);
	void MoveFront2(float d);
	void MoveSide(float d);
	void MoveSide2(float d);
	D3DXVECTOR3 m_ZAxis;
	D3DXVECTOR3 m_YAxis;
	D3DXVECTOR3 m_XAxis;

	D3DXVECTOR3 * GetRot(){ return &m_Rot;};
};

class CSpringCamera : public CCamera
{
public:
	CSpringCamera();
	~CSpringCamera();
	
	D3DXVECTOR3 m_ExFocus;

	void SetBaseDist(float d){m_BaseDist = d;}
	void SetSprngConst(float d){m_SprngConst = d;}
	float m_BaseDist;
	float m_SprngConst;
	void Move();
};
