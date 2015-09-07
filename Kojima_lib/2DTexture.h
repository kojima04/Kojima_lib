#pragma once

#include "Chara.h"
#include "InstantEffect.h"

class C2DTexture :
	public CChara
{
public:
	C2DTexture();
	~C2DTexture();

	void Create();
	void Draw();
protected:
	CInstantEffect m_Anim;
public:
	CInstantEffect * GetpAnim(){return &m_Anim;}
};