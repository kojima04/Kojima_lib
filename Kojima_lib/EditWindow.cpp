#include "Common.h"
#include "EditWindow.h"

HWND CreateControlWindow(HWND hwndParent, int Left, int Top,int Width, int Height,int dwExStyle ,LPCTSTR WindowName, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance);
HWND CreateControlWindowEx(HWND hwndParent, int Left, int Top,int Width, int Height,int dwExStyle ,int dwFlag ,LPCTSTR Caption, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance);
int	OpenDiaog(HWND hwnd,LPCSTR Filter,char *FileName,DWORD Flags);
void SetRelativePath(char * CurPath);
int	CEditWindow::m_stEditMenuNum[ENUM_EDITWINDOW_TAB_MAX];

CEditWindow::CEditWindow()
{
	
}

CEditWindow::~CEditWindow()
{
	
}


////--------------------------------
////			プロシージャ
////--------------------------------

HWND CEditWindow::m_hWnd_Combo;
HWND CEditWindow::edit;	
std::string CEditWindow::m_stFileName;
EditWindowInfo	CEditWindow::m_stEditWindowInfo;
HWND CEditWindow::m_hWnd;
HWND CEditWindow::m_hWndEditArray[EDITWINDOW_IDNUM * ENUM_EDITWINDOW_TAB_OPTION_MAX];

HRESULT CALLBACK CEditWindow::WindowProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CEditWindow::SetEditMenuNum();
		CEditWindow::SetEditWindow_ANIMATION(hWnd,lParam);
		CEditWindow::SetEditWindow_STATIC(hWnd,lParam);
		CEditWindow::SetEditWindow_SHOT(hWnd,lParam);
		CEditWindow::m_stEditWindowInfo.Init();
		CEditWindow::m_stEditWindowInfo;
		break;

    case WM_COMMAND:
		for(int i = 0;i < EDITWINDOW_IDNUM;++i)
		{
			//エディットボックスの内容が変更されたとき
			if (LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX) && HIWORD(wParam) == EN_UPDATE) 
			{
				char charbuf[64] = {0};
				int ID = CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX);
				GetWindowText(CEditWindow::m_hWndEditArray[ID],charbuf, 64);
				int	intbuf	= atoi((LPCSTR)charbuf);
				SetEditWindowInfofromEditBox(GetEditTabfromID(i),GetEditMenuTabfromID(i),charbuf,intbuf);
			}
			//セットボタンを押した時
			if (LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_SETBUTTON))
			{
				EditWindowInfo * p = CEditWindow::GetpEditWindowInfo();
				int fs = GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX);
				CEditWindow::m_stEditWindowInfo.SetisUpdate(GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX),true);
				int f = 0;
			}
			//削除ボタンを押した時
			if (LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_DELETEBUTTON))
			{
				CEditWindow::m_stEditWindowInfo.SetisDelete(GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX),true);
			}
			//ロードボタンを押した時
			if (LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_LOADBUTTON))
			{
				CEditWindow::m_stEditWindowInfo.SetisLoad(GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_MAINEDITBOX),true);
			}
			//ファイルロードボタンを押した時
			if (LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_FILELOADBUTTON))
			{
				char FileName[MAX_PATH*2]; 
				//初期化(これをしないとごみが入る)
				ZeroMemory(FileName,MAX_PATH*2);
				//「ファイルを開く」ダイアログを表示
				if (OpenDiaog(m_hWnd,"Exe Files(*.png)\0*.png\0All Files(*.*)\0*.*\0\0",FileName,OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY))
				{
					//MessageBox(m_hWnd,strcat(FileName,"\nを選択しました。"),"情報",MB_OK);
					m_stEditWindowInfo.SetFileName(FileName);
					CEditWindow::GetpEditWindowInfo()->isFileLoad = true;
				}
			}
			if (
				LOWORD(wParam) == CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_COMBOBOX) &&
				HIWORD(wParam) == CBN_SELCHANGE) 
			{
				char charbuf[64] = {0};
				int ID = CEditWindow::GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_COMBOBOX);
				GetWindowText(CEditWindow::m_hWndEditArray[ID],charbuf, 64);
				int	intbuf	= atoi((LPCSTR)charbuf);
				CEditWindow::GetpEditWindowInfo()->SetisComboUpdate(GetEditMenuID(i,ENUM_EDITWINDOW_TAB_OPTION_COMBOBOX),true); 
				SetEditWindowInfofromComboBox(GetEditTabfromID(i),GetEditMenuTabfromID(i),charbuf,intbuf);
			}
		}

      break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		// どのキーが押されたか判定
		switch(wParam)
		{
		case VK_ESCAPE:
			//WM_DESTROYメッセージを送信
			SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;

		default:
			break;
		}
		break;

	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(true) > 0)
				break;
		}
		return 0;
		
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}



//-------------------------------------
//			ウィンドウ生成
//-----------------------------------

HRESULT CEditWindow::Init(HINSTANCE hInstance, int nCmdShow, bool is_full, int width, int height)
{	
	//char clsName[] = WINDOW_NAME;	// ウィンドウクラス名
	char clsName[] = "EditWindowName";

	m_isFullScreen = is_full;
	m_WindowHeight = height;
	m_WindowWidth  = width;


	WNDCLASS			wc;

	wc.style				 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc			 = WindowProc;
	wc.cbClsExtra			 = 0;
	wc.cbWndExtra			 = 0;
	wc.hInstance			 = hInstance;
	wc.hIcon				 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor				 = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground		 = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName			 = 0;
	wc.lpszClassName		 = clsName;

	// ウィンドウクラスの登録
	if(RegisterClass(&wc) == 0)
	{
		ERROR_MSG("ウィンドウクラスの作成に失敗しました.");
		return 0;	// 登録失敗
	}

	// ウィンドウの作成
		m_hWnd = CreateWindow(
					clsName, 				// 登録されているクラス名
					EDITWINDOW_NAME, 			// ウインドウ名
					WS_OVERLAPPEDWINDOW | WS_VISIBLE,				// ウインドウスタイル（ポップアップウインドウを作成）
					EDITWINDOW_X, 						// ウインドウの横方向の位置
					EDITWINDOW_Y, 						// ウインドウの縦方向の位置
					EDITWINDOW_WIDTH, 			// ウインドウの幅
					EDITWINDOW_HEIGHT,			// ウインドウの高さ
					NULL,					// 親ウインドウのハンドル（省略）
					NULL,					// メニューや子ウインドウのハンドル
					hInstance, 				// アプリケーションインスタンスのハンドル
					NULL					// ウインドウの作成データ
				);

		if(m_hWnd == NULL)
		{
			ERROR_MSG("ウィンドウの生成に失敗しました.");
			return 0;	// ウィンドウの生成に失敗
		}

	//}

	// ウィンドウの表示


	return true;
}

void CEditWindow::SetEditMenuNum()
{
	m_stEditMenuNum[ENUM_EDITWINDOW_TAB_STATIC] = ENUM_EDITWINDOW_TAB_STATIC_MAX;
	m_stEditMenuNum[ENUM_EDITWINDOW_TAB_ANIMATION] = ENUM_EDITWINDOW_TAB_ANIMATION_MAX;
	m_stEditMenuNum[ENUM_EDITWINDOW_TAB_SHOT] = ENUM_EDITWINDOW_TAB_SHOT_MAX;
}

int	CEditWindow::GetEditMenuID(int tab,int menu,int option)
{
	int id = 0;
	int tabbuf = tab;
	while(tabbuf != 0)
	{
		id += m_stEditMenuNum[tabbuf - 1];
		tabbuf--;
	}
	return ENUM_EDITWINDOW_TAB_OPTION_MAX*(id + menu) + option;
}

int	CEditWindow::GetEditMenuID(int tab,int option)
{
	return tab*ENUM_EDITWINDOW_TAB_OPTION_MAX + option;
}

int	CEditWindow::GetEditTabfromID(int id)
{
	int intbuf = id;
	int i = 0;
	for(;intbuf >= 0;i++)
	{
		intbuf -= GetEditMenuNum(i);
	}	
	return i - 1;
}
int	CEditWindow::GetEditMenuTabfromID(int id)
{
	int intbuf = id;
	int i = 0;
	for(;intbuf >= 0;i++)
	{
		intbuf -= GetEditMenuNum(i);
	}	
	return GetEditMenuNum(i - 1) + intbuf;
}

HWND CEditWindow::CreateControlWindow(HWND hwndParent, int X, int Y,int Width, int Height,int dwExStyle ,LPCTSTR WindowName, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance)
 { 
	return CreateWindow(ClassName,WindowName,WS_CHILD | WS_VISIBLE | dwExStyle,
			X - Width/2,Y - Height/2,Width,Height, hwndParent, ChildID, hInstance, NULL);
 }

HWND CEditWindow::CreateControlWindowEx(HWND hwndParent, int Left, int Top,int Width, int Height,int dwExStyle ,int dwFlag ,LPCTSTR Caption, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance)
 { 
	return CreateWindowEx(dwExStyle,ClassName,Caption,WS_CHILD | WS_VISIBLE | dwFlag    ,
							Left, Top, Width, Height, hwndParent, ChildID, hInstance, NULL);
 }



void SetRelativePath(char * CurPath)
{
	//カレントディレクトリの絶対パスを取得
	char lpBuffer[MAX_PATH*2];
	GetCurrentDirectory(MAX_PATH*2,lpBuffer);

	//ドライバ部分のパスをカット
	strncpy(CurPath, CurPath+3,strlen(CurPath));
	strncpy(lpBuffer, lpBuffer+3,strlen(lpBuffer));

	//絶対パスの同一な部分をカット
	int num = 0;
	for(int i = 0;lpBuffer[i] == CurPath[i];i++)
	{
		num ++;
	}
	strncpy(CurPath, CurPath+num,strlen(CurPath));
	strncpy(lpBuffer, lpBuffer+num,strlen(lpBuffer));

	//枝分かれしたフォルダまで戻る為にカウント
	int count = 0;
	while(lpBuffer[0] != 0)
	{
		count++;
		PathRemoveFileSpec(lpBuffer);
	}

	//上のディレクトリへのパス部分をつける
	std::string cGo = CurPath;
	std::string cBack = "../";
	for(int j = 0;j < count;j++)
	{
		cGo = cBack + cGo;
	}
	strcpy(CurPath,cGo.c_str());

	//相対パスが戻る必要がない場合先頭をカット
	if(count == 0)
	{
		strncpy(CurPath, CurPath+1,strlen(CurPath));
	}

	//￥を/に変更
	for(int j = 0;j < (int)strlen(CurPath);j++)
	{
		if(CurPath[j] == '\\')
			CurPath[j] = '/';
	}
}


int OpenDiaog(HWND hwnd,LPCSTR Filter,char * FileName,DWORD Flags)
{
   OPENFILENAME OFN; 

   ZeroMemory(&OFN,sizeof(OPENFILENAME));
   OFN.lStructSize = sizeof(OPENFILENAME); 
   OFN.hwndOwner = hwnd;
   OFN.lpstrFilter =Filter;
   OFN.lpstrFile =FileName;  
   OFN.nMaxFile = MAX_PATH*2;
   OFN.Flags = Flags;    
   OFN.lpstrTitle = "ファイルを開く";
   char lpBuffer[MAX_PATH*2];
   GetCurrentDirectory(MAX_PATH*2,lpBuffer);
   OFN.lpstrInitialDir = lpBuffer;
   int isevolv = GetOpenFileName(&OFN);

   SetCurrentDirectory(lpBuffer);
   SetRelativePath(FileName);
   return (isevolv);
}