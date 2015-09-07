#include "Common.h"
#include "Wave.h"

CWave::CWave()
{
//	m_wFmt = 0;
	m_pData = 0;
	m_dataSize = 0;
}
CWave::~CWave()
{

}

bool CWave::GetDatafromFile(std::string filepath,WAVEFORMATEX &waveFormatEX)
{
	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;

	// Wave�t�@�C���I�[�v��
	memset( &mmioInfo, 0, sizeof(MMIOINFO) );

	hMmio = mmioOpen((LPSTR)filepath.c_str(), &mmioInfo, MMIO_READ );
	if( !hMmio )
		return false; // �t�@�C���I�[�v�����s


	MMRESULT mmRes;
	MMCKINFO riffChunk;

	// RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend( hMmio, &riffChunk, NULL, MMIO_FINDRIFF );
	if( mmRes != MMSYSERR_NOERROR ) 
	{
		mmioClose( hMmio, 0 );
		return false;
	}

	// �t�H�[�}�b�g�`�����N����
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend( hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK );
	if( mmRes != MMSYSERR_NOERROR ) 
	{
		mmioClose( hMmio, 0 );
		return false;
	}

	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead( hMmio, (HPSTR)&waveFormatEX, fmsize );
	if( size == -1 || size == 0) 
	{
		mmioClose(hMmio, 0 );
		return false;
	}



	if( size != fmsize ) 
	{
		mmioClose(hMmio, 0 );
		return false;
	}

	mmioAscend(hMmio,&formatChunk, 0 );

	// �f�[�^�`�����N����
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend( hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK );
	if( mmRes != MMSYSERR_NOERROR ) 
	{
		mmioClose( hMmio, 0 );
		return false;
	}

	//�f�[�^�i�[
	m_pData = new char[ dataChunk.cksize ];

	size = mmioRead( hMmio, (HPSTR)m_pData, dataChunk.cksize );

	if(size != dataChunk.cksize ) 
	{
		delete[] m_pData;
		return false;
	}

	m_dataSize = size;

	mmioClose( hMmio, 0 );

	return true;
}
