/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : FxAnimationInstance.h
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#include "stdafx.h"

#include "DefineConst.h"
#include "FxXAnimation.h"
#include "FxAllocateHierarchy.h"
#include "FxAnimationInstance.h"

extern bool  g_bShowPickMesh;   // 是否显示网格

FxAnimInstance::FxAnimInstance(IDirect3DDevice9 *pD3DDevice)
{
	m_spD3DDevice = pD3DDevice;

	m_pFxAllocateHierarchy = new FxAllocateHierarchy;

	m_pFrameRoot = NULL;
	m_spAnimController = NULL;

	ZeroMemory( &m_D3DMaterialWhite, sizeof(m_D3DMaterialWhite) );
	m_D3DMaterialWhite.Diffuse.a = 1.0f;
	m_D3DMaterialWhite.Diffuse.b = 1.0f;
	m_D3DMaterialWhite.Diffuse.g = 1.0f;
	m_D3DMaterialWhite.Diffuse.r = 1.0f;
	m_D3DMaterialWhite.Ambient = m_D3DMaterialWhite.Diffuse; 
}

FxAnimInstance::~FxAnimInstance()
{
	if ( m_pD3DMesh != NULL )
	{
		m_pD3DMesh.Release();
	}

	if ( m_spD3DDevice != NULL )
	{
		m_spD3DDevice.Release();
	}
	m_pFxAllocateHierarchy = NULL;

	m_pFrameRoot = NULL;
	m_spAnimController = NULL;

}

HRESULT FxAnimInstance::InitialiseResource(LPCSTR pcFileFullName, LPCSTR pcFilePath)
{
	//先以普通网格的形式加载
	HRESULT hr = D3DXLoadMeshFromX( pcFileFullName, D3DXMESH_MANAGED, m_spD3DDevice, NULL, NULL, NULL, &m_dwNumMaterial, &m_pD3DMesh );
	if( SUCCEEDED(hr) )
	{
		LPD3DXMESH	pMesh = 0;
		hr = m_pD3DMesh->CloneMeshFVF( D3DXMESH_MANAGED, D3DFVF_XYZ, m_spD3DDevice, &pMesh );
		if( SUCCEEDED(hr) )
		{
			m_pD3DMesh.Release();
			m_pD3DMesh = pMesh;
		}
	}

	m_pFxAllocateHierarchy->SetFilePath(pcFilePath);
	HRESULT hRes = D3DXLoadMeshHierarchyFromX(pcFileFullName, D3DXMESH_MANAGED, m_spD3DDevice, m_pFxAllocateHierarchy, NULL, &m_pFrameRoot, &m_spAnimController);
	if(SUCCEEDED(hRes))
		hRes = SetupBoneMatrixPointers(m_pFrameRoot);

	return hRes;
}

HRESULT FxAnimInstance::ReleaseResource()
{
	if( m_pD3DMesh != NULL )
	{
		m_pD3DMesh.Release();
	}

	if(m_pFrameRoot)
		ReleaseAttributeTable(m_pFrameRoot);

	HRESULT hRes = D3DXFrameDestroy(m_pFrameRoot, m_pFxAllocateHierarchy);

	if ( m_spAnimController != NULL )
	{
		m_spAnimController.Release();
	}

	SAFE_DELETE( m_pFxAllocateHierarchy );

	return S_OK;
}


HRESULT FxAnimInstance::SetupBoneMatrixPointers(LPD3DXFRAME pFrame )
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

HRESULT FxAnimInstance::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	int iBone = 0;
	int iBones = 0;
	D3DXFRAME_DERIVED * pFrame = NULL;

	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	if (pMeshContainer->pSkinInfo != NULL)
	{
		iBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[iBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < iBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
			if (pFrame == NULL)
				return E_FAIL;

			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}

	return S_OK;
}

void FxAnimInstance::ReleaseAttributeTable( LPD3DXFRAME pFrameBase )
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;

	pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;

	while( pMeshContainer != NULL )
	{
		delete[] pMeshContainer->pAttributeTable;

		pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		ReleaseAttributeTable(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		ReleaseAttributeTable(pFrame->pFrameFirstChild);
	}
}

void FxAnimInstance::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, const LPD3DXMATRIX pParentMatrix )
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


void FxAnimInstance::Update(float fElapsedTime, const LPD3DXMATRIX pParentMatrix)
{
	if( m_spAnimController != NULL )
		m_spAnimController->AdvanceTime( fElapsedTime, NULL );

	UpdateFrameMatrices(m_pFrameRoot, pParentMatrix);

	//----------------------------------------------------------------------
	if( g_bShowPickMesh && m_pD3DMesh != NULL )
	{
		//OutputDebugString(".....");
		m_spD3DDevice->SetTransform( D3DTS_WORLD, pParentMatrix );

		m_spD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		for( DWORD i = 0; i < m_dwNumMaterial; i++ )
		{
			m_spD3DDevice->SetTexture(0,0);
			m_spD3DDevice->SetFVF(D3DFVF_XYZ);
			//m_pkD3DDevice->SetMaterial(&m_WhiteMaterial);
			m_pD3DMesh->DrawSubset(i);
		}
		m_spD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}

}

void FxAnimInstance::RenderMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, LPDIRECT3DTEXTURE9 pD3DTexture )
{
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	DWORD iMaterial = 0;
	DWORD iAttrib =0;

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		D3DXMATRIX  D3DMatrixIdentity;
		DWORD       dwBones  = pMeshContainer->pSkinInfo->GetNumBones();
		DWORD       dwBone;
		PBYTE       pbyteVerticesSrc;
		PBYTE       pbyteVerticesDest;

		// set up bone transforms
		for (dwBone = 0; dwBone < dwBones; ++dwBone)
		{
			D3DXMatrixMultiply(&pMeshContainer->pBoneMatrices[dwBone], &pMeshContainer->pBoneOffsetMatrices[dwBone], pMeshContainer->ppBoneMatrixPtrs[dwBone]);
			//D3DXMatrixMultiply(&g_pBoneMatrices[iBone], &pMeshContainer->pBoneOffsetMatrices[iBone], pMeshContainer->ppBoneMatrixPtrs[iBone]);
		}
		// set world transform
		D3DXMatrixIdentity(&D3DMatrixIdentity);
		m_spD3DDevice->SetTransform(D3DTS_WORLD, &D3DMatrixIdentity);

		pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbyteVerticesSrc);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbyteVerticesDest);

		// generate skinned mesh
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pBoneMatrices, NULL, pbyteVerticesSrc, pbyteVerticesDest);
		//pMeshContainer->pSkinInfo->UpdateSkinnedMesh(g_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);

		pMeshContainer->pOrigMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			if( pD3DTexture == NULL )
			{
				m_spD3DDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
			}
			else
			{
				if( iAttrib == 0 )
				{
					m_spD3DDevice->SetTexture(0, pD3DTexture);
				}
				else
				{
					m_spD3DDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
				}
			}

			m_spD3DDevice->SetMaterial(&(pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
			pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
		}
	}
	else  // standard mesh, just draw it after setting material properties
	{
		m_spD3DDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			if( pD3DTexture == NULL )
			{
				m_spD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
			}
			else
			{
				if( iMaterial == 0 )
				{
					m_spD3DDevice->SetTexture(0, pD3DTexture);
				}
				else
				{
					m_spD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
				}
			}
			m_spD3DDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

void FxAnimInstance::Render( LPDIRECT3DTEXTURE9 pD3DTexture, LPD3DXFRAME pD3DFrame )
{
	if( NULL == pD3DFrame)
		pD3DFrame = m_pFrameRoot;

	LPD3DXMESHCONTAINER pMeshContainer = pD3DFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		RenderMeshContainer(pMeshContainer, pD3DFrame, pD3DTexture );

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pD3DFrame->pFrameSibling != NULL)
	{
		Render( pD3DTexture, pD3DFrame->pFrameSibling );
	}

	if (pD3DFrame->pFrameFirstChild != NULL)
	{
		Render( pD3DTexture, pD3DFrame->pFrameFirstChild );
	}
}

//-----------------------------------------------------------------------------------------------------
//2008-7-22: 添加的有关拾取的代码，（射线拾取）
void FxAnimInstance::GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const
{ 
	D3DXFrameCalculateBoundingSphere(m_pFrameRoot, pObjectCenter, pfObjectRadius); 
}

void FxAnimInstance::GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const
{
	D3DXVECTOR3 kCenter;
	float fRadius;
	GetBoundingSphereRadius(&kCenter, &fRadius);

	*pObjMin = kCenter - D3DXVECTOR3(fRadius, fRadius, fRadius);
	*pObjMax = kCenter + D3DXVECTOR3(fRadius, fRadius, fRadius);
}

bool FxAnimInstance::Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, LPD3DXFRAME pFrame, float* pfDist )
{
	BOOL bHit = 0;
	float _fDist = 0.0f;
	HRESULT hRes = D3DXIntersect(m_pD3DMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, &_fDist, NULL, NULL);
	if( SUCCEEDED(hRes) )
	{
		//OutputDebugString("Pick operator is succeeded!");
		if( bHit && pfDist != NULL )
		{
			//OutputDebugString("Pick......");
			if( _fDist < *pfDist )
			{
				*pfDist = _fDist;
				return true;
			}
		}
	
	}
	return false;

}

bool FxAnimInstance::PickMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, float* pfDist )
{
	BOOL bHit = 0;
	float _fDist = 0.0f;
	HRESULT hRes = D3DXIntersect(pMeshContainer->MeshData.pMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, &_fDist, NULL, NULL);

	//2008-10-13距离判断
	if( bHit == TRUE && pfDist != NULL )
	{	
		if( _fDist < *pfDist )	//这个值可以设定,针对《神泣》
		{
			*pfDist = _fDist;
			return true;
		}
	}

	return false; 
}


