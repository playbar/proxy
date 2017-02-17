
#pragma  once


#include <d3d8.h>
#include <d3dx8.h>
#include <list>
#include "vector"
#include <time.h>
#include "tinyxml.h""
#include "define.h"
#include "z_Memory.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "IGA3D8.h"
#include "MeshLibrary.h"

class IGA3DModule
{
public:
	IGA3DModule();
	~IGA3DModule();

public:
	int Init();
	void Destory();
	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();
	void IGA3DInputState();
	void RunScript();
	int ReLoad();
private:
	void CheckAdInfo();
	void SaveShowInfo();
//--------------------------------------
//2008-7-24添加：
//逻辑资源: 用来保存被拾取上的对象ID。
public:

	int		m_PickID;			//被拾取上模型的ID,如果小于0表示没有拾取到任何3D对象
	float   m_fDistMin;			//记录射线的最小值
	int		m_CurObjID;			//当前的对象
	int		m_UpdataObjID;		//当前的对象	

	bool		m_bGetMsg;	
	bool		m_bUpdate;
	SIGA3DMSG	m_Msg;			//当前消息
//-------------------------------------
public:

	void GetCameraPos();
//系统资源
public:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;
	D3DXMATRIX			m_matWorld;
	bool				m_bInitView;
	bool				m_bInitProj;
	bool				m_bInitWorld;

public:
	BOOL		m_bFrist;
	long		m_lElapseTime;	//当前相同运行时间
	long		m_lStart;	
	bool		m_bIsLoad;		//模型是否加载成功
	DWORD		m_dwState;
	int			m_iDrawPos;		//在Core里绘制的位置

	ENUM_IGA_GAME_LIST			m_enumGameID; 

private:
	CFxSkeletalMesh *m_pkFxSkelMesh;
	STDrawElement	m_stDrawElement;

private:

	list<STDrawElement>				m_listDrawLibrary;
	list<STDrawElement>::iterator	m_listIterator;
	int								m_iCurrentScene;

public:
	LPIGAFUN					m_pFun;
	LPCallBack3DFocus			m_pFun3DFocus;		//点击后的回调函数
	LPCallBackDeviceRefCount	m_pDeviceRefCount; //设备引用计数

//2008-8-14:IGA纹理
public:

	
	bool		m_bUseClip;

private:

	typedef HRESULT(*pFunInit)(LPDIRECT3DDEVICE8);
	typedef LPDIRECT3DTEXTURE8(*pFunGetTexture)(int,bool*, int *, int *);
	typedef void(*pFunDestroy)();
	typedef void(*pFunStopPlayer)(DWORD);

	HMODULE			m_hModule;
	pFunInit		m_pFunInit;
	pFunGetTexture	m_pFunGetTexture;
	pFunDestroy		m_pFunDestory;
	pFunStopPlayer  m_pFunStopPlayer;

	bool			m_bIGATexture;


	
};

extern IGA3DModule g_IGA3DModule;




