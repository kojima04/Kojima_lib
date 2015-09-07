#include "Common.h"
#include "HitSquare.h"

CHitSquare::CHitSquare()
{
	SetLocalPos(0,0,0);
	SetLocalScale(1,1,1);
	SetScale(0,0,0);
	SetRot(0,0,0);
	m_Direction = 1;
#ifdef DEBUG
	m_pDebugFont = new CDebugFont;
	m_pDebugFont->SetUp();
#endif
}

CHitSquare::~CHitSquare()
{
#ifdef DEBUG
	SAFE_DELETE(m_pDebugFont);
#endif
}

void CHitSquare::Draw()
{

	//m_Pos += m_LocalPos;
	ConvertLocaltoWorld();

	D3DXMatrixIdentity(&m_World);

	Transform();
	m_Tex.Draw(&m_World);

//	DebugDraw();

	//m_Pos -= m_LocalPos;
	ConvertWorldtoLocal();
}

void CHitSquare::DebugDraw()
{
	char buff[80];
	_stprintf(buff, "sqr_x = %f sqr_y = %f\n",GetPos()->x,GetPos()->y);
	m_pDebugFont->Draw((int)(m_Direction< 0) * 550,340,buff);
	_stprintf(buff, "sqr_Lx = %f sqr_Ly = %f\n",GetLocalPos()->x,GetLocalPos()->y);
	m_pDebugFont->Draw((int)(m_Direction< 0) * 550,360,buff);
}
	
void CHitSquare::Create()
{
	m_Tex.Create();
}

void CHitSquare::SetVertexColor(int r, int g, int b, int alfa)
{
	m_Tex.SetAlfa(alfa);
	for(int i = 0;i < 4;++i)
		m_Tex.SetVertexColor(i,r,g,b);
}

void CHitSquare::SetLocalPos(float x, float y, float z)
{
	m_LocalPos.x = x;
	m_LocalPos.y = y;
	m_LocalPos.z = z;
}

void CHitSquare::SetLocalScale(float x, float y, float z)
{
	m_LocalScale.x = x;
	m_LocalScale.y = y;
	m_LocalScale.z = z;
}

void CHitSquare::SetLocalPara()
{

}

void CHitSquare::ConvertWorldtoLocal()
{
	//m_Pos -= m_LocalPos;
	SetScale(m_Scale.x / m_LocalScale.x, m_Scale.y / m_LocalScale.y,0);

	if(m_LocalScale.x == 0&&m_LocalScale.y == 0)
		SetScale(0,0,0);
	if(m_LocalScale.x == 0)
		SetScale(0, m_Scale.y / m_LocalScale.y,0);
	else if(m_LocalScale.y == 0)
		SetScale(m_Scale.x / m_LocalScale.x,0,0);	

	SetPos(m_Pos.x - m_Direction * m_LocalPos.x*m_Scale.x, m_Pos.y - m_LocalPos.y*m_Scale.y,0);
}

void CHitSquare::ConvertLocaltoWorld()
{
	//m_Pos += m_LocalPos;
	SetPos(m_Pos.x + m_Direction * m_LocalPos.x*m_Scale.x, m_Pos.y + m_LocalPos.y*m_Scale.y,0);
	SetScale(m_Scale.x * m_LocalScale.x, m_Scale.y * m_LocalScale.y, m_Scale.z * m_LocalScale.z);
}

void CHitSquare::ConvertPosWorldtoLocal(D3DXVECTOR3 * point)
{
	SetLocalPos((GetPos()->x - point->x)/(m_Scale.x),(GetPos()->y - point->y)/(m_Scale.y),0);
}
bool CHitSquare::CheckHitPoint(D3DXVECTOR3 *point)
{
	ConvertLocaltoWorld();

	if(point->x >= GetPos()->x - GetScale()->x/2 && point->x <= GetPos()->x + GetScale()->x/2
	&& point->y >= GetPos()->y - GetScale()->y/2 && point->y <= GetPos()->y + GetScale()->y/2)
	{
		ConvertWorldtoLocal();
		return true;
	}
	ConvertWorldtoLocal();
	return false;
}

bool CHitSquare::CheckHitCircle(Circle * pcircle)
{
	D3DXVECTOR3 vec3;
	vec3 = D3DXVECTOR3(pcircle->center.x,pcircle->center.y,0);
	if(CheckHitPoint(&vec3))
		return true;
	ConvertLocaltoWorld();

	D3DXVECTOR2 Vertex[4];

	Vertex[0].x = GetPos()->x - GetScale()->x/2;	Vertex[0].y = GetPos()->y + GetScale()->y/2;
	Vertex[1].x = GetPos()->x + GetScale()->x/2;	Vertex[1].y = GetPos()->y + GetScale()->y/2;
	Vertex[2].x = GetPos()->x + GetScale()->x/2;	Vertex[2].y = GetPos()->y - GetScale()->y/2;
	Vertex[3].x = GetPos()->x - GetScale()->x/2;	Vertex[3].y = GetPos()->y - GetScale()->y/2;

	for(int i = 0;i < 4;++i)
	{
		Segment s;
		s.s = Vertex[i];
		s.v = Vertex[(i + 1)%4] - Vertex[i];
		if(CheckHitCircleLineSegment(pcircle,&s))
		{
			ConvertWorldtoLocal();
			return true;
		}
	}

	ConvertWorldtoLocal();
	return false;
}

bool CHitSquare::CheckHitLineSegment(Segment * pseg)
{
	ConvertLocaltoWorld();

	D3DXVECTOR2 Vertex[4];

	Vertex[0].x = GetPos()->x - GetScale()->x/2;	Vertex[0].y = GetPos()->y + GetScale()->y/2;
	Vertex[1].x = GetPos()->x + GetScale()->x/2;	Vertex[1].y = GetPos()->y + GetScale()->y/2;
	Vertex[2].x = GetPos()->x + GetScale()->x/2;	Vertex[2].y = GetPos()->y - GetScale()->y/2;
	Vertex[3].x = GetPos()->x - GetScale()->x/2;	Vertex[3].y = GetPos()->y - GetScale()->y/2;

	for(int i = 0;i < 4;++i)
	{
		Segment s;
		s.s = Vertex[i];
		s.v = Vertex[(i + 1)%4] - Vertex[i];
		if(CheckHitLineSegmentLineSegment(pseg,&s))
		{
			ConvertWorldtoLocal();
			return true;
		}
	}

	ConvertWorldtoLocal();
	return false;
}


// 2Dベクトルの外積
float D3DXVec2Cross( D3DXVECTOR2* v1, D3DXVECTOR2* v2 ) 
{
   return v1->x * v2->y - v1->y * v2->x;
}

bool CHitSquare::CheckHitMovingCircle(CChara * pchar,float halfdiameter)
{
	Circle Cirbuf,Cirbuf2;
	Cirbuf.center.x = pchar->GetPos()->x;
	Cirbuf.center.y = pchar->GetPos()->y;
	Cirbuf.halfdiameter = halfdiameter;

	if(CheckHitCircle(&Cirbuf))
		return true;

	Cirbuf2.center.x = pchar->GetPos()->x + pchar->GetVelo()->x;
	Cirbuf2.center.y = pchar->GetPos()->y + pchar->GetVelo()->y;
	Cirbuf2.halfdiameter = halfdiameter;

	if(CheckHitCircle(&Cirbuf2))
		return true;

	Segment segbuf;
	segbuf.s.x = pchar->GetPos()->x; segbuf.s.y = pchar->GetPos()->y;
	segbuf.v.x = pchar->GetVelo()->x; segbuf.v.y = pchar->GetVelo()->y;

	Segment segbuf1,segbuf2;
	D3DXVECTOR2 vecMove;
	vecMove.x = -pchar->GetVelo()->x; vecMove.y = pchar->GetVelo()->y;

	D3DXVec2Normalize(&vecMove,&vecMove);

	segbuf1.v = segbuf2.v = segbuf.v;

	segbuf1.s = segbuf.s + vecMove;
	segbuf2.s = segbuf.s - vecMove;

	if(CheckHitLineSegment(&segbuf1))
		return true;

	if(CheckHitLineSegment(&segbuf2))
		return true;

	return false;
}

bool CHitSquare::CheckHitLineSegmentLineSegment(Segment * seg1,Segment * seg2,D3DXVECTOR2* outPos)
{
	D3DXVECTOR2 v = seg2->s - seg1->s;
	float Crs_v1_v2 = D3DXVec2Cross( &seg1->v, &seg2->v );
	if ( Crs_v1_v2 == 0.0f ) {
	  // 平行状態
	  return false;
	}

	float Crs_v_v1 = D3DXVec2Cross( &v, &seg1->v );
	float Crs_v_v2 = D3DXVec2Cross( &v, &seg2->v );

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	const float eps = 0.00001f;
	if ( t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1 ) {
	  // 交差していない
	  return false;
	}

	if( outPos )
	  *outPos = seg1->s + seg1->v * t1;

	return true;	
}

bool CHitSquare::CheckHitCircleLineSegment(Circle * pcircle,Segment * seg)
{
	Segment segbuf,segbuf2;
	segbuf.s = seg->s;
	segbuf.v = pcircle->center - seg->s;

	segbuf2.s = seg->s + seg->v;
	segbuf2.v = segbuf.v - seg->v;

	float lenge = D3DXVec2Cross(&seg->v,&segbuf.v)/D3DXVec2Length(&seg->v);
	if(D3DXVec2Length(&seg->v) == 0)
		lenge = D3DXVec2Length(&(segbuf.v));

	if(lenge < 0)
		lenge = -lenge;
	
	if(pcircle->halfdiameter < lenge)
		return false;

	if(D3DXVec2Dot(&segbuf.v,&seg->v)*D3DXVec2Dot(&segbuf2.v,&seg->v) <= 0)
		return true;
	else
	if(pcircle->halfdiameter > D3DXVec2Length(&segbuf.v )||pcircle->halfdiameter > D3DXVec2Length(&segbuf2.v ))
		return true;

	return false;
}