/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGACamera.cpp
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"

#include "IGA3DModule.h"
#include "IGACamera.h"


extern IGA_SHAREMEMORY_DATA  g_IGAShareMem_Date;

extern float g_fDistPick;

CIGACamera g_CIGACamera;


CIGACamera::CIGACamera()
{
	m_vCameraPt		= D3DXVECTOR3(0,0,0);
	m_vLookAtDir	= D3DXVECTOR3(0,0,1);
	m_vUpDir		= D3DXVECTOR3(0,1,0);	
	m_vRightDir		= D3DXVECTOR3(1,0,0);

	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f;	m_vtx[4].y =  1.0f;	m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f;	m_vtx[5].y =  1.0f;	m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f;	m_vtx[6].y =  1.0f;	m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f;	m_vtx[7].y =  1.0f;	m_vtx[7].z = 1.0f;

	m_fClipDistance = 50.0f * 50.0f;
}

CIGACamera::~CIGACamera()
{

}

void CIGACamera::SetClipDistance( float fDist )
{
	m_fClipDistance = fDist * fDist;
}

void CIGACamera::ComputeCamera( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj )
{
	//计算摄影机
	m_matView = *pmatView;
	m_matProj = *pmatProj;

	//近平面中心在3D空间中的位置（计算摄影机的位置）
	D3DXMATRIX matCombViewProj, matInv;
	D3DXVECTOR3 vCamera(0.0f,0.0f,0.0f);
	matCombViewProj = m_matView * m_matProj;
	D3DXMatrixInverse(&matInv, NULL, &matCombViewProj );
	D3DXVec3TransformCoord( &vCamera, &vCamera, &matInv );
	m_vCameraPt = vCamera;

	
	static int istaticCount = 0;   //每30帧更新一次坐标
	if( istaticCount > 30 )
	{
		try
		{
			g_IGAShareMem_Date[0].fx = m_vCameraPt.x;
			g_IGAShareMem_Date[0].fy = m_vCameraPt.y;
			g_IGAShareMem_Date[0].fz = m_vCameraPt.z;
			//g_IGAShareMem_Date.Update();
		}
		catch(...)
		{
			char szOutputDebug[256];
			sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
			OutputDebugString( szOutputDebug );
		}
	
		istaticCount = 0;
	}
	istaticCount++;

	//char szOutputString[256];
	//sprintf(szOutputString, "x = %f, y = %f, z = %f", m_vCameraPt.x, m_vCameraPt.y, m_vCameraPt.z );
	//OutputDebugString( szOutputString );

	//计算摄影机的参数
	D3DXVec3Normalize( &m_vLookAtDir, &D3DXVECTOR3(m_matView._13, m_matView._23, m_matView._33 ) );
	D3DXVec3Normalize( &m_vUpDir,	  &D3DXVECTOR3(m_matView._12, m_matView._22, m_matView._32 ) );
	D3DXVec3Normalize( &m_vRightDir,  &D3DXVECTOR3(m_matView._11, m_matView._21, m_matView._31 ) );

	//更新视椎体
	UpdataViewCentrum();
}

void CIGACamera::UpdataViewCentrum()
{
	D3DXMATRIXA16	matInv;
	D3DXMATRIX matViewProj = m_matView * m_matProj;
	D3DXMatrixInverse(&matInv, NULL, &matViewProj );
	for(int  i = 0; i < 8; i++ )
	{
		D3DXVec3TransformCoord( &m_vViewCentrum[i], &m_vtx[i], &matInv );
	}
	
	D3DXVECTOR3	vHorizontalL = m_vViewCentrum[7] - m_vViewCentrum[4]; 
	D3DXVECTOR3 vHorizontalR = m_vViewCentrum[6] - m_vViewCentrum[5];
	D3DXVECTOR3 vVerticalR   = m_vViewCentrum[3] - m_vViewCentrum[0];
	D3DXVec3Normalize(&vHorizontalL, &vHorizontalL );
	D3DXVec3Normalize(&vHorizontalR, &vHorizontalR );
	D3DXVec3Normalize(&vVerticalR, &vVerticalR );

	m_fHorizontalClipAngle = D3DXVec3Dot( &vHorizontalL, &vHorizontalR );
	m_fVerticalClipAngle   = D3DXVec3Dot( &vHorizontalL, &vVerticalR );
}


bool CIGACamera::InViewCentrum( D3DXVECTOR3* pv )
{
	////计算点到摄影机的距离
	D3DXVECTOR3 vDir;
	vDir = *pv - m_vCameraPt;
	double	doubleDistance = D3DXVec3LengthSq(&vDir);
	if( doubleDistance > m_fClipDistance )
	{
		return false;
	}

	//计算点到摄影机的方向
	D3DXVec3Normalize(&vDir,&vDir);
	float ClipAngle = D3DXVec3Dot( &vDir, &m_vLookAtDir );
	
	//距离摄影机比较近的点也算到视椎体内
	bool bDist = (doubleDistance < g_fDistPick);
	if( ClipAngle < 0.0f && !bDist )	//在摄影机的后面
	{
		return false;
	}
	if( bDist ) 
	{
		return true;
	}

	//视椎裁剪
	if( ClipAngle >= m_fHorizontalClipAngle && ClipAngle >= m_fVerticalClipAngle )
	{
		return true;
	}
	
	return false;

}

bool CIGACamera::GetCameraPt( D3DXVECTOR3* pvCameraPt )
{
	*pvCameraPt = m_vCameraPt;
	return true;
}