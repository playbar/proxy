

#include "Terrain.h"




CTerrain::CTerrain( D3DVECTOR position )
{
	m_pDevice = g_pMyD3D->GetDevice();
	m_vPos = position;
	m_nCol = 0;
	m_nRow = 0;
	m_fCellWidth = 0;//单元格宽度
	m_pIB = NULL;
	m_pVB = NULL;
	m_pTexture = NULL;
	m_pHeightData = NULL;
	m_pVerticesBackUp = NULL;
	m_maxHeight = 800;

	D3DCOLORVALUE rgbaDiffuse  = {1.0, 1.0, 1.0, 0.0,};
	D3DCOLORVALUE rgbaAmbient  = {1.0, 1.0, 1.0, 0.0,};
	D3DCOLORVALUE rgbaSpecular = {0.0, 0.0, 0.0, 0.0,};
	D3DCOLORVALUE rgbaEmissive = {0.0, 0.0, 0.0, 0.0,};
	SetMaterial( rgbaDiffuse, rgbaAmbient, rgbaSpecular, rgbaEmissive, 0 );

	m_dwBaseType = OBJECT_TYPE_TERRAIN;

}
CTerrain::~CTerrain()
{
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	SAFE_DELETE( m_pTexture );
	delete []m_pHeightData;
	
	for( int i = 0; i < m_pStaticMeshObj.Num(); i ++ )
	{
		SAFE_DELETE( m_pStaticMeshObj.Item( i ) );
	}
	for( int i = 0; i < m_pStaticMesh.Num(); i ++ )
	{
		SAFE_DELETE( m_pStaticMesh[i] );
	}

	for( int i = 0; i < m_pSkinMeshObj.Num(); i ++ )
	{
		SAFE_DELETE( m_pSkinMeshObj.Item( i ) );
	}
	for( int i = 0; i < m_pSkinMesh.Num(); i ++ )
	{
		SAFE_DELETE( m_pSkinMesh[0] );
	}
}


void CTerrain::CreateTerrain(TCHAR* strTerrain,TCHAR* strTexture,
							 float cellWidth,float height)
{
	m_fCellWidth=cellWidth;
	m_maxHeight=height; //最大高度
	SAFE_DELETE( m_pTexture );
	m_pTexture = new CTexture;
	m_pTexture->CreateFromFile( strTexture,false );

	long i=0,j=0;
	BITMAP  bmp;
	HBITMAP	hBmp;
	HDC		hDC;
	HBITMAP	oldBmp;
	hDC=CreateCompatibleDC(NULL); 
	hBmp=(HBITMAP)LoadImage(NULL,strTerrain,
		IMAGE_BITMAP,m_nCol,m_nRow,LR_LOADFROMFILE);
	oldBmp=(HBITMAP)SelectObject(hDC,hBmp);

	GetObject(hBmp,sizeof(bmp),&bmp);
	m_nCol=bmp.bmWidth;
	m_nRow=bmp.bmHeight;
	SAFE_DELETE_ARRAY( m_pHeightData );
	m_pHeightData=new BYTE[m_nCol*m_nRow];
	DWORD color;
	for(i=0;i<m_nRow;i++)
	{
		for(j=0;j<m_nCol;j++)
		{	
			color=GetPixel(hDC,j,i);
			m_pHeightData[i*m_nCol+j]=*(BYTE*)&color;		//取红色通道 0xAABBGGRR
		}
	}
	DeleteObject(oldBmp);
	DeleteObject(hDC);
	DeleteObject(hBmp);


	SAFE_RELEASE( m_pVB );
	SAFE_DELETE_ARRAY( m_pVerticesBackUp );
	m_pVerticesBackUp = new VERTEX_TERRAIN[m_nCol*m_nRow];

	float tu=0,tv=-0.5;
	m_pDevice->CreateVertexBuffer(m_nCol*m_nRow*sizeof(VERTEX_TERRAIN),
		D3DUSAGE_WRITEONLY,D3DFVF_TERRAIN, 
		D3DPOOL_DEFAULT, &m_pVB,NULL);//创建顶点缓冲
	VERTEX_TERRAIN* pVertices;

	m_pVB->Lock(0,m_nCol*m_nRow*sizeof(VERTEX_TERRAIN),(void**)&pVertices,0);
	for(i=0;i<m_nCol*m_nRow;i++)
	{
		pVertices[i].y=m_maxHeight*m_pHeightData[i]/255.0f;//m_maxHeight*((pBmpData[i]&0xff)/256.0f);
		pVertices[i].x=cellWidth*(i%m_nCol);
		pVertices[i].z=cellWidth*(i/m_nCol);
		tu=((tu+0.05f>1.0f)?(-1.0f):(tu+0.05f));		//tu=-1~1,-1~1,......
		if(i%m_nCol==0) 
		{
			tv=((tv+0.05f>1.0f)?(-1.0f):(tv+0.05f));
			tu=0;
		}

		pVertices[i].tu=tu>0?tu:-tu;
		pVertices[i].tv=tv>0?tv:-tv;
	}
	memcpy( m_pVerticesBackUp, pVertices, m_nCol*m_nRow*sizeof(VERTEX_TERRAIN) );
	m_pVB->Unlock();
	SAFE_RELEASE( m_pIB );

	long nBufferSize=(m_nCol-1)*(m_nRow-1)*6*sizeof(DWORD);
	m_pDevice->CreateIndexBuffer(nBufferSize,D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,D3DPOOL_DEFAULT,&m_pIB,NULL);
	DWORD* pIndex;
	m_pIB->Lock(0,nBufferSize,(void**)&pIndex,0);
	for(i=0,j=0;i<m_nCol*m_nRow;i++)
	{
		if((i+1)%m_nCol && i/m_nCol!=m_nRow-1)
		{
			pIndex[j+0]=(WORD)i;
			pIndex[j+1]=i+m_nCol;
			pIndex[j+2]=i+1;
			pIndex[j+3]=i+1;
			pIndex[j+4]=i+m_nCol;
			pIndex[j+5]=i+1+m_nCol;
			j+=6;
		}
	}
	m_pIB->Unlock();

	//法线
	m_pVB->Lock(0,m_nCol*m_nRow*sizeof(VERTEX_TERRAIN),(void**)&pVertices,0);
	m_pIB->Lock(0,nBufferSize,(void**)&pIndex,0);

	int num = ( m_nCol - 1 ) * ( m_nRow - 1 ) * 2 * 3;
	D3DXVECTOR3 vNormal;
	for(i = 0; i < num; i += 3)
	{
		DWORD dwVertex1 = pIndex[i];
		DWORD dwVertex2 = pIndex[i + 1];
		DWORD dwVertex3 = pIndex[i + 2];

		APP_Tools::ComputeTriNormal( &D3DXVECTOR3(pVertices[dwVertex1].x, pVertices[dwVertex1].y, pVertices[dwVertex1].z), 
							   &D3DXVECTOR3(pVertices[dwVertex2].x, pVertices[dwVertex2].y, pVertices[dwVertex2].z), 
							   &D3DXVECTOR3(pVertices[dwVertex3].x, pVertices[dwVertex3].y, pVertices[dwVertex3].z),
							   vNormal);

		pVertices[dwVertex1].nx += vNormal.x;
		pVertices[dwVertex1].ny += vNormal.y;
		pVertices[dwVertex1].nz += vNormal.z;
		
		pVertices[dwVertex2].nx += vNormal.x;
		pVertices[dwVertex2].ny += vNormal.y;
		pVertices[dwVertex2].nz += vNormal.z;

		pVertices[dwVertex3].nx += vNormal.x;
		pVertices[dwVertex3].ny += vNormal.y;
		pVertices[dwVertex3].nz += vNormal.z;
	}
	for( i = 0; i < m_nCol * m_nRow; ++ i )
	{
		vNormal.x = pVertices[i].nx;
		vNormal.y = pVertices[i].ny;
		vNormal.z = pVertices[i].nz;

		D3DXVec3Normalize( &vNormal, &vNormal );
		pVertices[i].nx = vNormal.x;
		pVertices[i].ny = vNormal.y;
		pVertices[i].nz = vNormal.z;
	}

	m_pIB->Unlock();
	m_pVB->Unlock();

	SetCustomRenderState( COLOR_MODULATE_TEXTURE_CURRENT, ALPHA_NONE, ALPHABLEND_NONE, FILTER_LINEAR, CULL_CCW, TRUE,false, 0 );
}

void CTerrain::Render()
{

	//雾
	g_pMyD3D->SetFog( 300.f, 1000.f, 0.001f, 0x009BB1E9, 0 );

	UseRenderState();

	m_pDevice->SetFVF(D3DFVF_TERRAIN);
	m_pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(VERTEX_TERRAIN) );

	m_pDevice->SetTexture( 0, m_pTexture->GetBuffer() );
	m_pDevice->SetMaterial( &m_matMaterial );

	m_pDevice->SetIndices( m_pIB );
	this->SetMatrix(); 
	m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,m_nCol*m_nRow,0,(m_nCol-1)*(m_nRow-1)*2 );

	//m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	//m_pCheckDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_nCol * m_nRow, 0, ( m_nCol - 1 ) * ( m_nRow - 1 ) * 2 );
	
}

void CTerrain::SetMatrix()
{	
	D3DXMATRIX	matWorld;
	D3DXMatrixTranslation( &matWorld,m_vPos.x,m_vPos.y, m_vPos.z);
	m_pDevice->SetTransform( D3DTS_WORLD, &matWorld );

}

bool CTerrain::SetMaterial( D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower )
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


bool CTerrain::InitialiseLight(int LightType)
{
	D3DLIGHT9 d3dLight;
	//初始化灯光的属性
	ZeroMemory(&d3dLight, sizeof(D3DLIGHT9));

	d3dLight.Type = (D3DLIGHTTYPE)LightType;
	switch (LightType) 
	{
	case D3DLIGHT_POINT://点光源
		// 发光点位置
		d3dLight.Position.x	= 0.0f;
		d3dLight.Position.y	= 0.0f;
		d3dLight.Position.z	= -10.0f;
		// 指定各个光的衰减度。和发光范围。
		d3dLight.Attenuation0 = 1.0f; 
		d3dLight.Attenuation1 = 0.0f; 
		d3dLight.Attenuation2 = 0.0f; 


		break;
	case D3DLIGHT_SPOT://聚光灯

		// 发光点位置
		d3dLight.Position.x	= 0.0f;
		d3dLight.Position.y	= 0.0f;
		d3dLight.Position.z	= -80.0f;
		// 指定各个光的衰减度。和发光范围。
		d3dLight.Attenuation0 = 1.0f; 


		d3dLight.Direction.x = 0.0f;
		d3dLight.Direction.y = 0.0f;
		d3dLight.Direction.z = -40.0f;
		//设置聚光灯发光内径和外径
		d3dLight.Falloff = 1.0f;
		d3dLight.Theta = 0.5f;
		d3dLight.Phi = 1.0f;
		break;
	case D3DLIGHT_DIRECTIONAL://直射光
		
		d3dLight.Diffuse.r = 1.f;
		d3dLight.Diffuse.g = 1.f;
		d3dLight.Diffuse.b = 0.85f;

		d3dLight.Direction.x = -1.0f;
		d3dLight.Direction.y = -1.0f;
		d3dLight.Direction.z = 1.0f;

		d3dLight.Ambient.r = 0.0f;
		d3dLight.Ambient.g = 0.0f;
		d3dLight.Ambient.b = 0.0f;

		d3dLight.Specular.r = 0.0f;
		d3dLight.Specular.g	= 0.0f;
		d3dLight.Specular.b	= 0.0f;

		break;
	}

	// 设置灯光
	if(FAILED(m_pDevice->SetLight(0, &d3dLight)))
	{
		return false;
	}

	// 使灯光有效。
	if(FAILED(m_pDevice->LightEnable(0, TRUE)))
	{
		return false;
	}

	// 打开灯光。
	if(FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
	{
		return false;
	}

	// 设置环境光强度。
	if(FAILED(m_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(120, 120, 100))))	
	{
		return false;
	}

	return true;
}


bool CTerrain::LineCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 & vOut )
{
	vPos1 -= m_vPos;
	vPos2 -= m_vPos;

	int iCol = (int)( vPos1.x / m_fCellWidth );
	int iRow = (int)( vPos1.z / m_fCellWidth );


	if( iCol < 0 || iRow < 0 || iCol >= m_nCol || iRow >= m_nRow )
		return false;

	VERTEX_TERRAIN * pVertices = m_pVerticesBackUp;

	D3DXVECTOR3 v1,v2,v3,v4;
	v1.x = pVertices[ iCol + iRow * m_nCol ].x;
	v1.y = pVertices[ iCol + iRow * m_nCol ].y;
	v1.z = pVertices[ iCol + iRow * m_nCol ].z;
	
	v2.x = pVertices[ iCol + ( iRow + 1 ) * m_nCol ].x;
	v2.y = pVertices[ iCol + ( iRow + 1 ) * m_nCol ].y;
	v2.z = pVertices[ iCol + ( iRow + 1 ) * m_nCol ].z;

	v3.x = pVertices[ iCol + 1 + iRow * m_nCol ].x;
	v3.y = pVertices[ iCol + 1 + iRow * m_nCol ].y;
	v3.z = pVertices[ iCol + 1 + iRow * m_nCol ].z;
	
	v4.x = pVertices[ iCol + 1 + ( iRow + 1 ) * m_nCol ].x;
	v4.y = pVertices[ iCol + 1 + ( iRow + 1 ) * m_nCol ].y;
	v4.z = pVertices[ iCol + 1 + ( iRow + 1 ) * m_nCol ].z;

	if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v1, v2, v3, vOut ) )
	{
		if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v2, v3, v4, vOut ) )
			return false;
	}

	vOut.y += m_vPos.y;
	
	return true;
}


bool CTerrain::MoveCheck( RAY ray )
{
	return true;
}

float CTerrain::GetHeight(float x,float z)
{
	long col,row;
	col=(long)(x/m_fCellWidth);
	row=(long)(z/m_fCellWidth);
	if(col> m_nCol || col<0 ||row>m_nRow || row<0) 
		return 0;
	else 
	{
		float tmp = 0;
		float f = m_pHeightData[row*m_nCol+col]*m_maxHeight/256.0f;
		if( tmp < f )
			tmp = f;
		f = m_pHeightData[row*m_nCol+col+1]*m_maxHeight/256.0f;
		if( tmp < f )
			tmp = f;
		f = m_pHeightData[(row+1)*m_nCol+col]*m_maxHeight/256.0f;
		if( tmp < f )
			tmp = f;
		f = m_pHeightData[(row+1)*m_nCol+col+1]*m_maxHeight/256.0f;
		if( tmp < f )
			tmp = f;

		return tmp;
	}
}

bool CTerrain::RayCheck( RAY ray, float length, D3DXVECTOR3 & vOut )
{
	D3DXVECTOR3 vPos1 = ray.vOrig;
	D3DXVECTOR3 vPos2 = ray.vOrig + ray.vVector * length;

	vPos1 -= m_vPos;
	vPos2 -= m_vPos;

	float fHeight;
	D3DXVECTOR3 v0,v1,v2,v3;
	int iCol,iRow;
	int index = (int)length / 10 + 2;
	for( int i = 0; i < index; ++ i )
	{
		D3DXVECTOR3 vTmp = vPos1 + ray.vVector * (float)i * 10.f;
		fHeight = GetHeight( vTmp.x, vTmp.z );

		if( vTmp.y <= fHeight )
		{
			iCol = (int)( vTmp.x / m_fCellWidth );
			iRow = (int)( vTmp.z / m_fCellWidth );

			GetFourVertices( v0, v1, v2, v3, iCol, iRow );

			if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v0, v1, v2, vOut ) )
			{
				if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v1, v2, v3, vOut ) )
				{
					int tmpx[8] = {-1,0,1,1,1,0,-1,-1};
					int tmpz[8] = {-1,-1,-1,0,1,1,1,0};
					for( int j = 0; j < 8; ++ j )
					{
						GetFourVertices( v0, v1, v2, v3, iCol + tmpx[j], iRow + tmpz[j] );
						if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v0, v1, v2, vOut ) )
						{
							if( !APP_Tools::appRayPlaneCheck( vPos1, vPos2, v1, v2, v3, vOut ) )
							{
								continue;
							}
						}
						vOut += m_vPos;
						return true;
					}
					break;
				}
			}
			vOut += m_vPos;
			return true;
		}
	}
	return false;
}

void CTerrain::GetFourVertices( D3DXVECTOR3 & v0, D3DXVECTOR3 & v1, D3DXVECTOR3 & v2, D3DXVECTOR3 & v3, int iCol, int iRow )
{
	v0.x = m_pVerticesBackUp[iCol + iRow * m_nCol].x;
	v0.y = m_pVerticesBackUp[iCol + iRow * m_nCol].y;
	v0.z = m_pVerticesBackUp[iCol + iRow * m_nCol].z;

	v1.x = m_pVerticesBackUp[iCol + ( iRow + 1 ) * m_nCol].x;
	v1.y = m_pVerticesBackUp[iCol + ( iRow + 1 ) * m_nCol].y;
	v1.z = m_pVerticesBackUp[iCol + ( iRow + 1 ) * m_nCol].z;

	v2.x = m_pVerticesBackUp[iCol + 1 + iRow * m_nCol].x;
	v2.y = m_pVerticesBackUp[iCol + 1 + iRow * m_nCol].y;
	v2.z = m_pVerticesBackUp[iCol + 1 + iRow * m_nCol].z;

	v3.x = m_pVerticesBackUp[iCol + 1 + ( iRow + 1 ) * m_nCol].x;
	v3.y = m_pVerticesBackUp[iCol + 1 + ( iRow + 1 ) * m_nCol].y;
	v3.z = m_pVerticesBackUp[iCol + 1 + ( iRow + 1 ) * m_nCol].z;
}


