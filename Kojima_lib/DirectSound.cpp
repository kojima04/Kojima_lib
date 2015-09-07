#include "Common.h"
#include "DirectSound.h"

CDirectSound * CDirectSound::m_spStoppedSound;
std::list<PlayThreadData *> CDirectSound::m_pThredDateList;


CDirectSound::CDirectSound(bool isPrimary,HWND hWnd)
{
	m_lpDS = NULL;
	m_isPrimary =  isPrimary;
	m_pWave = new CWave;
	//m_pBuffer = new LPDIRECTSOUNDBUFFER8[BUFFERNUM];
	m_BufferIDX = 0;
	m_Count = 0;
	m_isLoop = false;
	m_hEvent = NULL;
	m_pDSNotify = NULL;
	m_pThredData = NULL;
	m_hWnd = hWnd;
}

CDirectSound::~CDirectSound()
{
	if(m_Count)
		delete[] m_pWave->GetWaveData();

	for(int i = 0 ;i < BUFFERNUM;i++){
		m_pBuffer[i]->Release();
	}
	SAFE_DELETE(m_pWave);
	if(m_pThredData)
		SAFE_DELETE(m_pThredData);
}

bool CDirectSound::SetNewWaveFile(std::string filepath)
{
	HRESULT hr;
	WAVEFORMATEX wFmt;

	if(m_pWave->GetDatafromFile(filepath,wFmt) == false)
	{
		ERROR_MSG("CDirectSound::m_pWave->GetDatafromFile()");
		return false;
	}
	
	// サウンドデバイス作成
	hr = DirectSoundCreate8(NULL,&m_lpDS,NULL);
	hr = m_lpDS->SetCooperativeLevel(m_hWnd,DSSCL_PRIORITY);

	//セカンダリバッファの作成
	DSBUFFERDESC DSBufferDesc;
	memset(&DSBufferDesc, 0, sizeof(DSBUFFERDESC)); 
	DSBufferDesc.dwSize          = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags         = 0;
	DSBufferDesc.dwBufferBytes   = m_pWave->GetdataSize();
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;
	DSBufferDesc.lpwfxFormat     = &wFmt;

	IDirectSoundBuffer * ptmpBuf = 0;
	m_pBuffer[0] = NULL;

	hr = m_lpDS->CreateSoundBuffer( &DSBufferDesc, &ptmpBuf, NULL );
	
	if( hr != DS_OK ){
		ERROR_MSG("CDirectSound::m_pWave->GetDatafromFile()");
		return false;
	}
	

	IDirectSoundBuffer8 *pDSBuffer;
	hr = ptmpBuf->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&m_pBuffer[0]);

	if(m_pBuffer[0] == NULL){
		m_lpDS->Release();
		ERROR_MSG("CDirectSound::m_pWave->GetDatafromFile()");
		return false;
	}

	// セカンダリバッファにWaveデータ書き込み
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		
		if ( DS_OK == m_pBuffer[0]->Lock( 0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER ) ) 
		{
			memcpy( lpvWrite, m_pWave->GetWaveData(), dwLength);
			m_pBuffer[m_BufferIDX]->Unlock( lpvWrite, dwLength, NULL, 0);
		}

	for(int j = 1;j < BUFFERNUM;j++)
	{
		IDirectSoundBuffer * pDSBufferDuplicate = 0;
		m_lpDS->DuplicateSoundBuffer(m_pBuffer[0],&pDSBufferDuplicate);
	//	ptmpBuf[j]->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&m_pBuffer[j]);
		pDSBufferDuplicate->QueryInterface( IID_IDirectSoundBuffer8 ,(void**)&m_pBuffer[j]);
	}
	m_BufferIDX = 0;

	//SAFE_DELETEARRAY(ptmpBuf);
	m_Count++;
			
	return S_OK;
}

void CDirectSound::PlaybySetTime()
{
	m_pBuffer[m_BufferIDX]->SetCurrentPosition(GetpWave()->GetdataSize()/(m_TotalTime)*m_LoopHeadTime);
}

void CDirectSound::PlaybySetTime(float playtime,int alltime)
{
	m_pBuffer[m_BufferIDX]->SetCurrentPosition(GetpWave()->GetdataSize()/(alltime)*playtime);
}

void CDirectSound::SetspStoppedSound(CDirectSound * psound)
{
	m_spStoppedSound = psound;
}

unsigned __stdcall CDirectSound::NotifyFunction(LPVOID param)  
{  
	PlayThreadData *data = (PlayThreadData*)param;  

	// 再生終了通知が来るまで待機  
	WaitForMultipleObjects(1, &data->hEvent, FALSE, INFINITE);  

//	data->soundobj->SetTime(10.883,60*2 + 8);
///	data->soundobj->Play();
	data->isdelete = true;
	CDirectSound::SetspStoppedSound(data->soundobj);
	SAFE_DELETE(data);
	return 0;
}  

bool CDirectSound::CreateLoopThread()
{
	m_pBuffer[m_BufferIDX]->QueryInterface(IID_IDirectSoundNotify,(void**)&m_pDSNotify);
	m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL); 
	DSBPOSITIONNOTIFY PositionNotify;
	PositionNotify.dwOffset = DSBPN_OFFSETSTOP;
	PositionNotify.hEventNotify = m_hEvent;
	m_pDSNotify->SetNotificationPositions(1,&PositionNotify);

	if(m_pDSNotify)
	{  
		PlayThreadData * pThredData;
		pThredData = new PlayThreadData;

		pThredData->hEvent = m_hEvent;  
		pThredData->soundobj = this; 
		pThredData->isdelete = false;
		unsigned int threadID;

		m_pThredDateList.push_back(pThredData);

		_beginthreadex(NULL, 0,&CDirectSound::NotifyFunction, pThredData, 0, &threadID);  
	}  
	return true;
}

bool CDirectSound::Play()
{
#ifndef SILENT
	if(m_isLoop)
		m_pBuffer[m_BufferIDX]->Play(0,0,DSBPLAY_LOOPING);
	else
		m_pBuffer[m_BufferIDX]->Play(0,0,0);
	if(m_isPrimary == false)
	{
		if(m_BufferIDX < BUFFERNUM - 1)
			m_BufferIDX ++;
		else
			m_BufferIDX = 0;
	}
	
#endif
	return true;
}

bool CDirectSound::PlayFromHead()
{
#ifndef SILENT
	m_pBuffer[m_BufferIDX]->SetCurrentPosition(0);
	if(m_isLoop)
		m_pBuffer[m_BufferIDX]->Play(0,0,DSBPLAY_LOOPING);
	else
		m_pBuffer[m_BufferIDX]->Play(0,0,0);
	if(m_isPrimary == false)
	{
		if(m_BufferIDX < BUFFERNUM - 1)
			m_BufferIDX ++;
		else
			m_BufferIDX = 0;
	}
	
#endif
	return true;
}

bool CDirectSound::Stop()
{
	for(int j = 0;j < BUFFERNUM;j++)
	{
		m_pBuffer[j]->Stop();
	}
	return true;
}