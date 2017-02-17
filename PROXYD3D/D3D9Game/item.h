#pragma once

#include "D3D/D3D.h"
#include "D3D/StaticMesh.h"


///////////////////////////////////////////////////////////////
class CItemMeshObj : public CStaticMeshObj
{
public:
	CItemMeshObj(CStaticMesh * mesh,D3DXVECTOR3 vPos,D3DXVECTOR3 vSvaling,float fRotationY)
		:CStaticMeshObj(mesh,vPos,vSvaling,fRotationY)
	{
		
	}
	~CItemMeshObj()
	{
		
	}
	void Render();
protected:
private:
};
/////////////////////////////////////////////////////////////
class CItem : public CItemMeshObj
{
public:
	CItem(CStaticMesh * mesh,D3DXVECTOR3 vPos,D3DXVECTOR3 vSvaling,float fRotationY)
		:CItemMeshObj(mesh,vPos,vSvaling,fRotationY)
	{
		m_dTime = APP_Tools::appGetTime();
		m_dTimeEnd = m_dTime + 10;
		m_bPickUp = false;
	}
	~CItem()
	{

	}
	
	bool m_bPickUp;
	DWORD m_dwKey;
	double m_dTime;
	double m_dTimeEnd;
	int m_iType;
	int m_iNum;
protected:
private:
};