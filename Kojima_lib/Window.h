#pragma once

#define CLIENT_WIDTH	800			// クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT	600			// クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度
#define WINDOW_NAME		"WindowName"	// ウィンドウ名

#include "EditWindow.h"

class CWindow
{
public:

	// コンストラクタ
	CWindow();

	// デストラクタ
	~CWindow();

	HRESULT	Init(HINSTANCE hInst, int nCmdShow, bool is_full, int wedth, int height);					
	static LRESULT CALLBACK	WindowProc(HWND, unsigned int, WPARAM, LPARAM);

	HRESULT	MutexCreate();							//ミューテックス開始
	void	MutexDestroy();							//ミューテックス終了

	void	Loop();									//メッセージループ

	bool	CheckMessage(bool *pisEndApp);

	
protected:
	HWND	m_hWnd;				//ウィンドウハンドル
	MSG		m_msg;				//メッセージ
	HANDLE	m_hMutex;			//ミューテックス
	bool 	m_isFullScreen;		//フルスクリーンのフラグ
	int		m_WindowWidth;		//画面の幅
	int 	m_WindowHeight;		//画面の高さ
public:
	HWND	GetWindowHandle(){return m_hWnd;}
	MSG		GetMessage(){return m_msg;}
	bool	GetisFullScreen(){return m_isFullScreen;}
	long	GetWindowWidth()	const{return m_WindowWidth;}
	long	GetWindowHeight()	const{return m_WindowHeight;}
};



