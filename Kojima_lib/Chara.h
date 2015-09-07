#pragma once

#include "DirectInput.h"
#include "DrawObject.h"
#include "Texture.h"
#include "DebugFont.h"
#include "Figure.h"
#include "FPSManager.h"

#define FRICRATE 0.999f
#define GRAV 3.0f
#define DELTAVELO 0.00001f

class CChara
	: public CDrawObject
{
public:
	CChara();
	~CChara();

	void	SetpDirectInput(CDirectInput * pdinpt,int joypadpass){m_pDirectInput = pdinpt;m_JoyPadPass = joypadpass;}
	int		GetJoyPass(){return m_JoyPadPass;}

	void	SetVelo(float x, float y, float z);	//���x
	void	SetVelo(D3DXVECTOR3 * velo){m_Velo = *velo;}
	void	ReverseVelo(bool x,bool y,bool z);

	void	SetAccel(float x, float y, float z);	//�����x
	void	SetAccel(D3DXVECTOR3 *accel){m_Accel = *accel;}

	void	SetGrav(float x, float y, float z);	//�����x
	void	SetGrav(D3DXVECTOR3 *grav){m_Grav = *grav;}	

	void	ResetVelo(){m_Velo.x = m_Velo.y = m_Velo.z = 0.0f;};
	void	ResetAccel(){m_Accel.x = m_Accel.y = m_Accel.z = 0.0f;};
	void	ResetGrav(){m_Grav.x = m_Grav.y = m_Grav.z = 0.0f;};

	D3DXVECTOR3 * GetVelo(){return &m_Velo;}
	D3DXVECTOR3 * GetAccel(){return &m_Accel;}
	D3DXVECTOR3 * GetGrav(){return &m_Grav;}

	void	SetMass(float m){m_Mass = m;}
	float	GetMass(){return m_Mass;}

	void	SetFricrate(float fricrate){m_Fricrate = fricrate;}
	float	GetFricrate(){return m_Fricrate;}

	void	Move();					//�ړ�
	void	MoveCancel();			//�ړ��̃L�����Z��
	void	Acceleration();			//����
	void	VeloCalculation();		//���x�̓��o
	void	Gravitation();			//�d��
	void	Friction(float rate);
	void	SetEdgeGPos();			//���_�̈ʒu���Z�b�g

	void	AddVelo(float xtmp,float ytmp);
	void	MultiplyVelo(float xtmp,float ytmp);

	D3DXVECTOR2 *	GetEdgeGPos(int i){return &m_EdgeGPos[i];}
	D3DXVECTOR2 *	GetEdgeHitPos(int i){return &m_EdgeHitPos[i];}
	D3DXVECTOR2 *	GetEdgeHitGPos(int i){return &m_EdgeHitGPos[i];}
	D3DXVECTOR2 *	GetEdgeHitGPosMoved(int i){return &m_EdgeHitGPosMoved[i];}

protected:
 
	CDirectInput *	m_pDirectInput;

	int				m_JoyPadPass;

	D3DXVECTOR3 m_Velo;//���x
	D3DXVECTOR3 m_VecVelo;//���x�̕���
	D3DXVECTOR3	m_PotentialVelo;//�ێ����鑬�x
	D3DXVECTOR3 m_Accel;//�����x
	D3DXVECTOR3 m_VecAccel;//�����x�̕���
	D3DXVECTOR3	m_PotentialAccel;//�ێ���������x
	D3DXVECTOR2 m_EdgeGPos[4];//���_�̃O���[�o�����W
	D3DXVECTOR2 m_EdgeHitPos[4];//�����蔻��ł̒��_�̈ʒu
	D3DXVECTOR2 m_EdgeHitGPos[4];//�����蔻��ł̒��_�̃O���[�o�����W
	D3DXVECTOR2 m_EdgeHitGPosMoved[4];//�����蔻��ł̒��_�̃O���[�o�����W

	D3DXVECTOR3	m_Grav;//�d�͒萔

	float	m_Mass;//����
	float	m_Fricrate;//���C�萔
	float	m_Jumprate;//�W�����v�̐���萔
	float	m_deltaVelo;//�ɏ����x

	CFPSManager m_FPSManager;

public:

protected:
	CDebugFont *	m_pDebugFont;

public:

void SetEdgeHitPos(float Height,float Width)//�����蔻��̒��_�̈ʒu���Z�b�g
{
	m_EdgeHitPos[0].x = - GetScale()->x/2*Width;
	m_EdgeHitPos[0].y = GetScale()->y/2*Height;

	m_EdgeHitPos[1].x = GetScale()->x/2*Width;
	m_EdgeHitPos[1].y = GetScale()->y/2*Height;

	m_EdgeHitPos[2].x = GetScale()->x/2*Width;
	m_EdgeHitPos[2].y = - GetScale()->y/2*Height;

	m_EdgeHitPos[3].x = - GetScale()->x/2*Width;
	m_EdgeHitPos[3].y = - GetScale()->y/2*Height;
};
void SetEdgeHitGPos()//�����蔻��̒��_�̃O���[�o���ʒu���Z�b�g
{
	m_EdgeHitGPos[0].x = GetPos()->x + m_EdgeHitPos[0].x;
	m_EdgeHitGPos[0].y = GetPos()->y + m_EdgeHitPos[0].y;

	m_EdgeHitGPos[1].x = GetPos()->x + m_EdgeHitPos[1].x;
	m_EdgeHitGPos[1].y = GetPos()->y + m_EdgeHitPos[1].y;

	m_EdgeHitGPos[2].x = GetPos()->x + m_EdgeHitPos[2].x;
	m_EdgeHitGPos[2].y = GetPos()->y + m_EdgeHitPos[2].y;

	m_EdgeHitGPos[3].x = GetPos()->x + m_EdgeHitPos[3].x;
	m_EdgeHitGPos[3].y = GetPos()->y + m_EdgeHitPos[3].y;
};
void SetEdgeHitGPosMoved()//�����蔻��̒��_�̃O���[�o���ʒu���Z�b�g
{
	m_EdgeHitGPosMoved[0].x = GetPos()->x + m_EdgeHitPos[0].x;
	m_EdgeHitGPosMoved[0].y = GetPos()->y + m_EdgeHitPos[0].y;

	m_EdgeHitGPosMoved[1].x = GetPos()->x + m_EdgeHitPos[1].x;
	m_EdgeHitGPosMoved[1].y = GetPos()->y + m_EdgeHitPos[1].y;

	m_EdgeHitGPosMoved[2].x = GetPos()->x + m_EdgeHitPos[2].x;
	m_EdgeHitGPosMoved[2].y = GetPos()->y + m_EdgeHitPos[2].y;

	m_EdgeHitGPosMoved[3].x = GetPos()->x + m_EdgeHitPos[3].x;
	m_EdgeHitGPosMoved[3].y = GetPos()->y + m_EdgeHitPos[3].y;

	for(int i = 0;i < 4;++i)
	{
		m_EdgeHitGPosMoved[i].x += m_Velo.x;
		m_EdgeHitGPosMoved[i].y += m_Velo.y;
	}
};
};