/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGACamera.h
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once 

#include <d3d9.h>
#include <d3dx9.h>

class CIGACamera
{
public:
	CIGACamera();
	~CIGACamera();

	void ComputeCamera( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj );
	bool InViewCentrum( D3DXVECTOR3* pv );
	bool GetCameraPt( D3DXVECTOR3* pvCameraPt );

	void SetClipDistance( float fDist );

protected:
	void UpdataViewCentrum();

private:
	D3DXMATRIX		m_matView;	//视图矩阵
	D3DXMATRIX		m_matProj;	//投影矩阵

	//得到相关参数
	D3DXVECTOR3		m_vLookAtDir;	//摄影机的观察方向
	D3DXVECTOR3		m_vUpDir;		//摄影机的上方向
	D3DXVECTOR3		m_vRightDir;    //摄影机的有方向
	D3DXVECTOR3     m_vCameraPt;    //摄影机的位置

	//视椎体
	D3DXVECTOR3		m_vtx[8];
	D3DXVECTOR3		m_vViewCentrum[8];

	//裁剪有效距离及裁剪角度
	double			m_fClipDistance;
	float			m_fHorizontalClipAngle;	//水平裁剪角
	float			m_fVerticalClipAngle;	//垂直裁剪角
};

extern CIGACamera g_CIGACamera;


//CIGACamera* GetIGACamera();