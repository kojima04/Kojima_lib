#include "Common.h"
#include "Block.h"

void CBlock::SetUp()
{
	m_pTex = new CTexture();
	m_pTex->Create();
	m_pTex->SetpTexturefromFile("Picture/Ground/Grass.bmp");
	CBlock::ResetRot();
	CBlock::ResetPos();

	m_Accel = m_Velo = m_VecVelo = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Grav = 0;
	m_Mass = 100;
	BlockKind = 0;
	SetScale(1000,200,0);
}

CBlock::~CBlock()
{
	SAFE_DELETE(m_pTex);
}
void CBlock::Draw()
{
	D3DXMatrixIdentity(&m_World);
	//Transform();
	//TransformStatic();
	TransformBillBoard();

	m_pTex->Draw(&m_World);
}


void CBlock::Acceleration()
{
	m_NowTime = timeGetTime();
	m_FlmAngl = (float)(m_NowTime - m_BeforeTime);
	m_BeforeTime = m_NowTime;
	if(m_FlmAngl != 0)
	{
		m_FlmAngl *= (float)60/100;
	}
	else
	{
		m_VecVelo = m_Velo;
		m_VecAccel = m_Accel;
		m_Accel = m_Velo = D3DXVECTOR3(0,0,0);


		return;
	}
	
	//m_Velo = m_PotentialVelo*m_FlmAngl;

	//m_PotentialVelo.x += m_Accel.x/m_Mass*m_FlmAngl;
	//m_PotentialVelo.y += m_Accel.y/m_Mass*m_FlmAngl;
	//m_PotentialVelo.z += m_Accel.z/m_Mass*m_FlmAngl;

	Gravitation();
	m_Velo += m_Accel/m_Mass;
	m_Velo *= m_FlmAngl;
	m_Accel *= m_FlmAngl;
}

void CBlock::Move()
{
	m_Pos.x += m_Velo.x;
	m_Pos.y += m_Velo.y;
	m_Pos.z += m_Velo.z;
	if(m_FlmAngl != 0)
	{
		m_Velo = m_Velo/m_FlmAngl;
		m_Accel = m_Accel/m_FlmAngl;
	}
	else
	{
		m_Velo = m_VecVelo;
		m_Accel = m_VecAccel;
	}
}

void CBlock::Gravitation()
{
	m_Accel.y -= m_Grav*100;
}

void CBlock::CalVeloHit(CChara *pChar)
{
	if(pChar->GetEdgeHitGPos(1)->y < 0)
		int p = 0;
	if(
		( pChar->GetEdgeHitGPosMoved(1)->x > GetPos()->x - GetScale()->x/2
		&&pChar->GetEdgeHitGPosMoved(0)->x < GetPos()->x - GetScale()->x/2)
		||( pChar->GetEdgeHitGPosMoved(1)->x > GetPos()->x + GetScale()->x/2
		&&pChar->GetEdgeHitGPosMoved(0)->x < GetPos()->x + GetScale()->x/2)
		||( pChar->GetEdgeHitGPosMoved(1)->x < GetPos()->x + GetScale()->x/2
		&&pChar->GetEdgeHitGPosMoved(0)->x > GetPos()->x - GetScale()->x/2)
		)
	{
		if(pChar->GetEdgeHitGPos(2)->y > GetPos()->y + GetScale()->y/2 + m_Velo.y
		&& pChar->GetEdgeHitGPosMoved(2)->y < GetPos()->y + GetScale()->y/2 + m_Velo.y)
		{
			pChar->SetVelo(pChar->GetVelo()->x,0,0);
			//m_Grav = 0.1;
		}
	}
}

D3DXVECTOR3 * CBlock::GetJumpStand()
{
	m_Pos2.y = m_Pos.y + m_Scale.y/2 + m_Velo.y;
	m_Pos2.x = m_Pos.y + (float)(rand()%((int)m_Scale.y/2*100))/100 + m_Velo.x;
	m_Pos2.z = 0;
	return &m_Pos2;
}

void	CBlock::SetJumpPort(D3DXVECTOR3 * pVec3)
{
	
}