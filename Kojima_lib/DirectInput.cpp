#include "Common.h"
#include "DirectInput.h"

// Joypad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

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
// Direct Input ������
//-----------------------------------------------------------------------------
bool CDirectInput::Init(HWND hWnd)
{
	HRESULT		hr;

	hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);
	if ( FAILED(hr) )
	{
		return false;	// DirectInput8�̍쐬�Ɏ��s
	}

	//�L�[�{�[�h�̏�����
	if(InitKeybord(hWnd) == false)
	{
		return false;
	}
	//�W���C�p�b�h�̏�����
	if(InitJoypad() == false)
	{
		return false;
	}
	//�}�E�X�̏�����
	if(InitMouse(hWnd) == false)
	{
		return false;
	}


	return true;

}

//-----------------------------------------------------------------------------
// Direct Input �J������
//-----------------------------------------------------------------------------
void CDirectInput::Release(void)
{
	// DirectInput�̃f�o�C�X���J��
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
// �L�[�{�[�h�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
bool CDirectInput::InitKeybord(HWND hWnd)
{
	HRESULT		hr;

	//�L�[�{�[�h�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL); 
	if ( FAILED(hr) )
		return false;  // �f�o�C�X�̍쐬�Ɏ��s

	//�L�[�{�[�h�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	hr = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	
	if ( FAILED(hr) )
		return false; // �f�o�C�X�̍쐬�Ɏ��s

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return false; // ���[�h�̐ݒ�Ɏ��s

	//�L�[�{�[�h���͐���J�n
	m_pDIKeyboard->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetKeyboardState()
// �@�\�T�v�F�@�L�[�{�[�h�̏�Ԃ��擾
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
//	�����̃W���C�X�e�B�b�N�E�f�o�C�X��񋓁i�R�[���o�b�N�֐��j
//-----------------------------------------------------------------------------
BOOL CALLBACK CDirectInput::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,void *pContext)
{
	CDirectInput *pThis = (CDirectInput *)pContext;
	HRESULT hr;

	hr = pThis->m_pDInput->CreateDevice(lpddi->guidInstance, &pThis->m_pDIJoypad[pThis->m_diJoyCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}



//-----------------------------------------------------------------------------
//  ������Joypad�p�I�u�W�F�N�g���쐬
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoypad(void)
{
	HRESULT		hr;
	int			i;

	// �W���C�p�b�h��T��
	m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, (void *) this, DIEDFL_ATTACHEDONLY);

//	m_diJoyCount = 2;
	for ( i=0 ; i<m_diJoyCount ; i++ ) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		hr = m_pDIJoypad[i]->SetDataFormat(&c_dfDIJoystick);
		if ( FAILED(hr) )
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

		// ���̒l�͈̔͂�ݒ�
		// X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
		// (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
		// ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�́A�֌W�Ȃ�)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize		= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow		= DIPH_BYOFFSET; 
		diprg.lMin				= RANGE_MIN;
		diprg.lMax				= RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_X; 
		m_pDIJoypad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj		= DIJOFS_Y;
		m_pDIJoypad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_X;
		m_pDIJoypad[i]->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj		= DIJOFS_Y;
		m_pDIJoypad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			
		//�W���C�X�e�B�b�N���͐���J�n
		m_pDIJoypad[i]->Acquire();
	}
		
	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetJoypadState2()
// �@�\�T�v�F�@�����̃W���C�p�b�h�̏�Ԃ��擾
//-----------------------------------------------------------------------------
void CDirectInput::GetJoypadState(void)
{
	HRESULT			hr = 0;
	int				i;

	for ( i=0 ; i<m_diJoyCount ; i++ ) 
		for(int j = 0;j < DI_BUTTON_MAX;j++)
		{
			m_ExdiJoyState[i][j] = m_diJoyState[i][j];
			m_diJoyState[i][j] = false;	// ������
		}

	for ( i=0 ; i<m_diJoyCount ; i++ ) 
	{		
		m_pDIJoypad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if ( FAILED(hr) ) {
			hr = m_pDIJoypad[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = m_pDIJoypad[i]->Acquire();
		}

		DIJOYSTATE2		dijs;

		hr = m_pDIJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			hr = m_pDIJoypad[i]->Acquire();
			while ( hr == DIERR_INPUTLOST )
				hr = m_pDIJoypad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
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

		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_A] = true;
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_B] = true;			
		//* �b�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_C] = true;			
		//* �w�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_X] = true;		
		//* �x�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Y] = true;		
		//* �y�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Z] = true;		
		//* �k�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_L] = true;		
		//* �q�{�^��
		if ( dijs.rgbButtons[7] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_R] = true;
		//* �r�s�`�q�s�{�^��
		if ( dijs.rgbButtons[8] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_START] = true;
		//* �l�{�^��
		if ( dijs.rgbButtons[9] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_M] = true;
#endif		
		
		
#ifdef BUSITSU	
		//* �`�{�^��
		if ( dijs.rgbButtons[0] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_C] = true;
		//* �a�{�^��
		if ( dijs.rgbButtons[1] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_B] = true;
		//* �b�{�^��
		if ( dijs.rgbButtons[2] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_X] = true;
		//* �w�{�^��
		if ( dijs.rgbButtons[3] & 0x80 )
			m_diJoyState[i][DI_BUTTON_A] = true;
			
		//* �x�{�^��
		if ( dijs.rgbButtons[4] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_L] = true;	
		//* �y�{�^��
		if ( dijs.rgbButtons[5] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Y] = true;
		//* �k�{�^��
		if ( dijs.rgbButtons[6] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_R] = true;
		//* �q�{�^��
		if ( dijs.rgbButtons[7] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_Z] = true;

		//* �r�s�`�q�s�{�^��
		if ( dijs.rgbButtons[9] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_START] = true;
		//* �l�{�^��
		if ( dijs.rgbButtons[8] & 0x80 )	
			m_diJoyState[i][DI_BUTTON_M] = true;
#endif
	}

}

//-----------------------------------------------------------------------------
// �}�E�X�p�I�u�W�F�N�g�쐬
//-----------------------------------------------------------------------------
bool CDirectInput::InitMouse(HWND hWnd)
{
	HRESULT		hr;

	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);
	if ( FAILED(hr) )
		return false;	// �f�o�C�X�̍쐬�Ɏ��s

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if ( FAILED(hr) )
		return false;	// �f�[�^�t�H�[�}�b�g�Ɏ��s

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if ( FAILED(hr) )
		return false;	// ���[�h�̐ݒ�Ɏ��s

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD					diprop;
	diprop.diph.dwSize			= sizeof(diprop);
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph);
	diprop.diph.dwObj			= 0;
	diprop.diph.dwHow			= DIPH_DEVICE;
	diprop.dwData				= DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	hr = m_pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if ( FAILED(hr) )
		return false;	// �f�o�C�X�̐ݒ�Ɏ��s

	// ���͐���J�n
	m_pDIMouse->Acquire();

	return true;

}

//-----------------------------------------------------------------------------
// �֐����@�F�@GetMouseState()
// �@�\�T�v�F�@�}�E�X�̏�Ԃ��擾
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
//		�e��Ԃ̍X�V
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