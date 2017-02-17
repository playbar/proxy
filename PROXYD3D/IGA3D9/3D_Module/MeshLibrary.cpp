/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : MeshLibrary.cpp
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"
#include "IGA3D9.h"
#include "IGA3DModule.h"
#include "MeshLibrary.h"

extern float g_fDistPick;  //距离参数

CMeshLibrary g_CMeshLibrary;

CMeshLibrary::CMeshLibrary()
 : m_pd3dDevice(NULL),
   m_pAnimInstance(NULL)
{
}

CMeshLibrary::~CMeshLibrary()
{
	Destory();
}


HRESULT CMeshLibrary::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;

	m_pd3dDevice = pd3dDevice;

	if ( ! m_mapMeshLibrary.empty() )
	{
		m_mapMeshLibrary.clear();
	}
	if (! m_listMeshIDLibrary.empty() )
	{
		m_listMeshIDLibrary.clear();
	}
	if ( !m_setErrorID.empty() )
	{
		m_setErrorID.clear();
	}
	m_lRefCount = 0;

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
				m_mapIterator->second->ReleaseResource();
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


void CMeshLibrary::Render( DWORD dwID, D3DXMATRIX* pmatWorld, LPDIRECT3DTEXTURE9 pAdvTex, char* pchURL )
{
	//char szOutputDebug[256];
	//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
	//OutputDebugString( szOutputDebug );

	//先判断是不是没有的资源
	m_setIterator = m_setErrorID.find(dwID);
	if( m_setIterator != m_setErrorID.end() )
	{
		g_IGA3DModule.m_bIsLoad = false;
		return;
	}
	
	m_mapIterator = m_mapMeshLibrary.find( dwID );
	if( m_mapIterator == m_mapMeshLibrary.end() )
	{
		AddResource( dwID );
		return;	//不绘制这个对象
	}

	m_pAnimInstance = m_mapIterator->second;

	m_pAnimInstance->Update(1.0f/60.0f, pmatWorld );

	m_pAnimInstance->Render( pAdvTex );

	//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
	//OutputDebugString( szOutputDebug );

	//-------------------------------
	//2008-7-22: 拾取代码
		//static bool bIsPick = false;
		//short sStatus = 0;

		//SIGA3DMSG msg = g_IGA3DModule.m_Msg;
		//if( msg.key == IGA3D_MSG_RBUTTONDOWN )
		//{
		//	sStatus = -1;
		//}
	
		//if(sStatus < 0 ) 
		{
			//if( !bIsPick )
			{
				HWND hWnd = GetActiveWindow();
				if(hWnd != NULL )
				{
					//得到屏幕的大小
					IDirect3DSurface9 *pkD3DSurf;
					HRESULT hRes = m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pkD3DSurf);
					D3DSURFACE_DESC kD3DSurfDesc;
					pkD3DSurf->GetDesc(&kD3DSurfDesc);
					pkD3DSurf->Release();

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

					//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
					//OutputDebugString( szOutputDebug );

					float _fDist = 10000.0f;
					bool bHit = m_pAnimInstance->Pick(&vPickRayOrig, &vPickRayDir, NULL, &_fDist );
					if(bHit)		//拾取成功
					{
						//char text[256];
						//sprintf( text, "_fDist:%f, DistPick: %f", _fDist, fDistPick );
						//OutputDebugString(text);
						if( _fDist > g_fDistPick )	//距离参数
						{
							return;						
						}
					
						int _PickID = g_IGA3DModule.m_iPickID;
						if( _PickID == -1 )	//还没有对象被拾取上
						{
							g_IGA3DModule.m_iPickID = dwID;	//保存ID到3D模块
							g_IGA3DModule.m_fDistMin = _fDist;
							g_IGA3DModule.m_iUpdataObjID = g_IGA3DModule.m_iCurObjID;
						}
						else	//否则，这一帧中已经有对象被选中
						{
							//判断距离，选择距离摄影机最近的一个
							if( g_IGA3DModule.m_fDistMin > _fDist )
							{
								g_IGA3DModule.m_iPickID = dwID;	//保存ID到3D模块
								g_IGA3DModule.m_fDistMin = _fDist;
								g_IGA3DModule.m_iUpdataObjID = g_IGA3DModule.m_iCurObjID;
							}
						}

						//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
						//OutputDebugString( szOutputDebug );

						//char text[MAX_PATH];
						//sprintf( text,"拾取到模型;%d............",id );
						//OutputDebugString(text);

						//if( url[0] != '\0' )
						//{
						//	ShellExecute( hWnd, "open", url, NULL, NULL, SW_SHOW );
						//	//OutputDebugString("打开 url.............");
						//}
						//else
						//{
						//	//OutputDebugString("NULL 的 url.............");
						//}

						//if( g_IGA3DModule.m_p3DFocus != NULL )
						//{
						//	//OutputDebugString("m_pFun != NULL ");
						//	g_IGA3DModule.m_p3DFocus(id, 0);
						//}

						//....
						//g_IGA3DModule.m_PickID = -1;
					}
				}
			}
		}
		//else
		//{
		//	bIsPick = false; 
		//}

		//sprintf(szOutputDebug,  "file = %s, Line = %d", __FILE__, __LINE__ );
		//OutputDebugString( szOutputDebug );
	//2008-7-22 ↑
	//-------------------------------
}	

void CMeshLibrary::AddResource( DWORD id )
{
	//long  lRefCount = m_pd3dDevice->AddRef(); 

	//加载这个资源
	FxAnimInstance* pSkinMesh = new FxAnimInstance(m_pd3dDevice);

	char	szFileName[MAX_PATH];
	sprintf( szFileName, "m_%d.X", id );

	char acXFileName[MAX_PATH], acXFilePath[MAX_PATH], acXFileFullName[MAX_PATH];
	strcpy(acXFileName, szFileName);
	strcpy(acXFilePath, "InterAdtive\\EMMOA\\3D\\");
	strcpy(acXFileFullName, acXFilePath);
	strcat(acXFileFullName, acXFileName);
	GetModulePath( acXFileFullName );
	GetModulePath( acXFileName );

	HRESULT hr = pSkinMesh->InitialiseResource(acXFileFullName, acXFilePath);
	if( FAILED(hr) )
	{
		if( IS_3D_TOOL )
		{
			OutputDebugString("CSkinMeshLibrary:加载文件失败！");
		}
		delete pSkinMesh;
		m_setErrorID.insert(id);

		//m_pd3dDevice->Release();

		return;
	}
	g_IGA3DModule.m_bIsLoad = true;

	//把加载的网格放到MESH库中
	m_mapMeshLibrary.insert( MAP_MESH_NODE(id, pSkinMesh) );

	//--------------------------------------------
	//m_pd3dDevice->AddRef();
 //   long  lRefCount1 = m_pd3dDevice->Release();
	//long  lRefCount2 = lRefCount1 - lRefCount ;
	//m_pd3dDevice->Release();

	//m_lRefCount += lRefCount2;

	//if( g_IGA3DModule.m_pFunDeviceRefCount != NULL && lRefCount2 != 0 )
	//{
	//	g_IGA3DModule.m_pFunDeviceRefCount( m_pd3dDevice, lRefCount2, 0 );
	//}

}

void CMeshLibrary::ReMove( DWORD id )
{

}

void CMeshLibrary::Destory()
{
	Lost();
	m_listMeshIDLibrary.clear();
	m_setErrorID.clear();

	//if( g_IGA3DModule.m_pFunDeviceRefCount != NULL && m_lRefCount != 0 )
	//{
	//	g_IGA3DModule.m_pFunDeviceRefCount( m_pd3dDevice, -m_lRefCount, 0 );
	//}
	//m_lRefCount = 0;

	/*if ( m_pd3dDevice != NULL )
	{
		m_pd3dDevice.Release();
	}*/
	m_pd3dDevice = NULL;
}
