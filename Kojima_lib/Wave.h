#pragma comment ( lib, "winmm.lib" )
#include <mmsystem.h>


class CWave
{
public:
	CWave();
	~CWave();

	bool GetDatafromFile(std::string filepath,WAVEFORMATEX &wFmt);
	DWORD GetdataSize(){return m_dataSize;}
	char * GetWaveData(){return m_pData;};

protected:
//	WAVEFORMATEX m_wFmt;
//	LPWAVEFORMATEX m_lpwFmt;

	/*
	HMMIO m_hMmio;
	MMIOINFO m_mmioInfo;

	MMCKINFO m_riffChunk;
	MMCKINFO m_formatChunk;
	MMCKINFO m_dataChunk;
	*/

	char * m_pData;
	DWORD m_dataSize;
};