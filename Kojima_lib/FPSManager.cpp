#include "Common.h"
#include "FPSManager.h"

double	CFPSManager::m_stFlmAngl;//キーフレームの速度
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

	///* FPSを求めて表示する */
	//m_FPSCount++; // カウントアップ
	//if ( m_stNowTime - m_stBeforeTime >= 1000 ) {
	//	
	//	wsprintf(buff, "%04d FPS\n", m_FPSCount);
	//	// 初期化
	//	m_FPSCount = 0;
	//	m_stBeforeTime = m_stNowTime;
	//}
	//// FPSの表示
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
