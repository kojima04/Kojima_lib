#pragma once

#include "DrawObject.h"
#include "Texture.h"
#include "Chara.h"
#include "DebugFont.h"

// 線分構造体
struct Segment 
{
   D3DXVECTOR2 s; // 始点
   D3DXVECTOR2 v; // 方向ベクトル
};

struct Circle
{
	D3DXVECTOR2 center;
	float halfdiameter;
};

class CHitSquare :
	public CDrawObject
{
public:
	CHitSquare();
	~CHitSquare();

	void Draw();
	void DebugDraw();
	void Create();
	void SetVertexColor(int r,int g,int b,int alfa);

	void	SetLocalScale(float x, float y, float z);	//相対尺度
	void	SetLocalScale(D3DXVECTOR3 * lscale){m_LocalScale = *lscale;}
	D3DXVECTOR3 * GetLocalScale(){return &m_LocalScale;}

	void	SetLocalPos(float x, float y, float z);	//相対位置
	void	SetLocalPos(D3DXVECTOR3 *lpos){m_LocalPos = *lpos;}	
	D3DXVECTOR3 * GetLocalPos(){return &m_LocalPos;}

	void	SetLocalPara();

	bool	CheckHitMovingCircle(CChara * pchar,float halfdiameter);

	bool	CheckHitPoint(D3DXVECTOR3 * point);
	bool	CheckHitCircle(Circle * pcircle);

	bool	CheckHitCircleLineSegment(Circle * pcircle,Segment * psegment);
	bool	CheckHitLineSegmentLineSegment(Segment * seg1,Segment * seg2,D3DXVECTOR2* outPos = 0);

	bool	CheckHitLineSegment(Segment * psegment);

	void	ConvertPosWorldtoLocal(D3DXVECTOR3 * point);
	void	ConvertScaleWorldtoLocal(D3DXVECTOR3 * point);

	void	ConvertWorldtoLocal();
	void	ConvertLocaltoWorld();

	void	SetDirection(int itmp){m_Direction = itmp;}

	void	SetAlfa(int alfa){m_Tex.SetAlfa(alfa);}

protected:
	CDebugFont *	m_pDebugFont;
	int	m_Direction;
	CTexture m_Tex;
	D3DXVECTOR3 m_LocalScale;
	D3DXVECTOR3 m_LocalPos;
};