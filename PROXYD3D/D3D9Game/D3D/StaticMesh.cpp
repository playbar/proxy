

#include "StaticMesh.h"


bool CStaticMesh::LoadMesh( TCHAR * szFileName )
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	if( FAILED( D3DXLoadMeshFromX( szFileName, 
									D3DXMESH_MANAGED, 
                                   g_pMyD3D->GetDevice(), 
								   NULL,
                                   &pD3DXMtrlBuffer, 
								   NULL,
								   &m_dwNumMaterials, 
                                   &m_pMesh ) ) )
	{
		return false;
	}

	//从pD3DXMtrlBuffer中得到材质信息和纹理信息
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
	//用索引m_dwNumMaterials装载文理图片
    for( int i=0; i < (int)m_dwNumMaterials; i ++ )
	{
        //拷贝材质
        m_pMaterials[i] = d3dxMaterials[i].MatD3D;
        //设置材质的环境光
        m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;        
        //创建纹理
        if( FAILED( D3DXCreateTextureFromFileA( g_pMyD3D->GetDevice(), 
                                               d3dxMaterials[i].pTextureFilename, 
                                               &m_pTextures[i] ) ) )
        {
            m_pTextures[i] = NULL;
        }
    }

    pD3DXMtrlBuffer->Release();
	void * p;
	m_pMesh->LockVertexBuffer(0,&p);
    D3DXComputeBoundingBox((D3DXVECTOR3 *)p,m_pMesh->GetNumVertices(),m_pMesh->GetNumBytesPerVertex(),&m_vMin,&m_vMax);
	m_pMesh->UnlockVertexBuffer();
	return true;
}

void CStaticMesh::Render()
{

	//网格被分割成子网格，每个都具有各自的材质，需要在循环中逐个渲染
    for( DWORD i = 0; i < 1; i ++ )
    {
		//设置子网格的材质和纹理
        g_pMyD3D->GetDevice()->SetMaterial( &m_pMaterials[i] );
        g_pMyD3D->GetDevice()->SetTexture( i, m_pTextures[i] );        
        //绘制子网格
        m_pMesh->DrawSubset( i );
    }
}

void CStaticMesh::Render( DWORD dwColor )
{
	 for( DWORD i = 0; i < m_dwNumMaterials; i ++ )
    {
		//设置子网格的材质和纹理
        //g_pD3D->GetDevice()->SetMaterial( &m_pMaterials[i] );
        g_pMyD3D->GetDevice()->SetTexture( 0, g_pMyD3D->m_pColorTextures[0] );        
        //绘制子网格
        m_pMesh->DrawSubset( i );
    }
}



void CStaticMeshObj::Render()
{
	D3DXMATRIX matWorld,matScaling,matRotation,matTranslation;

	D3DXMatrixScaling( &matScaling, m_vScaling.x, m_vScaling.y, m_vScaling.z );
	D3DXMatrixRotationY( &matRotation, m_fRotationY );
	D3DXMatrixTranslation( &matTranslation, m_vPos.x, m_vPos.y, m_vPos.z );

	D3DXMatrixMultiply( &matWorld, &matScaling, &matRotation );
	D3DXMatrixMultiply( &matWorld, &matWorld, &matTranslation );

	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	UseRenderState();

	m_pMesh->Render();

	//m_pMesh->Render( m_dwCheckColor );
}


bool CStaticMeshObj::RayCheck( D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
{
	D3DXMATRIX  mMat;
	D3DXVECTOR3 vMin = m_pMesh->m_vMin;
	D3DXVECTOR3 vMax = m_pMesh->m_vMax;
	D3DXMatrixScaling(&mMat,m_vScaling.x,m_vScaling.y,m_vScaling.z);
	APP_Tools::appVec3Transform(vMin,mMat);
	APP_Tools::appVec3Transform(vMax,mMat);
	vMin = vMin + m_vPos;
	vMax = vMax + m_vPos;
	if (D3DXBoxBoundProbe(&vMin,&vMax,&vPos,&vDir))
		return true;
	else 
		return false;
}