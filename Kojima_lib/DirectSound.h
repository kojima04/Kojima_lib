#pragma once
#include <list>
#include <dsound.h>
#include <process.h>
#include "Wave.h"
#define BUFFERNUM 10

#pragma comment(lib, "dsound.lib")

class CDirectSound;

// 別スレッドに渡すデータを格納する構造体  
struct PlayThreadData  
{  
    HANDLE hEvent;  
    CDirectSound * soundobj;
	bool	isdelete;
};  

class CDirectSound
{
public:
	CDirectSound(bool isPrimary,HWND hwnd);
	~CDirectSound();
	bool SetNewWaveFile(std::string filepath);
	bool Play();
	bool PlayFromHead();
	bool Stop();
	bool GetisPrimary(){return m_isPrimary;};
	CWave * GetpWave(){return m_pWave;}
	void SetisLoop(bool isl){m_isLoop = isl;}
	void SetVolume(long vol);
	void PlaybySetTime();
	void PlaybySetTime(float playtime,int alltime);
	void SetTime(float playtime,int alltime){m_LoopHeadTime = playtime; m_TotalTime = alltime;}
	bool CreateLoopThread();
	static unsigned __stdcall NotifyFunction(LPVOID param);

	LPDIRECTSOUNDBUFFER8	GetpBuffer(){return m_pBuffer[0];}

	static CDirectSound * GetspStoppedSound(){return m_spStoppedSound;}
	static void SetspStoppedSound(CDirectSound * psound);
	static std::list<PlayThreadData *> m_pThredDateList;

protected:
	LPDIRECTSOUNDBUFFER8	m_pBuffer[BUFFERNUM];				//	サウンドバッファ
	HWND					m_hWnd;		
	int						m_BufferIDX;
	int						m_Count;
	bool					m_isPrimary;
	long					m_BufferSize;			//	バッファのサイズ
	CWave *					m_pWave;				//	wavファイルクラスへのポインタ
	int						m_NumBuffers;			//	バッファの数
	bool					m_isCreation;			//	再生のフラグ
	IDirectSound8 			* m_lpDS;					//	サウンドデバイス
	bool					m_isLoop;
	HANDLE					m_hEvent;
	LPDIRECTSOUNDNOTIFY		m_pDSNotify;
	static CDirectSound *	m_spStoppedSound;
	PlayThreadData *		m_pThredData;
	int						m_TotalTime;
	float					m_LoopHeadTime;
};