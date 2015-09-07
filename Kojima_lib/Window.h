#pragma once

#define CLIENT_WIDTH	800			// �N���C�A���g�̈�̃f�t�H���g�̕�, �t���X�N���[���̏ꍇ�͐��������𑜓x
#define	CLIENT_HEIGHT	600			// �N���C�A���g�̈�̃f�t�H���g�̍���, �t���X�N���[���̏ꍇ�͐��������𑜓x
#define WINDOW_NAME		"WindowName"	// �E�B���h�E��

#include "EditWindow.h"

class CWindow
{
public:

	// �R���X�g���N�^
	CWindow();

	// �f�X�g���N�^
	~CWindow();

	HRESULT	Init(HINSTANCE hInst, int nCmdShow, bool is_full, int wedth, int height);					
	static LRESULT CALLBACK	WindowProc(HWND, unsigned int, WPARAM, LPARAM);

	HRESULT	MutexCreate();							//�~���[�e�b�N�X�J�n
	void	MutexDestroy();							//�~���[�e�b�N�X�I��

	void	Loop();									//���b�Z�[�W���[�v

	bool	CheckMessage(bool *pisEndApp);

	
protected:
	HWND	m_hWnd;				//�E�B���h�E�n���h��
	MSG		m_msg;				//���b�Z�[�W
	HANDLE	m_hMutex;			//�~���[�e�b�N�X
	bool 	m_isFullScreen;		//�t���X�N���[���̃t���O
	int		m_WindowWidth;		//��ʂ̕�
	int 	m_WindowHeight;		//��ʂ̍���
public:
	HWND	GetWindowHandle(){return m_hWnd;}
	MSG		GetMessage(){return m_msg;}
	bool	GetisFullScreen(){return m_isFullScreen;}
	long	GetWindowWidth()	const{return m_WindowWidth;}
	long	GetWindowHeight()	const{return m_WindowHeight;}
};



