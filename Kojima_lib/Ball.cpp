#include "Common.h"
#include "Ball.h"

CBall::CBall()
{
	m_Velo = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Accel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Mass = 0.0f;
	m_VeloRimitMax = 100;
	m_VeloRimitMin = 0.030f;
	m_Sphere.m_Center = m_Pos;
	m_isPutter = false;
	m_isReflected = false;
}

void CBall::Reset()
{
	m_Velo = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Accel = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Mass = 0.0f;
	m_VeloRimitMax = 100;
	m_VeloRimitMin = 0.030f;
	m_Sphere.m_Center = m_Pos;
	m_isPutter = false;
	m_isReflected = false;
}

CBall::~CBall()
{

}

void CBall::SetVelo(float x, float y, float z)
{
	m_Velo.x = x;
	m_Velo.y = y;
	m_Velo.z = z;
}


void CBall::SetAccel(float x, float y, float z)
{
	m_Accel.x = x;
	m_Accel.y = y;
	m_Accel.z = z;
}

void CBall::SetCenter(float x, float y, float z)
{
	m_Sphere.m_Center.x = x;	
	m_Sphere.m_Center.y = y;
	m_Sphere.m_Center.z = z;
}


void CBall::Move()
{
	m_Pos.x += m_Velo.x;
	m_Pos.y += m_Velo.y;
	m_Pos.z += m_Velo.z;
	m_Sphere.m_Center = m_Pos;
}

void CBall::MoveCancel()
{
	m_Pos.x -= m_Velo.x;
	m_Pos.y -= m_Velo.y;
	m_Pos.z -= m_Velo.z;
	m_Sphere.m_Center = m_Pos;
}


void CBall::Acceleration()
{
	m_Velo.x += m_Accel.x/m_Mass;
	m_Velo.y += m_Accel.y/m_Mass;
	m_Velo.z += m_Accel.z/m_Mass;
	m_Sphere.m_Center = m_Pos;
}


bool  CBall::CalcSpherePlaneCollision(CSimpleSphere * sphr,CSimpleTriangle * tri,D3DXVECTOR3 * velo,D3DXVECTOR3 * collipos)
{
	D3DXVECTOR3 C0 = sphr->m_Center - tri->m_Vertex[0];
	D3DXVECTOR3 D  = *velo;
	D3DXVECTOR3 r1 = tri->m_Vertex[0] - tri->m_Vertex[1];
	D3DXVECTOR3 r2 = tri->m_Vertex[1] - tri->m_Vertex[2];
	D3DXVECTOR3 N;

	D3DXVec3Cross(&N,&r1,&r2);
	D3DXVec3Normalize(&N,&N); 

	// 平面と中心点の距離を算出
	float Dot_C0 = D3DXVec3Dot(&C0, &N );
	float dist_plane_to_point = fabs( Dot_C0 );

	float Dot = D3DXVec3Dot( &D, &N );
	float t = (sphr->m_HalfAcross - Dot_C0)/Dot;

	* collipos = sphr->m_Center + (t) * D;
	

	// めり込んでいたら衝突として処理終了
   if ( dist_plane_to_point < sphr->m_HalfAcross )
      return true;

   // 壁に対して移動が逆向きなら衝突していない
   if( Dot >= 0 )
   {
      return false;
   }

   // 時間が0～1の間にあれば衝突
   if( (0 <= t) && (t <= 1) )
      return true;

   return false;
}

bool CBall::CalcGoalTexRoling(D3DXVECTOR3 *v_goal,D3DXVECTOR3 * velo)
{
	float t;
	D3DXVECTOR3 e_velo;
	e_velo = *velo;
	e_velo.y = 0;
	D3DXVec3Normalize(&e_velo,&e_velo);
	D3DXVECTOR3 v_togoal = *v_goal - *GetPos();
	v_togoal.y = 0;

	t = D3DXVec3Dot(&e_velo,&v_togoal)/D3DXVec3Length(&e_velo);

	if(t >= 1||t <= 0)
		return false;

	D3DXVECTOR3 h = e_velo*t - v_togoal;
	
	if(D3DXVec3Length(&h) < 0.6f)
	{
		velo->y = 0;
		if(D3DXVec3Length(&h) < 0.4f)
		if(D3DXVec3Length(velo) < 0.3f)
		{	
			return true;
		}
	//	return true;
		//if(D3DXVec3Length(velo) > 0.5f)
		velo->y = D3DXVec3Length(velo)/20;
		SetisPutter(false);
	}

	return false;
}

bool CBall::CalcGoalTexCollision(CSimpleSphere * sphr,CSimpleSquare * sqar,D3DXVECTOR3 * velo)
{
	D3DXVECTOR3 pos_coli;
	D3DXVECTOR2 pos_coli2D;
	D3DXVECTOR2 pos_goal2D;

	if(CalcSpherePlaneCollision(sphr,&sqar->m_TriangleVertex[0],velo,&pos_coli) == false)
		return false;

	if(CheckContained(sqar->m_TriangleVertex[0].m_Vertex[0],sqar->m_TriangleVertex[0].m_Vertex[1],sqar->m_TriangleVertex[0].m_Vertex[2],pos_coli)
	|| CheckContained(sqar->m_TriangleVertex[1].m_Vertex[0],sqar->m_TriangleVertex[1].m_Vertex[1],sqar->m_TriangleVertex[1].m_Vertex[2],pos_coli))
	{
		pos_coli2D.x = pos_coli.x;
		pos_coli2D.y = pos_coli.z;

		pos_goal2D.x = (sqar->m_TriangleVertex[0].m_Vertex[0].x + sqar->m_TriangleVertex[0].m_Vertex[1].x)/2;
		pos_goal2D.y = (sqar->m_TriangleVertex[0].m_Vertex[0].z + sqar->m_TriangleVertex[0].m_Vertex[2].z)/2;
	/*	pos_goal2D.x -= 5/16;
		pos_goal2D.y -= 5/16;*/

		D3DXVECTOR2 Dis = pos_goal2D - pos_coli2D;


		if(D3DXVec2Length(&Dis) < 1.0f
			//&&m_isReflected == false
			)
		{
			D3DXVECTOR2 velo2;
			velo2.x = velo->x;
			velo2.y = velo->z;
			if(D3DXVec2Length(&velo2) < 0.5f)
			{	
				return true;
			}
			else
			{
				velo->x += Dis.x*((1-D3DXVec2Length(&Dis))/10);
				velo->z += Dis.y*((1-D3DXVec2Length(&Dis))/10);	
				velo->y = D3DXVec2Length(&velo2)/2;
			}
		}
		else
		{
			velo->x *= 0.6f;
			velo->y *= 0.4f;
			velo->z *= 0.6f;
		}
	}

	return false;
}

bool CBall::CalcRoolingVelo(CSimpleTriangle *tri,float grav,D3DXVECTOR3 *velo,CSimpleSphere * sphr)
{
	D3DXVECTOR3 VecG;
	VecG = D3DXVECTOR3(0,grav,0);
	D3DXVECTOR3 N;
	D3DXVECTOR3 gravN,gravH;
	D3DXVECTOR3 r1 = tri->m_Vertex[0] - tri->m_Vertex[1];
	D3DXVECTOR3 r2 = tri->m_Vertex[1] - tri->m_Vertex[2];

	D3DXVec3Cross(&N,&r1,&r2);

	if(N.y > 0)
	{
		N.x = -N.x;
		N.y = -N.y;
		N.z = -N.z;
	}

	D3DXVec3Normalize(&N,&N);

	gravN = D3DXVec3Dot(&N,&VecG)*N;

	gravH = VecG - gravN;

	
	
	/*float rate = grav/N.y;
	N *= rate;*/
	if(gravH.y > 0)
	{
		gravH.x = -gravH.x;
		gravH.y = -gravH.y;
		gravH.z = -gravH.z;
	}

	velo->x += gravH.x;
	velo->z += gravH.z;
	velo->y += gravH.y;

	float d = D3DXVec3Length(&gravN);
	D3DXVECTOR3 ev; 
	D3DXVec3Normalize(&ev,velo);
	float n = 0.8f;

	if(d < 0.001f)
		SetisPutter(false);
	
	ev *= d*n;
	*velo -= ev;
	
	m_RolingNextHeight = sphr->m_Center.y + velo->y;

	float rate = 1; 
	//if(tri->m_Mark == true)
	//	rate = 0.97f;
	//else
	//	rate = 0.98f;
	
	*velo *= rate;

	return true;
}


bool CBall::CalcSphereTriangleCollision(CSimpleSphere * sphr,CSimpleTriangle * tri,D3DXVECTOR3 * velo)
{
	D3DXVECTOR3 pos_coli;
	if(CalcSpherePlaneCollision(sphr,tri,velo,&pos_coli) == false)
		return false;

	if(CheckContained(tri->m_Vertex[0],tri->m_Vertex[1],tri->m_Vertex[2],pos_coli))
		return true;
	return false;
}

void CBall::CalcReflect(CSimpleTriangle *tri, D3DXVECTOR3 *velo,bool isGreen)
{
	D3DXVECTOR3 r1 = tri->m_Vertex[0] - tri->m_Vertex[1];
	D3DXVECTOR3 r2 = tri->m_Vertex[1] - tri->m_Vertex[2];

	D3DXVECTOR3 N;
	D3DXVec3Cross(&N,&r1,&r2);
	//N = -N;
	D3DXVec3Normalize(&N,&N);

	*velo = *velo - 2*D3DXVec3Dot(&N,velo)*N;
	float V = D3DXVec3Length(velo);
	if(m_isReflected)
	{
		if(m_isPutter)
		{
			if(isGreen)
			{
				velo->x *= 0.8f;
				velo->y *= 0.05f;
				velo->z *= 0.8f;
			}
			else
			{
				velo->x *= 0.8f;
				velo->y *= 0.01f;
				velo->z *= 0.8f;
			}
		}
		else
		{
			if(isGreen == false)
			{
				velo->x *= 0.5f;
				velo->y *= 0.4f;
				velo->z *= 0.5f;
			}
			else
			{
				velo->x *= 0.7f;
				velo->y *= 0.5f;
				velo->z *= 0.7f;
			}
		}
		m_isReflected = true;
	}
}
