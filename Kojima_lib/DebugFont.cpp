#include "Common.h"
#include "DebugFont.h"
//�R���X�g���N�^
CDebugFont::CDebugFont()
{
	m_pFont = NULL;
}
//�f�X�g���N�^
CDebugFont::~CDebugFont()
{
	if(m_pFont)
		SAFE_RELEASE(m_pFont);
}
bool CDebugFont::SetUp()
{
	if (FAILED(D3DXCreateFont(
			GetDevice(),				//�f�o�C�X
            20,                         //�����̍��� 
            0,                          //������
            100,						//�t�H���g�̑���
            1,                          //MIPMAP�̃��x��
            FALSE,                      //�C�^���b�N���H
            DEFAULT_CHARSET,            //�����Z�b�g
            OUT_DEFAULT_PRECIS,         //�o�͐��x
            DEFAULT_QUALITY,			//�o�͕i��
            DEFAULT_PITCH | FF_SWISS,   //�t�H���g�s�b�`�ƃt�@�~��
            TEXT("�l�r �o�S�V�b�N"),         // �t�H���g�� 
            &m_pFont)))					//Direct3D�t�H���g�ւ̃|�C���^�ւ̃A�h���X
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