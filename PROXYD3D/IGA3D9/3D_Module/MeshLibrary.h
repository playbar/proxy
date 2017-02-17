/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : MeshLibrary.h
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once 
#include "d3d9.h"
#include "d3dx9.h"

#include <map>
#include <set>
#include <list>
using namespace std;

#include "FxAnimationInstance.h"
//---------------------------------------
//网格库
class CMeshLibrary
{
public:
	CMeshLibrary();
	~CMeshLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );
	void	Lost();	//设备丢失
	void	Resume();	//设备恢复
	void	Render( DWORD dwID, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* pchURL = NULL );	//根据需求绘制指定的图形
	void	Destory();	//释放资源
	void    ClearError(){ m_setErrorID.clear();};

	void	AddResource( DWORD id );	//根据ID添加资源
	void	ReMove( DWORD id );			//根据ID移除资源
	

private:
	IDirect3DDevice9*	m_pd3dDevice;
	FxAnimInstance *	m_pAnimInstance;

	typedef pair<DWORD, FxAnimInstance *>		MAP_MESH_NODE;
	map<DWORD, FxAnimInstance *>			m_mapMeshLibrary;	//保存网格的容器
	map<DWORD, FxAnimInstance*>::iterator	m_mapIterator;

//设备丢失、恢复时使用
	list<DWORD>				m_listMeshIDLibrary;	//在设备丢失时保存当前使用的网格信息的ID,在设备恢复的时候来重新加载他们
	list<DWORD>::iterator	m_listIterator;

	set<DWORD>				m_setErrorID;      //下面的对象用来记录加载时失败的资源ID,当加载失败的ID时候，就跳过不执行加载操作
	set<DWORD>::iterator	m_setIterator;	

public:
	long		m_lRefCount;

};

extern CMeshLibrary g_CMeshLibrary;

//CMeshLibrary* GetMeshLibrary();


