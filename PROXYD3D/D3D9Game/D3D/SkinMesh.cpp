
#include "SkinMesh.h"




HRESULT AllocateName( LPCSTR Name, LPSTR *pNewName )
{
    UINT cbLength;

    if( Name != NULL )
    {
        cbLength = (UINT)strlen(Name) + 1;
        *pNewName = new CHAR[cbLength];
        if (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy( *pNewName, Name, cbLength*sizeof(CHAR) );
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////////

HRESULT CAllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;

    *ppNewFrame = NULL;

    pFrame = new D3DXFRAME_DERIVED;
    if (pFrame == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    hr = AllocateName(Name, &pFrame->Name);
    if (FAILED(hr))
        goto e_Exit;

    // initialize other data members of the frame
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;

    *ppNewFrame = pFrame;
    pFrame = NULL;

e_Exit:
    delete pFrame;
    return hr;
}





HRESULT CAllocateHierarchy::CreateMeshContainer(
    LPCSTR Name, 
    CONST D3DXMESHDATA *pMeshData,
    CONST D3DXMATERIAL *pMaterials, 
    CONST D3DXEFFECTINSTANCE *pEffectInstances, 
    DWORD NumMaterials, 
    CONST DWORD *pAdjacency, 
    LPD3DXSKININFO pSkinInfo, 
    LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;

    LPD3DXMESH pMesh = NULL;

    *ppNewMeshContainer = NULL;

    // this sample does not handle patch meshes, so fail when one is found
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;

    // this sample does not FVF compatible meshes, so fail when one is found
    if (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;        

    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();

    // if no normals are in the mesh, add them
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // clone the mesh to make room for the normals
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pd3dDevice, &pMeshContainer->MeshData.pMesh );
        if (FAILED(hr))
            goto e_Exit;

        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;

        // now generate the normals for the pmesh
        D3DXComputeNormals( pMesh, NULL );
    }
    else  // if no normals, just add a reference to the mesh for the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }
        
    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // if materials provided, copy them
    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                if( FAILED( D3DXCreateTextureFromFileA( pd3dDevice, pMeshContainer->pMaterials[iMaterial].pTextureFilename,
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = NULL;

                // don't remember a pointer into the dynamic memory, just forget the name after loading
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }

        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
        hr = m_pSkinMesh->GenerateSkinnedMesh( pMeshContainer );
        if (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;

e_Exit:
    SAFE_RELEASE(pd3dDevice);

    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}

HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree ) 
{
	SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}


HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED * pMeshContainer = ( D3DXMESHCONTAINER_DERIVED * )pMeshContainerBase;
	
	SAFE_DELETE_ARRAY( pMeshContainer->Name );						
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	
    //释放Texture
    if( pMeshContainer->ppTextures != NULL )
    {
        for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial ++ )
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }
	
    SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
    SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pSkinInfo );
    SAFE_RELEASE( pMeshContainer->pOrigMesh );
    SAFE_DELETE( pMeshContainer );
    return S_OK;
}




////////////////////////////////////////////////////////////////////////////////////////

CSkinMesh::CSkinMesh() : m_vMax(0,0,0), m_vMin(0,0,0)  
{
	m_pDevice = g_pMyD3D->GetDevice();
	m_fElapsedTime = 0.0f;
	m_pDevice->GetDeviceCaps( &m_d3dCaps );
    m_pAnimController = NULL;
    m_pFrameRoot = NULL;
    m_SkinningMethod = D3DINDEXED;
    m_pBoneMatrices = NULL;
    m_NumBoneMatricesMax = 0;
	m_pMesh = NULL;
}

CSkinMesh::~CSkinMesh()
{
	CAllocateHierarchy Alloc( this );
	D3DXFrameDestroy( m_pFrameRoot, &Alloc );

    SAFE_RELEASE( m_pAnimController );
}

bool CSkinMeshObj::RayCheck( D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
{
	D3DXMATRIX  mMat;
	D3DXVECTOR3 vMin = m_pSkinMesh->m_vMin;
	D3DXVECTOR3 vMax = m_pSkinMesh->m_vMax;
	D3DXMatrixScaling(&mMat,m_fScaling,m_fScaling,m_fScaling);
	APP_Tools::appVec3Transform(vMin,mMat);
	APP_Tools::appVec3Transform(vMax,mMat);
	vMin = vMin + m_vPos;
	vMax = vMax + m_vPos;
	if (D3DXBoxBoundProbe(&vMin,&vMax,&vPos,&vDir))
		return true;
	else 
		return false;
}

HRESULT CSkinMesh::GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED *pMeshContainer )
{
    HRESULT hr = S_OK;

    if( pMeshContainer->pSkinInfo == NULL )
        return hr;

    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

    // if non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
    if( m_SkinningMethod == D3DNONINDEXED )
    {

        hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
                                   (
                                       pMeshContainer->pOrigMesh,
                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
                                       pMeshContainer->pAdjacency, 
                                       NULL, NULL, NULL, 
                                       &pMeshContainer->NumInfl,
                                       &pMeshContainer->NumAttributeGroups, 
                                       &pMeshContainer->pBoneCombinationBuf, 
                                       &pMeshContainer->MeshData.pMesh
                                   );
        if (FAILED(hr))
            goto e_Exit;


        // If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
        // Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
        // drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
        LPD3DXBONECOMBINATION rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

        // look for any set of bone combinations that do not fit the caps
        for (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups; pMeshContainer->iAttributeSW++)
        {
            DWORD cInfl   = 0;

            for (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
            {
                if (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
                {
                    ++cInfl;
                }
            }

            if (cInfl > m_d3dCaps.MaxVertexBlendMatrices)
            {
                break;
            }
        }

        // if there is both HW and SW, add the Software Processing flag
        if (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
        {
            LPD3DXMESH pMeshTmp;

            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING|pMeshContainer->MeshData.pMesh->GetOptions(), 
                                                pMeshContainer->MeshData.pMesh->GetFVF(),
                                                m_pDevice, &pMeshTmp);
            if (FAILED(hr))
            {
                goto e_Exit;
            }

            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMeshTmp;
            pMeshTmp = NULL;
        }
    }
    // if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if( m_SkinningMethod == D3DINDEXED )
    {
        DWORD NumMaxFaceInfl;
        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl);
        pIB->Release();
        if (FAILED(hr))
            goto e_Exit;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

        if( m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl )
        {
            // HW does not support indexed vertex blending. Use SW instead
            pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            // using hardware - determine palette size from caps and number of bones
            // If normals are present in the vertex data that needs to be blended for lighting, then 
            // the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
            pMeshContainer->NumPaletteEntries = min( ( m_d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2, 
                                                     pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL, 
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;
    }
    // if vertex shader indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if( ( m_SkinningMethod == D3DINDEXEDVS ) || ( m_SkinningMethod == D3DINDEXEDHLSLVS ) )
    {
        // Get palette size
        // First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
        // (96 - 9) /3 i.e. Maximum constant count - used constants 
        UINT MaxMatrices = 26; 
        pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
        if (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
        {
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }
        else
        {
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }

        SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL,             
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;


        // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
        DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
        if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
        {
            LPD3DXMESH pMesh;
            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, m_pDevice, &pMesh);
            if (!FAILED(hr))
            {
                pMeshContainer->MeshData.pMesh->Release();
                pMeshContainer->MeshData.pMesh = pMesh;
                pMesh = NULL;
            }
        }

        D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
        LPD3DVERTEXELEMENT9 pDeclCur;
        hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
        if (FAILED(hr))
            goto e_Exit;

        // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
        //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
        //          this is more of a "cast" operation
        pDeclCur = pDecl;
        while (pDeclCur->Stream != 0xff)
        {
            if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
                pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
            pDeclCur++;
        }

        hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
        if (FAILED(hr))
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if( m_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
        {
            m_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            delete[] m_pBoneMatrices; 
            m_pBoneMatrices  = new D3DXMATRIXA16[m_NumBoneMatricesMax];
            if( m_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }

    }
    // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    else if( m_SkinningMethod == SOFTWARE )
    {
        hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
                                              m_pDevice, &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
        if (FAILED(hr))
            goto e_Exit;

        delete[] pMeshContainer->pAttributeTable;
        pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
        if (pMeshContainer->pAttributeTable == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
        if (FAILED(hr))
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if (m_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
        {
            m_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            delete[] m_pBoneMatrices; 
            m_pBoneMatrices  = new D3DXMATRIXA16[m_NumBoneMatricesMax];
            if( m_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }
    }
    else  // invalid g_SkinningMethod value
    {        
        // return failure due to invalid skinning method value
        hr = E_INVALIDARG;
        goto e_Exit;
    }

e_Exit:
    return hr;
}



HRESULT CSkinMesh::LoadFromXFile( TCHAR * strFileName )
{
    HRESULT hr;
	CAllocateHierarchy Alloc( this );
	
    hr = D3DXLoadMeshHierarchyFromX( strFileName, D3DXMESH_MANAGED, m_pDevice, &Alloc, NULL, &m_pFrameRoot, &m_pAnimController );
    if( FAILED( hr ) )
        return hr;
	
    hr = SetupBoneMatrixPointers( m_pFrameRoot );
    if( FAILED( hr ) )
        return hr;
	
    hr = D3DXFrameCalculateBoundingSphere( m_pFrameRoot, &m_vObjectCenter, &m_fObjectRadius );
    if( FAILED( hr ) )
        return hr;

	D3DCOLORVALUE rgbaDiffuse  = {1.0, 1.0, 1.0, 0.0,};
	D3DCOLORVALUE rgbaAmbient  = {1.0, 1.0, 1.0, 0.0,};
	D3DCOLORVALUE rgbaSpecular = {0.0, 0.0, 0.0, 0.0,};
	D3DCOLORVALUE rgbaEmissive = {0.0, 0.0, 0.0, 0.0,};
	SetMaterial( rgbaDiffuse, rgbaAmbient, rgbaSpecular, rgbaEmissive, 0 );
	
	//box
	LPD3DXBUFFER pD3DXMtrlBuffer;
	DWORD		m_dwNumMaterials;
	D3DXLoadMeshFromX( strFileName, D3DXMESH_MANAGED, g_pMyD3D->GetDevice(), NULL,&pD3DXMtrlBuffer, NULL,&m_dwNumMaterials, &m_pMesh ) ;
	
	void * p;
	m_pMesh->LockVertexBuffer(0,&p);
	D3DXComputeBoundingBox((D3DXVECTOR3 *)p,m_pMesh->GetNumVertices(),m_pMesh->GetNumBytesPerVertex(),&m_vMin,&m_vMax);
	m_pMesh->UnlockVertexBuffer();
	
	m_pMesh->Release();
	return S_OK;
}


HRESULT CSkinMesh::SetupBoneMatrixPointers( LPD3DXFRAME pFrame )
{
    HRESULT hr;

    if( pFrame->pMeshContainer != NULL )
    {	
		//左序遍历
        hr = SetupBoneMatrixPointersOnMesh( pFrame->pMeshContainer );
        if( FAILED( hr ) )
            return hr;
    }

    if( pFrame->pFrameSibling != NULL )
    {
        hr = SetupBoneMatrixPointers( pFrame->pFrameSibling );
        if( FAILED( hr ) )
            return hr;
    }

    if( pFrame->pFrameFirstChild != NULL )
    {
        hr = SetupBoneMatrixPointers( pFrame->pFrameFirstChild );
        if( FAILED( hr ) )
            return hr;
    }

    return S_OK;
}

HRESULT CSkinMesh::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase )
{
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	//蒙皮动画，则设置骨骼的变换矩阵
    if( pMeshContainer->pSkinInfo != NULL )
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();

        pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];	//创建骨骼
        if( pMeshContainer->ppBoneMatrixPtrs == NULL )
            return E_OUTOFMEMORY;

        for( iBone = 0; iBone < cBones; iBone ++ )
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( m_pFrameRoot, 
				pMeshContainer->pSkinInfo->GetBoneName( iBone ) );

            if( pFrame == NULL )
                return E_FAIL;
			//初始化矩阵
            pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
        }
    }

    return S_OK;
}


VOID CSkinMesh::DrawFrame( LPD3DXFRAME pFrame )
{
    LPD3DXMESHCONTAINER pMeshContainer;
	
    pMeshContainer = pFrame->pMeshContainer;
    while( pMeshContainer != NULL )
    {
        DrawMeshContainer( pMeshContainer, pFrame );
        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }
	
    if( pFrame->pFrameSibling != NULL )
    {
        //DrawFrame( pFrame->pFrameSibling );
    }
	
    if( pFrame->pFrameFirstChild != NULL )
    {
        DrawFrame( pFrame->pFrameFirstChild );
    }
}

void CSkinMesh::DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase )
{
	D3DXMESHCONTAINER_DERIVED * pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    UINT NumBlend;
    UINT iAttrib;
    DWORD AttribIdPrev;
    LPD3DXBONECOMBINATION pBoneComb;

    UINT iMatrixIndex;
    UINT iPaletteEntry;
    D3DXMATRIXA16 matTemp;

    // first check for skinning
    if( pMeshContainer->pSkinInfo != NULL )
    {
        if( m_SkinningMethod == D3DNONINDEXED )
        {
            AttribIdPrev = UNUSED32; 
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

            // Draw using default vtx processing of the device (typically HW)
            for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
            {
                NumBlend = 0;
                for( DWORD i = 0; i < pMeshContainer->NumInfl; ++ i )
                {
                    if( pBoneComb[iAttrib].BoneId[i] != UINT_MAX )
                    {
                        NumBlend = i;
                    }
                }

                if( m_d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1 )
                {
                    // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                    for( DWORD i = 0; i < pMeshContainer->NumInfl; ++ i )
                    {
                        iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                        if( iMatrixIndex != UINT_MAX )
                        {
                            D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                            m_pDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp );
                        }
                    }

                    m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, NumBlend );

                    // lookup the material used for this subset of faces
                    if( ( AttribIdPrev != pBoneComb[iAttrib].AttribId) || ( AttribIdPrev == UNUSED32 ) )
                    {
                        m_pDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                        m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                        AttribIdPrev = pBoneComb[iAttrib].AttribId;
                    }

                    // draw the subset now that the correct material and matrices are loaded
                    pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
                }
            }

            // If necessary, draw parts that HW could not handle using SW
            if( pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups )
            {
                AttribIdPrev = UNUSED32; 
                m_pDevice->SetSoftwareVertexProcessing( TRUE );
                for( iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
                {
                    NumBlend = 0;
                    for( DWORD i = 0; i < pMeshContainer->NumInfl; ++ i )
                    {
                        if( pBoneComb[iAttrib].BoneId[i] != UINT_MAX )
                        {
                            NumBlend = i;
                        }
                    }

                    if( m_d3dCaps.MaxVertexBlendMatrices < NumBlend + 1 )
                    {
                        // first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
                        for( DWORD i = 0; i < pMeshContainer->NumInfl; ++ i )
                        {
                            iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                            if( iMatrixIndex != UINT_MAX )
                            {
                                D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                                m_pDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp );
                            }
                        }

                        m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, NumBlend );

                        // lookup the material used for this subset of faces
                        if( ( AttribIdPrev != pBoneComb[iAttrib].AttribId) || ( AttribIdPrev == UNUSED32 ) )
                        {
                            m_pDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                            m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );
                            AttribIdPrev = pBoneComb[iAttrib].AttribId;
                        }

                        // draw the subset now that the correct material and matrices are loaded
                        pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
                    }
                }
                m_pDevice->SetSoftwareVertexProcessing( FALSE );
            }

            m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, 0 );
        }
        else if( m_SkinningMethod == D3DINDEXED )
        {
            // if hw doesn't support indexed vertex processing, switch to software vertex processing
            if( pMeshContainer->UseSoftwareVP )
            {
                m_pDevice->SetSoftwareVertexProcessing( TRUE );
            }

            // set the number of vertex blend indices to be blended
            if( pMeshContainer->NumInfl == 1 )
                m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS );
            else
                m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1 );

            if( pMeshContainer->NumInfl )
                m_pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );

            // for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer() );
            for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
            {
                // first calculate all the world matrices
                for( iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++ iPaletteEntry )
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if( iMatrixIndex != UINT_MAX )// && iMatrixIndex != pMeshContainer->NumPaletteEntries - 1 )
                    {
                        D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                        m_pDevice->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
                    }
                }
                
                // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
                m_pDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
                m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );

                // finally draw the subset with the current world matrix palette and material state
                pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
            }

			//D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[25], pMeshContainer->ppBoneMatrixPtrs[25] );
			//for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
   //         {
   //             // first calculate all the world matrices
   //             for( iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++ iPaletteEntry )
   //             {
   //                 iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
   //                 if( iMatrixIndex != UINT_MAX && iMatrixIndex == pMeshContainer->NumPaletteEntries - 1 )
   //                 {
   //                     //D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
   //                     m_pDevice->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
   //                 }
			//		else
			//		{
			//			//memset( &matTemp, 0, sizeof( D3DXMATRIX ) );
			//			
			//			//D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
			//			m_pDevice->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
			//		}
   //             }
   //             
   //             // setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
   //             m_pDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
   //             m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );

   //             // finally draw the subset with the current world matrix palette and material state
			//	pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib );
			//}

			// reset blending state
            m_pDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
            m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, 0 );

            // remember to reset back to hw vertex processing if software was required
            if( pMeshContainer->UseSoftwareVP )
            {
                m_pDevice->SetSoftwareVertexProcessing( FALSE );
            }
        }
        else if( m_SkinningMethod == SOFTWARE )
        {
            D3DXMATRIX  Identity;
            DWORD       cBones  = pMeshContainer->pSkinInfo->GetNumBones();
            DWORD       iBone;
            PBYTE       pbVerticesSrc;
            PBYTE       pbVerticesDest;

            // set up bone transforms
            for( iBone = 0; iBone < cBones; ++ iBone )
            {
                D3DXMatrixMultiply
                (
                    &m_pBoneMatrices[iBone],                 // output
                    &pMeshContainer->pBoneOffsetMatrices[iBone], 
                    pMeshContainer->ppBoneMatrixPtrs[iBone]
                );
            }

            //世界变换
            D3DXMatrixIdentity( &Identity );
            m_pDevice->SetTransform( D3DTS_WORLD, &Identity );

            pMeshContainer->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc );
            pMeshContainer->MeshData.pMesh->LockVertexBuffer( 0, (LPVOID*)&pbVerticesDest );

            // generate skinned mesh
            pMeshContainer->pSkinInfo->UpdateSkinnedMesh( m_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest );

            pMeshContainer->pOrigMesh->UnlockVertexBuffer();
            pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

            for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
            {
                //m_pDevice->SetMaterial( &( pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D ) );
				m_pDevice->SetMaterial( &m_matMaterial );
                m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId] );
                pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[iAttrib].AttribId );
            }
			
			//for( iBone = 0; iBone < cBones; ++ iBone )
   //         {
   //            m_pBoneMatrices[iBone] = m_pBoneMatrices[25];
   //         }
			//m_pDevice->SetTransform( D3DTS_WORLD, &Identity );

   //         pMeshContainer->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc );
   //         pMeshContainer->MeshData.pMesh->LockVertexBuffer( 0, (LPVOID*)&pbVerticesDest );

   //         // generate skinned mesh
   //         pMeshContainer->pSkinInfo->UpdateSkinnedMesh( m_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest );

   //         pMeshContainer->pOrigMesh->UnlockVertexBuffer();
   //         pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

   //         for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib ++ )
   //         {
   //             m_pDevice->SetMaterial( &( pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D ) );
   //             m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId] );
   //             pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[iAttrib].AttribId );
   //         }
			
        }
        else // bug out as unsupported mode
        {
            return;
        }
    }
    else  // standard mesh, just draw it after setting material properties
    {
        m_pDevice->SetTransform( D3DTS_WORLD, &pFrame->CombinedTransformationMatrix );

        for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial ++ )
        {
            m_pDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
            m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
            pMeshContainer->MeshData.pMesh->DrawSubset( iMaterial );
        }
    }
}

VOID CSkinMesh::Render( LPD3DXANIMATIONCONTROLLER pAnimController, float fElapsedAppTime, D3DXVECTOR3 vPos, float angle, float scale )
{
	/*if( 0.00f == fElapsedAppTime )    
		return;*/

	m_fElapsedTime = fElapsedAppTime;
	
	//世界变换矩阵
    D3DXMATRIXA16 matWorld,matWorld2;

	D3DXMatrixRotationY( &matWorld2, angle );
	D3DXMatrixScaling( &matWorld, scale, scale, scale );

	D3DXMatrixMultiply( &matWorld, &matWorld, &matWorld2 );

    D3DXMatrixTranslation( &matWorld2, vPos.x, vPos.y, vPos.z );

	D3DXMatrixMultiply( &matWorld, &matWorld, &matWorld2 );



  //  m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	if( pAnimController )
	{

#if (D3D_SDK_VERSION &31)
			pAnimController->SetTime( pAnimController->GetTime() + m_fElapsedTime );
			
#else 
			pAnimController->AdvanceTime( m_fElapsedTime, NULL ); 
#endif

	}
	
    UpdateFrameMatrices( m_pFrameRoot, &matWorld );
	
	DrawFrame( m_pFrameRoot );
}

BOOL CSkinMesh::SetAnimationName( CHAR * strAnimName, LPD3DXANIMATIONCONTROLLER pAnimController )
{
	if( !pAnimController )
		return FALSE;
	DWORD nAnimSet;
	CHAR strTempName[256];
	nAnimSet = pAnimController->GetNumAnimationSets();
	LPD3DXANIMATIONSET pAnimSet;

	for( DWORD i=0; i < nAnimSet; i ++ )
	{	
		pAnimController->GetAnimationSet( i, &pAnimSet );
		strcpy( strTempName, pAnimSet->GetName() );
		if( strcmp( strAnimName, strTempName ) == 0 ) 
		{
			pAnimController->SetTrackAnimationSet( 0, pAnimSet );
			return TRUE;
		}
	}
	return FALSE;
};

VOID CSkinMesh::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
	D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    if( pParentMatrix != NULL )
        D3DXMatrixMultiply( &pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix );
    else
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    if( pFrame->pFrameSibling != NULL )
    {
        UpdateFrameMatrices( pFrame->pFrameSibling, pParentMatrix );
    }

    if( pFrame->pFrameFirstChild != NULL )
    {
        UpdateFrameMatrices( pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix );
    }
}

bool CSkinMesh::SetMaterial( D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower )
{
	//设置漫反射 
	m_matMaterial.Diffuse = rgbaDiffuse; 

	//设置环境光 
	m_matMaterial.Ambient = rgbaAmbient; 

	//设置镜面反射 
	m_matMaterial.Specular = rgbaSpecular; 
	m_matMaterial.Power = rPower;

	//设置反射
	m_matMaterial.Emissive = rgbaEmissive;

	return true;
}