#pragma once
#include <map>
#include "DirectSound.h"

class CSoundManager
{
public:
	CSoundManager(HWND hWnd);
	~CSoundManager();
	bool Init(HWND hWnd);
	void Destroy();
	bool AddDatafromFile(std::string filepath,std::string filename,bool isPrimary,bool isLoop);
	bool Play(std::string key);
	bool Stop(std::string key);
	bool PlayfromHead(std::string key);
	CDirectSound * RequestSound(std::string filepath);

	void	Update();

	CDirectSound * GetpPrimarity(){return m_Primarity;}

protected:
	HWND	m_hWnd;
	std::map<std::string,CDirectSound *> m_SoundBook;
	CDirectSound * m_Primarity;
};