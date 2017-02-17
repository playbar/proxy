

#pragma once


#include "D3D.h"


class CStaticMesh
{
public:
	CStaticMesh()
	{
		m_pMesh = NULL;
		m_pMaterials = NULL;
		m_pTextures = NULL;
		m_dwNumMaterials = 0;
	}
	~CStaticMesh()
	{
		for( int i = 0; i < (int)m_dwNumMaterials; i ++ )
		{
			SAFE_RELEASE( m_pTextures[i] );
		}
		SAFE_DELETE_ARRAY( m_pMaterials );
		SAFE_DELETE_ARRAY( m_pTextures );

		SAFE_RELEASE( m_pMesh );
	}


	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vMax;

	bool LoadMesh( TCHAR * szFileName );

	LPD3DXMESH GetMesh()
	{
		return m_pMesh;
	}

	void Render();
	void Render( DWORD dwColor );

protected:

	LPD3DXMESH				m_pMesh;
	D3DMATERIAL9 *			m_pMaterials;
	LPDIRECT3DTEXTURE9 *	m_pTextures;

	DWORD				m_dwNumMaterials;

};


class CStaticMeshObj : public CBase
{
public:
	CStaticMeshObj( CStaticMesh * mesh, D3DXVECTOR3 vPos, D3DXVECTOR3 vScaling, float fRotationY )
	{
		m_pMesh = mesh;
		m_dwBaseType = OBJECT_TYPE_STATIC_MESH;
		m_vPos = vPos;
		m_vScaling = vScaling;
		m_fRotationY = fRotationY;
		
		m_pDevice = g_pMyD3D->GetDevice();

		SetCustomRenderState( COLOR_SELECT_TEXTURE, ALPHA_SELECT_TEXTURE, ALPHABLEND_SRCALPHA_INVSRCALPHA, FILTER_LINEAR, CULL_NONE, TRUE,true, 0 );
	}
	~CStaticMeshObj()
	{
	}


	void Render();

	void SetMesh( CStaticMesh * pMesh )
	{
		m_pMesh = pMesh;
	}

	CStaticMesh * GetMesh()
	{
		return m_pMesh;
	}

	bool RayCheck( D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);

protected:

	CStaticMesh	*		m_pMesh;

	D3DXVECTOR3			m_vScaling;
	float				m_fRotationY;
};