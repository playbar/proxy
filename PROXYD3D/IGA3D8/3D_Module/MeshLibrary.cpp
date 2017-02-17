#include "stdafx.h"
#include "tchar.h"
#include "z_FileIni.h"
#include "3D_Core.h"
#include "MeshLibrary.h"


CMeshLibrary g_CMeshLibrary;

CMeshLibrary::CMeshLibrary()
{
	m_pd3dDevice = NULL;
}

CMeshLibrary::~CMeshLibrary()
{
}


HRESULT CMeshLibrary::Init( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	m_mapMeshLibrary.clear();
	
	m_pFxSkeletalMesh	= NULL;
	m_listMeshIDLibrary.clear();
	m_setErrorID.clear();
	return hr;
}

void CMeshLibrary::Lost()
{
	for( m_mapIterator = m_mapMeshLibrary.begin(); m_mapIterator != m_mapMeshLibrary.end(); m_mapIterator++ )
	{
		DWORD	dwID = m_mapIterator->first;
		m_listMeshIDLibrary.push_back(dwID);
		try
		{
			if( m_mapIterator->second != NULL )
			{
				delete m_mapIterator->second;
				m_mapIterator->second = NULL;
			}
		}
		catch(...)
		{
			if( IS_3D_TOOL )
			{
				OutputDebugString("occur exception at resource release!");
			}
		}
	}

	m_mapMeshLibrary.clear();
}

void CMeshLibrary::Resume()
{
	for( m_listIterator = m_listMeshIDLibrary.begin(); m_listIterator != m_listMeshIDLibrary.end(); m_listIterator++ )
	{
		DWORD	dwID = *m_listIterator;
		AddResource( dwID );	
	}
	m_listMeshIDLibrary.clear();
}

extern float g_floatPick;

void CMeshLibrary::Render( DWORD dwID, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE8 pAdvTex,  char* pchURL )
{
	m_setIterator = m_setErrorID.find(dwID);
	if( m_setIterator != m_setErrorID.end() )
	{
		g_IGA3DModule.m_bIsLoad = false;
		return;
	}

	//在看看是否在当前库中
	m_mapIterator = m_mapMeshLibrary.find( dwID );
	if( m_mapIterator == m_mapMeshLibrary.end() )
	{
		AddResource( dwID );
		return;	//不绘制这个对象
	}

	//否则绘制这个网格对象
	m_pFxSkeletalMesh = m_mapIterator->second;

	m_pd3dDevice->SetTransform(D3DTS_WORLD, pmatWorld );
	m_pFxSkeletalMesh->Update(g_IGA3DModule.m_lElapseTime * 5, true );
	m_pFxSkeletalMesh->Render( pAdvTex );

	//----------------------
	//2008-7-23
	//添加的拾取代码

	HWND hWnd = GetActiveWindow();
	if(NULL != hWnd)
	{
		//得到屏幕的大小
		IDirect3DSurface8 *pickUpD3DSurface;
		HRESULT hRes = m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pickUpD3DSurface);
		D3DSURFACE_DESC kD3DSurfDesc;
		pickUpD3DSurface->GetDesc(&kD3DSurfDesc);
		pickUpD3DSurface->Release();

		//计算射线的方向和起点
		D3DXVECTOR3 vPickRayDir;
		D3DXVECTOR3 vPickRayOrig;

		//分别为视图矩阵，模型（世界）矩阵，投影矩阵
		D3DXMATRIX m_kView, m_kWorld, m_kProjection;
		m_kView = g_IGA3DModule.m_matView;
		m_kProjection = g_IGA3DModule.m_matProj;
		m_kWorld      = *pmatWorld;

		POINT kCursor;
		GetCursorPos(&kCursor);
		ScreenToClient(hWnd, &kCursor);

		D3DVECTOR v;
		v.x = (((2.0f * kCursor.x) / kD3DSurfDesc.Width) - 1) / m_kProjection._11;
		v.y = -(((2.0f * kCursor.y) / kD3DSurfDesc.Height) - 1) / m_kProjection._22;
		v.z = 1.0f;

		D3DXMATRIX mWorldView = m_kWorld * m_kView;
		D3DXMATRIX m;
		D3DXMatrixInverse(&m, NULL, &mWorldView);

		vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
		vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
		vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
		vPickRayOrig.x = m._41;
		vPickRayOrig.y = m._42;
		vPickRayOrig.z = m._43;

		float _fDist = 10000.0f;
		bool bHit = m_pFxSkeletalMesh->Pick(&vPickRayOrig, &vPickRayDir, NULL, &_fDist );
		if(bHit)		//拾取成功
		{
			//OutputDebugString("pick.....");
			//进行有效距离判断
			if( _fDist > g_floatPick )
			{
				return;
			}

			int nPickID = g_IGA3DModule.m_PickID;
			if( nPickID == -1 )	//还没有网格被拾取上
			{
				g_IGA3DModule.m_PickID = dwID;	//保存ID到3D模块
				g_IGA3DModule.m_fDistMin = _fDist;
				g_IGA3DModule.m_UpdataObjID = g_IGA3DModule.m_CurObjID;
			}
			else 
			{
				//判断距离，选择距离摄影机最近的一个
				if( g_IGA3DModule.m_fDistMin > _fDist )
				{
					g_IGA3DModule.m_PickID = dwID;	//保存ID到3D模块
					g_IGA3DModule.m_fDistMin = _fDist;
					g_IGA3DModule.m_UpdataObjID = g_IGA3DModule.m_CurObjID;
				}
			}
		}
	}
	
}

void CMeshLibrary::AddResource( DWORD id )
{
	long  lRefCount = m_pd3dDevice->AddRef(); 
	
	//加载这个资源
	CFxSkeletalMesh* pSkinMesh = new CFxSkeletalMesh(m_pd3dDevice);

	char	szFileName[MAX_PATH];
	sprintf( szFileName, "m_%d.X", id );

	char szXFileName[MAX_PATH], szXFilePath[MAX_PATH], szXFileFullName[MAX_PATH];
	strcpy(szXFileName, szFileName);
	strcpy(szXFilePath, "InterAdtive\\EMMOA\\3D\\");
	strcpy(szXFileFullName, szXFilePath);
	strcat(szXFileFullName, szXFileName);

	GetModulePath( szXFileFullName );
	GetModulePath( szXFileName );

	bool bIsLoad = pSkinMesh->LoadFromXFile(szXFileFullName, szXFilePath, 0, D3DXMESH_SYSTEMMEM);
	if( !bIsLoad )
	{
		if( IS_3D_TOOL )
		{
			OutputDebugString("CSkinMeshLibrary:加载文件失败！");
		}
		delete pSkinMesh;
		m_setErrorID.insert(id);
		m_pd3dDevice->Release();
		return;
	}

	g_IGA3DModule.m_bIsLoad = true;
	m_mapMeshLibrary.insert( MESH_NODE(id, pSkinMesh) );

	//--------------------------------------------
	m_pd3dDevice->AddRef();
    long  lRefCount1 = m_pd3dDevice->Release();
	long  lRefCount2 = lRefCount1 - lRefCount ;
	m_pd3dDevice->Release();

	m_longRefCount += lRefCount2;
		//char text[MAX_PATH];
		//sprintf(text, "RefCount: %d", lRefCount2);
		//OutputDebugString(text);
	if( g_IGA3DModule.m_pDeviceRefCount != NULL && lRefCount2 != 0 )
	{
		g_IGA3DModule.m_pDeviceRefCount( m_pd3dDevice, lRefCount2, 0 );
	}
}

void CMeshLibrary::ReMove( DWORD id )
{

}

void CMeshLibrary::Destory()
{
	Lost();
	m_listMeshIDLibrary.clear();
	m_setErrorID.clear();

	if( g_IGA3DModule.m_pDeviceRefCount != NULL && m_longRefCount != 0 )
	{
		g_IGA3DModule.m_pDeviceRefCount( m_pd3dDevice, -m_longRefCount, 0 );
	}

	m_longRefCount = 0;
}
