#pragma once
#pragma warning( disable : 4996 )

#include <tchar.h>
#include <stdlib.h>
#include <string>
#include "DXObject.h"

using namespace std;

// Direct3D�p�f�o�b�O�t�H���g�N���X
class CDebugFont 
	: public CDXObject
{
public:
	CDebugFont();
	~CDebugFont();
protected:
	LPD3DXFONT		m_pFont;		// �t�H���g�I�u�W�F�N�g�|�C���^
	D3DXFONT_DESC	D3DFD;	// �t�H���g����
public:
	bool SetUp();
	bool Destroy();
	void Draw(int x,int y,LPCSTR str);
};