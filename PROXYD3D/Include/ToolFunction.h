/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : ToolFunction.h
*	工程使用的结构体定义, add by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/
#pragma once


#include <d3d9.h>
#include <d3dx9.h>

typedef struct
{
	D3DXVECTOR3	vOrig;
	D3DXVECTOR3 vVector;
}RAY,*LPRAY;

namespace APP_Tools
{
	void AppIntToChar(int num,char * text);
	void AppIntToChar(int num,TCHAR * text);

	bool AppInRect(int x,int y,RECT rect);
	bool AppInRect(int x,int y,int rectX,int rectY,int rectWidth,int rectHeight);

	bool AppRectangleBumpCheck( RECT rect1, RECT rect2 );
	bool AppRectangleBumpCheck( int x1, int y1, int width1, int height1,
		int x2, int y2, int width2, int height2 );


	////////////////////////////////////////////////////////////////////////////

	void AppVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans );

	///////////////////////////////////////////////////////////////////////////////

	double AppGetTime();

	////////////////////////////////////////////////////////////////////////////////

	bool AppRayPlaneCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 & vOut );

	/////////////////////////////////////////////////////////////////////////////////

	void ComputeTriNormal( D3DXVECTOR3 * v1, D3DXVECTOR3 * v2, D3DXVECTOR3 * v3,D3DVECTOR & normalVec );

	///////////////////////////////////////////////////////////////////////////////////

	//获得射线
	void GetRay(HWND hWnd, D3DXMATRIXA16 &matProj, D3DXMATRIXA16& matView, D3DXVECTOR3 vPos, int x, int y, LPRAY ray );

	bool RayCheck(D3DXVECTOR3 vMin,D3DXVECTOR3 vMax, float fScaling, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);

	void ScreenPosToWorldPos(const HWND hWnd,const D3DXMATRIXA16 &matProj,const D3DXMATRIXA16& matView,int x, int y, D3DXVECTOR3 &vWorldPos);

};