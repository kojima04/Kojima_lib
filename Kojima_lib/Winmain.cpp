#include "Common.h"

#include "System.h"

//--------------------------------
//			���C���֐�
//---------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//���������[�N�𔭌�����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CSystem *pSystem;
	pSystem = new CSystem;
	//static CWindow wnd_main;

	if(pSystem->SetUp(FULLSCREEN,hInstance,nCmdShow) == false)
	{
		ERROR_MSG("SYSTEM::SETUP");
		return 0;
	}

	   // COM �̏�����
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
	pSystem->Loop();

	SAFE_DELETE(pSystem);
	
	return 0;
}

