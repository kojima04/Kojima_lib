#include"DrawObject.h"
#include"Model.h"

class CPhoneShade : public CModel
{
protected:
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE	 m_pTechnique;
	D3DXHANDLE   m_pWVP;
	D3DXHANDLE   m_pLightDir;
	D3DXHANDLE   m_pEyePos;
	D3DXHANDLE   m_pAmbient;
	D3DXHANDLE   m_pSpecular;
	D3DXHANDLE   m_pSpecularPower;
	D3DXMATRIX	 m_matView, m_matProj;

//	LPDIRECT3DTEXTURE9 m_HeightMap;

	//平行光源の光の方向ベクトル
	D3DXVECTOR4  LightDir;
	//太陽の位置ベクトル
	D3DXVECTOR4  LightPos; 
		
	LPDIRECT3DTEXTURE9 m_pNormalMap;//法線マップ
	//LPDIRECT3DDEVICE9 m_pd3dDevice;

public:
	CPhoneShade();
	~CPhoneShade();
	bool SetUp();
	void Invalidate();
	void Restore();
	HRESULT Load();
	void Begin();
	void BeginPass();
	void SetAmbient( float Ambient );
	void SetAmbient( D3DXVECTOR4* pAmbient );
	void SetSpecular( float Specular );
	void SetSpecularPower( float SpecularPower );
	void SetMatrix( D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir );
	void EndPass();
	void End();
	void CommitChanges();
	BOOL IsOK();
	LPD3DXEFFECT GetEffect(){ return m_pEffect; };
	void Draw();
};

