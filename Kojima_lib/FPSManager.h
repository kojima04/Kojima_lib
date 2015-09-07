#pragma once
#include "DebugFont.h"

class CFPSManager
{
public:
	CFPSManager();
	~CFPSManager();

	void		FPSCountInit();
	void			SetFPS();
	void			FPSDraw();

	static void		SetFlmAngl(double dtmp){m_stFlmAngl = dtmp;}
	static double	GetFlmAngl(){return m_stFlmAngl;}
	static double	GetNowTime(){
		return m_stNowTime;}

private:
	static double	m_stFlmAngl;//キーフレームの速度
	static double	m_stNowTime;
	static double	m_stBeforeTime;
	static double	m_stBeforeFlame;

    int	 m_FPSCount;             // FTPカウンタ
    char buff[80];				 // 文字列表示用バッファ

	CDebugFont *	m_pDebugFont;
};

class CFPSTimer
{
public:
	CFPSTimer(){m_BeforeTime = (float)CFPSManager::GetNowTime();m_isActive = false;m_Direction = 1;};
	~CFPSTimer(){};
protected:
	float	m_BeforeTime;
	float	m_TimeLimit;
	bool	m_isActive;
	int	m_Direction;

public:

	void SetisActive(bool istmcnt){
		m_isActive = istmcnt;
	}
	bool GetisActive(){return m_isActive;}

	void SetDirection(int istmcnt){m_Direction = istmcnt;}
	int GetDirection(){return m_Direction;}
	void ReverseDirection(){m_Direction = -m_Direction;}

	void	SetTimeLimit(float count)
	{
		m_TimeLimit = count;
	}

	float	GetTimeLimit(){return m_TimeLimit;}

	void Start()
	{
		m_BeforeTime = (float)CFPSManager::GetNowTime();
		m_isActive = true;
	}

	void Stop()
	{
		m_isActive = false;
	}

	void SetBeforeTime()
	{
		m_BeforeTime = (float)CFPSManager::GetNowTime();
	}

	void ReStart()
	{
		m_isActive = true;
	}

	bool CheckisTimeCountPast()
	{
		if(CFPSManager::GetNowTime() - m_BeforeTime > m_TimeLimit&&m_isActive)
		{
			m_isActive = false;
			return true;
		}
		return false;
	}

	int GetFlameSub()
	{
		return (int)(CFPSManager::GetNowTime() - m_BeforeTime);
	}
};