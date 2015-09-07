#include "Common.h"
#include "DrawObject.h"

D3DXVECTOR2 CDrawObject::m_StStaticFilter;

CDrawObject::CDrawObject()
{
	m_isTransStatic = false;
}

void CDrawObject::Transform()
{
	D3DXMatrixRotationYawPitchRoll(&m_World,m_Rot.y,m_Rot.x,m_Rot.z);

	m_World._11 *= m_Scale.x;
	m_World._12 *= m_Scale.x;
	m_World._13 *= m_Scale.x;

	m_World._21 *= m_Scale.y;
	m_World._22 *= m_Scale.y;
	m_World._23 *= m_Scale.y;

	m_World._31 *= m_Scale.z;
	m_World._32 *= m_Scale.z;
	m_World._33 *= m_Scale.z;

	m_World._41 = m_Pos.x;
	m_World._42 = m_Pos.y;
	m_World._43 = m_Pos.z;

}

void CDrawObject::SetPos(float x, float y, float z)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
}


void CDrawObject::SetRot(float x, float y, float z)
{
	m_Rot.x = x;
	m_Rot.y = y;
	m_Rot.z = z;
}


void CDrawObject::SetScale(float x, float y, float z,bool isstatic)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
	if(m_isTransStatic&&isstatic)
	{
		m_Scale.x = x*m_StStaticFilter.x;
		m_Scale.y = y*m_StStaticFilter.y;
	}
}

D3DXMATRIX * CDrawObject::MatrixMultiply(D3DXMATRIX * mat1,D3DXMATRIX * mat2)
{
	D3DXMATRIX * pmat;
	D3DXMATRIX mat;

	mat._11 = mat1->_11*mat2->_11 + mat1->_12*mat2->_21 + mat1->_13*mat2->_31 + mat1->_14*mat2->_41;
	mat._12 = mat1->_11*mat2->_12 + mat1->_12*mat2->_22 + mat1->_13*mat2->_32 + mat1->_14*mat2->_42;
	mat._13 = mat1->_11*mat2->_13 + mat1->_12*mat2->_23 + mat1->_13*mat2->_33 + mat1->_14*mat2->_43;
	mat._14 = mat1->_11*mat2->_14 + mat1->_12*mat2->_24 + mat1->_13*mat2->_34 + mat1->_14*mat2->_44;

	mat._21 = mat1->_21*mat2->_11 + mat1->_22*mat2->_21 + mat1->_23*mat2->_31 + mat1->_24*mat2->_41;
	mat._22 = mat1->_21*mat2->_12 + mat1->_22*mat2->_22 + mat1->_23*mat2->_32 + mat1->_24*mat2->_42;
	mat._23 = mat1->_21*mat2->_13 + mat1->_22*mat2->_23 + mat1->_23*mat2->_33 + mat1->_24*mat2->_43;
	mat._24 = mat1->_21*mat2->_14 + mat1->_22*mat2->_24 + mat1->_23*mat2->_34 + mat1->_24*mat2->_44;

	mat._31 = mat1->_31*mat2->_11 + mat1->_32*mat2->_21 + mat1->_33*mat2->_31 + mat1->_34*mat2->_41;
	mat._32 = mat1->_31*mat2->_12 + mat1->_32*mat2->_22 + mat1->_33*mat2->_32 + mat1->_34*mat2->_42;
	mat._33 = mat1->_31*mat2->_13 + mat1->_32*mat2->_23 + mat1->_33*mat2->_33 + mat1->_34*mat2->_43;
	mat._34 = mat1->_31*mat2->_14 + mat1->_32*mat2->_24 + mat1->_33*mat2->_34 + mat1->_34*mat2->_44;

	mat._41 = mat1->_41*mat2->_11 + mat1->_42*mat2->_21 + mat1->_43*mat2->_31 + mat1->_44*mat2->_41;
	mat._42 = mat1->_41*mat2->_12 + mat1->_42*mat2->_22 + mat1->_43*mat2->_32 + mat1->_44*mat2->_42;
	mat._43 = mat1->_41*mat2->_13 + mat1->_42*mat2->_23 + mat1->_43*mat2->_33 + mat1->_44*mat2->_43;
	mat._44 = mat1->_41*mat2->_14 + mat1->_42*mat2->_24 + mat1->_43*mat2->_34 + mat1->_44*mat2->_44;

	pmat = &mat;
	return pmat;
}

D3DXMATRIX * CDrawObject::MatrixInverse(D3DXMATRIX * mat1)
{
	D3DXMATRIX * pmat;
	D3DXMATRIX mat;

	//pmat = D3DXMatrixInverse(mat1,NULL,mat1);

	float a[4][4]={{mat1->_11,mat1->_12,mat1->_13,mat1->_14},{mat1->_21,mat1->_22,mat1->_23,mat1->_24},
					{mat1->_31,mat1->_32,mat1->_33,mat1->_34},{mat1->_41,mat1->_42,mat1->_43,mat1->_44}}; //入力用の配列
	float inv_a[4][4]; //ここに逆行列が入る
	float buf; //一時的なデータを蓄える
	int i,j,k; //カウンタ
	int n=4;  //配列の次数
	//単位行列を作る
	for(i=0;i<n;i++)
	{
	  for(j=0;j<n;j++)
	  {
		inv_a[i][j]=(i==j)?1.0f:0.0f;
	  }
	}
	//掃き出し法
	for(i=0;i<n;i++)
	{
	  buf=1/a[i][i];
	  if(a[i][i] == 0)
	  {

		  buf = 0;
	  }
	  
	  for(j=0;j<n;j++)
	  {
		a[i][j]*=buf;
		inv_a[i][j]*=buf;
	  }
	  for(j=0;j<n;j++)
	  {
		if(i!=j)
		{
		  buf=a[j][i];
		  for(k=0;k<n;k++)
		  {
			a[j][k]-=a[i][k]*buf;
			inv_a[j][k]-=inv_a[i][k]*buf;
		  }
		}
	  }
	}

	mat._11 = inv_a[0][0];
	mat._12 = inv_a[0][1];
	mat._13 = inv_a[0][2];
	mat._14 = inv_a[0][3];

	mat._21 = inv_a[1][0];
	mat._22 = inv_a[1][1];
	mat._23 = inv_a[1][2];
	mat._24 = inv_a[1][3];

	mat._31 = inv_a[2][0];
	mat._32 = inv_a[2][1];
	mat._33 = inv_a[2][2];
	mat._34 = inv_a[2][3];

	mat._41 = inv_a[3][0];
	mat._42 = inv_a[3][1];
	mat._43 = inv_a[3][2];
	mat._44 = inv_a[3][3];

	pmat = &mat;
	return pmat;
}

void CDrawObject::TransformStatic()
{
	SetPos(GetPos()->x,GetPos()->y,0.99f);
	D3DXMATRIX * viewMat;
	D3DXMATRIX * projMat;
	D3DXMATRIX * scrMat;
	viewMat = projMat = scrMat = NULL;

	viewMat = GetTransformView();
	projMat = GetTransformProj();
	scrMat  = GetTransformScr();

	D3DXMATRIX  matInvViewToScreen;
	D3DXVECTOR3 out;
	D3DXMatrixMultiply(&matInvViewToScreen,viewMat,projMat);
	D3DXMatrixMultiply(&matInvViewToScreen,&matInvViewToScreen,scrMat);
	if(D3DXMatrixInverse(&matInvViewToScreen,NULL,&matInvViewToScreen) == NULL)
	{
		int p = 0;
	}
	else
	{
		int o = 0;
	}
	//MatrixInverse(&matInvViewToScreen);
	D3DXVec3TransformCoord(&out,&m_Pos,&matInvViewToScreen);   

	D3DXMATRIX tmp;
	D3DXMatrixRotationYawPitchRoll(&tmp,m_Rot.y,m_Rot.x,m_Rot.z);
	D3DXMatrixMultiply(viewMat,viewMat,&tmp);
   
	m_World._41 = out.x;
	m_World._42 = out.y; 
	m_World._43 = out.z;

	m_World._11 = m_Scale.x * viewMat->_11;
	m_World._12 = m_Scale.x * viewMat->_21;
	m_World._13 = m_Scale.x * viewMat->_31;

	m_World._21 = m_Scale.y * viewMat->_12;
	m_World._22 = m_Scale.y * viewMat->_22;
	m_World._23 = m_Scale.y * viewMat->_32;

	m_World._31 = m_Scale.z * viewMat->_13;
	m_World._32 = m_Scale.z * viewMat->_23;
	m_World._33 = m_Scale.z * viewMat->_33;

}

void CDrawObject::TransformBillBoard()
{

	D3DMATRIX *mat = NULL;

	mat = GetTransformView();
	
	if(m_stpD3DDevice->GetTransform(D3DTS_VIEW,mat) == D3DERR_INVALIDCALL)
	{
		mat = GetTransformView();
	}

	//if(mat != D3DERR_INVALIDCALL)
	//{
		m_World._11 = m_Scale.x * mat->_11;
		m_World._12 = m_Scale.x * mat->_21;
		m_World._13 = m_Scale.x * mat->_31;
			
		m_World._21 = m_Scale.y * mat->_12;
		m_World._22 = m_Scale.y * mat->_22;
		m_World._23 = m_Scale.y * mat->_32;

		m_World._31 = m_Scale.z * mat->_13;
		m_World._32 = m_Scale.z * mat->_23;
		m_World._33 = m_Scale.z * mat->_33;

		m_World._41 = m_Pos.x;
		m_World._42 = m_Pos.y;
		m_World._43 = m_Pos.z;
	//}
}

void CDrawObject::SetStaticFilterfromFile()
{
	LPCTSTR FileName = "data/StaticFilter.txt";

	FILE * fp;

	if((fp = fopen(FileName,"r")) != NULL)
	{
		fscanf(fp,"%f",&m_StStaticFilter.x);
		fscanf(fp,"%f",&m_StStaticFilter.y);
		fclose(fp);
	}
	m_StStaticFilter.x = 82.2f/m_StStaticFilter.x;
	m_StStaticFilter.y = 82.2f/m_StStaticFilter.y;
	if(!FULLSCREEN)
		m_StStaticFilter.x = m_StStaticFilter.y = 82.2f/600;
}