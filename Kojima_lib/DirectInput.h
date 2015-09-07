#pragma once

#pragma comment(lib, "dinput8.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

/* Joypad情報 */

#define DI_BUTTON_UP		0	// 方向キー上(dijs.IY<0)
#define DI_BUTTON_DOWN		1	// 方向キー下(dijs.IY>0)
#define DI_BUTTON_LEFT		2	// 方向キー左(dijs.IX<0)
#define DI_BUTTON_RIGHT	3	// 方向キー右(dijs.IX>0)
#define DI_BUTTON_A		4	// Ａボタン(dijs.rgbButtons[0]&0x80)
#define DI_BUTTON_B		5	// Ｂボタン(dijs.rgbButtons[1]&0x80)
#define DI_BUTTON_C		6	// Ｃボタン(dijs.rgbButtons[2]&0x80)
#define DI_BUTTON_X		7	// Ｘボタン(dijs.rgbButtons[3]&0x80)
#define DI_BUTTON_Y		8	// Ｙボタン(dijs.rgbButtons[4]&0x80)
#define DI_BUTTON_Z		9	// Ｚボタン(dijs.rgbButtons[5]&0x80)
#define DI_BUTTON_L		10	// Ｌボタン(dijs.rgbButtons[6]&0x80)
#define DI_BUTTON_R		11	// Ｒボタン(dijs.rgbButtons[7]&0x80)
#define DI_BUTTON_START	12	// ＳＴＡＲＴボタン(dijs.rgbButtons[8]&0x80)
#define DI_BUTTON_M		13	// Ｍボタン(dijs.rgbButtons[9]&0x80)
#define DI_BUTTON_MAX		14	// Ｍボタン(dijs.rgbButtons[9]&0x80)




#define JOYPADMAX		4			// 同時に接続するジョイパッドの最大数をセット
//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
/* マウスポインタの情報 */
typedef struct _MSTATE {
    RECT    moveRect;     // 画面上で動ける範囲
    int     x;            // X座標
    int     y;            // Y座標
    bool    lButton;      // 左ボタン
    bool    rButton;      // 右ボタン
    bool    cButton;      // 真ん中ボタン
	bool    exlButton;    // 左ボタン
    bool    exrButton;    // 右ボタン
    bool    excButton;    // 真ん中ボタン	
    int     moveAdd;      // 移動量
    RECT    imgRect;      // マウス用画像矩形
    int     imgWidth;     // マウス画像幅
    int     imgHeight;    // マウス画像高さ
} MSTATE, *LPMSTATE;

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------

class CDirectInput
{
public:
	CDirectInput();
	~CDirectInput();

	bool Init(HWND hwnd);
	void Release();
	void Update();

	bool CheckisKeybordPush(int keycord);
	bool CheckisKeybordPushChata(int keycord);

	bool CheckisJoypadPush(int idx,int keycord);
	bool CheckisJoypadPushChata(int idx,int keycord);

	/* キーボード用 */
	bool InitKeybord(HWND hwnd);
	void GetKeybordState(void);

	/* ジョイパッド用 */
	bool InitJoypad(void);
	void GetJoypadState(void);

	/* マウス用 */
	bool InitMouse(HWND hwnd);
	void GetMouseState(void);
	POINT& GetMouseLocalPos();

	bool m_diJoyState[JOYPADMAX][DI_BUTTON_MAX];
	bool m_ExdiJoyState[JOYPADMAX][DI_BUTTON_MAX];

	int	GetJoyCount(){return m_diJoyCount;}

	bool m_diLclick;
	bool m_ExdiLclick;

	/* キーボード用 */
	BYTE					m_diKeyState[256];
	BYTE					m_ExdiKeyState[256];

protected:

	/* ジョイパッド用 */
	
	int						m_diJoyCount;
	/* マウス用 */
	DIMOUSESTATE			m_diMouseState;

	/* DirectInput共通 */
	LPDIRECTINPUT8			m_pDInput;			// DirectInputオブジェクト
	/* キーボード用 */
	LPDIRECTINPUTDEVICE8	m_pDIKeyboard;		// キーボードデバイス
	/* ジョイパッド用 */
	LPDIRECTINPUTDEVICE8	m_pDIJoypad[JOYPADMAX];// ジョイパッドデバイス
	/* マウス用 */
	LPDIRECTINPUTDEVICE8	m_pDIMouse;			// マウスデバイス

	static BOOL CALLBACK	GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, void *pThis);


};