#include "Common.h"

#include "System.h"

//--------------------------------
//			メイン関数
//---------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//メモリリークを発見する
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CSystem *pSystem;
	pSystem = new CSystem;
	//static CWindow wnd_main;

	if(pSystem->SetUp(FULLSCREEN,hInstance,nCmdShow) == false)
	{
		ERROR_MSG("SYSTEM::SETUP");
		return 0;
	}

	   // COM の初期化
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
	pSystem->Loop();

	SAFE_DELETE(pSystem);
	
	return 0;
}

