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
	void    ClearError(){ m_ErrorID.clear();};

private:
	void	AddResource( DWORD id );	//根据ID添加资源
	void	ReMove( DWORD id );			//根据ID移除资源


private:
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	typedef map<DWORD, FxSkeletalMesh *>		MESH_LIBRARY;
	typedef pair<DWORD, FxSkeletalMesh *>		MESH_NODE;

	MESH_LIBRARY					m_MeshLibrary;	//保存网格的容器
	map<DWORD, FxSkeletalMesh *>::iterator	m_pNode;

	FxSkeletalMesh *							m_pMesh;

//设备丢失、恢复时使用
private:
	typedef list<DWORD>		MESH_ID;
	MESH_ID					m_MeshIDLibrary;	//在设备丢失时保存当前使用的网格信息的ID
												//在设备恢复的时候来重新加载他们

	list<DWORD>::iterator	m_pNodeID;

//下面的对象用来记录加载时失败的资源ID
//当加载失败的ID时候，就跳过不执行加载操作
private:
	typedef set<DWORD>			MESH_ERROR_ID;
	MESH_ERROR_ID				m_ErrorID;

	set<DWORD>::iterator		m_pErrorNode;	

};

CMeshLibrary* GetMeshLibrary();


