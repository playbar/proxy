

#include "Sky.h"




CSkyBox::CSkyBox( float width, TCHAR *strTextureA6[] )
{	
	m_pDevice = g_pMyD3D->GetDevice();
	m_uSpeed = 0.012f;
	m_vSpeed = 0.017f;
	m_nSegment = 16;
	m_nSkyTile = 10;
	m_fAngle = D3DX_PI/2;
	m_vPos.x = 0;
	m_vPos.y = 0;
	m_vPos.z = 0;

	m_dwBaseType = OBJECT_TYPE_SKY;

	m_fFrameTime = APP_Tools::appGetTime();

	m_fWidth = width;
	
	for(int i=0;i<7;i++)	
		m_pTexture[i] = NULL;

	Initialize();

}			
CSkyBox::~CSkyBox()
{
	SAFE_RELEASE(m_pVBSky);
	SAFE_RELEASE(m_pIBSky);
	SAFE_RELEASE(m_pVB);

	for(int i=0;i<7;i++)
	{	
		if (m_pTexture[i])
		{
			SAFE_DELETE( m_pTexture[i] );
		}
	}

}
void CSkyBox::Render()
{
	VERTEX_BOX *pV;

	double fTime = APP_Tools::appGetTime() - m_fFrameTime;
	m_fFrameTime = APP_Tools::appGetTime();

	//修改UV使云层运动
	m_pVBSky->Lock(0,0,(void**)&pV,0);
	if(pV[m_nSegment/2].tu<2.0f)
	{
		for(int i=0;i<(m_nSegment+1)*(m_nSegment+1) ;i++)	
			pV[i].tu+=m_uSpeed*(float)fTime;
	}
	else
	{	
		float tu=pV[m_nSegment/2].tu;
		for(int i=(m_nSegment+1)*(m_nSegment+1)-1;i>=0;i--)	
			pV[i].tu-=tu;
	}

	if(pV[m_nSegment/2].tv<2.0f)
	{	
		for(int i=0;i<(m_nSegment+1)*(m_nSegment+1) ;i++)	
			pV[i].tv+=m_vSpeed*(float)fTime;
	}
	else
	{	
		float tv=pV[m_nSegment/2].tv;
		for(int i=(m_nSegment+1)*(m_nSegment+1)-1;i>=0;i--)	
			pV[i].tv-=tv;
	}
	m_pVBSky->Unlock();


	SetMatrix();


	//渲染天空
	//m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,D3DTOP_BLENDTEXTUREALPHA );	//使用透明贴图

	g_pMyD3D->SetFog( 1800.f, 2600.f, 0.001f, 0xffffff, 0 );
	

	SetCustomRenderState( COLOR_SELECT_TEXTURE, ALPHA_NONE, ALPHABLEND_NONE, FILTER_POINT, CULL_CCW, TRUE,false, 0 );
	UseRenderState();

	m_pDevice->SetFVF(D3DFVF_SKYBOX);
	m_pDevice->SetStreamSource(0,m_pVB,0,sizeof(VERTEX_BOX));

	for(int i=0;i<6;i++)
	{
		m_pDevice->SetTexture( 0,m_pTexture[i]->GetBuffer() );
		m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, i*4, 2 );
	}


	//渲染云层
	SetCustomRenderState( FILTER_LINEAR );
	UseRenderState();
	
	m_pDevice->SetTexture( 0, m_pTexture[6]->GetBuffer() );							//云的纹理
	m_pDevice->SetStreamSource( 0, m_pVBSky, 0, sizeof(VERTEX_BOX) );	
	m_pDevice->SetIndices( m_pIBSky );

	m_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,
		(m_nSegment+1)*(m_nSegment+1),0,m_nSegment*m_nSegment*2 );
	
}


void CSkyBox::Initialize()
{	
	//天空盒
	VERTEX_BOX VB[]=
	{	
		{	0.5f,-0.5f, 0.5f,-1.0f,0,0, 0.0f,1.0f,},		//East
		{	0.5f, 0.5f, 0.5f,-1.0f,0,0, 0.0f,0.0f,},		
		{	0.5f,-0.5f,-0.5f,-1.0f,0,0, 1.0f,1.0f,},		
		{	0.5f, 0.5f,-0.5f,-1.0f,0,0, 1.0f,0.0f,},

		{  0.5f,-0.5f,-0.5f,0,0,1.0f, 0.0f,1.0f,},		//North 
		{  0.5f, 0.5f,-0.5f,0,0,1.0f, 0.0f,0.0f,},
		{ -0.5f,-0.5f,-0.5f,0,0,1.0f, 1.0f,1.0f,},		
		{ -0.5f, 0.5f,-0.5f,0,0,1.0f, 1.0f,0.0f,},

		{ -0.5f,-0.5f,-0.5f,1.0f,0,0, 0.0f,1.0f,},		//West
		{ -0.5f, 0.5f,-0.5f,1.0f,0,0, 0.0f,0.0f,},
		{ -0.5f,-0.5f, 0.5f,1.0f,0,0, 1.0f,1.0f,},		
		{ -0.5f, 0.5f, 0.5f,1.0f,0,0, 1.0f,0.0f,},

		{-0.5f,-0.5f, 0.5f,0,0,-1.0f,0.0f,1.0f,},		//South
		{-0.5f, 0.5f, 0.5f,0,0,-1.0f,0.0f,0.0f,},
		{ 0.5f,-0.5f, 0.5f,0,0,-1.0f,1.0f,1.0f,},	
		{ 0.5f, 0.5f, 0.5f,0,0,-1.0f,1.0f,0.0f,},

		{ -0.5f,-0.5f,  0.5f,0,1.0f,0, 0.0f,0.0f,},		//Earth
		{  0.5f,-0.5f,  0.5f,0,1.0f,0, 0.0f,1.0f,},
		{ -0.5f,-0.5f, -0.5f,0,1.0f,0, 1.0f,0.0f,},	
		{  0.5f,-0.5f, -0.5f,0,1.0f,0, 1.0f,1.0f,},

		{ 0.5f,  0.5f,  0.5f,0,-1.0f,0, 0.0f,0.0f,},		//Sky
		{-0.5f,  0.5f,  0.5f,0,-1.0f,0, 1.0f,0.0f,},		
		{ 0.5f,  0.5f, -0.5f,0,-1.0f,0, 0.0f,1.0f,},	     
		{-0.5f,  0.5f, -0.5f,0,-1.0f,0, 1.0f,1.0f,},

	};

	//---------------------------------------------------------------------------------------------
	//创建SkyDome,方形圆顶
	//---------------------------------------------------------------------------------------------

	int nVertex=(m_nSegment+1)*(m_nSegment+1);		//顶点数
	int nIndex=m_nSegment*m_nSegment*6;				//索引点数


	float R=0.5f/sinf(m_fAngle/2);	//半径满足弧形宽度为1，和天空盒相等大小

	VERTEX_BOX *VBSKY=new VERTEX_BOX[nVertex];	//顶点缓冲区
	DWORD	   *IBSKY=new DWORD[nIndex];		//索引数据
	//平水平间距相同的网格

	//		int	  row,col;
	//		float temp;

	float angleCol = 0, angleRow = 0;
	float anglePerSeg=m_fAngle/m_nSegment;  
	float angleStart=D3DX_PI/2-m_fAngle/2;
	float dYDown=R*sinf(angleStart);	//云层向下的偏移量，边缘位于地平线上

	int i = 0;
	for ( i=0;i<nVertex;i++)	//顶点数据
	{
		angleCol=angleStart+(i%(m_nSegment+1))*anglePerSeg;
		angleRow=angleStart+(i/(m_nSegment+1))*anglePerSeg;

		VBSKY[i].nx=-cosf(angleCol);				//法线指向圆心
		VBSKY[i].ny=-sinf(angleCol)*sinf(angleRow);
		VBSKY[i].nz=-sinf(angleCol)*cosf(angleRow);

		VBSKY[i].x=-R*VBSKY[i].nx;
		VBSKY[i].y=-R*VBSKY[i].ny-dYDown;			//偏移，边缘和地平线(y=0)平齐
		VBSKY[i].z=-R*VBSKY[i].nz;
		VBSKY[i].tu=angleCol/m_fAngle - D3DX_PI * 3.0f / 8.0f;
		VBSKY[i].tv=angleRow/m_fAngle - D3DX_PI * 3.0f / 8.0f;	
		if(VBSKY[i].y<0.0f)
			VBSKY[i].y=0.0f;
	}

	DWORD *lpIB=IBSKY;						//索引数据
	long   iMax=(m_nSegment+1)*m_nSegment;	//最后一个对应到矩形的左下角的顶点的索引号
	for (i=0;i<iMax;i++ )	
	{
		if(i%(m_nSegment+1)!=m_nSegment)	//每行的最后一个点不对应一个矩形
		{
			*(lpIB++)=i;
			*(lpIB++)=i+m_nSegment+2;		//每行顶点数量m_nSegment+1;
			*(lpIB++)=i+m_nSegment+1;

			*(lpIB++)=i;
			*(lpIB++)=i+1;
			*(lpIB++)=i+m_nSegment+2;
		}

	}

	//天空盒
	m_pDevice->CreateVertexBuffer(sizeof(VB),D3DUSAGE_WRITEONLY,D3DFVF_SKYBOX, 
		D3DPOOL_DEFAULT, &m_pVB,NULL);
	void* pVertices;
	m_pVB->Lock(0,sizeof(VB),&pVertices,0);
	memcpy(pVertices,(void*)VB,sizeof(VB));
	m_pVB->Unlock();

	//云的顶点数据
	m_pDevice->CreateVertexBuffer(sizeof(VERTEX_BOX)*nVertex,0,	D3DFVF_SKYBOX, 
		D3DPOOL_MANAGED, &m_pVBSky,NULL);
	m_pVBSky->Lock(0,sizeof(VERTEX_BOX)*nVertex,&pVertices,0);
	memcpy(pVertices,(void*)VBSKY,sizeof(VERTEX_BOX)*nVertex);
	m_pVBSky->Unlock();

	//天空的索引数据
	m_pDevice->CreateIndexBuffer(sizeof(DWORD)*nIndex,D3DUSAGE_WRITEONLY,D3DFMT_INDEX32, 
		D3DPOOL_DEFAULT, &m_pIBSky,NULL);
	m_pIBSky->Lock(0,sizeof(DWORD)*nIndex,&pVertices,0);
	memcpy(pVertices,(void*)IBSKY,sizeof(DWORD)*nIndex);
	m_pIBSky->Unlock();

	delete []IBSKY;
	delete []VBSKY; 	
}


void CSkyBox::LoadSkyBox(TCHAR * strConfigFile,TCHAR * strSkyName)
{
	_ASSERT(!m_pTexture[0]);
	TCHAR strFileName[256];
	TCHAR *strKeyName[]=
	{
			L"East",
			L"South",
			L"West",
			L"North",
			L"Earth",
			L"Sky",
			L"Cloud"
	};
	if(!m_pTexture[0])
	{
		for(int i=0;i<7;i++)
		{
			GetPrivateProfileString(strSkyName,strKeyName[i],L"\0",strFileName,sizeof(strFileName),strConfigFile);
			_ASSERT(strFileName[0]);
			TCHAR strName[256] = L"bin/";
			_tcscat( strName, strFileName );
			m_pTexture[i] = new CTexture;
			m_pTexture[i]->CreateFromFile( strName, false );
		}
	}

}



void CSkyBox::SetMatrix()
{
	D3DXMATRIX	matWorld;
	//m_pDevice->GetTransform(D3DTS_VIEW,&matWorld);
	//D3DXMatrixInverse(&matWorld,NULL,&matWorld);

	matWorld._11=m_fWidth;	matWorld._12=0;			matWorld._13=0;			matWorld._14=0;
	matWorld._21=0;			matWorld._22=m_fWidth;	matWorld._23=0;			matWorld._24=0;
	matWorld._31=0;			matWorld._32=0;			matWorld._33=m_fWidth;	matWorld._34=0;
	matWorld._41=m_vPos.x;	matWorld._42=m_vPos.y;	matWorld._43=m_vPos.z;	matWorld._44=1;
	m_pDevice->SetTransform(D3DTS_WORLD,&matWorld);

}
