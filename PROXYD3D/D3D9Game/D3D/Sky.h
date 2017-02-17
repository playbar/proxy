
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <crtdbg.h>


#include "D3D.h"
#include "Base/Texture.h"


#define D3DFVF_SKYBOX	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )



class CSkyBox : public CBase
{
protected:
	struct VERTEX_BOX
	{
		float	x,y,z;
		float   nx,ny,nz;
		float	tu,tv;
	};
public:
	~CSkyBox();


protected:
	CTexture *				m_pTexture[7];
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DVERTEXBUFFER9	m_pVBSky;
	LPDIRECT3DINDEXBUFFER9	m_pIBSky;

	int						m_nSegment;
	float					m_nSkyTile;
	float					m_fWidth;
	float					m_uSpeed;
	float					m_vSpeed;
	double					m_fFrameTime;

public:
	
	CSkyBox( float width,TCHAR *strTextureA6[] = NULL );
	void LoadSkyBox( TCHAR *strConfigFile, TCHAR * strSkyName );
	void Initialize();

	void Render();
	void SetMatrix();

};
