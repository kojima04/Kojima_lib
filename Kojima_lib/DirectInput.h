#pragma once

#pragma comment(lib, "dinput8.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

/* Joypad��� */

#define DI_BUTTON_UP		0	// �����L�[��(dijs.IY<0)
#define DI_BUTTON_DOWN		1	// �����L�[��(dijs.IY>0)
#define DI_BUTTON_LEFT		2	// �����L�[��(dijs.IX<0)
#define DI_BUTTON_RIGHT	3	// �����L�[�E(dijs.IX>0)
#define DI_BUTTON_A		4	// �`�{�^��(dijs.rgbButtons[0]&0x80)
#define DI_BUTTON_B		5	// �a�{�^��(dijs.rgbButtons[1]&0x80)
#define DI_BUTTON_C		6	// �b�{�^��(dijs.rgbButtons[2]&0x80)
#define DI_BUTTON_X		7	// �w�{�^��(dijs.rgbButtons[3]&0x80)
#define DI_BUTTON_Y		8	// �x�{�^��(dijs.rgbButtons[4]&0x80)
#define DI_BUTTON_Z		9	// �y�{�^��(dijs.rgbButtons[5]&0x80)
#define DI_BUTTON_L		10	// �k�{�^��(dijs.rgbButtons[6]&0x80)
#define DI_BUTTON_R		11	// �q�{�^��(dijs.rgbButtons[7]&0x80)
#define DI_BUTTON_START	12	// �r�s�`�q�s�{�^��(dijs.rgbButtons[8]&0x80)
#define DI_BUTTON_M		13	// �l�{�^��(dijs.rgbButtons[9]&0x80)
#define DI_BUTTON_MAX		14	// �l�{�^��(dijs.rgbButtons[9]&0x80)




#define JOYPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
/* �}�E�X�|�C���^�̏�� */
typedef struct _MSTATE {
    RECT    moveRect;     // ��ʏ�œ�����͈�
    int     x;            // X���W
    int     y;            // Y���W
    bool    lButton;      // ���{�^��
    bool    rButton;      // �E�{�^��
    bool    cButton;      // �^�񒆃{�^��
	bool    exlButton;    // ���{�^��
    bool    exrButton;    // �E�{�^��
    bool    excButton;    // �^�񒆃{�^��	
    int     moveAdd;      // �ړ���
    RECT    imgRect;      // �}�E�X�p�摜��`
    int     imgWidth;     // �}�E�X�摜��
    int     imgHeight;    // �}�E�X�摜����
} MSTATE, *LPMSTATE;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
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

	/* �L�[�{�[�h�p */
	bool InitKeybord(HWND hwnd);
	void GetKeybordState(void);

	/* �W���C�p�b�h�p */
	bool InitJoypad(void);
	void GetJoypadState(void);

	/* �}�E�X�p */
	bool InitMouse(HWND hwnd);
	void GetMouseState(void);
	POINT& GetMouseLocalPos();

	bool m_diJoyState[JOYPADMAX][DI_BUTTON_MAX];
	bool m_ExdiJoyState[JOYPADMAX][DI_BUTTON_MAX];

	int	GetJoyCount(){return m_diJoyCount;}

	bool m_diLclick;
	bool m_ExdiLclick;

	/* �L�[�{�[�h�p */
	BYTE					m_diKeyState[256];
	BYTE					m_ExdiKeyState[256];

protected:

	/* �W���C�p�b�h�p */
	
	int						m_diJoyCount;
	/* �}�E�X�p */
	DIMOUSESTATE			m_diMouseState;

	/* DirectInput���� */
	LPDIRECTINPUT8			m_pDInput;			// DirectInput�I�u�W�F�N�g
	/* �L�[�{�[�h�p */
	LPDIRECTINPUTDEVICE8	m_pDIKeyboard;		// �L�[�{�[�h�f�o�C�X
	/* �W���C�p�b�h�p */
	LPDIRECTINPUTDEVICE8	m_pDIJoypad[JOYPADMAX];// �W���C�p�b�h�f�o�C�X
	/* �}�E�X�p */
	LPDIRECTINPUTDEVICE8	m_pDIMouse;			// �}�E�X�f�o�C�X

	static BOOL CALLBACK	GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, void *pThis);


};