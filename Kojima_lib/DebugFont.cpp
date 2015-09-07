#include "Common.h"
#include "DebugFont.h"
//コンストラクタ
CDebugFont::CDebugFont()
{
	m_pFont = NULL;
}
//デストラクタ
CDebugFont::~CDebugFont()
{
	if(m_pFont)
		SAFE_RELEASE(m_pFont);
}
bool CDebugFont::SetUp()
{
	if (FAILED(D3DXCreateFont(
			GetDevice(),				//デバイス
            20,                         //文字の高さ 
            0,                          //文字幅
            100,						//フォントの太さ
            1,                          //MIPMAPのレベル
            FALSE,                      //イタリックか？
            DEFAULT_CHARSET,            //文字セット
            OUT_DEFAULT_PRECIS,         //出力精度
            DEFAULT_QUALITY,			//出力品質
            DEFAULT_PITCH | FF_SWISS,   //フォントピッチとファミリ
            TEXT("ＭＳ Ｐゴシック"),         // フォント名 
            &m_pFont)))					//Direct3Dフォントへのポインタへのアドレス
		return false;
	return true;
}
void CDebugFont::Draw(int x, int y, LPCSTR str)
{
	RECT rect = {0};
	rect.left = x;
	rect.top = y;
	m_pFont->DrawText(NULL,str, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
}