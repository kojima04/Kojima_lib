#include "Common.h"
#include "BumpMap.h"

CPhoneShade::CPhoneShade()
{
   m_pEffect = NULL;
   m_pNormalMap = NULL;
   LightPos = D3DXVECTOR4( 72.0f, 100.0f, 620.0f, 0.0f );
   LightDir = D3DXVECTOR4( -LightPos.x, -LightPos.y, -LightPos.z, 0.0f );
   D3DXVec3Normalize( (D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir );
   m_pTechnique = 0;
//   m_HeightMap = NULL;
}

CPhoneShade::~CPhoneShade()
{
	SAFE_RELEASE( m_pEffect );
}

bool CPhoneShade::SetUp()
{

	return true;
}

void CPhoneShade::Invalidate()
{
   if( m_pEffect )
      m_pEffect->OnLostDevice();
}

void CPhoneShade::Restore()
{
   if( m_pEffect )
      m_pEffect->OnResetDevice();
}

HRESULT CPhoneShade::Load()
{
   D3DCAPS9 caps;

   m_stpD3DDevice->GetDeviceCaps( &caps );
   if( caps.VertexShaderVersion >= D3DVS_VERSION( 1, 1 ) && caps.PixelShaderVersion >= D3DPS_VERSION( 2, 0 ) )
   {
      LPD3DXBUFFER pErr = NULL;
      HRESULT hr = D3DXCreateEffectFromFile( m_stpD3DDevice, "FX/PhongShading.fx", NULL, NULL, 0, NULL, &m_pEffect, &pErr );
      if( SUCCEEDED( hr ) )
      {
         m_pTechnique     = m_pEffect->GetTechniqueByName( "TShader" );
         m_pWVP           = m_pEffect->GetParameterByName( NULL, "m_WVP" );
         m_pLightDir      = m_pEffect->GetParameterByName( NULL, "m_LightDir" );
         m_pEyePos        = m_pEffect->GetParameterByName( NULL, "m_EyePos" );
         m_pAmbient       = m_pEffect->GetParameterByName( NULL, "m_Ambient" );
         m_pSpecularPower = m_pEffect->GetParameterByName( NULL, "m_SpecularPower" );
         m_pSpecular      = m_pEffect->GetParameterByName( NULL, "m_Specular" );

		 if(m_pTechnique == NULL)
		 {
			 int i = 0;
		 }

         m_pEffect->SetTechnique( m_pTechnique );   
      }

      else
      {
         return -1;
      }
   }

   else
   {
      return -2;
   }

   return S_OK;
}

void CPhoneShade::Begin()
{
   if( m_pEffect )
   {
 /*     m_stpD3DDevice->GetTransform( D3DTS_VIEW, &m_matView );
      m_stpD3DDevice->GetTransform( D3DTS_PROJECTION, &m_matProj );*/
	  //m_matView = GetTransformView();
	  //m_matProj = GetTransformProj();
      m_pEffect->Begin( NULL, 0 );
   }
}

void CPhoneShade::BeginPass()
{
   if( m_pEffect )
      m_pEffect->BeginPass(0);   
}

void CPhoneShade::SetAmbient( float Ambient )
{
   if( m_pEffect )
   {
      D3DXVECTOR4 A;
      A = D3DXVECTOR4( Ambient, Ambient, Ambient, 1.0f );
      m_pEffect->SetVector( m_pAmbient, &A );
   }

   else
   {
      D3DMATERIAL9 old_material;
      m_stpD3DDevice->GetMaterial( &old_material );
      old_material.Ambient.r = Ambient;
      old_material.Ambient.g = Ambient;
      old_material.Ambient.b = Ambient;
      old_material.Ambient.a = 1.0f;
      m_stpD3DDevice->SetMaterial( &old_material );
   }
}

void CPhoneShade::SetAmbient( D3DXVECTOR4* pAmbient )
{
   if( m_pEffect )
      m_pEffect->SetVector( m_pAmbient, pAmbient );

   else
   {
      D3DMATERIAL9 old_material;
      m_stpD3DDevice->GetMaterial( &old_material );
      old_material.Ambient.r = pAmbient->x;
      old_material.Ambient.g = pAmbient->y;
      old_material.Ambient.b = pAmbient->z;
      old_material.Ambient.a = pAmbient->w;
      m_stpD3DDevice->SetMaterial( &old_material );
   }
}

void CPhoneShade::SetSpecular( float Specular )
{
   if( m_pEffect )
      m_pEffect->SetFloat( m_pSpecular, Specular );
}

void CPhoneShade::SetSpecularPower( float SpecularPower )
{
   if( m_pEffect )
      m_pEffect->SetFloat( m_pSpecularPower, SpecularPower );
}

void CPhoneShade::SetMatrix( D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir )
{
   if( m_pEffect )
   {
      D3DXMATRIX m, m1;
      D3DXVECTOR4 LightDir;
      D3DXVECTOR4 v;

	  m = (*pMatWorld) * (*GetTransformView()) * (*GetTransformProj());
      m_pEffect->SetMatrix( m_pWVP, &m );

      //カメラ位置
      m1 = (*pMatWorld) * (*GetTransformView());
      D3DXMatrixInverse( &m1, NULL, &m1 );
      D3DXVec4Transform( &v, pCameraPos, &m1 );
      m_pEffect->SetVector( m_pEyePos, &v );

      //Light
      LightDir = *pLightDir;
      D3DXMatrixInverse( &m1, NULL, pMatWorld );
      D3DXVec4Transform( &v, &LightDir, &m1 );
      D3DXVec3Normalize( (D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v );
      m_pEffect->SetVector( m_pLightDir, &v );
   }

   else
      m_stpD3DDevice->SetTransform( D3DTS_WORLD, pMatWorld );
}

void CPhoneShade::EndPass()
{
   if( m_pEffect )
   {
      m_pEffect->EndPass();
   }
}

void CPhoneShade::End()
{
   if( m_pEffect )
   {
      m_pEffect->End();
   }
}

void CPhoneShade::CommitChanges()
{
   if( m_pEffect )
      m_pEffect->CommitChanges();
}

BOOL CPhoneShade::IsOK()
{
   if( m_pEffect )
      return TRUE;

   return FALSE;
}

void CPhoneShade::Draw()
{
	HRESULT hr;
	m_stpD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    m_stpD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    m_stpD3DDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );       
	m_stpD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );       
	m_stpD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );       
	m_stpD3DDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

	  //ワールド座標変換
	D3DXMatrixIdentity( &m_World );
	  m_stpD3DDevice->SetTransform( D3DTS_WORLD, &m_World );

	  //ティーポットのテクスチャーをステージ０にセットする
	  m_stpD3DDevice->SetTexture( 0, *GetTexture(0) );
	  //法線マップをステージ１にセットする
	  m_stpD3DDevice->SetTexture( 1, m_pNormalMap );

	//ID3DXMeshインターフェースを使用する場合必要なし
	//      //頂点シェーダ宣言をバンプマッピング用に設定する
	//      m_pd3dDevice->SetVertexDeclaration( m_pDecl );

	  D3DXVECTOR4 m_pCameraPos = D3DXVECTOR4(m_stpCameraPos->x,m_stpCameraPos->y,m_stpCameraPos->z, 1.0f );
	  
	  for(DWORD i = 0; i < m_NumMaterial; ++i)
	  {

	  //ティーポットをレンダリング
	  Begin();
	  //マテリアルを設定する
	  SetAmbient( 0.0f );
	  SetSpecular( 20.0f );
	  SetSpecularPower( 0.75f );
	  SetMatrix( &m_World, &m_pCameraPos, &LightDir );

		GetDevice()->SetMaterial(&m_pMaterial[i]);
		m_stpD3DDevice->SetTexture( 0,m_pTexture[i]);

		 BeginPass();
		 GetMesh()->DrawSubset(i);
	  
	  //m_pLocalMesh->DrawSubset( 0 );
	     EndPass();
	     End();
	  }

	  m_stpD3DDevice->SetTexture( 1, NULL );
	     
	  hr = m_stpD3DDevice->Present( NULL, NULL, NULL, NULL );
	  
	  //デバイスロストのチェック
	  switch( hr )
	  {
	  //デバイスロスト
	  case D3DERR_DEVICELOST:
		// RenderOK = false;
		 break;

	  //内部ドライバーエラー
	  case D3DERR_DRIVERINTERNALERROR:
		// return FALSE;
		 break;

	  //メソッドの呼び出しが無効です
	  case D3DERR_INVALIDCALL:
		// return FALSE;
		 break;
	  }
   

}

