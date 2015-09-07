#include "Common.h"
#include "InstantEffect.h"

CInstantEffect::CInstantEffect()
{
	m_pTexture = NULL;
}

CInstantEffect::~CInstantEffect()
{

}


void CInstantEffect::Create()
{
	m_fAlfa = 255;
	CAnimation::Create();
}

void CInstantEffect::FeedOut()
{
	if(m_TimerArray[ENUM_EFFECT_COUNT_FEEDOUT].GetisActive())
	{
		int i = GetAlfa();
		m_fAlfa = m_fAlfa - 255*(((float)m_TimerArray[ENUM_EFFECT_COUNT_FEEDOUT].GetFlameSub())/m_TimerArray[ENUM_EFFECT_COUNT_FEEDOUT].GetTimeLimit());
		m_TimerArray[ENUM_EFFECT_COUNT_FEEDOUT].SetBeforeTime();
		if(m_fAlfa <= 0)
		{
			m_fAlfa = 0;
			m_TimerArray[ENUM_EFFECT_COUNT_FEEDOUT].Stop();
		}
		SetAlfa((int)m_fAlfa);
	}
	else if(m_TimerArray[ENUM_EFFECT_COUNT_FLASH].GetisActive())
	{
		int i = GetAlfa();
		m_fAlfa = m_fAlfa - (float)(m_TimerArray[ENUM_EFFECT_COUNT_FLASH].GetDirection())*
				(((float)m_TimerArray[ENUM_EFFECT_COUNT_FLASH].GetFlameSub())*(float)(255/m_TimerArray[ENUM_EFFECT_COUNT_FLASH].GetTimeLimit()));
		m_TimerArray[ENUM_EFFECT_COUNT_FLASH].SetBeforeTime();
		if(m_fAlfa < 0)
		{
			m_fAlfa = 0;
			m_TimerArray[ENUM_EFFECT_COUNT_FLASH].SetDirection(-1);			
			m_TimerArray[ENUM_EFFECT_COUNT_FLASH].Start();
		}
		if(m_fAlfa > 255)
		{
			m_fAlfa = 255;
			m_TimerArray[ENUM_EFFECT_COUNT_FLASH].SetDirection(1);			
			m_TimerArray[ENUM_EFFECT_COUNT_FLASH].Start();
		}
		SetAlfa((int)m_fAlfa);;
	}
	else 
	{
		int p = 0;
	}
}

void CInstantEffect::FeedIn()
{
	if(m_TimerArray[ENUM_EFFECT_COUNT_FEEDIN].GetisActive())
	{
		int i = GetAlfa();
		m_fAlfa = m_fAlfa + 255*(((float)m_TimerArray[ENUM_EFFECT_COUNT_FEEDIN].GetFlameSub())/m_TimerArray[ENUM_EFFECT_COUNT_FEEDIN].GetTimeLimit());
		m_TimerArray[ENUM_EFFECT_COUNT_FEEDIN].SetBeforeTime();
		if(m_fAlfa >= 255)
		{
			m_fAlfa = 255;
			m_TimerArray[ENUM_EFFECT_COUNT_FEEDIN].Stop();
		}
		SetAlfa((int)m_fAlfa);
		SetUpVertex();
	}
}


void CInstantEffect::Animation()
{
	//if(m_isActive)
	FeedOut();
	FeedIn();

	CAnimation::Animation();
}