#pragma once

class CArrayList;
class DirectManager;

enum VertexProcessingType
{
    SOFTWARE_VP,
    MIXED_VP,
    HARDWARE_VP,
    PURE_HARDWARE_VP
};

struct D3DAdapterInfo
{
    int AdapterOrdinal;
    D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
    CArrayList* pDisplayModeList; // List of D3DDISPLAYMODEs
    CArrayList* pDeviceInfoList; // List of D3DDeviceInfo pointers
    ~D3DAdapterInfo( void );
};

struct D3DDeviceInfo
{
    int AdapterOrdinal;
    D3DDEVTYPE DevType;
    D3DCAPS9 Caps;
    CArrayList* pDeviceComboList; // List of D3DDeviceCombo pointers
    ~D3DDeviceInfo( void );
};

struct D3DDSMSConflict
{
    D3DFORMAT DSFormat;
    D3DMULTISAMPLE_TYPE MSType;
};

struct D3DDeviceCombo
{
public:
	D3DDeviceCombo();
	~D3DDeviceCombo();
    int AdapterOrdinal;
    D3DDEVTYPE DevType;
    D3DFORMAT AdapterFormat;
    D3DFORMAT BackBufferFormat;
	bool m_bWindowed;
    CArrayList* pDepthStencilFormatList; // List of D3DFORMATs
    CArrayList* pMultiSampleTypeList; // List of D3DMULTISAMPLE_TYPEs
    CArrayList* pMultiSampleQualityList; // List of DWORDs (number of quality 
    CArrayList* pDSMSConflictList; // List of D3DDSMSConflicts
    CArrayList* pVertexProcessingTypeList; // List of VertexProcessingTypes
    CArrayList* pPresentIntervalList; // List of D3DPRESENT_INTERVALs
};

typedef bool(* CONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, 
    VertexProcessingType vertexProcessingType, D3DFORMAT backBufferFormat );

enum ArrayListType
{
    AL_VALUE,       // entry data is copied into the list
    AL_REFERENCE,   // entry pointers are copied into the list
};

class CArrayList
{

protected:
    ArrayListType m_ArrayListType;
    void* m_pData;
    UINT m_BytesPerEntry;
    UINT m_NumEntries;
    UINT m_NumEntriesAllocated;

public:
    CArrayList( ArrayListType Type, UINT BytesPerEntry = 0 );
    ~CArrayList( void );
    HRESULT Add( void* pEntry );
    void Remove( UINT Entry );
    void* GetPtr( UINT Entry );
    UINT Count( void ) { return m_NumEntries; }
    bool Contains( void* pEntryData );
    void Clear( void ) { m_NumEntries = 0; }
};

class CD3DEnumeration
{
public:
	CD3DEnumeration();
    ~CD3DEnumeration();
private:
    HRESULT EnumerateDevices( D3DAdapterInfo* pAdapterInfo, CArrayList* pAdapterFormatList );
    HRESULT EnumerateDeviceCombos( D3DDeviceInfo* pDeviceInfo, CArrayList* pAdapterFormatList );
    void BuildDepthStencilFormatList( D3DDeviceCombo* pDeviceCombo );
    void BuildMultiSampleTypeList( D3DDeviceCombo* pDeviceCombo );
    void BuildDSMSConflictList( D3DDeviceCombo* pDeviceCombo );
   void BuildVertexProcessingTypeList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
    void BuildPresentIntervalList( D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo );
public:
	IDirect3D9* m_pD3D;
    CArrayList* m_pAdapterInfoList;
    CONFIRMDEVICECALLBACK ConfirmDeviceCallback;
    UINT AppMinFullscreenWidth;
    UINT AppMinFullscreenHeight;
    UINT AppMinColorChannelBits; // min color bits per channel in adapter format
    UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
    UINT AppMinDepthBits;
    UINT AppMinStencilBits;
    bool AppUsesDepthBuffer;
    bool AppUsesMixedVP; // whether app can take advantage of mixed vp mode
    bool AppRequiresWindowed;
    bool AppRequiresFullscreen;
	 void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
    CArrayList* m_pAllowedAdapterFormatList; // list of D3DFORMATs
    HRESULT Enumerate(int width,int height);
};

//------------------------------------------------------
//		DirectManager
//------------------------------------------------------
class CDirectManager
{
public:

	CDirectManager();
	~CDirectManager();

	bool	Init(HWND hwnd,long width,long height, bool bWindow);
	void	Release();
	void	ChangeWindowStyle(bool isWindowed, long width, long height);
	bool	BeginScene();		//描画開始
	bool	EndScene();			//描画終了
	void	InitRenderStage();				//テクスチャステージ設定
	LPDIRECT3DDEVICE9	GetDevice(){return m_pD3DDevice;}
	

protected:
    D3DPRESENT_PARAMETERS	m_D3DPP;			//	CreateDevice Param
    HWND					m_hWnd;             //	ウィンドウハンドル
    LPDIRECT3D9				m_pD3D;             //  IDirect3D9インターフェイスへのポインタ
    LPDIRECT3DDEVICE9		m_pD3DDevice;       //	レンダリングデバイス
	long					m_WindowWidth;		//	ウィンドウの幅
    long					m_WindowHeight;		//	ウィンドウの高さ

	static HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT );

    CD3DEnumeration	m_d3dEnumeration;
    BOOL			m_isWindowed;
	struct
	{
		D3DAdapterInfo* pAdapterInfo;
		D3DDeviceInfo* pDeviceInfo;
		D3DDeviceCombo* pDeviceCombo;
		D3DDISPLAYMODE DisplayMode; // not changable by the user
		D3DFORMAT DepthStencilBufferFormat;
		D3DMULTISAMPLE_TYPE MultisampleType;
		DWORD MultisampleQuality;
		VertexProcessingType VertexProcessingType;
		UINT PresentInterval;
	}m_WindowMode,m_FullScreenMode;

    static bool ConfirmDeviceHelper( D3DCAPS9* pCaps, 
        VertexProcessingType vertexProcessingType, D3DFORMAT backBufferFormat );
    VOID    BuildPresentParamsFromSettings();
    BOOL    FindBestWindowedMode(BOOL bRequireHAL, BOOL bRequireREF );
    BOOL    FindBestFullscreenMode(BOOL bRequireHAL, BOOL bRequireREF );
    HRESULT Initialize3DEnvironment();

};