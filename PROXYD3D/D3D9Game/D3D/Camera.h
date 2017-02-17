

#pragma once

#include <d3dx9.h>
#include <math.h>

#include "D3D.h"
#include "../ToolFunction.h"


//视口类
class CViewport
{
public:
	CViewport( float fSize )
	{
		m_fAngle = D3DX_PI / 4;
		m_fSize	 = fSize;
		m_fNear  = 1.f;
		m_fFar	 = 5000.0f;
	}

	~CViewport()	{}

public:
private:
protected:

	float				m_fAngle;
	float				m_fSize;
	float				m_fNear;
	float				m_fFar;

};

//相机类
class CCamera : public CViewport
{

public:
	CCamera( float fViewportSize ):CViewport( fViewportSize )
	{
		m_fAngleY = 0;
		m_fAngleXZ = 0;
		m_pDevice = g_pMyD3D->GetDevice();
	}

	~CCamera(){}

	void Init( float fMinDis, float fMaxDis, float fAngleXZMin, float fAngleXZMax );

	void Rotate( float fAngleY, float fAngleXZ );

	void ToWorld();
	void UpdateCamera();

	void SetDesPos( D3DXVECTOR3 vPos )
	{
		m_vFirstPos += vPos - m_vDesPos;
		m_vFirstLookAt = m_vDesPos = vPos;
		m_vLookAt = m_vDesPos;
	}
	void SetPos(D3DXVECTOR3 vPos)
	{
		m_vFirstPos = vPos;
	}
	D3DXVECTOR3 GetDesPos()
	{
		return m_vDesPos;
	}

	D3DXVECTOR3 GetPos()
	{
		return m_vPos;
	}
	D3DXVECTOR3 GetLookAt()
	{
		return m_vLookAt;
	}
	float GetAngleY()
	{
		return m_fAngleY;
	}
	float GetDistance()
	{
		return m_fDistance;
	}

	void GetRay( int x, int y, LPRAY ray );
	void GetViewRay( LPRAY ray );



	//viewport

	void UpdateViewport()
	{
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, m_fAngle, m_fSize, m_fNear, m_fFar );
		m_pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}

	inline float GetViewportAngle()
	{
		return m_fAngle;
	}
	inline float GetViewportSize()
	{
		return m_fSize;
	}
	inline float GetViewportNear()
	{
		return m_fNear;
	}
	inline float GetViewportFar()
	{
		return m_fFar;
	}

	void SetViewportAngle( float fAngle )
	{
		m_fAngle = fAngle;
		//UpdateViewport();
	}
	void SetViewportSize( float fSize )
	{
		m_fSize = fSize;
		//UpdateViewport();
	}
	void SetViewportNear( float fNear )
	{
		m_fNear = fNear;
		//UpdateViewport();
	}
	void SetViewportFar( float fFar )
	{
		m_fFar = fFar;
		//UpdateViewport();
	}
	void SetViewport( float fAngle, float fSize, float fNear, float fFar )
	{
		m_fAngle = fAngle;
		m_fSize  = fSize;
		m_fNear  = fNear;
		m_fFar   = fFar;
		//UpdateViewport();
	}

	void Translate( float fDistance );

private:

	D3DXVECTOR3				m_vFirstPos;
	D3DXVECTOR3				m_vFirstLookAt;

	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vLookAt;
	D3DXVECTOR3				m_vTop;

	D3DXVECTOR3				m_vDesPos;					//跟随点
	float					m_fDistance;
	float					m_fMinDis;					//最小距离
	float					m_fMaxDis;					//最大距离
	
	float					m_fFirstAngleY;
	float					m_fFirstAngleXZ;

	float					m_fAngleY;
	float					m_fAngleXZ;
	float					m_fAngleXZMin;
	float					m_fAngleXZMax;

	D3DXMATRIX				m_matView;

	LPDIRECT3DDEVICE9		m_pDevice;

protected:

	void AllRotate( const D3DXMATRIX & matTrans );
	void AllTranslate( const D3DXMATRIX & matTrans );

	void RotateX( float fAngle );
	void RotateY( float fAngle );
	void RotateZ( float fAngle );
	void Translate( float x, float y, float z );
	void Translate( const D3DXVECTOR3 & vTrans );	


};
extern CCamera * g_pCamera;