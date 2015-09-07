#pragma once

#include "Texture.h"
#include "FPSManager.h"

typedef enum eAnimationTimeCount
{	
	ENUM_ACTION_COUNT_FEEDOUT, 
	ENUM_ACTION_COUNT_MAX
};

//-----------------------------------------------
//			            CAnimation
//------------------------------------------------

class CAnimation
	: public CTexture
{
public:
	CAnimation::CAnimation();
	CAnimation::~CAnimation();

	virtual void Animation();

	void Destroy();

	void Flame(int num);
	void SetAnimPara(int num,int time);
	void SetKeyFlame(int num,int time){m_KeyFlameArray[num] = time;}

	void Reset()
	{
		m_KeyFlameCount = 0;
		m_fFlameCount = 0;
		m_isActive = true;
		SetAlfa(255);
	}
	void Stop(){ m_isActive = false; }
	void Replay(){ m_isActive = true; }
	bool GetisActive(){return m_isActive;}

	void SetisLoop(bool btmp){m_isLoop = btmp;}
	void SetFeedOutCount(int count)
	{
		m_FeedOutCount = count; m_BeforeTimeArray[ENUM_ACTION_COUNT_FEEDOUT] = (float)CFPSManager::GetNowTime();
	};

	void SetKeyFlameNum(int num);

	int ConvertFlametoMSecond(int flame){return (int)((float)flame*60/1000);}

	int GetKeyFlameNum(){return m_KeyFlameNum;}

	void SetKeyFlameArray(int idx,int key){m_KeyFlameArray[idx] = key;}
	int GetKeyFlameArray(int idx){return m_KeyFlameArray[idx];}

	void SetAnimFlameAll(int intbuf){m_AnimFlameAll = intbuf;}
	int	GetAnimFlameAll(){return m_AnimFlameAll;}

	void SetKeyFlameCount(int idx){m_KeyFlameCount = idx;}
	int GetKeyFlameCount(){return m_KeyFlameCount;}

	void SetFlameCount(float fbuf){m_fFlameCount = fbuf;}
	float GetFlameCount(){return m_fFlameCount;}

	void SetTexReverse(bool istr){m_isTexReverse = istr;}
	bool GetTexReverse(){return m_isTexReverse;}

	void SetisActive(bool isac){m_isActive = isac;}

	void SetUpVertex();

	void SetAlfa(int A);

protected:
	bool	m_isTexReverse;
	bool	m_isLoop;
	int		m_FeedOutCount;
	float	m_fAlfa;
	float	m_BeforeTimeArray[ENUM_ACTION_COUNT_MAX];
	int		m_KeyFlameCount;
	int		m_KeyFlameNum;
	int *	m_KeyFlameArray;
	float	m_fFlameCount;
	int		m_FlameCount;
	bool	m_isActive;
	int		m_AnimFlameAll;

};
