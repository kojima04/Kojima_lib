#include "Common.h"

#include "SoundManager.h"

//コンストラクタ
CSoundManager::CSoundManager(HWND hWnd)
{
	m_hWnd = hWnd;
	m_Primarity = NULL;
}

//デストラクタ
CSoundManager::~CSoundManager()
{
	Destroy();
}

//開始処理
bool CSoundManager::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	return true;
}

//終了処理
void CSoundManager::Destroy()
{
	std::map<std::string, CDirectSound *>::iterator iter = m_SoundBook.begin();

	while(iter != m_SoundBook.end())
	{
		SAFE_DELETE((*iter).second);
		++iter;
	}

	std::list<PlayThreadData *>::iterator it = CDirectSound::m_pThredDateList.begin(); // イテレータ
	while( it != CDirectSound::m_pThredDateList.end() )  // listの末尾まで
	{
		if(!(*it)->isdelete)
			SAFE_DELETE(*it);
		++it;  // イテレータを１つ進める
	}
	CDirectSound::m_pThredDateList.clear();

	m_SoundBook.clear();
}

//ファイル名からサウンドを追加する
bool CSoundManager::AddDatafromFile(std::string filepath,std::string filename,bool isPrimary,bool isLoop)
{
	CDirectSound * temp;
	temp = new CDirectSound(isPrimary,m_hWnd);

	if(m_hWnd == NULL)
	{
		ERROR_MSG("CSoundManager::AddData  m_hWndがない");
		return false;
	}
	//temp->Init(m_hWnd);
	temp->SetNewWaveFile(filename);
	temp->SetisLoop(isLoop);

	m_SoundBook.insert(std::pair<std::string,CDirectSound *>(filepath,temp));

	return true;
}

CDirectSound *  CSoundManager::RequestSound(std::string filepath)
{
	std::map<std::string,CDirectSound *>::iterator iter;

	iter = m_SoundBook.find(filepath);
	if(iter == m_SoundBook.end())
	{
		ERROR_MSG("CSoundManager::Play　そんな音ねぇ！");
		return NULL;
	}
	return (*iter).second;
}

//サウンドの再生
bool CSoundManager::Play(std::string filepath)
{
	std::map<std::string,CDirectSound *>::iterator iter;

	iter = m_SoundBook.find(filepath);
	if(iter == m_SoundBook.end())
	{
		ERROR_MSG("CSoundManager::Play　そんな音ねぇ！");
		return false;
	}

	if((*iter).second->GetisPrimary())
	{
		if(m_Primarity != (*iter).second)
		{
			if(m_Primarity != NULL)
				m_Primarity->Stop();
			m_Primarity = (*iter).second;
		}
	}
#ifndef SILENT
	(*iter).second->Play();
#endif

	return true;
}

//サウンドの停止
bool CSoundManager::Stop(std::string filepath)
{
	std::map<std::string,CDirectSound *>::iterator iter;

	iter = m_SoundBook.find(filepath);
	if(iter == m_SoundBook.end())
	{
		ERROR_MSG("CSoundManager::Play　そんな音ねぇ！");
		return false;
	}
	
	(*iter).second->Stop();
	return true;
}

void CSoundManager::Update()
{
	if(CDirectSound::GetspStoppedSound())
	{
		if(m_Primarity == CDirectSound::GetspStoppedSound())
		{
			CDirectSound::GetspStoppedSound()->PlaybySetTime();
			CDirectSound::GetspStoppedSound()->CreateLoopThread();
			CDirectSound::GetspStoppedSound()->Play();
		}
		CDirectSound::SetspStoppedSound(NULL);
	}
}

bool CSoundManager::PlayfromHead(std::string filepath)
{
	std::map<std::string,CDirectSound *>::iterator iter;

	iter = m_SoundBook.find(filepath);
	if(iter == m_SoundBook.end())
	{
		ERROR_MSG("CSoundManager::Play　そんな音ねぇ！");
		return false;
	}
	
	(*iter).second->PlayFromHead();

	return true;
}