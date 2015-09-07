#pragma once

#include "DXObject.h"
class CTextureVertex
{
public:
	D3DXVECTOR3	p;
	DWORD 	color;
	D3DXVECTOR2	t;
};

//=============================================================
// DrawObject
//=============================================================
class CDrawObject :
	public CDXObject
{
public:
	CDrawObject();
	virtual ~CDrawObject(){};

protected:
	D3DXVECTOR3 m_Pos;//位置
	D3DXVECTOR3 m_Scale;//サイズ
	D3DXVECTOR3 m_Rot;//回転

	D3DXMATRIX m_World;//ワールドマトリックス
	bool	m_isTransStatic;
	float	m_StaticFilter;

public:
	D3DXVECTOR2 GetStaticFilter(){return m_StStaticFilter;}

	D3DXMATRIX * MatrixMultiply(D3DXMATRIX * mat1,D3DXMATRIX * mat2);
	D3DXMATRIX * MatrixInverse(D3DXMATRIX * mat1);

	void Transform();
	void TransformStatic();
	void TransformBillBoard();

	D3DXVECTOR3 * GetPos(){ return &m_Pos;};
	D3DXVECTOR3 * GetScale(){ return &m_Scale;};
	D3DXVECTOR3 * GetRot(){ return &m_Rot;};

	void SetPos(D3DXVECTOR3 *pos){m_Pos = *pos;};
	void SetPos(float x, float y, float z);
	void ResetPos(){m_Pos.x = m_Pos.y = m_Pos.z = 0.0f;};

	void SetScale(D3DXVECTOR3 *scale){m_Scale = *scale;};
	void SetScale(float x, float y, float z,bool isstatic = true);

	void SetRot(D3DXVECTOR3 *rot){m_Rot = *rot;};
	void SetRot(float x, float y, float z);
	void ResetRot(){m_Rot.x = m_Rot.y = m_Rot.z = 0.0f;};

	void ResetWorld(){ResetRot(); ResetPos();};
	void SetWorld(D3DXMATRIX *world){m_World = *world;};

	D3DXMATRIX * GetWorld(){ return &m_World;};

	void SetisTransStatic(bool isst){m_isTransStatic = isst;}

	static void SetStaticFilterfromFile();

	static D3DXVECTOR2 m_StStaticFilter;

};