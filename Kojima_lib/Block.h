#include "DrawObject.h"
#include "Chara.h"
#include "Texture.h"

class CBlock 
	: public CDrawObject
{
public:
	CBlock(){};
	~CBlock();
	void Draw();

	int BlockKind;

	void SetUp();
	float	m_Mass;//質量
	D3DXVECTOR3 m_VecVelo;//速度の方向
	D3DXVECTOR3 m_VecAccel;//加速度の方向
	void CalVeloHit(CChara * pChar);
	CTexture * m_pTex;
	D3DXVECTOR3 m_Velo;//速度
	D3DXVECTOR3 m_Accel;//加速度
	void Move();
	void Acceleration();
	void Gravitation();

	float			m_FlmAngl;//キーフレームの速度
	DWORD			m_NowTime;
	DWORD			m_BeforeTime;
	D3DXVECTOR3		m_Pos2;
	D3DXVECTOR3		m_Velo2;

	D3DXVECTOR3	*	GetJumpStand();
	void			SetJumpPort(D3DXVECTOR3 * pVec3);

	float	m_Grav;//重力定数
};