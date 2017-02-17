
#include "SkinMesh.h"


HRESULT AllocateName( LPCSTR Name, CHAR ** pNewName )
{
    UINT cbLength;
	
    if( Name != NULL )
    {
        cbLength = (UINT)(strlen( Name ) + 1);
        *pNewName = new CHAR[cbLength];
        if( *pNewName == NULL )
            return E_OUTOFMEMORY;
        memcpy( *pNewName, Name, cbLength * sizeof( CHAR ) );
    }
    else
    {
        *pNewName = NULL;
    }
	
    return S_OK;
}

HRESULT AllocateName( LPCTSTR Name, CHAR ** pNewName )
{
    UINT cbLength;
	
    if( Name != NULL )
    {
        cbLength = lstrlen( Name ) + 1;
        *pNewName = new CHAR[cbLength];
        if( *pNewName == NULL )
            return E_OUTOFMEMORY;
        for( UINT i = 0; i < cbLength; i ++ )
		{
			(*pNewName)[i] = (CHAR)Name[i];
		}
    }
    else
    {
        *pNewName = NULL;
    }
	
    return S_OK;
}



HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED * pFrame;
	
    *ppNewFrame = NULL;
	
    pFrame = new D3DXFRAME_DERIVED;
    if( pFrame == NULL )
    {
        hr = E_OUTOFMEMORY;
        return hr;
    }
	
	hr = AllocateName( Name, &pFrame->Name );
    if( FAILED( hr ) )
	{
		delete pFrame;
		return hr;
	}
	
    D3DXMatrixIdentity( &pFrame->TransformationMatrix );
    D3DXMatrixIdentity( &pFrame->CombinedTransformationMatrix );
	
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
	
    *ppNewFrame = pFrame;
    return hr;
}



LRESULT CAllocateHierarchy::CreateMeshContainer(
		LPCSTR Name, 
		CONST D3DXMESHDATA * pMeshData,
		CONST D3DXMATERIAL * pMaterials, 
		CONST D3DXEFFECTINSTANCE * pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD * pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER * ppNewMeshContainer ) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED * pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	
    LPD3DXMESH pMesh = NULL;
	
    *ppNewMeshContainer = NULL;
	
    //是否Mesh文件
    if( pMeshData->Type != D3DXMESHTYPE_MESH )
    {
		hr = E_FAIL;
		goto e_Exit;
    }
	
    pMesh = pMeshData->pMesh;
	
	//是否FVFMesh
    if( pMesh->GetFVF() == 0 )
    {
        hr = E_FAIL;
		goto e_Exit;
    }
	
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    if( pMeshContainer == NULL )
    {
		hr = E_OUTOFMEMORY;
		goto e_Exit;
    }

    memset( pMeshContainer, 0, sizeof( D3DXMESHCONTAINER_DERIVED ) );
	
	//名字
	hr = AllocateName( Name, &pMeshContainer->Name );
    if( FAILED( hr ) )
	{
		goto e_Exit;
	}
	
    pMesh->GetDevice( &pd3dDevice );
    NumFaces = pMesh->GetNumFaces();
	
	//如果NORMAL
    if( ( pMesh->GetFVF() & D3DFVF_NORMAL ) )
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		
        //克隆Mesh
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
			pMesh->GetFVF() | D3DFVF_NORMAL, 
			pd3dDevice, &pMeshContainer->MeshData.pMesh );
        if( FAILED( hr ) )
		{
			goto e_Exit;
		}
		
        pMesh = pMeshContainer->MeshData.pMesh;
		
        D3DXComputeNormals( pMesh, NULL );
    }
    else
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		
        pMesh->AddRef();
    }
	
	//为材质纹理分配空间
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    if( ( pMeshContainer->pAdjacency == NULL ) || ( pMeshContainer->pMaterials == NULL ) )
    {
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	
    memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof( DWORD ) * NumFaces * 3 );
    memset( pMeshContainer->ppTextures, 0, sizeof( LPDIRECT3DTEXTURE9 ) * pMeshContainer->NumMaterials );
	
    if (NumMaterials > 0)            
    {
        memcpy( pMeshContainer->pMaterials, pMaterials, sizeof( D3DXMATERIAL ) * NumMaterials );
		
        for( iMaterial = 0; iMaterial < NumMaterials; iMaterial ++ )
        {
            if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL )
            {
				if( FAILED( D3DXCreateTextureFromFileA( pd3dDevice, pMeshContainer->pMaterials[iMaterial].pTextureFilename, 
					&pMeshContainer->ppTextures[iMaterial] ) ) )
				{
                    pMeshContainer->ppTextures[iMaterial] = NULL;
				}
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else//没材质，用特定材质
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset( &pMeshContainer->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
	
    //如果有skin信息,储存信息    HW
    if( pSkinInfo != NULL )
    {
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
		
        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();
		
        //从图形空间移动到骨骼空间的变换矩阵
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
		
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *( pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( iBone ) );
        }
		
        hr = m_pSkinMesh->GenerateSkinnedMesh( pMeshContainer );
        if( FAILED( hr ) )
            goto e_Exit;
    }
	
    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;



e_Exit:
	
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



///////////////////////////////////////////////////////////////////////////////////////////////
//SkinMesh

CSkinMesh::CSkinMesh() : m_vMax(0,0,0), m_vMin(0,0,0)  
{
	m_pDevice = g_pD3D->GetDevice();
	m_fElapsedTime = 0.0f;
	m_pDevice->GetDeviceCaps( &m_d3dCaps );
	m_bMoving = TRUE;
    m_pAnimController = NULL;
    m_pFrameRoot = NULL;
    m_SkinningMethod = D3DINDEXED;
    m_pBoneMatrices = NULL;
    m_NumBoneMatricesMax = 0;
}

CSkinMesh::~CSkinMesh()
{
	CAllocateHierarchy Alloc( this );
	D3DXFrameDestroy( m_pFrameRoot, &Alloc );
	
    SAFE_RELEASE( m_pAnimController );
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
	if( m_pFrameRoot )
		CalculateBondingBox( m_pFrameRoot, &m_vMin, &m_vMax );  
	
	return S_OK;
}


HRESULT CSkinMesh::GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED * pMeshContainer )
{
    HRESULT hr = S_OK;

    if( pMeshContainer->pSkinInfo == NULL )
        return hr;

    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

    if( m_SkinningMethod == D3DNONINDEXED )
    {
        hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
                                   (
                                       pMeshContainer->pOrigMesh,
                                       D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE, 
                                       pMeshContainer->pAdjacency, 
                                       NULL, NULL, NULL, 
                                       &pMeshContainer->NumInfl,
                                       &pMeshContainer->NumAttributeGroups, 
                                       &pMeshContainer->pBoneCombinationBuf, 
                                       &pMeshContainer->MeshData.pMesh
                                   );
        if( FAILED( hr ) )
            goto e_Exit;


        /* If the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
           Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
           drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing. */
        LPD3DXBONECOMBINATION rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer() );

        // look for any set of bone combinations that do not fit the caps
        for( pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups; pMeshContainer->iAttributeSW ++ )
        {
            DWORD cInfl = 0;

            for( DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl ++ )
            {
                if( rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX )
                {
                    ++ cInfl;
                }
            }

            if( cInfl > m_d3dCaps.MaxVertexBlendMatrices )
            {
                break;
            }
        }

        // if there is both HW and SW, add the Software Processing flag
        if( pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups )
        {
            LPD3DXMESH pMeshTmp;

            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF( D3DXMESH_SOFTWAREPROCESSING | pMeshContainer->MeshData.pMesh->GetOptions(), 
                                                pMeshContainer->MeshData.pMesh->GetFVF(),
                                                m_pDevice, &pMeshTmp );
            if( FAILED( hr ) )
            {
                goto e_Exit;
            }

            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMeshTmp;
            pMeshTmp = NULL;
        }
    }
    // if indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else if ( m_SkinningMethod == D3DINDEXED )
    {
        DWORD NumMaxFaceInfl;
        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pMeshContainer->pOrigMesh->GetIndexBuffer( &pIB );
        if( FAILED( hr ) )
            goto e_Exit;

        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences( pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl );
        pIB->Release();
        if( FAILED( hr ) )
            goto e_Exit;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        NumMaxFaceInfl = min( NumMaxFaceInfl, 12 );

        if( m_d3dCaps.MaxVertexBlendMatrixIndex + 1 < 100 )//NumMaxFaceInfl )
        {
            // HW does not support indexed vertex blending. Use SW instead
            pMeshContainer->dwNumPaletteEntries = min( 256, pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            // using hardware - determine palette size from caps and number of bones
            // If normals are present in the vertex data that needs to be blended for lighting, then 
            // the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
            pMeshContainer->dwNumPaletteEntries = min( 256,//( m_d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2, 
                                                     pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->dwNumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL, 
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh );
        if( FAILED( hr ) )
            goto e_Exit;
    }
     // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    else if( m_SkinningMethod == SOFTWARE )
    {
        hr = pMeshContainer->pOrigMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
                                              m_pDevice, &pMeshContainer->MeshData.pMesh );
        if( FAILED( hr ) )
            goto e_Exit;

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->NumAttributeGroups );
        if( FAILED( hr ) )
            goto e_Exit;

        delete[] pMeshContainer->pAttributeTable;
        pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
        if( pMeshContainer->pAttributeTable == NULL )
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( pMeshContainer->pAttributeTable, NULL );
        if( FAILED( hr ) )
            goto e_Exit;

        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if( m_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
        {
            m_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            delete []m_pBoneMatrices; 
            m_pBoneMatrices  = new D3DXMATRIXA16[m_NumBoneMatricesMax];
            if( m_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }
    }
    else  // invalid m_SkinningMethod value
    {        
        // return failure due to invalid skinning method value
        hr = E_INVALIDARG;
        goto e_Exit;
    }

e_Exit:
    return hr;
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

VOID CSkinMesh::Render( float fElapsedAppTime, D3DXVECTOR3 vPos, float angle, float scale )
{
	
	// Get the app's time, in seconds. Skip rendering if no time elapsed
//    FLOAT fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );
	
    if( 0.00f == fElapsedAppTime )    
		return;

	m_fElapsedTime = fElapsedAppTime;
	
	//世界变换矩阵
    D3DXMATRIXA16 matWorld,matWorld2;

	D3DXMatrixRotationY( &matWorld2, angle );
	D3DXMatrixScaling( &matWorld, scale, scale, scale );

	D3DXMatrixMultiply( &matWorld, &matWorld, &matWorld2 );

    D3DXMatrixTranslation( &matWorld2, vPos.x, vPos.y, vPos.z );

	D3DXMatrixMultiply( &matWorld, &matWorld, &matWorld2 );



  //  m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
	if( m_pAnimController )
	{
		if( m_bMoving )

#if (D3D_SDK_VERSION &31)
			m_pAnimController->SetTime( m_pAnimController->GetTime() + m_fElapsedTime );
		else
			m_pAnimController->SetTime( 0 );
			
#else 
			m_pAnimController->AdvanceTime( m_fElapsedTime, NULL ); 
		else
			m_pAnimController->ResetTime();
#endif

	}
	
    UpdateFrameMatrices( m_pFrameRoot, &matWorld );
	
	DrawFrame( m_pFrameRoot );
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
        DrawFrame( pFrame->pFrameSibling );
    }
	
    if( pFrame->pFrameFirstChild != NULL )
    {
        DrawFrame( pFrame->pFrameFirstChild );
    }
}


VOID CSkinMesh::DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase )
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
                for( iPaletteEntry = 0; iPaletteEntry < pMeshContainer->dwNumPaletteEntries; ++ iPaletteEntry )
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    if( iMatrixIndex != UINT_MAX )
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
                m_pDevice->SetMaterial( &( pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D ) );
                m_pDevice->SetTexture( 0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId] );
                pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[iAttrib].AttribId );
            }
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


VOID CSkinMesh::SetAnim( BOOL bAnim )
{
	m_bMoving = bAnim;
	if( !m_bMoving && m_pAnimController )
	{	
#if (D3D_SDK_VERSION & 32)		
		m_pAnimController->ResetTime();//
#else		
		m_pAnimController->SetTime( 0 );
#endif
	}
}



BOOL CSkinMesh::SetAnimationName( CHAR * strAnimName )
{
	if( !m_bMoving || !m_pAnimController )
		return FALSE;
	DWORD nAnimSet;
	CHAR strTempName[256];
	nAnimSet = m_pAnimController->GetNumAnimationSets();
	LPD3DXANIMATIONSET pAnimSet;

	for( DWORD i=0; i < nAnimSet; i ++ )
	{	
		m_pAnimController->GetAnimationSet( i, &pAnimSet );
		strcpy( strTempName, pAnimSet->GetName() );
		if( strcmp( strAnimName, strTempName ) == 0 ) 
		{
			m_pAnimController->SetTrackAnimationSet( 0,pAnimSet );
			return TRUE;
		}
	}
	return FALSE;
};


void CSkinMesh::CalculateBondingBox( LPD3DXFRAME pFrameParent, D3DXVECTOR3 * pVmin, D3DXVECTOR3 * pVmax )
{
	static LPVOID pV; 
	static LPD3DXMESH pMesh;
	static LPD3DXMATRIX pMat;
	static D3DXVECTOR3 vMin,vMax; 
	static D3DXVECTOR3 vTransFormed;

	if(pFrameParent->pMeshContainer)
	{
		pMesh=pFrameParent->pMeshContainer->MeshData.pMesh;    
		pMat=&(((D3DXFRAME_DERIVED*)pFrameParent)->CombinedTransformationMatrix);

		pMesh->LockVertexBuffer(0,&pV); 
		D3DXComputeBoundingBox((LPD3DXVECTOR3)pV,pMesh->GetNumVertices(),
			pMesh->GetNumBytesPerVertex(),&vMin,&vMax); 

		vTransFormed.x=pMat->_11*vMin.x+pMat->_21*vMin.y+pMat->_31*vMin.z+pMat->_41;
		vTransFormed.y=pMat->_12*vMin.x+pMat->_22*vMin.y+pMat->_32*vMin.z+pMat->_42;
		vTransFormed.z=pMat->_13*vMin.x+pMat->_23*vMin.y+pMat->_33*vMin.z+pMat->_43;

		if(vTransFormed.x<pVmin->x) pVmin->x=vTransFormed.x;
		if(vTransFormed.y<pVmin->y) pVmin->y=vTransFormed.y;
		if(vTransFormed.z<pVmin->z) pVmin->z=vTransFormed.z;

		vTransFormed.x=pMat->_11*vMax.x+pMat->_21*vMax.y+pMat->_31*vMax.z+pMat->_41;
		vTransFormed.y=pMat->_12*vMax.x+pMat->_22*vMax.y+pMat->_32*vMax.z+pMat->_42;
		vTransFormed.z=pMat->_13*vMax.x+pMat->_23*vMax.y+pMat->_33*vMax.z+pMat->_43;

		if(vTransFormed.x>pVmax->x) pVmax->x=vTransFormed.x;
		if(vTransFormed.y>pVmax->y) pVmax->y=vTransFormed.y;
		if(vTransFormed.z>pVmax->z) pVmax->z=vTransFormed.z;
		pMesh->UnlockVertexBuffer(); 
	}

	if(pFrameParent->pFrameSibling)
		CalculateBondingBox(pFrameParent->pFrameSibling,pVmin,pVmax);

	if(pFrameParent->pFrameFirstChild)
		CalculateBondingBox(pFrameParent->pFrameFirstChild,pVmin,pVmax);

}





//射线交点
BOOL CSkinMesh::InterSect( D3DVECTOR *pRayOrig,
						  D3DVECTOR *pRayDir,D3DVECTOR* pVRet)
{
	return S_OK;
}





///////////////////////////////////////////////////////////////////////////////
//CSkinMeshObj

void CSkinMeshObj::Render()
{
	static double fTimeLast = APP::appGetTime();
	double fTimeNow = APP::appGetTime();

	m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

	SetAction("walk");

	m_pSkinMesh->Render( (float)(fTimeNow - fTimeLast), m_vPos, m_fAngle, m_fScaling );
	fTimeLast = fTimeNow;
}