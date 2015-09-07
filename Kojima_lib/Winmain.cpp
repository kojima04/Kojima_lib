#include "Common.h"

#include "System.h"

//--------------------------------
//			ƒƒCƒ“ŠÖ”
//---------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//ƒƒ‚ƒŠƒŠ[ƒN‚ð”­Œ©‚·‚é
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CSystem *pSystem;
	pSystem = new CSystem;
	//static CWindow wnd_main;

	if(pSystem->SetUp(FULLSCREEN,hInstance,nCmdShow) == false)
	{
		ERROR_MSG("SYSTEM::SETUP");
		return 0;
	}

	   // COM ‚Ì‰Šú‰»
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
	pSystem->Loop();

	SAFE_DELETE(pSystem);
	
	return 0;
}

