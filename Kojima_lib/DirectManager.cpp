#include "Common.h"
#include "DirectManager.h"
#include "DXObject.h"

CDirectManager::CDirectManager()
{

    m_pD3D              = NULL;
    m_pD3DDevice        = NULL;
    m_hWnd              = 0;
    m_isWindowed         = true;
    m_d3dEnumeration.AppUsesDepthBuffer   = true;

}

CDirectManager::~CDirectManager()
{
	Release();
}

/**
*@brief		セットアップ	Important2009
*@return	true : 成功 false : 失敗
*@param	hwnd	ウィンドウハンドル
*@param	width	スクリーンの幅
*@param	height	スクリーンの高さ
*@param	bWindow	フルスクリーンかどうか
*/
bool CDirectManager::Init(HWND hwnd,long width,long height,bool bWindow)
{
	m_isWindowed = !bWindow;		//情報をメンバに格納
	m_WindowWidth = width;
	m_WindowHeight = height;

    //Direct3Dオブジェクトの生成
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if(m_pD3D == NULL)
	{
		return false;
	}

    // Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
    m_d3dEnumeration.SetD3D(m_pD3D);
    m_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
    if(FAILED(m_d3dEnumeration.Enumerate(width,height)))
    {
		SAFE_RELEASE(m_pD3D);
		return false;
    }

	m_hWnd = hwnd;
	//フルスクリーン、ウィンドウモードにおいて最適な環境を探す
    FindBestFullscreenMode(FALSE, FALSE);	
	FindBestWindowedMode(FALSE, FALSE);

    // Initialize the 3D environment for the app
    if(FAILED(Initialize3DEnvironment()))
    {
		SAFE_RELEASE(m_pD3D);
		return false;
    }

	CDXObject::m_stpD3DDevice = m_pD3DDevice;
	CDXObject::m_stpD3DPP = &m_D3DPP;
	D3DXMATRIX mat;

	D3DXMatrixIdentity(&mat);
	CDXObject::m_stScreenMat._11 = (float)m_WindowWidth/2;
	CDXObject::m_stScreenMat._22 = -(float)m_WindowHeight/2;
	CDXObject::m_stScreenMat._33 = 1.0f;
	CDXObject::m_stScreenMat._41 = (float)m_WindowWidth/2;
	CDXObject::m_stScreenMat._42 = (float)m_WindowHeight/2;
	CDXObject::m_stScreenMat._44 = 1.0f;

	//CDXObject::m_sttScreenMat = &mat;
	//ChangeWindowStyle(false,800,600);

	InitRenderStage();

    return true;
}

void CDirectManager::Release()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DDevice);
}

//----------------------------------------
//		描画開始
//----------------------------------------

bool CDirectManager::BeginScene()
{
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
	if(FAILED(hr))
	{
		hr = m_pD3DDevice->Reset(&m_D3DPP);
	}
	m_pD3DDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(0, 0, 0),//	クリアする色 
						1.0f, 0);

	m_pD3DDevice->BeginScene();

	return true;
}

//----------------------------------------
//		描画終了
//----------------------------------------

bool CDirectManager::EndScene()
{	
	m_pD3DDevice->EndScene();

	HRESULT hr;
	hr = m_pD3DDevice->Present(0, 0, 0, 0);

	if(FAILED(hr))
	{
		hr = m_pD3DDevice->Reset(&m_D3DPP);
	}

	return true;
}
//---------------------------------------------------
//		レンダリングステージの設定
//---------------------------------------------------

void CDirectManager::InitRenderStage()
{
	//RGBカラー要素の設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//アルファ要素の設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	// 描画モードの指定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// Ｚ比較を行なう
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	m_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	//アルファブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//半透明処理を行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//透過処理を行う
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	
	//テクスチャフィルタ
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//ライトのデフォルト設定
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);

}

//------------------------------------------
//		ウィンドウスタイルの変更
//-----------------------------------------

void CDirectManager::ChangeWindowStyle(bool isWindowed, long width, long height)
{
	m_isWindowed = isWindowed;

	BuildPresentParamsFromSettings();

	if(FAILED(m_pD3DDevice->Reset(&m_D3DPP)))
	{
		return ;
	}
	
	InitRenderStage();
}


//==============================================================================================
//								global
//==============================================================================================
//-----------------------------------------------------------------------------
// Name: ColorChannelBits
// Desc: Returns the number of color channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT ColorChannelBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_R8G8B8:
            return 8;
        case D3DFMT_A8R8G8B8:
            return 8;
        case D3DFMT_X8R8G8B8:
            return 8;
        case D3DFMT_R5G6B5:
            return 5;
        case D3DFMT_X1R5G5B5:
            return 5;
        case D3DFMT_A1R5G5B5:
            return 5;
        case D3DFMT_A4R4G4B4:
            return 4;
        case D3DFMT_R3G3B2:
            return 2;
        case D3DFMT_A8R3G3B2:
            return 2;
        case D3DFMT_X4R4G4B4:
            return 4;
        case D3DFMT_A2B10G10R10:
            return 10;
        case D3DFMT_A2R10G10B10:
            return 10;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: AlphaChannelBits
// Desc: Returns the number of alpha channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT AlphaChannelBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_R8G8B8:
            return 0;
        case D3DFMT_A8R8G8B8:
            return 8;
        case D3DFMT_X8R8G8B8:
            return 0;
        case D3DFMT_R5G6B5:
            return 0;
        case D3DFMT_X1R5G5B5:
            return 0;
        case D3DFMT_A1R5G5B5:
            return 1;
        case D3DFMT_A4R4G4B4:
            return 4;
        case D3DFMT_R3G3B2:
            return 0;
        case D3DFMT_A8R3G3B2:
            return 8;
        case D3DFMT_X4R4G4B4:
            return 0;
        case D3DFMT_A2B10G10R10:
            return 2;
        case D3DFMT_A2R10G10B10:
            return 2;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: DepthBits
// Desc: Returns the number of depth bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT DepthBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_D16:
            return 16;
        case D3DFMT_D15S1:
            return 15;
        case D3DFMT_D24X8:
            return 24;
        case D3DFMT_D24S8:
            return 24;
        case D3DFMT_D24X4S4:
            return 24;
        case D3DFMT_D32:
            return 32;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: StencilBits
// Desc: Returns the number of stencil bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT StencilBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_D16:
            return 0;
        case D3DFMT_D15S1:
            return 1;
        case D3DFMT_D24X8:
            return 0;
        case D3DFMT_D24S8:
            return 8;
        case D3DFMT_D24X4S4:
            return 4;
        case D3DFMT_D32:
            return 0;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: D3DAdapterInfo destructor
// Desc: 
//-----------------------------------------------------------------------------
D3DAdapterInfo::~D3DAdapterInfo( void )
{
    if( pDisplayModeList != NULL )
        delete pDisplayModeList;
    if( pDeviceInfoList != NULL )
    {
        for( UINT idi = 0; idi < pDeviceInfoList->Count(); idi++ )
            delete (D3DDeviceInfo*)pDeviceInfoList->GetPtr(idi);
        delete pDeviceInfoList;
    }
}

//-----------------------------------------------------------------------------
// Name: D3DDeviceInfo destructor
// Desc: 
//-----------------------------------------------------------------------------
D3DDeviceInfo::~D3DDeviceInfo( void )
{
    if( pDeviceComboList != NULL )
    {
        for( UINT idc = 0; idc < pDeviceComboList->Count(); idc++ )
            delete (D3DDeviceCombo*)pDeviceComboList->GetPtr(idc);
        delete pDeviceComboList;
    }
}

D3DDeviceCombo::D3DDeviceCombo()
{


}

//-----------------------------------------------------------------------------
// Name: D3DDeviceCombo destructor
// Desc: 
//-----------------------------------------------------------------------------
D3DDeviceCombo::~D3DDeviceCombo( void )
{
    if( pDepthStencilFormatList != NULL )
        delete pDepthStencilFormatList;
    if( pMultiSampleTypeList != NULL )
        delete pMultiSampleTypeList;
    if( pMultiSampleQualityList != NULL )
        delete pMultiSampleQualityList;
    if( pDSMSConflictList != NULL )
        delete pDSMSConflictList;
    if( pVertexProcessingTypeList != NULL )
        delete pVertexProcessingTypeList;
    if( pPresentIntervalList != NULL )
        delete pPresentIntervalList;
}

//-----------------------------------------------------------------------------
// Name: CD3DEnumeration constructor
// Desc: 
//-----------------------------------------------------------------------------
CD3DEnumeration::CD3DEnumeration()
{
    m_pAdapterInfoList = NULL;
    m_pAllowedAdapterFormatList = NULL;
    AppMinFullscreenWidth = 800;
    AppMinFullscreenHeight = 600;
    AppMinColorChannelBits = 5;
    AppMinAlphaChannelBits = 0;
    AppMinDepthBits = 15;
    AppMinStencilBits = 0;
    AppUsesDepthBuffer = false;
    AppUsesMixedVP = false;
    AppRequiresWindowed = false;
    AppRequiresFullscreen = false;
}

//-----------------------------------------------------------------------------
// Name: CD3DEnumeration destructor
// Desc: 
//-----------------------------------------------------------------------------
CD3DEnumeration::~CD3DEnumeration()
{
    if(m_pAdapterInfoList != NULL)
    {
        for( UINT iai = 0; iai < m_pAdapterInfoList->Count(); iai++ )
            delete (D3DAdapterInfo*)m_pAdapterInfoList->GetPtr(iai);
        delete m_pAdapterInfoList;
    }

    SAFE_DELETE( m_pAllowedAdapterFormatList );
}

//-----------------------------------------------------------------------------
// Name: SortModesCallback
// Desc: Used to sort D3DDISPLAYMODEs
//-----------------------------------------------------------------------------
static int __cdecl SortModesCallback( const void* arg1, const void* arg2 )
{
    D3DDISPLAYMODE* pdm1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* pdm2 = (D3DDISPLAYMODE*)arg2;

    if (pdm1->Width > pdm2->Width)
        return 1;
    if (pdm1->Width < pdm2->Width)
        return -1;
    if (pdm1->Height > pdm2->Height)
        return 1;
    if (pdm1->Height < pdm2->Height)
        return -1;
    if (pdm1->Format > pdm2->Format)
        return 1;
    if (pdm1->Format < pdm2->Format)
        return -1;
    if (pdm1->RefreshRate > pdm2->RefreshRate)
        return 1;
    if (pdm1->RefreshRate < pdm2->RefreshRate)
        return -1;
    return 0;
}

//-----------------------------------------------------------------------------
// Name: Enumerate
// Desc: Enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------
HRESULT CD3DEnumeration::Enumerate(int width,int height)
{
	AppMinFullscreenWidth = width;
	AppMinFullscreenHeight = height;
    HRESULT hr;
    CArrayList adapterFormatList( AL_VALUE, sizeof(D3DFORMAT) );

    if( m_pD3D == NULL )
        return E_FAIL;

    m_pAdapterInfoList = new CArrayList( AL_REFERENCE );
    if( m_pAdapterInfoList == NULL )
        return E_OUTOFMEMORY;

    m_pAllowedAdapterFormatList = new CArrayList( AL_VALUE, sizeof(D3DFORMAT) );
    if( m_pAllowedAdapterFormatList == NULL )
        return E_OUTOFMEMORY;
    D3DFORMAT fmt;
    if( FAILED( hr = m_pAllowedAdapterFormatList->Add( &( fmt = D3DFMT_X8R8G8B8 ) ) ) )
        return hr;
    if( FAILED( hr = m_pAllowedAdapterFormatList->Add( &( fmt = D3DFMT_X1R5G5B5 ) ) ) )
        return hr;
    if( FAILED( hr = m_pAllowedAdapterFormatList->Add( &( fmt = D3DFMT_R5G6B5 ) ) ) )
        return hr;
    if( FAILED( hr = m_pAllowedAdapterFormatList->Add( &( fmt = D3DFMT_A2R10G10B10 ) ) ) )
        return hr;

    D3DAdapterInfo* pAdapterInfo = NULL;
    UINT numAdapters = m_pD3D->GetAdapterCount();

    for (UINT adapterOrdinal = 0; adapterOrdinal < numAdapters; adapterOrdinal++)
    {
        pAdapterInfo = new D3DAdapterInfo;
        if( pAdapterInfo == NULL )
            return E_OUTOFMEMORY;
        pAdapterInfo->pDisplayModeList = new CArrayList( AL_VALUE, sizeof(D3DDISPLAYMODE)); 
        pAdapterInfo->pDeviceInfoList = new CArrayList( AL_REFERENCE );
        if( pAdapterInfo->pDisplayModeList == NULL ||
            pAdapterInfo->pDeviceInfoList == NULL )
        {
            delete pAdapterInfo;
            return E_OUTOFMEMORY;
        }
        pAdapterInfo->AdapterOrdinal = adapterOrdinal;
        m_pD3D->GetAdapterIdentifier(adapterOrdinal, 0, &pAdapterInfo->AdapterIdentifier);

        // Get list of all display modes on this adapter.  
        // Also build a temporary list of all display adapter formats.
        adapterFormatList.Clear();
        for( UINT iaaf = 0; iaaf < m_pAllowedAdapterFormatList->Count(); iaaf++ )
        {
            D3DFORMAT allowedAdapterFormat = *(D3DFORMAT*)m_pAllowedAdapterFormatList->GetPtr( iaaf );
            UINT numAdapterModes = m_pD3D->GetAdapterModeCount( adapterOrdinal, allowedAdapterFormat );
            for (UINT mode = 0; mode < numAdapterModes; mode++)
            {
                D3DDISPLAYMODE displayMode;
                m_pD3D->EnumAdapterModes( adapterOrdinal, allowedAdapterFormat, mode, &displayMode );
                if( displayMode.Width < AppMinFullscreenWidth ||
                    displayMode.Height < AppMinFullscreenHeight ||
                    ColorChannelBits(displayMode.Format) < AppMinColorChannelBits )
                {
                    continue;
                }
                pAdapterInfo->pDisplayModeList->Add(&displayMode);
                if( !adapterFormatList.Contains( &displayMode.Format ) )
                    adapterFormatList.Add( &displayMode.Format );
            }
        }

        // Sort displaymode list
        qsort( pAdapterInfo->pDisplayModeList->GetPtr(0), 
            pAdapterInfo->pDisplayModeList->Count(), sizeof( D3DDISPLAYMODE ),
            SortModesCallback );

        // Get info for each device on this adapter
        if( FAILED( hr = EnumerateDevices( pAdapterInfo, &adapterFormatList ) ) )
        {
            delete pAdapterInfo;
            return hr;
        }

        // If at least one device on this adapter is available and compatible
        // with the app, add the adapterInfo to the list
        if (pAdapterInfo->pDeviceInfoList->Count() == 0)
            delete pAdapterInfo;
        else
            m_pAdapterInfoList->Add(pAdapterInfo);
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumerateDevices
// Desc: Enumerates D3D devices for a particular adapter.
//-----------------------------------------------------------------------------
HRESULT CD3DEnumeration::EnumerateDevices( D3DAdapterInfo* pAdapterInfo, 
                                           CArrayList* pAdapterFormatList )
{
    const D3DDEVTYPE devTypeArray[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF };
    const UINT devTypeArrayCount = sizeof(devTypeArray) / sizeof(devTypeArray[0]);
    HRESULT hr;

    D3DDeviceInfo* pDeviceInfo = NULL;
    for( UINT idt = 0; idt < devTypeArrayCount; idt++ )
    {
        pDeviceInfo = new D3DDeviceInfo;
        if( pDeviceInfo == NULL )
            return E_OUTOFMEMORY;
        pDeviceInfo->pDeviceComboList = new CArrayList( AL_REFERENCE ); 
        if( pDeviceInfo->pDeviceComboList == NULL )
        {
            delete pDeviceInfo;
            return E_OUTOFMEMORY;
        }
        pDeviceInfo->AdapterOrdinal = pAdapterInfo->AdapterOrdinal;
        pDeviceInfo->DevType = devTypeArray[idt];
        if( FAILED( m_pD3D->GetDeviceCaps( pAdapterInfo->AdapterOrdinal, 
            pDeviceInfo->DevType, &pDeviceInfo->Caps ) ) )
        {
            delete pDeviceInfo;
            continue;
        }

        // Get info for each devicecombo on this device
        if( FAILED( hr = EnumerateDeviceCombos(pDeviceInfo, pAdapterFormatList) ) )
        {
            delete pDeviceInfo;
            return hr;
        }

        // If at least one devicecombo for this device is found, 
        // add the deviceInfo to the list
        if (pDeviceInfo->pDeviceComboList->Count() == 0)
        {
            delete pDeviceInfo;
            continue;
        }
        pAdapterInfo->pDeviceInfoList->Add(pDeviceInfo);
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumerateDeviceCombos
// Desc: Enumerates DeviceCombos for a particular device.
//-----------------------------------------------------------------------------
HRESULT CD3DEnumeration::EnumerateDeviceCombos( D3DDeviceInfo* pDeviceInfo, 
                                               CArrayList* pAdapterFormatList )
{
    const D3DFORMAT backBufferFormatArray[] = 
        {   D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_A2R10G10B10, 
            D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5 };
    const UINT backBufferFormatArrayCount = sizeof(backBufferFormatArray) / sizeof(backBufferFormatArray[0]);
    bool isWindowedArray[] = { false, true };

    // See which adapter formats are supported by this device
    D3DFORMAT adapterFormat;
    for( UINT iaf = 0; iaf < pAdapterFormatList->Count(); iaf++ )
    {
        adapterFormat = *(D3DFORMAT*)pAdapterFormatList->GetPtr(iaf);
        D3DFORMAT backBufferFormat;
        for( UINT ibbf = 0; ibbf < backBufferFormatArrayCount; ibbf++ )
        {
            backBufferFormat = backBufferFormatArray[ibbf];
            if (AlphaChannelBits(backBufferFormat) < AppMinAlphaChannelBits)
                continue;
            bool isWindowed;
            for( UINT iiw = 0; iiw < 2; iiw++)
            {
                isWindowed = isWindowedArray[iiw];
                if (!isWindowed && AppRequiresWindowed)
                    continue;
                if (isWindowed && AppRequiresFullscreen)
                    continue;
                if (FAILED(m_pD3D->CheckDeviceType(pDeviceInfo->AdapterOrdinal, pDeviceInfo->DevType, 
                    adapterFormat, backBufferFormat, isWindowed)))
                {
                    continue;
                }
                // At this point, we have an adapter/device/adapterformat/backbufferformat/iswindowed
                // DeviceCombo that is supported by the system.  We still need to confirm that it's 
                // compatible with the app, and find one or more suitable depth/stencil buffer format,
                // multisample type, vertex processing type, and present interval.
                D3DDeviceCombo* pDeviceCombo = NULL;
                pDeviceCombo = new D3DDeviceCombo;
                if( pDeviceCombo == NULL )
                    return E_OUTOFMEMORY;
                pDeviceCombo->pDepthStencilFormatList = new CArrayList( AL_VALUE, sizeof( D3DFORMAT ) );
                pDeviceCombo->pMultiSampleTypeList = new CArrayList( AL_VALUE, sizeof( D3DMULTISAMPLE_TYPE ) );
                pDeviceCombo->pMultiSampleQualityList = new CArrayList( AL_VALUE, sizeof( DWORD ) );
                pDeviceCombo->pDSMSConflictList = new CArrayList( AL_VALUE, sizeof( D3DDSMSConflict ) );
                pDeviceCombo->pVertexProcessingTypeList = new CArrayList( AL_VALUE, sizeof( VertexProcessingType ) );
                pDeviceCombo->pPresentIntervalList = new CArrayList( AL_VALUE, sizeof( UINT ) );
                if( pDeviceCombo->pDepthStencilFormatList == NULL ||
                    pDeviceCombo->pMultiSampleTypeList == NULL || 
                    pDeviceCombo->pMultiSampleQualityList == NULL || 
                    pDeviceCombo->pDSMSConflictList == NULL || 
                    pDeviceCombo->pVertexProcessingTypeList == NULL ||
                    pDeviceCombo->pPresentIntervalList == NULL )
                {
                    delete pDeviceCombo;
                    return E_OUTOFMEMORY;
                }
                pDeviceCombo->AdapterOrdinal = pDeviceInfo->AdapterOrdinal;
                pDeviceCombo->DevType = pDeviceInfo->DevType;
                pDeviceCombo->AdapterFormat = adapterFormat;
                pDeviceCombo->BackBufferFormat = backBufferFormat;
                pDeviceCombo->m_bWindowed = isWindowed;
                if (AppUsesDepthBuffer)
                {
                    BuildDepthStencilFormatList(pDeviceCombo);
                    if (pDeviceCombo->pDepthStencilFormatList->Count() == 0)
                    {
                        delete pDeviceCombo;
                        continue;
                    }
                }
                BuildMultiSampleTypeList(pDeviceCombo);
                if (pDeviceCombo->pMultiSampleTypeList->Count() == 0)
                {
                    delete pDeviceCombo;
                    continue;
                }
                BuildDSMSConflictList(pDeviceCombo);
                BuildVertexProcessingTypeList(pDeviceInfo, pDeviceCombo);
                if (pDeviceCombo->pVertexProcessingTypeList->Count() == 0)
                {
                    delete pDeviceCombo;
                    continue;
                }
                BuildPresentIntervalList(pDeviceInfo, pDeviceCombo);

                pDeviceInfo->pDeviceComboList->Add(pDeviceCombo);
            }
        }
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: BuildDepthStencilFormatList
// Desc: Adds all depth/stencil formats that are compatible with the device 
//       and app to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void CD3DEnumeration::BuildDepthStencilFormatList( D3DDeviceCombo* pDeviceCombo )
{
    const D3DFORMAT depthStencilFormatArray[] = 
    {
        D3DFMT_D16,
        D3DFMT_D15S1,
        D3DFMT_D24X8,
        D3DFMT_D24S8,
        D3DFMT_D24X4S4,
        D3DFMT_D32,
    };
    const UINT depthStencilFormatArrayCount = sizeof(depthStencilFormatArray) / 
                                              sizeof(depthStencilFormatArray[0]);

    D3DFORMAT depthStencilFmt;
    for( UINT idsf = 0; idsf < depthStencilFormatArrayCount; idsf++ )
    {
        depthStencilFmt = depthStencilFormatArray[idsf];
        if (DepthBits(depthStencilFmt) < AppMinDepthBits)
            continue;
        if (StencilBits(depthStencilFmt) < AppMinStencilBits)
            continue;
        if (SUCCEEDED(m_pD3D->CheckDeviceFormat(pDeviceCombo->AdapterOrdinal, 
            pDeviceCombo->DevType, pDeviceCombo->AdapterFormat, 
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthStencilFmt)))
        {
            if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(pDeviceCombo->AdapterOrdinal, 
                pDeviceCombo->DevType, pDeviceCombo->AdapterFormat, 
                pDeviceCombo->BackBufferFormat, depthStencilFmt)))
            {
                pDeviceCombo->pDepthStencilFormatList->Add(&depthStencilFmt);
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: BuildMultiSampleTypeList
// Desc: Adds all multisample types that are compatible with the device and app to
//       the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void CD3DEnumeration::BuildMultiSampleTypeList( D3DDeviceCombo* pDeviceCombo )
{
    const D3DMULTISAMPLE_TYPE msTypeArray[] = { 
        D3DMULTISAMPLE_NONE,
        D3DMULTISAMPLE_NONMASKABLE,
        D3DMULTISAMPLE_2_SAMPLES,
        D3DMULTISAMPLE_3_SAMPLES,
        D3DMULTISAMPLE_4_SAMPLES,
        D3DMULTISAMPLE_5_SAMPLES,
        D3DMULTISAMPLE_6_SAMPLES,
        D3DMULTISAMPLE_7_SAMPLES,
        D3DMULTISAMPLE_8_SAMPLES,
        D3DMULTISAMPLE_9_SAMPLES,
        D3DMULTISAMPLE_10_SAMPLES,
        D3DMULTISAMPLE_11_SAMPLES,
        D3DMULTISAMPLE_12_SAMPLES,
        D3DMULTISAMPLE_13_SAMPLES,
        D3DMULTISAMPLE_14_SAMPLES,
        D3DMULTISAMPLE_15_SAMPLES,
        D3DMULTISAMPLE_16_SAMPLES,
    };
    const UINT msTypeArrayCount = sizeof(msTypeArray) / sizeof(msTypeArray[0]);

    D3DMULTISAMPLE_TYPE msType;
    DWORD msQuality;
    for( UINT imst = 0; imst < msTypeArrayCount; imst++ )
    {
        msType = msTypeArray[imst];
        if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(pDeviceCombo->AdapterOrdinal, pDeviceCombo->DevType, 
            pDeviceCombo->BackBufferFormat, pDeviceCombo->m_bWindowed, msType, &msQuality)))
        {
            pDeviceCombo->pMultiSampleTypeList->Add(&msType);
            pDeviceCombo->pMultiSampleQualityList->Add( &msQuality );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: BuildDSMSConflictList
// Desc: Find any conflicts between the available depth/stencil formats and
//       multisample types.
//-----------------------------------------------------------------------------
void CD3DEnumeration::BuildDSMSConflictList( D3DDeviceCombo* pDeviceCombo )
{
    D3DDSMSConflict DSMSConflict;

    for( UINT ids = 0; ids < pDeviceCombo->pDepthStencilFormatList->Count(); ids++ )
    {
        D3DFORMAT dsFmt = *(D3DFORMAT*)pDeviceCombo->pDepthStencilFormatList->GetPtr(ids);
        for( UINT ims = 0; ims < pDeviceCombo->pMultiSampleTypeList->Count(); ims++ )
        {
            D3DMULTISAMPLE_TYPE msType = *(D3DMULTISAMPLE_TYPE*)pDeviceCombo->pMultiSampleTypeList->GetPtr(ims);
            if( FAILED( m_pD3D->CheckDeviceMultiSampleType( pDeviceCombo->AdapterOrdinal, pDeviceCombo->DevType,
                dsFmt, pDeviceCombo->m_bWindowed, msType, NULL ) ) )
            {
                DSMSConflict.DSFormat = dsFmt;
                DSMSConflict.MSType = msType;
                pDeviceCombo->pDSMSConflictList->Add( &DSMSConflict );
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: BuildVertexProcessingTypeList
// Desc: Adds all vertex processing types that are compatible with the device 
//       and app to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void CD3DEnumeration::BuildVertexProcessingTypeList( D3DDeviceInfo* pDeviceInfo, 
                                                     D3DDeviceCombo* pDeviceCombo )
{
    VertexProcessingType vpt;
    if ((pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
    {
        if ((pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_PUREDEVICE) != 0)
        {
            if (ConfirmDeviceCallback == NULL ||
                ConfirmDeviceCallback(&pDeviceInfo->Caps, PURE_HARDWARE_VP, pDeviceCombo->BackBufferFormat))
            {
                vpt = PURE_HARDWARE_VP;
                pDeviceCombo->pVertexProcessingTypeList->Add(&vpt);
            }
        }
        if (ConfirmDeviceCallback == NULL ||
            ConfirmDeviceCallback(&pDeviceInfo->Caps, HARDWARE_VP, pDeviceCombo->BackBufferFormat))
        {
            vpt = HARDWARE_VP;
            pDeviceCombo->pVertexProcessingTypeList->Add(&vpt);
        }
        if (AppUsesMixedVP && (ConfirmDeviceCallback == NULL ||
            ConfirmDeviceCallback(&pDeviceInfo->Caps, MIXED_VP, pDeviceCombo->BackBufferFormat)))
        {
            vpt = MIXED_VP;
            pDeviceCombo->pVertexProcessingTypeList->Add(&vpt);
        }
    }
    if (ConfirmDeviceCallback == NULL ||
        ConfirmDeviceCallback(&pDeviceInfo->Caps, SOFTWARE_VP, pDeviceCombo->BackBufferFormat))
    {
        vpt = SOFTWARE_VP;
        pDeviceCombo->pVertexProcessingTypeList->Add(&vpt);
    }
}

//-----------------------------------------------------------------------------
// Name: BuildPresentIntervalList	Important2009
// Desc: Adds all present intervals that are compatible with the device and app 
//       to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void CD3DEnumeration::BuildPresentIntervalList( D3DDeviceInfo* pDeviceInfo, 
                                                D3DDeviceCombo* pDeviceCombo )
{
    const UINT piArray[] = { 		
		D3DPRESENT_INTERVAL_IMMEDIATE,
		D3DPRESENT_INTERVAL_DEFAULT,
        D3DPRESENT_INTERVAL_ONE,
        D3DPRESENT_INTERVAL_TWO,
        D3DPRESENT_INTERVAL_THREE,
        D3DPRESENT_INTERVAL_FOUR,
    };
    const UINT piArrayCount = sizeof(piArray) / sizeof(piArray[0]);

    UINT pi;
    for( UINT ipi = 0; ipi < piArrayCount; ipi++ )
    {
        pi = piArray[ipi];
        if( pDeviceCombo->m_bWindowed )
        {
            if( pi == D3DPRESENT_INTERVAL_TWO ||
                pi == D3DPRESENT_INTERVAL_THREE ||
                pi == D3DPRESENT_INTERVAL_FOUR )
            {
                // These intervals are not supported in windowed mode.
                continue;
            }
        }
        // Note that D3DPRESENT_INTERVAL_DEFAULT is zero, so you
        // can't do a caps check for it -- it is always available.
        if( pi == D3DPRESENT_INTERVAL_DEFAULT ||
            (pDeviceInfo->Caps.PresentationIntervals & pi) )
        {
            pDeviceCombo->pPresentIntervalList->Add( &pi );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: CArrayList constructor
// Desc: 
//-----------------------------------------------------------------------------
CArrayList::CArrayList( ArrayListType Type, UINT BytesPerEntry )
{
    if( Type == AL_REFERENCE )
        BytesPerEntry = sizeof(void*);
    m_ArrayListType = Type;
    m_pData = NULL;
    m_BytesPerEntry = BytesPerEntry;
    m_NumEntries = 0;
    m_NumEntriesAllocated = 0;
}

//-----------------------------------------------------------------------------
// Name: CArrayList destructor
// Desc: 
//-----------------------------------------------------------------------------
CArrayList::~CArrayList( void )
{
    if( m_pData != NULL )
        delete[] m_pData;
}

//-----------------------------------------------------------------------------
// Name: CArrayList::Add
// Desc: Adds pEntry to the list.
//-----------------------------------------------------------------------------
HRESULT CArrayList::Add( void* pEntry )
{
    if( m_BytesPerEntry == 0 )
        return E_FAIL;
    if( m_pData == NULL || m_NumEntries + 1 > m_NumEntriesAllocated )
    {
        void* pDataNew;
        UINT NumEntriesAllocatedNew;
        if( m_NumEntriesAllocated == 0 )
            NumEntriesAllocatedNew = 16;
        else
            NumEntriesAllocatedNew = m_NumEntriesAllocated * 2;
        pDataNew = new BYTE[NumEntriesAllocatedNew * m_BytesPerEntry];
        if( pDataNew == NULL )
            return E_OUTOFMEMORY;
        if( m_pData != NULL )
        {
            CopyMemory( pDataNew, m_pData, m_NumEntries * m_BytesPerEntry );
            delete[] m_pData;
        }
        m_pData = pDataNew;
        m_NumEntriesAllocated = NumEntriesAllocatedNew;
    }

    if( m_ArrayListType == AL_VALUE )
        CopyMemory( (BYTE*)m_pData + (m_NumEntries * m_BytesPerEntry), pEntry, m_BytesPerEntry );
    else
        *(((void**)m_pData) + m_NumEntries) = pEntry;
    m_NumEntries++;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CArrayList::Remove
// Desc: Remove the item at Entry in the list, and collapse the array. 
//-----------------------------------------------------------------------------
void CArrayList::Remove( UINT Entry )
{
    // Decrement count
    m_NumEntries--;

    // Find the entry address
    BYTE* pData = (BYTE*)m_pData + (Entry * m_BytesPerEntry);

    // Collapse the array
    MoveMemory( pData, pData + m_BytesPerEntry, ( m_NumEntries - Entry ) * m_BytesPerEntry );
}

//-----------------------------------------------------------------------------
// Name: CArrayList::GetPtr
// Desc: Returns a pointer to the Entry'th entry in the list.
//-----------------------------------------------------------------------------
void* CArrayList::GetPtr( UINT Entry )
{
    if( m_ArrayListType == AL_VALUE )
        return (BYTE*)m_pData + (Entry * m_BytesPerEntry);
    else
        return *(((void**)m_pData) + Entry);
}

//-----------------------------------------------------------------------------
// Name: CArrayList::Contains
// Desc: Returns whether the list contains an entry identical to the 
//       specified entry data.
//-----------------------------------------------------------------------------
bool CArrayList::Contains( void* pEntryData )
{
    for( UINT iEntry = 0; iEntry < m_NumEntries; iEntry++ )
    {
        if( m_ArrayListType == AL_VALUE )
        {
            if( memcmp( GetPtr(iEntry), pEntryData, m_BytesPerEntry ) == 0 )
                return true;
        }
        else
        {
            if( GetPtr(iEntry) == pEntryData )
                return true;
        }
    }
    return false;
}


/**
*@brief	ウィンドウモードでの最適なモードを探す
*@return	TRUE : 成功 FALSE : 失敗
*/
BOOL CDirectManager::FindBestWindowedMode(BOOL bRequireHAL, BOOL bRequireREF )
{
    // Get display mode of primary adapter (which is assumed to be where the window 
    // will appear)
    D3DDISPLAYMODE primaryDesktopDisplayMode;
    m_pD3D->GetAdapterDisplayMode(0, &primaryDesktopDisplayMode);

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo* pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
    {
        D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
        for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
        {
            D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
            if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
                continue;
            if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
                continue;
            for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
            {
                D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
                bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
                if (!pDeviceCombo->m_bWindowed)
                    continue;
                if (pDeviceCombo->AdapterFormat != primaryDesktopDisplayMode.Format)
                    continue;
                // If we haven't found a compatible DeviceCombo yet, or if this set
                // is better (because it's a HAL, and/or because formats match better),
                // save it
                if( pBestDeviceCombo == NULL || 
                    pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceCombo->DevType == D3DDEVTYPE_HAL ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
                {
                    pBestAdapterInfo = pAdapterInfo;
                    pBestDeviceInfo = pDeviceInfo;
                    pBestDeviceCombo = pDeviceCombo;
                    if( pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
                    {
                        // This windowed device combo looks great -- take it
                        goto EndWindowedDeviceComboSearch;
                    }
                    // Otherwise keep looking for a better windowed device combo
                }
            }
        }
    }
EndWindowedDeviceComboSearch:
    if (pBestDeviceCombo == NULL )
        return FALSE;

	m_WindowMode.pAdapterInfo = pBestAdapterInfo;
    m_WindowMode.pDeviceInfo = pBestDeviceInfo;
    m_WindowMode.pDeviceCombo = pBestDeviceCombo;
    m_WindowMode.DisplayMode = primaryDesktopDisplayMode;
    if (m_d3dEnumeration.AppUsesDepthBuffer)
         m_WindowMode.DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
    m_WindowMode.MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
    m_WindowMode.MultisampleQuality = 0;
	m_WindowMode.VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
    m_WindowMode.PresentInterval = *(UINT*)pBestDeviceCombo->pPresentIntervalList->GetPtr(0);
    return TRUE;
}

/**
*@brief		フルスクリーンでの最適な環境を探す
*@return	TRUE : 成功 FALSE : 失敗
*/
BOOL CDirectManager::FindBestFullscreenMode(BOOL bRequireHAL,BOOL bRequireREF )
{
    // For fullscreen, default to first HAL DeviceCombo that supports the current desktop 
    // display mode, or any display mode if HAL is not compatible with the desktop mode, or 
    // non-HAL if no HAL is available
    D3DDISPLAYMODE adapterDesktopDisplayMode;
    D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
    D3DDISPLAYMODE bestDisplayMode;
    bestAdapterDesktopDisplayMode.Width = 0;
    bestAdapterDesktopDisplayMode.Height = 0;
    bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
    bestAdapterDesktopDisplayMode.RefreshRate = 0;

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo* pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
    {
        D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
        m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
        for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
        {
            D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
            if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
                continue;
            if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
                continue;
            for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
            {
                D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
                bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
                bool bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
                if (pDeviceCombo->m_bWindowed)
                    continue;
                // If we haven't found a compatible set yet, or if this set
                // is better (because it's a HAL, and/or because formats match better),
                // save it
                if (pBestDeviceCombo == NULL ||
                    pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB )
                {
                    bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
                    pBestAdapterInfo = pAdapterInfo;
                    pBestDeviceInfo = pDeviceInfo;
                    pBestDeviceCombo = pDeviceCombo;
                    if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
                    {
                        // This fullscreen device combo looks great -- take it
                        goto EndFullscreenDeviceComboSearch;
                    }
                    // Otherwise keep looking for a better fullscreen device combo
                }
            }
        }
    }
EndFullscreenDeviceComboSearch:
    if (pBestDeviceCombo == NULL)
        return FALSE;

    // Need to find a display mode on the best adapter that uses pBestDeviceCombo->AdapterFormat
    // and is as close to bestAdapterDesktopDisplayMode's res as possible
    bestDisplayMode.Width = 0;
    bestDisplayMode.Height = 0;
    bestDisplayMode.Format = D3DFMT_UNKNOWN;
    bestDisplayMode.RefreshRate = 0;
    for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
    {
        D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
        if( pdm->Format != pBestDeviceCombo->AdapterFormat )
            continue;
        if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
            pdm->Height == bestAdapterDesktopDisplayMode.Height && 
            pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
        {
            // found a perfect match, so stop
            bestDisplayMode = *pdm;
            break;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
                 pdm->Height == bestAdapterDesktopDisplayMode.Height && 
                 pdm->RefreshRate > bestDisplayMode.RefreshRate )
        {
            // refresh rate doesn't match, but width/height match, so keep this
            // and keep looking
            bestDisplayMode = *pdm;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
        {
            // width matches, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
        else if( bestDisplayMode.Width == 0 )
        {
            // we don't have anything better yet, so keep this and keep looking
            bestDisplayMode = *pdm;
        }
    }

    m_FullScreenMode.pAdapterInfo = pBestAdapterInfo;
    m_FullScreenMode.pDeviceInfo = pBestDeviceInfo;
    m_FullScreenMode.pDeviceCombo = pBestDeviceCombo;
    m_FullScreenMode.DisplayMode = bestDisplayMode;
    if (m_d3dEnumeration.AppUsesDepthBuffer)
        m_FullScreenMode.DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
    m_FullScreenMode.MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
    m_FullScreenMode.MultisampleQuality = 0;
    m_FullScreenMode.VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
    m_FullScreenMode.PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
    return TRUE;
}


//=============================================================
//デバイスの初期化を最適な状態で作成します。
//=============================================================
HRESULT CDirectManager::Initialize3DEnvironment()
{
    HRESULT hr;

    D3DDeviceInfo* pDeviceInfo = m_isWindowed ? 
		m_WindowMode.pDeviceInfo : m_FullScreenMode.pDeviceInfo;

    // Set up the presentation parameters
    BuildPresentParamsFromSettings();

    if( pDeviceInfo->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
    {
        // Warn user about null ref device that can't render anything
	 return E_FAIL;
    }

    DWORD behaviorFlags;
	VertexProcessingType type = m_isWindowed ? m_WindowMode.VertexProcessingType : m_FullScreenMode.VertexProcessingType;
    if (type == SOFTWARE_VP)
        behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (type == MIXED_VP)
        behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (type == HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (type == PURE_HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        behaviorFlags = 0; // TODO: throw exception

    // Create the device

	D3DDeviceCombo* pCombo = m_isWindowed ? m_WindowMode.pDeviceCombo : m_FullScreenMode.pDeviceCombo;
    hr = m_pD3D->CreateDevice(pCombo->AdapterOrdinal, pDeviceInfo->DevType,
                               m_hWnd, behaviorFlags | D3DCREATE_MULTITHREADED, &m_D3DPP,
                               &m_pD3DDevice );
	
    if( SUCCEEDED(hr) )
    {
		return S_OK;
    }

	const int D3DAPPERR_MEDIANOTFOUND  = 0x8200000b;
    // If that failed, fall back to the reference rasterizer
    if( hr !=  D3DAPPERR_MEDIANOTFOUND && 
        hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) && 
        pDeviceInfo->DevType == D3DDEVTYPE_HAL )
    {
        if (FindBestWindowedMode(false, true))
        {
            m_isWindowed = true;
            // Make sure main window isn't topmost, so error message is visible

            // Let the user know we are switching from HAL to the reference rasterizer
		 return E_FAIL;
        }
    }
    return hr;
}

/**
*@brief		d3ppのセットアップ
*@return	
*/
VOID CDirectManager::BuildPresentParamsFromSettings()
{
    m_D3DPP.Windowed               = m_isWindowed;
    m_D3DPP.BackBufferCount        = 1;
    m_D3DPP.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_D3DPP.EnableAutoDepthStencil = m_d3dEnumeration.AppUsesDepthBuffer;
    m_D3DPP.hDeviceWindow          = m_hWnd;
    if(m_d3dEnumeration.AppUsesDepthBuffer)
    {
        m_D3DPP.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        m_D3DPP.AutoDepthStencilFormat = m_isWindowed ? 
			m_WindowMode.DepthStencilBufferFormat : m_FullScreenMode.DepthStencilBufferFormat;;
    }
    else
    {
        m_D3DPP.Flags              = 0;
    }

	if(m_isWindowed)
    {
        m_D3DPP.BackBufferWidth  = m_WindowWidth;
        m_D3DPP.BackBufferHeight = m_WindowHeight;
        m_D3DPP.BackBufferFormat = m_WindowMode.pDeviceCombo->BackBufferFormat;
        m_D3DPP.FullScreen_RefreshRateInHz = 0;
        m_D3DPP.PresentationInterval = m_WindowMode.PresentInterval;
		m_D3DPP.MultiSampleType        = m_WindowMode.MultisampleType;
		m_D3DPP.MultiSampleQuality     = m_WindowMode.MultisampleQuality;
    }
    else
    {
        m_D3DPP.BackBufferWidth  = m_FullScreenMode.DisplayMode.Width;
        m_D3DPP.BackBufferHeight = m_FullScreenMode.DisplayMode.Height;
        m_D3DPP.BackBufferFormat = m_FullScreenMode.pDeviceCombo->BackBufferFormat;
        m_D3DPP.FullScreen_RefreshRateInHz = m_FullScreenMode.DisplayMode.RefreshRate;
        m_D3DPP.PresentationInterval = m_FullScreenMode.PresentInterval;
		m_D3DPP.MultiSampleType        = m_FullScreenMode.MultisampleType;
		m_D3DPP.MultiSampleQuality     = m_FullScreenMode.MultisampleQuality;
    }
}

/**
*@brief		デバイスの確認
*@return	
*/
bool CDirectManager::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
                         D3DFORMAT backBufferFormat )
{
    DWORD dwBehavior;

    if (vertexProcessingType == SOFTWARE_VP)
        dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == MIXED_VP)
        dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (vertexProcessingType == HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == PURE_HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        dwBehavior = 0; // TODO: throw exception
    
    return SUCCEEDED(ConfirmDevice(pCaps, dwBehavior, backBufferFormat));
}

/**
*@brief	デバイスの確認
*@return	S_OK : 成功 E_FAIL : 失敗
*@param	pCaps	デバイスの性能
*@param	dwBehavior
*@param	Format	
*/
HRESULT CDirectManager::ConfirmDevice(D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format)
{
    UNREFERENCED_PARAMETER( Format );
    UNREFERENCED_PARAMETER( dwBehavior );
    UNREFERENCED_PARAMETER( pCaps );
    
    BOOL bCapsAcceptable;

    // TODO: Perform checks to see if these display caps are acceptable.
    bCapsAcceptable = TRUE;

    if(bCapsAcceptable)         
        return S_OK;
    else
	{
        return E_FAIL;
	}
}
