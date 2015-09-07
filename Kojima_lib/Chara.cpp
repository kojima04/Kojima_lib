#include "Common.h"
#include "Chara.h"

CChara::CChara()
{
	ResetPos();
	ResetRot();
	ResetVelo();
	ResetAccel();
	ResetGrav();

	m_VecVelo = D3DXVECTOR3(0.0f,0.0f,0.0f);

	timeBeginPeriod(1); 
	//m_NowTime = m_BeforeTime = timeGetTime();
	timeEndPeriod(1);

#ifdef DEBUG
	m_pDebugFont = new CDebugFont;
	m_pDebugFont->SetUp();
#endif

}
CChara::~CChara()
{
#ifdef DEBUG
	SAFE_DELETE(m_pDebugFont);
#endif
}

void CChara::SetVelo(float x, float y, float z)
{
	m_Velo.x = x;
	m_Velo.y = y;
	m_Velo.z = z;
}


void CChara::SetAccel(float x, float y, float z)
{
	m_Accel.x = x;
	m_Accel.y = y;
	m_Accel.z = z;
}

void CChara::SetGrav(float x, float y, float z)
{
	m_Grav.x = x;
	m_Grav.y = y;
	m_Grav.z = z;
}

void CChara::ReverseVelo(bool x,bool y,bool z)
{
	if(x)
		m_Velo.x = -m_Velo.x;
	if(y)
		m_Velo.y = -m_Velo.y;
	if(z)
		m_Velo.z = -m_Velo.z;
}

void CChara::Move()
{
	m_Pos.x += m_Velo.x;
	m_Pos.y += m_Velo.y;
	m_Pos.z += m_Velo.z;
	if(CFPSManager::GetFlmAngl() >= 0.01f)
	{
		m_Velo = m_Velo/(float)CFPSManager::GetFlmAngl();
		m_Accel = m_Accel/(float)CFPSManager::GetFlmAngl();
		m_Velo -= m_Accel/m_Mass*(1.0f - (float)CFPSManager::GetFlmAngl());
	}
	else
	{
		m_Velo = m_VecVelo;
		m_Accel = m_VecAccel;
	}
}

void CChara::MoveCancel()
{
	m_Pos.x -= m_Velo.x;
	m_Pos.y -= m_Velo.y;
	m_Pos.z -= m_Velo.z;
}


void CChara::Acceleration()
{
	if(CFPSManager::GetFlmAngl() >= 0.01f)
	{
	}
	else
	{
		m_VecVelo = m_Velo;
		m_VecAccel = m_Accel;
		m_Accel = m_Velo = D3DXVECTOR3(0,0,0);
		return;
	}
	
	if(m_Velo.x != 0)
	{
		int i = 0;
	}
	Gravitation();
	m_Velo *= (float)CFPSManager::GetFlmAngl();
	m_Accel *= (float)CFPSManager::GetFlmAngl();
	m_Velo += m_Accel/m_Mass;
}

void CChara::Gravitation()
{
	m_Accel += m_Grav;
}

void CChara::AddVelo(float xtmp,float ytmp)
{
	m_Velo.x += (float)(xtmp*CFPSManager::GetFlmAngl());
	m_Velo.y += (float)(ytmp*CFPSManager::GetFlmAngl());
}

void CChara::MultiplyVelo(float xtmp,float ytmp)
{
	float x = xtmp;
	if(x < 0)
		x = -x;
	float y = ytmp;
	if(y < 0)
		y = -y;

	m_Velo.x *= (float)pow((double)x,CFPSManager::GetFlmAngl());
	m_Velo.y *= (float)pow((double)y,CFPSManager::GetFlmAngl());

	if(xtmp < 0)
		m_Velo.x = -m_Velo.x;

	if(ytmp < 0)
		m_Velo.y = -m_Velo.y;
}

void CChara::Friction(float rate)
{
		m_Velo *= (float)pow((double)rate,CFPSManager::GetFlmAngl());

		//if(m_Velo.y*CFPSManager::GetFlmAngl() + m_Accel.y/m_Mass < m_deltaVelo && m_Velo.y*CFPSManager::GetFlmAngl() + m_Accel.y/m_Mass > -m_deltaVelo)
		//	m_Velo.y = 0;
		//if(m_Velo.x*CFPSManager::GetFlmAngl() + m_Accel.x/m_Mass < m_deltaVelo && m_Velo.x*CFPSManager::GetFlmAngl() + m_Accel.x/m_Mass > -m_deltaVelo)
		//	m_Velo.x = 0;
}

//--------------------------------------------------------------
//				’¸“_‚ÌˆÊ’u
//--------------------------------------------------------------
void CChara::SetEdgeGPos()
{
	m_EdgeGPos[0].x = GetPos()->x - GetScale()->x/2;
	m_EdgeGPos[0].y = GetPos()->y + GetScale()->y/2;

	m_EdgeGPos[1].x = GetPos()->x + GetScale()->x/2;
	m_EdgeGPos[1].y = GetPos()->y + GetScale()->y/2;

	m_EdgeGPos[2].x = GetPos()->x + GetScale()->x/2;
	m_EdgeGPos[2].y = GetPos()->y - GetScale()->y/2;

	m_EdgeGPos[3].x = GetPos()->x - GetScale()->x/2;
	m_EdgeGPos[3].y = GetPos()->y - GetScale()->y/2;
}

