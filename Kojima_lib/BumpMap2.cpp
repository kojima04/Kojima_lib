#include "Common.h"
#include "BumpMap2.h"


BUMPMAP::BUMPMAP( LPDIRECT3DDEVICE9 pd3dDevice )
{
	m_pEffect = NULL;
	m_pNormalMap = NULL;
	LightPos = D3DXVECTOR4( 72.0f, 100.0f, 620.0f, 0.0f );
	LightDir = D3DXVECTOR4( -LightPos.x, -LightPos.y, -LightPos.z, 0.0f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir );
	m_pTechnique = 0;
	m_pDecl = NULL;

	//頂点データを定義する(詳細はオンラインマニュアルを参照)
	D3DVERTEXELEMENT9 decl[] =
	{
	  {0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	  {0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
	  {0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
	  {0, 36, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	  {0, 48, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	  D3DDECL_END()
	};

	wchar_t name[24] = L"ball.x";
	//m_pMesh = new CDXUTMesh();
	//m_pMesh->Create( m_spD3DDevice,name);

	//新しい頂点情報に基づき頂点データを再生成する
	//SetVertexDecl( m_spD3DDevice, decl );

	m_spD3DDevice->CreateVertexDeclaration( decl, &m_pDecl );
}

BUMPMAP::~BUMPMAP()
{
	SAFE_RELEASE( m_pEffect );
	SAFE_DELETE(m_pMesh);
}

void BUMPMAP::Invalidate()
{
   if( m_pEffect )
      m_pEffect->OnLostDevice();
}

void BUMPMAP::Restore()
{
   if( m_pEffect )
      m_pEffect->OnResetDevice();
}

HRESULT BUMPMAP::Load()
{
   D3DCAPS9 caps;

   m_spD3DDevice->GetDeviceCaps( &caps );
   if( caps.VertexShaderVersion >= D3DVS_VERSION( 1, 1 ) && caps.PixelShaderVersion >= D3DPS_VERSION( 2, 0 ) )
   {
      LPD3DXBUFFER pErr = NULL;
      HRESULT hr = D3DXCreateEffectFromFile( m_spD3DDevice, "BumpMap.fx", NULL, NULL, 0, NULL, &m_pEffect, &pErr );
      if( SUCCEEDED( hr ) )
      {
         m_pTechnique     = m_pEffect->GetTechniqueByName( "TShader" );
         m_pWVP           = m_pEffect->GetParameterByName( NULL, "m_WVP" );
         m_pLightDir      = m_pEffect->GetParameterByName( NULL, "m_LightDir" );
         m_pEyePos        = m_pEffect->GetParameterByName( NULL, "m_EyePos" );
         m_pAmbient       = m_pEffect->GetParameterByName( NULL, "m_Ambient" );
         m_pSpecularPower = m_pEffect->GetParameterByName( NULL, "m_SpecularPower" );
         m_pSpecular      = m_pEffect->GetParameterByName( NULL, "m_Specular" );

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

void BUMPMAP::Begin()
{
   if( m_pEffect )
   {
      m_spD3DDevice->GetTransform( D3DTS_VIEW, &m_matView );
      m_spD3DDevice->GetTransform( D3DTS_PROJECTION, &m_matProj );
      m_pEffect->Begin( NULL, 0 );
   }
}

void BUMPMAP::BeginPass( BYTE Pass )
{
   if( m_pEffect )
      m_pEffect->BeginPass( Pass );   
}

void BUMPMAP::SetAmbient( float Ambient )
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
      m_spD3DDevice->GetMaterial( &old_material );
      old_material.Ambient.r = Ambient;
      old_material.Ambient.g = Ambient;
      old_material.Ambient.b = Ambient;
      old_material.Ambient.a = 1.0f;
      m_spD3DDevice->SetMaterial( &old_material );
   }
}

void BUMPMAP::SetAmbient( D3DXVECTOR4* pAmbient )
{
   if( m_pEffect )
      m_pEffect->SetVector( m_pAmbient, pAmbient );

   else
   {
      D3DMATERIAL9 old_material;
      m_spD3DDevice->GetMaterial( &old_material );
      old_material.Ambient.r = pAmbient->x;
      old_material.Ambient.g = pAmbient->y;
      old_material.Ambient.b = pAmbient->z;
      old_material.Ambient.a = pAmbient->w;
      m_spD3DDevice->SetMaterial( &old_material );
   }
}

void BUMPMAP::SetSpecular( float Specular )
{
   if( m_pEffect )
      m_pEffect->SetFloat( m_pSpecular, Specular );
}

void BUMPMAP::SetSpecularPower( float SpecularPower )
{
   if( m_pEffect )
      m_pEffect->SetFloat( m_pSpecularPower, SpecularPower );
}

void BUMPMAP::SetMatrix( D3DXMATRIX* pMatWorld, D3DXVECTOR4* pCameraPos, D3DXVECTOR4* pLightDir )
{
   if( m_pEffect )
   {
      D3DXMATRIX m, m1;
      D3DXVECTOR4 LightDir;
      D3DXVECTOR4 v;

      m = (*pMatWorld) * m_matView * m_matProj;
      m_pEffect->SetMatrix( m_pWVP, &m );

      //カメラ位置
      m1 = (*pMatWorld) * m_matView;
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
      m_spD3DDevice->SetTransform( D3DTS_WORLD, pMatWorld );
}

void BUMPMAP::EndPass()
{
   if( m_pEffect )
   {
      m_pEffect->EndPass();
   }
}

void BUMPMAP::End()
{
   if( m_pEffect )
   {
      m_pEffect->End();
   }
}

void BUMPMAP::CommitChanges()
{
   if( m_pEffect )
      m_pEffect->CommitChanges();
}

BOOL BUMPMAP::IsOK()
{
   if( m_pEffect )
      return TRUE;

   return FALSE;
}