#include "Common.h"

#include "MaltiThread.h"

DWORD WINAPI ThreadFunc(LPVOID hWnd) {
	HDC hdc;
	int iRed = 0 , iX = 0 , iY = 0;
	RECT rect;

	while (TRUE) {
		hdc = GetDC((HWND)hWnd);
		GetClientRect((HWND)hWnd , &rect);
		SetPixel(hdc , iX , iY , RGB(iRed , 0 , 0));
		ReleaseDC((HWND)hWnd , hdc);

		if (iX < rect.right) iX++;
		else if (iY < rect.bottom) {
			iX = 0; iY++;
		}
		else break;
		iRed = iRed == 255 ? 0 : iRed + 1;
	}
	ExitThread(TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd , UINT msg , WPARAM wp , LPARAM lp) {
	DWORD dwParam;
	TCHAR str[128];
	static HANDLE hThread;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		hThread = CreateThread(
			NULL , 0 , ThreadFunc , hWnd , 0 , &dwParam);
		return 0;
	case WM_RBUTTONUP:
		GetExitCodeThread(hThread , &dwParam);
		if (dwParam == STILL_ACTIVE)
			MessageBox(hWnd , TEXT("Thread is still alive") ,
				TEXT("スレッドは生きています") , MB_OK);
		else {
			wsprintf(str , TEXT("Code = %d") , dwParam);
			MessageBox(hWnd , str , TEXT("コード") , MB_OK);
		}
		return 0;
	}
	return DefWindowProc(hWnd , msg , wp , lp);
}

