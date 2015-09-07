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
	float	m_Mass;//����
	D3DXVECTOR3 m_VecVelo;//���x�̕���
	D3DXVECTOR3 m_VecAccel;//�����x�̕���
	void CalVeloHit(CChara * pChar);
	CTexture * m_pTex;
	D3DXVECTOR3 m_Velo;//���x
	D3DXVECTOR3 m_Accel;//�����x
	void Move();
	void Acceleration();
	void Gravitation();

	float			m_FlmAngl;//�L�[�t���[���̑��x
	DWORD			m_NowTime;
	DWORD			m_BeforeTime;
	D3DXVECTOR3		m_Pos2;
	D3DXVECTOR3		m_Velo2;

	D3DXVECTOR3	*	GetJumpStand();
	void			SetJumpPort(D3DXVECTOR3 * pVec3);

	float	m_Grav;//�d�͒萔
};