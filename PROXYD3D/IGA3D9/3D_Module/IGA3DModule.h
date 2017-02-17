/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : 3D_Core.h
*	功能描述：IGA3D接管实现类, modify by hgl
*	修改说明：添加类变量的访问权限，删除没有用到的变量的定义
*	D3D设备引用使用智能指针，自动管理设备释放
*	当前版本：2, 0, 9, 0304
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期： 
*************************************
*	修改说明：D3D设备引用使用智能指针，自动管理设备释放
*	当前版本：2, 0, 9, 0317
*	作者/修改者：hgl
*	完成日期：2009-3-17
*	取代版本: 2, 0, 9, 0304
*	修改人：
*	完成日期：
/************************************************************************/

#pragma  once 

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include <list>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "DefineConst.h"
#include "DefineStruct.h"
#include "GlobalFunction.h"

#include "MeshLibrary.h"
#include "IGA3D_UI.h"
#include "IGACamera.h"
#include "shellapi.h"
#include "z_Memory.h"
#include "tinyxml.h"
#include "IGA3D9.h"
#include "ShowPic.h"
#include "IGATexture9.h"

#include "atlbase.h"


typedef CShareMemory<STIGAStateData,1> IGA_SHAREMEMORY_DATA;


typedef HRESULT(WINAPI *pFunD3D9SearchInit)(LPDIRECT3DDEVICE9, LPARAM);
typedef void(WINAPI *pFunD3D9SearchRender)();
typedef void(WINAPI *pFunD3D9SearchDestory)();


//////////////////////////////////////////////////////////////////////////

class CIGA3DModule
{
public:
	CIGA3DModule();
	~CIGA3DModule();

public:
	int Init(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount);
	void Destory();

	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();
	void IGA3DInputState();
	void RunScript();
	void ComputeSceneID();
	int ReLoad();

public:
	void	SetGameWnd(HWND hWnd)	{ m_hGameWnd = hWnd; }
	HWND	GetGameWnd()			{ return m_hGameWnd; }

private:
	void CheckAdInfo();
	void SaveShowInfo();

//--------------------------------------
//2008-7-24添加：
//逻辑资源: 用来保存被拾取上的对象ID。
public:
	int		m_iPickID;	//被拾取上模型的ID,如果小于0表示没有拾取到任何3D对象
	float   m_fDistMin; //记录射线的最小值
	int		m_iCurObjID;	//当前的对象
	int		m_iUpdataObjID;	//当前的对象

	STIGA3DMSG	m_IGA3DMsg;	//当前消息
	bool		m_bGetMsg;	//
	bool		m_bUpdate;

public:
	//使用的D3D资源，是从游戏获取的 
	IDirect3DDevice9*	m_spD3DDevice;
	//IDirect3DDevice9 * m_pD3D9Device;
	IDirect3DDevice9*	m_spD3D9DeviceProxy; //接管d3d设备对象指针
	int m_iDeviceRefCount;

	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj; 
	D3DXMATRIX			m_matWorld;
	volatile bool		m_bInitView;
	volatile bool		m_bInitProj;
	volatile bool		m_bInitWprld;

private:	//保存游戏渲染状态
	DWORD	m_dwFVF;
	DWORD	m_dwLighting;
	DWORD	m_dwCullMode;
	DWORD	m_dwZEnable;
	DWORD	m_dwVertexBlend;
	DWORD	m_dwTexStage0ColorOp;
	DWORD	m_dwTexStage0ColorArg1;
	DWORD	m_dwTexStage0ColorArg2;

	//CComPtr<IDirect3DBaseTexture9>	m_D3DBaseTexture;

	//系统资源
	CComPtr<IDirect3DStateBlock9>	m_pkOriginalStateBlock;
	CComPtr<IDirect3DStateBlock9>	m_pkProxyStateBlock;
	HWND	m_hGameWnd;   //游戏窗口句柄

public:
	bool	m_bFrist;
	int		m_iDrawPos;	//在Core里绘制的位置
	DWORD	m_dwGameID;	//游戏ID
	bool	m_bIsLoad;	//模型是否加载成功

private:
	list<STDrawElement>	m_listDrawLibrary;    //模型列表
	bool				m_bUseClip;
	bool				m_bCheckXML;

public:
	LPCallBack3DFocus			m_pFun3DFocus;		//点击后的回调函数
	LPCallBackDeviceRefCount	m_pFunDeviceRefCount; //设备引用计数

private:
	//D3D9EditSearch游戏界面显示搜索框
	BOOL					InitD3D9Search();
	tagD3DSearchParam		m_SearchInitParam;
	bool					m_bSearchLoad;
	HMODULE					m_hD3D9Search;
	pFunD3D9SearchInit		m_pFunD3D9SearchInit;
	pFunD3D9SearchRender	m_pFunD3D9SearchRender;
	pFunD3D9SearchDestory	m_pFunD3D9SearchDestory;

private:
	//显示图片
	CShowPic			*m_pShowPic;
	CD3D9ManageTexture	 m_D3D9ManageTexture;

};

extern CIGA3DModule g_IGA3DModule;


