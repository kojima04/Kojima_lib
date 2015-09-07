#pragma once
#pragma warning( disable : 4996 )

#include <tchar.h>
#include <stdlib.h>
#include <string>
#include "DXObject.h"

using namespace std;

// Direct3D用デバッグフォントクラス
class CDebugFont 
	: public CDXObject
{
public:
	CDebugFont();
	~CDebugFont();
protected:
	LPD3DXFONT		m_pFont;		// フォントオブジェクトポインタ
	D3DXFONT_DESC	D3DFD;	// フォント属性
public:
	bool SetUp();
	bool Destroy();
	void Draw(int x,int y,LPCSTR str);
};