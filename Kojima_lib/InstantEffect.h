#pragma once

#include "Animation.h"

typedef enum eEffectTimeCount
{	
	ENUM_EFFECT_COUNT_FEEDOUT,
	ENUM_EFFECT_COUNT_FEEDIN,
	ENUM_EFFECT_COUNT_FLASH,
	ENUM_EFFECT_COUNT_BLINK,
	ENUM_EFFECT_COUNT_MAX
};

class CInstantEffect :
	public CAnimation
{
public:
	CInstantEffect();
	~CInstantEffect();
public:
	void Create();
	void Animation();
	void FeedOut();
	void FeedIn();

	CFPSTimer * GetpTimerArray(int idx){return &m_TimerArray[idx];}
//	SetVertexColor(int r,int g,int b){m_VertexColor[0] = r;m_VertexColor[1] = g;m_VertexColor[2] = b;}
protected:

	CFPSTimer m_TimerArray[ENUM_EFFECT_COUNT_MAX];

};