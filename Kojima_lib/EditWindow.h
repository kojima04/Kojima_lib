#pragma once

#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")



#define EDITWINDOW_WIDTH	400			// 
#define	EDITWINDOW_HEIGHT	400			//
#define EDITWINDOW_X	1050			// 
#define	EDITWINDOW_Y	200			//
#define EDITWINDOW_NAME		"EditWindow"	// ウィンドウ名
#define	EDITWINDOW_BOTTUNWIDTH	60
#define	EDITWINDOW_BOTTUNHEIGHT	30
#define	EDITWINDOW_EDITBOXWIDTH	11
#define	EDITWINDOW_EDITBOXHEIGHT 25
#define EDITWINDOW_IDNUM	(ENUM_EDITWINDOW_TAB_ANIMATION_MAX +	ENUM_EDITWINDOW_TAB_SHOT_MAX + ENUM_EDITWINDOW_TAB_STATIC_MAX)

typedef enum eEditWindowTABOption
{
	ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX,
	ENUM_EDITWINDOW_TAB_OPTION_COMBOBOX,
	ENUM_EDITWINDOW_TAB_OPTION_STATICBOX,
	ENUM_EDITWINDOW_TAB_OPTION_STATICBOX2,
	ENUM_EDITWINDOW_TAB_OPTION_SETBUTTON,
	ENUM_EDITWINDOW_TAB_OPTION_SAVEBUTTON,
	ENUM_EDITWINDOW_TAB_OPTION_LOADBUTTON,
	ENUM_EDITWINDOW_TAB_OPTION_DELETEBUTTON,
	ENUM_EDITWINDOW_TAB_OPTION_FILELOADBUTTON,
	ENUM_EDITWINDOW_TAB_OPTION_MAX
};

typedef enum eEditWindowTAB
{	
	ENUM_EDITWINDOW_TAB_STATIC,
	ENUM_EDITWINDOW_TAB_ANIMATION,
	ENUM_EDITWINDOW_TAB_SHOT,
	ENUM_EDITWINDOW_TAB_MAX,
};

typedef enum eEditWindowStatic
{
	ENUM_EDITWINDOW_TAB_STATIC_SAVELOADBUTTON,
	ENUM_EDITWINDOW_TAB_STATIC_KEYFLAMESELECTED,
	ENUM_EDITWINDOW_TAB_STATIC_MOTIONSELECTED,
	ENUM_EDITWINDOW_TAB_STATIC_MAX
};

typedef enum eEditWindowAnimation
{	
	ENUM_EDITWINDOW_TAB_ANIMATION_KEYFLAMENUM,
	ENUM_EDITWINDOW_TAB_ANIMATION_ANIMFLAMEALL,
	ENUM_EDITWINDOW_TAB_ANIMATION_KEYFLAME,
	ENUM_EDITWINDOW_TAB_ANIMATION_MAX
};

typedef enum eEditWindowSHOT
{	
	ENUM_EDITWINDOW_TAB_SHOT_BALLSPIN,
	ENUM_EDITWINDOW_TAB_SHOT_BALLCONTRL,
	ENUM_EDITWINDOW_TAB_SHOT_MINRUTEHEIGHT,
	ENUM_EDITWINDOW_TAB_SHOT_MAXRUTEHEIGHT,
	ENUM_EDITWINDOW_TAB_SHOT_RUTETRYNUM,
	ENUM_EDITWINDOW_TAB_SHOT_MAX
};


typedef struct _EditWindowInfo 
{
	float	MinRuteHeight;
	float	MaxRuteHeight;
	float	BallSpin;
	float	BallContrl;

	int		KeyFlameNum;
	int		AnimFlameAll;
	int		RuteTryNum;

	D3DXVECTOR2	HitSquare;
	D3DXVECTOR2	HitPos;

	D3DXVECTOR2 MouseLocalPos;

	std::string	FileName;
	std::string MotionName;
	std::string MotionNameSelected;
	std::string NewMotionName;
	int KeyFlameSelected;
	int KeyFlame;

	bool	isUpdate[EDITWINDOW_IDNUM*ENUM_EDITWINDOW_TAB_OPTION_MAX];
	bool	isLoad[EDITWINDOW_IDNUM*ENUM_EDITWINDOW_TAB_OPTION_MAX];
	bool	isDelete[EDITWINDOW_IDNUM*ENUM_EDITWINDOW_TAB_OPTION_MAX];
	bool	isComboUpdate[EDITWINDOW_IDNUM*ENUM_EDITWINDOW_TAB_OPTION_MAX];

	bool	isFileLoad;
	bool	isMouseClick[2];

	void	Init()
	{
		KeyFlameNum = -1;
		AnimFlameAll = -1;
		KeyFlameSelected = 1;
	}	

	void	SetisUpdate(int idx,bool isupdate){isUpdate[idx] = isupdate;}
	bool	GetisUpdate(int idx){return	isUpdate[idx];}

	void	SetisLoad(int idx,bool isload){isLoad[idx] = isload;}
	bool	GetisLoad(int idx){	return	isLoad[idx];}

	void	SetisDelete(int idx,bool isdelete){isDelete[idx] = isdelete;}
	bool	GetisDelete(int idx){	return	isDelete[idx];}

	void	SetisFileLoad(bool isfload){	isFileLoad = isfload;}
	bool	GetisFileLoad(){	return	isFileLoad;}

	void	SetisComboUpdate(int idx,bool iscmbupdate){	isComboUpdate[idx] = iscmbupdate;}
	bool	GetisComboUpdate(int idx){	return	isComboUpdate[idx];}

	void	SetShotInfo(int trynum,float minheight,float spin,float contrl)
	{
		RuteTryNum = trynum;
		MinRuteHeight = minheight;
		BallSpin = spin;
		BallContrl = contrl;
	}
	void	SetFileName(std::string str)
	{
		FileName = str;
	}
	//void	SetHitSquare()
	//{
	//	m_stEditWindowInfo.
	//}
} EditWindowInfo;

#define ID_BUTTON 101

class CEditWindow
{
public:

	// コンストラクタ
	CEditWindow();

	// デストラクタ
	virtual ~CEditWindow();

	HRESULT	Init(HINSTANCE hInst, int nCmdShow, bool is_full, int wedth, int height);					
	static LRESULT CALLBACK	WindowProc(HWND, unsigned int, WPARAM, LPARAM);

public:
	static	HWND	GetWindowHandle(){return m_hWnd;}
	MSG		GetMessage(){return m_msg;}
	bool	GetFullScreen(){return m_isFullScreen;}
	long	GetWindowWidth()	const{return m_WindowWidth;}
	long	GetWindowHeight()	const{return m_WindowHeight;}

	static	std::string * GetsFileName(){return &m_stFileName;}
	static	EditWindowInfo * GetpEditWindowInfo(){return &m_stEditWindowInfo;}

	static	void	UpLoadContents();

	static	void	SetEditMenuNum();
	static	int		GetEditMenuNum(int i){return m_stEditMenuNum[i];}
	static	int		GetEditMenuID(int tab,int menu,int option);
	static	int		GetEditMenuID(int tab,int option);

	static	void	SetEditWindow_STATIC(HWND hWnd,LPARAM lParam);
	static	void	SetEditWindow_ANIMATION(HWND hWnd,LPARAM lParam);
	static	void	SetEditWindow_SHOT(HWND hWnd,LPARAM lParam);

	static	int		GetEditTabfromID(int id);
	static	int		GetEditMenuTabfromID(int id);
	static	void	SetEditWindowInfofromEditBox(int tab,int menu,char * message,int nummessage);
	static	void	SetEditWindowInfofromComboBox(int tab,int menu,char * message,int nummessage);

	static HWND CreateControlWindow(HWND hwndParent, int X, int Y,int Width, int Height,int dwExStyle ,LPCTSTR WindowName, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance);
	static HWND CreateControlWindowEx(HWND hwndParent, int Left, int Top,int Width, int Height,int dwExStyle ,int dwFlag ,LPCTSTR Caption, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance);

	static	HWND GethWnd(){return m_hWnd;}
	static	HWND GethWndEditArray(int idx){return m_hWndEditArray[idx];}

protected:
	static	HWND	m_hWnd;				//ウィンドウハンドル
	static	HWND	m_hWndEditArray[EDITWINDOW_IDNUM * ENUM_EDITWINDOW_TAB_OPTION_MAX];
	static  HWND	edit;
	static	HWND	m_hWnd_Combo;	//
	MSG		m_msg;				//メッセージ
	bool 	m_isFullScreen;		//フルスクリーンのフラグ
	int		m_WindowWidth;		//画面の幅
	int 	m_WindowHeight;		//画面の高さ
	static	std::string	m_stFileName;
	static	EditWindowInfo	m_stEditWindowInfo;	
	
	static	int	m_stEditMenuNum[ENUM_EDITWINDOW_TAB_MAX];
};