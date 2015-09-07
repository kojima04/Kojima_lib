#include "Common.h"
#include "2DTexture.h"
C2DTexture::C2DTexture()
{
	Create();
}

C2DTexture::~C2DTexture()
{

}

void C2DTexture::Create()
{

}

void C2DTexture::Draw()
{
	m_Anim.Animation();
	D3DXMatrixIdentity(&m_World);
	if(m_isTransStatic)
		TransformStatic();
	else
		TransformBillBoard();
	m_Anim.Draw(&m_World);
}