
#pragma  once

#include <map>
#include <set>
#include <list>
using namespace std;

#include "d3d8.h"
#include "d3dx8.h"

//------------------------------------------------------------------------
#include "FxSkeletalMesh.h"
#include "FxSkeletalAnim.h"

//---------------------------------------
//网格库
class CMeshLibrary
{
public:
	CMeshLibrary();
	~CMeshLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE8 pd3dDevice );
	void	Lost();	//设备丢失
	void	Resume();	//设备恢复
	void	Render( DWORD id, D3DXMATRIX* matWorld, LPDIRECT3DTEXTURE8 pAdvTex, char* url = NULL );	//根据需求绘制指定的图形
	void	Destory();	//释放资源
	void    ClearError(){ m_setErrorID.clear();};

private:
	void	AddResource( DWORD id );	//根据ID添加资源
	void	ReMove( DWORD id );			//根据ID移除资源


private:
	typedef pair<DWORD, CFxSkeletalMesh *>		MESH_NODE;
	map<DWORD, CFxSkeletalMesh *>			m_mapMeshLibrary;	//保存网格的容器
	map<DWORD, CFxSkeletalMesh *>::iterator	m_mapIterator;

	CFxSkeletalMesh *	m_pFxSkeletalMesh;
	LPDIRECT3DDEVICE8	m_pd3dDevice;

//设备丢失、恢复时使用
private:

	list<DWORD>				m_listMeshIDLibrary;	//在设备丢失时保存当前使用的网格信息的ID,在设备恢复的时候来重新加载他们
	list<DWORD>::iterator	m_listIterator;

	set<DWORD>				m_setErrorID;
	set<DWORD>::iterator	m_setIterator;
	long					m_longRefCount;

};

extern CMeshLibrary g_CMeshLibrary;

//CMeshLibrary* GetMeshLibrary();


