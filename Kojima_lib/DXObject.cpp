#include"Common.h"
#include"DXObject.h"

LPDIRECT3DDEVICE9 CDXObject::m_stpD3DDevice = NULL;
D3DPRESENT_PARAMETERS * CDXObject::m_stpD3DPP = NULL;
D3DXMATRIX             * CDXObject::m_stpViewMat = NULL;
D3DXMATRIX             * CDXObject::m_stpProjectionMat = NULL;
D3DXMATRIX               CDXObject::m_stScreenMat;
D3DXVECTOR3			   * CDXObject::m_stpCameraPos = NULL;
D3DXVECTOR3			   * CDXObject::m_stpCameraFocs = NULL;