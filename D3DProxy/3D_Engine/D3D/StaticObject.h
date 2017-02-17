
#pragma once


#include "StaticMesh.h"


class CStaticObject : public CStaticMeshObj
{
public:
	CStaticObject( CStaticMesh * mesh, D3DXVECTOR3 vPos, D3DXVECTOR3 vScaling, float fRotationY )
		:CStaticMeshObj( mesh, vPos, vScaling, fRotationY )
	{
	}
	~CStaticObject()
	{
	}


protected:


public:


};