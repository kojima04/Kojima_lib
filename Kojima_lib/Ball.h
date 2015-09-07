#pragma once
#include "Model.h"
#include "Figure.h"

class CBall : public CModel
{
public:
	CBall();
	~CBall();

	void Reset();

	void SetVelo(float x, float y, float z);	//速度
	void SetVelo(D3DXVECTOR3 *velo){m_Velo = *velo;}

	void SetAccel(float x, float y, float z);	//加速度
	void SetAccel(D3DXVECTOR3 *accel){m_Accel = *accel;}

	void SetVeloRimitMax(float vr){m_VeloRimitMax = vr;}
	float GetVeloRimitMax(){return m_VeloRimitMax;}

	void SetVeloRimitMin(float vr){m_VeloRimitMin = vr;}
	float GetVeloRimitMin(){return m_VeloRimitMin;}

	D3DXVECTOR3	* GetVelo(){return &m_Velo;}
	D3DXVECTOR3 * GetAccel(){return &m_Accel;}

	void SetMass(float m){m_Mass = m;}

	float GetMass(){return m_Mass;}

	void SetCenter(float x, float y, float z);
	void SetCenter(D3DXVECTOR3 *center){m_Sphere.m_Center = *center;}
	void SetHalfAcross(float half){m_Sphere.m_HalfAcross = half;}
	D3DXVECTOR3	* GetCenter(){return &m_Sphere.m_Center;}

	CSimpleSphere * GetSphere(){return &m_Sphere;}


	bool m_isHited;
	void Move();
	void MoveCancel();
	void Acceleration();
	bool CalcSpherePlaneCollision(CSimpleSphere * sphr,CSimpleTriangle * tri,D3DXVECTOR3 * velo,D3DXVECTOR3 * collipos);
	bool CalcSphereTriangleCollision(CSimpleSphere * sphr,CSimpleTriangle * tri,D3DXVECTOR3 * velo);

	bool CalcGoalTexCollision(CSimpleSphere * sphr,CSimpleSquare * sqar,D3DXVECTOR3 * velo);
	bool CalcGoalTexRoling(D3DXVECTOR3 * vgoal,D3DXVECTOR3 * velo);
	
	bool CalcRoolingVelo(CSimpleTriangle *tri,float grav,D3DXVECTOR3 *velo,CSimpleSphere * sphr);
	float m_RolingNextHeight;

	void CalcReflect(CSimpleTriangle * tri,D3DXVECTOR3 * velo,bool isGreen);
	bool GetisPutter(){return m_isPutter;}
	void SetisPutter(bool t){m_isPutter = t;}
	bool GetisRef(){return m_isReflected;}
	void SetisRef(bool t){m_isReflected = t;}

protected:
	
	CSimpleSphere m_Sphere;
	float m_VeloRimitMax;
	float m_VeloRimitMin;
	bool m_isPutter;
	bool m_isReflected;
	D3DXVECTOR3 m_Velo;//速度
	D3DXVECTOR3 m_Accel;//加速度
	float m_Mass;//質量
};