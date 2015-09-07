//=============================================================
// CDXObject.h
// 描画オブジェクトの基底クラスの定義
//=============================================================
#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class CCamera;

class CDXObject
{
public: 
	CDXObject(){};
	virtual ~CDXObject(){};

	static LPDIRECT3DDEVICE9	   m_stpD3DDevice;      
	static D3DPRESENT_PARAMETERS * m_stpD3DPP;	
	static D3DXMATRIX            * m_stpViewMat;
	static D3DXMATRIX            * m_stpProjectionMat;
	static D3DXMATRIX			   m_stScreenMat;
	static D3DXVECTOR3			 * m_stpCameraPos;
	static D3DXVECTOR3			 * m_stpCameraFocs;


//public:

	void SetDevice(LPDIRECT3DDEVICE9 device){m_stpD3DDevice = device;}
	void SetD3DPP(D3DPRESENT_PARAMETERS * para){m_stpD3DPP = para;}
	void SetTransformView(D3DXMATRIX * mat){m_stpViewMat = mat;}
	void SetTransformProj(D3DXMATRIX * mat){m_stpProjectionMat = mat;}
	//void SetTransformScr(D3DXMATRIX * mat){m_stScreenMat = mat;}

	static LPDIRECT3DDEVICE9		GetDevice(){return m_stpD3DDevice;}
	D3DPRESENT_PARAMETERS *	GetD3DPP(){return m_stpD3DPP;}
	D3DXMATRIX			  * GetTransformView(){return m_stpViewMat;}
	D3DXMATRIX			  * GetTransformProj(){return m_stpProjectionMat;}
	D3DXMATRIX			  * GetTransformScr(){return &m_stScreenMat;}


};

