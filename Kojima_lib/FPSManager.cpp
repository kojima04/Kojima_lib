#include "Common.h"
#include "FPSManager.h"

double	CFPSManager::m_stFlmAngl;//�L�[�t���[���̑��x
double	CFPSManager::m_stNowTime;
double	CFPSManager::m_stBeforeTime;
double	CFPSManager::m_stBeforeFlame;

CFPSManager::CFPSManager()
{
	m_pDebugFont = new CDebugFont;
	m_pDebugFont->SetUp();
}

CFPSManager::~CFPSManager()
{
	SAFE_DELETE(m_pDebugFont);
}

void	CFPSManager::FPSCountInit()
{
	m_stNowTime = m_stBeforeTime = m_stBeforeFlame = timeGetTime();
}

void CFPSManager::FPSDraw()
{
	//m_stNowTime = timeGetTime();

	///* FPS�����߂ĕ\������ */
	//m_FPSCount++; // �J�E���g�A�b�v
	//if ( m_stNowTime - m_stBeforeTime >= 1000 ) {
	//	
	//	wsprintf(buff, "%04d FPS\n", m_FPSCount);
	//	// ������
	//	m_FPSCount = 0;
	//	m_stBeforeTime = m_stNowTime;
	//}
	//// FPS�̕\��
	//m_pDebugFont->Draw(700,580,buff);
}



void CFPSManager::SetFPS()
{
	timeBeginPeriod(1);
	m_stNowTime = timeGetTime();
	timeEndPeriod(1);
	m_stFlmAngl = (float)(m_stNowTime - m_stBeforeFlame);
	if(m_stNowTime - m_stBeforeFlame == 0)
		int i = 0;
	m_stBeforeFlame = m_stNowTime;
	m_stFlmAngl *= (float)60/1000;

}
