#include <map>
#include <set>
#include <list>
using namespace std;

//#include "d3d8.h"
//#include "d3dx8.h"

//------------------------------------------------------------------------
//#include "FxSkeletalMesh.h"
//#include "FxSkeletalAnim.h"

#include "3D_Module\FxAnimationInstance.h"
//---------------------------------------
//�����
class CMeshLibrary
{
public:
	CMeshLibrary();
	~CMeshLibrary();

public:
	HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );
	void	Lost();	//�豸��ʧ
	void	Resume();	//�豸�ָ�
	void	Render( DWORD id, D3DXMATRIX* matWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* url = NULL );	//�����������ָ����ͼ��
	void	Destory();	//�ͷ���Դ
	void    ClearError(){ m_ErrorID.clear();};

private:
	void	AddResource( DWORD id );	//����ID������Դ
	void	ReMove( DWORD id );			//����ID�Ƴ���Դ
	

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;

	typedef map<DWORD, FxAnimInstance *>		MESH_LIBRARY;
	typedef pair<DWORD, FxAnimInstance *>		MESH_NODE;

	MESH_LIBRARY					m_MeshLibrary;	//�������������
	map<DWORD, FxAnimInstance*>::iterator	m_pNode;

	FxAnimInstance *							m_pMesh;

//�豸��ʧ���ָ�ʱʹ��
private:
	typedef list<DWORD>		MESH_ID;
	MESH_ID					m_MeshIDLibrary;	//���豸��ʧʱ���浱ǰʹ�õ�������Ϣ��ID
												//���豸�ָ���ʱ�������¼�������

	list<DWORD>::iterator	m_pNodeID;

//����Ķ���������¼����ʱʧ�ܵ���ԴID
//������ʧ�ܵ�IDʱ�򣬾�������ִ�м��ز���
private:
	typedef set<DWORD>			MESH_ERROR_ID;
	MESH_ERROR_ID				m_ErrorID;

	set<DWORD>::iterator		m_pErrorNode;	

};

CMeshLibrary* GetMeshLibrary();

