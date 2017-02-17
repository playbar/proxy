#include "stdafx.h"
#include "FxAnimationInstance.h"
#include "FxXAnimation.h"
#include "FxAllocateHierarchy.h"


FxAnimInstance::FxAnimInstance(IDirect3DDevice9 *pkD3DDevice)
{
	m_pkD3DDevice = pkD3DDevice;
	m_pkAH = new FxAllocateHierarchy;

	m_pkFrameRoot = NULL;
	m_pkAnimController = NULL;
}

FxAnimInstance::~FxAnimInstance()
{
	delete m_pkAH;
}

HRESULT FxAnimInstance::InitialiseResource(const char *pcFileFullName, const char *pcFilePath)
{
	m_pkAH->SetFilePath(pcFilePath);
	HRESULT hRes = D3DXLoadMeshHierarchyFromX(pcFileFullName, D3DXMESH_MANAGED, m_pkD3DDevice, m_pkAH, NULL, &m_pkFrameRoot, &m_pkAnimController);
	if(SUCCEEDED(hRes))
		hRes = SetupBoneMatrixPointers(m_pkFrameRoot);

	return hRes;
}

HRESULT FxAnimInstance::ReleaseResource()
{
	if(m_pkFrameRoot)
		ReleaseAttributeTable(m_pkFrameRoot);

	HRESULT hRes = D3DXFrameDestroy(m_pkFrameRoot, m_pkAH);

	SAFE_RELEASE(m_pkAnimController);

	return hRes;
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
	UINT iBone, cBones;
	D3DXFRAME_DERIVED *pFrame;

	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( m_pkFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
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
	D3DXMESHCONTAINER_DERIVED *pMeshContainer;

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
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

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
	if( m_pkAnimController != NULL )
		m_pkAnimController->AdvanceTime( fElapsedTime, NULL );

	UpdateFrameMatrices(m_pkFrameRoot, pParentMatrix);
}

void FxAnimInstance::RenderMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, LPDIRECT3DTEXTURE9 pTex )
{
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	UINT iMaterial;
	UINT iAttrib;

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		D3DXMATRIX  Identity;
		DWORD       cBones  = pMeshContainer->pSkinInfo->GetNumBones();
		DWORD       iBone;
		PBYTE       pbVerticesSrc;
		PBYTE       pbVerticesDest;

		// set up bone transforms
		for (iBone = 0; iBone < cBones; ++iBone)
			D3DXMatrixMultiply(&pMeshContainer->pBoneMatrices[iBone], &pMeshContainer->pBoneOffsetMatrices[iBone], pMeshContainer->ppBoneMatrixPtrs[iBone]);
			//D3DXMatrixMultiply(&g_pBoneMatrices[iBone], &pMeshContainer->pBoneOffsetMatrices[iBone], pMeshContainer->ppBoneMatrixPtrs[iBone]);

		// set world transform
		D3DXMatrixIdentity(&Identity);
		m_pkD3DDevice->SetTransform(D3DTS_WORLD, &Identity);

		pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);

		// generate skinned mesh
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);
		//pMeshContainer->pSkinInfo->UpdateSkinnedMesh(g_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);

		pMeshContainer->pOrigMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
		{
			if( pTex == NULL )
			{
				m_pkD3DDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
			}
			else
			{
				if( iAttrib == 0 )
				{
					m_pkD3DDevice->SetTexture(0, pTex);
				}
				else
				{
					m_pkD3DDevice->SetTexture(0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
				}
			}

			m_pkD3DDevice->SetMaterial(&(pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
			pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
		}
	}
	else  // standard mesh, just draw it after setting material properties
	{
		m_pkD3DDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			if( pTex == NULL )
			{
				m_pkD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
			}
			else
			{
				if( iMaterial == 0 )
				{
					m_pkD3DDevice->SetTexture(0, pTex);
				}
				else
				{
					m_pkD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
				}
			}
			m_pkD3DDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

void FxAnimInstance::Render( LPDIRECT3DTEXTURE9 pTex, LPD3DXFRAME pFrame )
{
	if(!pFrame)
		pFrame = m_pkFrameRoot;

	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		RenderMeshContainer(pMeshContainer, pFrame, pTex );

		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		Render( pTex, pFrame->pFrameSibling );
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		Render( pTex, pFrame->pFrameFirstChild );
	}

}

//-----------------------------------------------------------------------------------------------------
//2008-7-22: 添加的有关拾取的代码，（射线拾取）
void FxAnimInstance::GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const
{ 
	D3DXFrameCalculateBoundingSphere(m_pkFrameRoot, pObjectCenter, pfObjectRadius); 
}

void FxAnimInstance::GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const
{
	D3DXVECTOR3 kCenter;
	float fRadius;
	GetBoundingSphereRadius(&kCenter, &fRadius);

	*pObjMin = kCenter - D3DXVECTOR3(fRadius, fRadius, fRadius);
	*pObjMax = kCenter + D3DXVECTOR3(fRadius, fRadius, fRadius);
}

bool FxAnimInstance::Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, LPD3DXFRAME pFrame)
{
	if(!pFrame)
		pFrame = m_pkFrameRoot;

	bool bRet = false;
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		bRet = PickMeshContainer(pMeshContainer, pRayPos, pRayDir);

		if(bRet)
			return true;
		else
			pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		bRet = Pick(pRayPos, pRayDir, pFrame->pFrameSibling);
		if(bRet)
			return true;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		bRet = Pick(pRayPos, pRayDir, pFrame->pFrameFirstChild);
		if(bRet)
			return true;
	}

	return false;
}

bool FxAnimInstance::PickMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir)
{
	BOOL bHit = 0;
	HRESULT hRes = D3DXIntersect(pMeshContainer->MeshData.pMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);

	return bHit != 0; 
}
//2008-7-22  ↑
//-------------------------------------------------------------------------------
