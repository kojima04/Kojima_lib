#include "Common.h"
#include "DirectInput.h"

// Joypad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

CDirectInput::CDirectInput()
{
	m_pDInput = NULL;
	m_pDIKeyboard = NULL;
	for(int i = 0;i < JOYPADMAX;i++)
	m_pDIJoypad[i] = NULL;
	m_pDIMouse = NULL;	

	m_diJoyCount = 0;

}

CDirectInput::~CDirectInput()
{
	Release();
}

//-----------------------------------------------------------------------------
// Direct Input 初期化
//-----------------------------------------------------------------------------
bool CDirectInput::Init(HWND hWnd)
{
	HRESULT		hr;

	hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);
	if ( FAILED(hr) )
	{
		return false;	// DirectInput8の作成に失敗
	}

	//キーボードの初期化
	if(InitKeybord(hWnd) == false)
	{
		return false;
	}
	//ジョイパッドの初期化
	if(InitJoypad() == false)
	{
		return false;
	}
	//マウスの初期化
	if(InitMouse(hWnd) == false)
	{
		return false;
	}


	return true;

}

//-----------------------------------------------------------------------------
// Direct Input 開放処理
//-----------------------------------------------------------------------------
void CDirectInput::Release(void)
{
	// DirectInputのデバイスを開放
	if ( m_pDIKeyboard ) m_pDIKeyboard->Unacquire();
	SAFE_RELEASE(m_pDIKeyboard);
	if ( m_pDIMouse ) m_pDIMouse->Unacquire();
	SAFE_RELEASE(m_pDIMouse);
	
	for (int i=0 ; i < JOYPADMAX ; i++) 
	{
		if ( m_pDIJoypad[i] != NULL) 
		{
			m_pDIJoypad[i]->Unacquire();
			SAFE_RELEASE(m_pDIJoypad[i]);
		}
	}
	SAFE_RELEASE(m_pDInput);
	m_diJoyCount = 0;

}

//-----------------------------------------------------------------------------
// キーボード用オブジェクト作成
//-----------------------------------------------------------------------------
bool CDirectInput::InitKeybord(HWND hWnd)
{
	HRESULT		hr;

	//キーボード用にデバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL); 
	if ( FAILED(hr) )
		return false;  // デバイスの作成に失敗

	//キーボード用のデータ・フォーマットを設定
	hr = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	
	if ( FAILED(hr) )
		return false; // デバイスの作成に失敗

	//モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return false; // モードの設定に失敗

	//キーボード入力制御開始
	m_pDIKeyboard->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// 関数名　：　GetKeyboardState()
// 機能概要：　キーボードの状態を取得
//-----------------------------------------------------------------------------
void CDirectInput::GetKeybordState(void)
{
	HRESULT			hr;

	if ( m_pDIKeyboard == NULL) return;

	for(int i = 0;i < 256;i++)
	{
		m_ExdiKeyState[i] = m_diKeyState[i];
		m_diKeyState[i] = 0;
	}
	hr = m_pDIKeyboard->GetDeviceState(256, m_diKeyState);
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = m_pDIKeyboard->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = m_pDIKeyboard->Acquire();
	}

}




//-----------------------------------------------------------------------------
//	複数のジョイスティック・デバイスを列挙（コールバック関数）
//-----------------------------------------------------------------------------
BOOL CALLBACK CDirectInput::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,void *pContext)
{
	CDirectInput *pThis = (CDirectInput *)pContext;
	HRESULT hr;

	hr = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIJoypad[pThis->m_diJoyCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}



//-----------------------------------------------------------------------------
//  複数のJoypad用オブジェクトを作成
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoypad(void)
{
	HRESULT		hr;
	int			i;

	// ジョイパッドを探す
	m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, (void *) this, DIEDFL_ATTACHEDONLY);

//	m_diJoyCount = 2;
	for ( i=0 ; i<m_diJoyCount ; i++ ) {
		// ジョイスティック用のデータ・フォーマットを設定
		hr = m_pDIJoypad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(hr) )
			return false; // データフォーマットの設定に失敗

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_X; 
		m_pDIJoypad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj		= DIJOFS_Y;
		m_pDIJoypad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		m_pDIJoypad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		m_pDIJoypad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//ジョイスティック入力制御開始
		m_pDIJoypad[i]->Acquire();
	}
		
	return true;

}

//-----------------------------------------------------------------------------
// 関数名　：　GetJoypadState2()
// 機能概要：　複数のジョイパッドの状態を取得
//-----------------------------------------------------------------------------
void CDirectInput::GetJoypadState(void)
{
	HRESULT			hr = 0;
	int				i;

	for ( i=0 ; i<m_diJoyCount ; i++ ) 
		for(int j = 0;j < DI_BUTTON_MAX;j++)
		{
			m_ExdiJoyState[i][j] = m_diJoyState[i][j];
			m_diJoyState[i][j] = false;	// 初期化
		}

	for ( i=0 ; i<m_diJoyCount ; i++ ) 
	{		
		m_pDIJoypad[i]->Poll();	// ジョイスティックにポールをかける
		if ( FAILED(hr) ) {
			hr = m_pDIJoypad[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = m_pDIJoypad[i]->Acquire();
		}

		DIJOYSTATE2		dijs;

		hr = m_pDIJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			hr = m_pDIJoypad[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = m_pDIJoypad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if ( dijs.lY < 0 )					
			m_diJoyState[i][DI_BUTTON_UP] = true;
		//* y-axis (backward)
		if ( dijs.lY > 0 )					
			m_diJoyState[i][DI_BUTTON_DOWN] = true;
		//* x-axis (left)
		if ( dijs.lX < 0 )					
			m_diJoyState[i][DI_BUTTON_LEFT] = true;
		//* x-axis (right)
		if ( dijs.lX > 0 )					
			m_diJoyState[i][DI_BUTTON_RIGHT] = true;
		#ifndef BUSITSU

		//* Ａボタン
		if ( dijs.rgbButtons[0] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_A] = true;
		//* Ｂボタン
		if ( dijs.rgbButtons[1] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_B] = true;			
		//* Ｃボタン
		if ( dijs.rgbButtons[2] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_C] = true;			
		//* Ｘボタン
		if ( dijs.rgbButtons[3] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_X] = true;		
		//* Ｙボタン
		if ( dijs.rgbButtons[4] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Y] = true;		
		//* Ｚボタン
		if ( dijs.rgbButtons[5] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Z] = true;		
		//* Ｌボタン
		if ( dijs.rgbButtons[6] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_L] = true;		
		//* Ｒボタン
		if ( dijs.rgbButtons[7] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_R] = true;
		//* ＳＴＡＲＴボタン
		if ( dijs.rgbButtons[8] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_START] = true;
		//* Ｍボタン
		if ( dijs.rgbButtons[9] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_M] = true;
#endif		
		
		
#ifdef BUSITSU	
		//* Ａボタン
		if ( dijs.rgbButtons[0] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_C] = true;
		//* Ｂボタン
		if ( dijs.rgbButtons[1] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_B] = true;
		//* Ｃボタン
		if ( dijs.rgbButtons[2] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_X] = true;
		//* Ｘボタン
		if ( dijs.rgbButtons[3] & 0x80 )
			m_diJoyState[i][DI_BUTTON_A] = true;
			
		//* Ｙボタン
		if ( dijs.rgbButtons[4] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_L] = true;	
		//* Ｚボタン
		if ( dijs.rgbButtons[5] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Y] = true;
		//* Ｌボタン
		if ( dijs.rgbButtons[6] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_R] = true;
		//* Ｒボタン
		if ( dijs.rgbButtons[7] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Z] = true;

		//* ＳＴＡＲＴボタン
		if ( dijs.rgbButtons[9] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_START] = true;
		//* Ｍボタン
		if ( dijs.rgbButtons[8] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_M] = true;
#endif
	}

}

//-----------------------------------------------------------------------------
// マウス用オブジェクト作成
//-----------------------------------------------------------------------------
bool CDirectInput::InitMouse(HWND hWnd)
{
	HRESULT		hr;

	// マウス用にデバイスオブジェクトを作成
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	if ( FAILED(hr) )
		return false;	// デバイスの作成に失敗

	// データフォーマットを設定
	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if ( FAILED(hr) )
		return false;	// データフォーマットに失敗

	// モードを設定（フォアグラウンド＆非排他モード）
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return false;	// モードの設定に失敗

	// デバイスの設定
	DIPROPDWORD					diprop;
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj			= 0;
	diprop.diph.dwHow			= DIPH_DEVICE;
	diprop.dwData				= DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	hr = m_pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if ( FAILED(hr) )
		return false;	// デバイスの設定に失敗

	// 入力制御開始
	m_pDIMouse->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// 関数名　：　GetMouseState()
// 機能概要：　マウスの状態を取得
//-----------------------------------------------------------------------------
void CDirectInput::GetMouseState(void)
{
	HRESULT			hr;
	DIMOUSESTATE2 dims;
//	hr = m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	for(int i = 0;i < 8;i++)	
	dims.rgbButtons[i] = 0;

	hr = m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &dims);
	if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
		hr = m_pDIMouse->Acquire();
		while ( hr == DIERR_INPUTLOST )
			hr = m_pDIMouse->Acquire();
	}

	m_ExdiLclick = m_diLclick;
}

//------------------------------------------
//		各状態の更新
//------------------------------------------
void CDirectInput::Update()
{
	GetKeybordState();
	GetJoypadState();
	GetMouseState();
}

bool CDirectInput::CheckisKeybordPush(int keycord)
{
	//return NULL;
	return ((m_diKeyState[keycord] & 0x80)&&(m_ExdiKeyState[keycord] & 0x80));
}
bool CDirectInput::CheckisKeybordPushChata(int keycord)
{
	//return NULL;
	return ((m_diKeyState[keycord] & 0x80)&&!(m_ExdiKeyState[keycord] & 0x80));
}

bool CDirectInput::CheckisJoypadPush(int idx,int keycord)
{
	if(m_diJoyCount < idx + 1)
		return false;
	return m_diJoyState[idx][keycord];
}
bool CDirectInput::CheckisJoypadPushChata(int idx,int keycord)
{
	if(m_diJoyCount < idx + 1)
		return false;
	return (m_diJoyState[idx][keycord]&&!m_ExdiJoyState[idx][keycord]);
}