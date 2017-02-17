#include "stdafx.h"
#include "FxXAnimation.h"


HRESULT GenerateSkinnedMesh( IDirect3DDevice9 *pd3dDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer )
{
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps( &d3dCaps );

	if( pMeshContainer->pSkinInfo == NULL )
		return hr;

	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

	hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(), pd3dDevice, &pMeshContainer->MeshData.pMesh);
	if (FAILED(hr))
		return hr;

	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
	if (FAILED(hr))
		return hr;

	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];

	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
	if (FAILED(hr))
		return hr;

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	unsigned int uiNumBoneMatrices = pMeshContainer->pSkinInfo->GetNumBones();

	// Allocate space for blend matrices
	delete[] pMeshContainer->pBoneMatrices; 
	pMeshContainer->pBoneMatrices  = new D3DXMATRIXA16[uiNumBoneMatrices];

	return hr;
}
