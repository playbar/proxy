#include "stdafx.h"

void fun( int id )
{
	char text[256];
	sprintf( text, "mesh_id: %d", id );
	OutputDebugString(text);
}

//-----------------------------------------------------------------------
//初始化3D模块
DWORD IGA3DInit( LPDIRECT3DDEVICE9 pd3dDeviceOrig/*, LPIGAFUN pFun */)
{
	//OutputDebugString("IGA3DInit:....................");
	GetIGA3DModule()->m_pd3dDevice = pd3dDeviceOrig;
	int draw_pos = GetIGA3DModule()->Init();
	GetIGA3DModule()->m_DrawPos = draw_pos;
	GetIGA3DModule()->m_pFun = fun;
	return draw_pos;
}

//释放3D模块
void IGA3DDestory()
{
	//OutputDebugString("IGA3DDestory:....................");
	//return;
	GetIGA3DModule()->Destory();
}

//获取矩阵数据
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	//OutputDebugString("IGA3DGetMatrixData:....................");
	//return;
	if( pMatrix == NULL )
	{
		return;
	}

	if( D3DTS_WORLD == State )
	{
		//OutputDebugString("D3DTS_WORLD == State");
		memcpy( &GetIGA3DModule()->m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
		
		GetIGA3DModule()->ComputeSceneID();
		////static int index = 0;
		////static bool b    = true;
		////if( GetAsyncKeyState('B') )
		////{
		////	memcpy( &GetIGA3DModule()->m_mat[index], pMatrix, sizeof(D3DXMATRIX) );
		////	index++;
		////	if( index >= 100 )
		////	{
		////		index = 0;
		////		b     = false;
		////		
		////		char text[256];
		////		for( int i = 0; i < 100; i++ )
		////		{
		////			sprintf( text, "x: %f, y:%f, z: %f", GetIGA3DModule()->m_mat[i]._41, GetIGA3DModule()->m_mat[i]._42, GetIGA3DModule()->m_mat[i]._43 );
		////			OutputDebugString(text);
		////		}
		////	}
		////}

		////if( !b )
		////{
		//		//简单场景控制，《唯舞独尊》
		//		static bool flg = false;
		//		if( 47.86f < (*pMatrix)._41 && 47.87f > (*pMatrix)._41  )
		//		{
		//			GetIGA3DModule()->m_bShow = true;
		//			flg = true;
		//			//break;
		//		}

		//		if( !GetIGA3DModule()->m_bFrist )
		//		{
		//			GetIGA3DModule()->m_bShow = false;
		//		}
		////}
	}

	if( D3DTS_VIEW == State )
	{
		//OutputDebugString("D3DTS_VIEW == State");
		if( !(GetIGA3DModule()->m_bInitView)  && fabsf(pMatrix->_41) > 0.0f )
		{
			memcpy( &GetIGA3DModule()->m_matView, pMatrix, sizeof(D3DXMATRIX) );
			GetIGA3DModule()->m_bInitView = true;
		}
	}

	if( D3DTS_PROJECTION == State )
	{
		//OutputDebugString("D3DTS_PROJECTION == State");
		//在《神泣》......
		if( !(GetIGA3DModule()->m_bInitProj) && pMatrix->_44 == 0.0f )				//perspective projection
		{
			if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && pMatrix->_33 >= 1.0f )
			{
				memcpy( &GetIGA3DModule()->m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				GetIGA3DModule()->m_bInitProj = true;
			}
		}

		//光之国度
		//if(!(GetIGA3DModule()->m_bInitProj) && pMatrix->_44 == 0.0f)    //perspective projection
		//{
		//	if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && fabsf(pMatrix->_33) >= 1.0f && fabsf(pMatrix->_43) >= 3.0f )
		//	{
		//		memcpy( &GetIGA3DModule()->m_matProj, pMatrix, sizeof(D3DXMATRIX) );
		//		GetIGA3DModule()->m_bInitProj = true;
		//	}
		//}


		//在《唯舞独尊》......
		//if( !(GetIGA3DModule()->m_bInitProj) && pMatrix->_44 == 0.0f )				//perspective projection
		//{
		//	if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f )
		//	{
		//		memcpy( &GetIGA3DModule()->m_matProj, pMatrix, sizeof(D3DXMATRIX) );
		//		GetIGA3DModule()->m_bInitProj = true;
		//	}
		//}

		////正交投影用这段代码，《春秋Q传》专用
		//if(!(GetIGA3DModule()->m_bInitProj) && pMatrix->_44 == 1.0f)					//ortho projection
		//{
		//	if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && pMatrix->_33 < 1.0f)
		//	{
		//		memcpy( &GetIGA3DModule()->m_matProj, pMatrix, sizeof(D3DXMATRIX) );
		//		GetIGA3DModule()->m_bInitProj = true;
		//	}
		//}
	}
}

//绘制
void IGA3DRender()
{
	//OutputDebugString("IGA3DRender:....................");
	//return;
	IGA3DModule* pIGA3D = GetIGA3DModule();
	
	if( pIGA3D->m_bFrist == FALSE )
	{
		return;
	}
	
	pIGA3D->m_bFrist = FALSE;

	//获取输入状态
    pIGA3D->IGA3DInputState();

	//绘制
	pIGA3D->PerIGA3DRender();

	//运行IGA3D脚本
	pIGA3D->RunScript();

	pIGA3D->OnIGA3DRender();
	pIGA3D->PostIGA3DRender();

}

void SetFristDraw()
{
	//OutputDebugString("SetFristDraw:....................");
	//return;
	GetIGA3DModule()->m_bFrist = TRUE;
	GetIGA3DModule()->m_bInitView	= false;
	GetIGA3DModule()->m_bInitProj	= false;
	GetIGA3DModule()->m_bInitWprld	= false;
}

void SetUpdate()
{
	GetIGA3DModule()->m_bUpdate = true;
}

//----------------------------------------------------------------------------------
//一个全局的3D模块
IGA3DModule* GetIGA3DModule()
{
	static IGA3DModule iga3D;
	return &iga3D;
}

void GetAbsPath( char fullPathName[MAX_PATH] )
{
	OutputDebugString(fullPathName);
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;

    char path[MAX_PATH];	
	sprintf( path,"%s%s", TmpPath, fullPathName );
	strcpy(fullPathName,path);
	
}

void IGA3DModule::ComputeSceneID()
{


}

//---------------------------------------------------
int IGA3DModule::Init()
{	
	m_Start = GetTickCount();

	//2008-8-5
	m_pFun = NULL;
	m_CurrentScene = -1;
		
	//2008-7-24添加
	m_PickID = -1;

	m_bShow = false;
	m_bUpdate = false;
  
	//矩阵初始化标志
	m_bInitView		= false;
	m_bInitProj		= false;
	m_bInitWprld	= false;

	//获得D3D系统状态
	m_pkOriginalStateBlock = NULL;
	m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pkOriginalStateBlock);

	m_pkProxyStateBlock = NULL;
	m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pkProxyStateBlock);
	if(m_pkProxyStateBlock)
		m_pkProxyStateBlock->Capture();

	m_bIsLoad	= false;
	m_bGetMsg	= false;

	//加载模型对象
	ReLoad();

	GetMessageManager()->Init();
	GetMeshLibrary()->Init( m_pd3dDevice );
//	GetTexLibrary()->Init( m_pd3dDevice );

	//GetDialog()->Init(m_pd3dDevice);

	
	//------------------------------------------------
	//脚本初始化
	//char acXFileName[2048], acXFilePath[2048], acXFileFullName[2048];
	//strcpy(acXFileName, "test.lua");
	//strcpy(acXFilePath, "InterAdtive\\EMMOA\\3D\\");
	//strcpy(acXFileFullName, acXFilePath);
	//strcat(acXFileFullName, acXFileName);

	//GetAbsPath( acXFileFullName );

	//int DrawPos = GetScript()->LoadLuaFile(acXFileFullName);
	//GetScript()->RegFunction( "CGetState", CGetState );
	//GetScript()->RegFunction( "CSetState", CSetState );

	return Per_Present;

}

void IGA3DModule::Destory()
{
	m_DrawLibrary.clear();

	//
	GetMeshLibrary()->Destory();
	//GetDialog()->Destory();
	GetMessageManager()->Destory();
		

	//if(m_pkAnimInstance != NULL )
	//{
	//	OutputDebugString("释放动态模型");
	//	m_pkAnimInstance->ReleaseResource();
	//	delete m_pkAnimInstance;
	//	m_pkAnimInstance = NULL;
	//}

	if(m_pkOriginalStateBlock)
	{
		m_pkOriginalStateBlock->Release();
		m_pkOriginalStateBlock = NULL;
	}

	if(m_pkProxyStateBlock)
	{
		m_pkProxyStateBlock->Release();
		m_pkProxyStateBlock = NULL;
	}

	//m_lock.UnLock();

}


//绘制之前的3D环境状态收集
void IGA3DModule::PerIGA3DRender()
{
	m_ElapseTime = GetTickCount() - m_Start;

	if( GetMessageManager()->GetIGA3DMessage( &m_Msg ) )
	{
		//消息获取成功...
		m_bGetMsg = true;
	}

	if( !m_bIsLoad )
	{
		static long timeLoad = GetTickCount();
		if( GetTickCount() - timeLoad > 10000 )
		{
			GetMeshLibrary()->ClearError();	//清空不存在的网格ID，重新加载
			timeLoad = GetTickCount();
		}
		//return;
	}
	//----------------------------------------------------
	//保存当前的渲染状态
	if(m_pkOriginalStateBlock)
	{
		m_pkOriginalStateBlock->Capture();
	}

	if(m_pkProxyStateBlock)
	{
		m_pkProxyStateBlock->Apply();
	}

	//开始绘制
	if( m_DrawPos == Per_Present )
	{
		if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
		{
			//OutputDebugString("IGA3DModule: beginScene.....");
		}
	}

	//------------------------------------------------------
	m_pd3dDevice->GetFVF(&ulFVF);
	m_pd3dDevice->GetRenderState(D3DRS_LIGHTING, &ulLighting);
	m_pd3dDevice->GetRenderState(D3DRS_CULLMODE, &ulCullMode);
	m_pd3dDevice->GetRenderState(D3DRS_ZENABLE, &ulZEnable);
	m_pd3dDevice->GetRenderState(D3DRS_VERTEXBLEND, &ulVertexBlend);
	m_pd3dDevice->GetTexture(0, &pkTexStage0);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &ulTexStage0ColorOp);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &ulTexStage0ColorArg1);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &ulTexStage0ColorArg2);

	m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);


	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_matProj );

}

//---------------------------------
//绘制3D模块
void IGA3DModule::OnIGA3DRender()
{
	for( m_pDrawNode = m_DrawLibrary.begin(); m_pDrawNode != m_DrawLibrary.end(); m_pDrawNode++ )
	{
		m_DrawElement	= (*m_pDrawNode);
		
		//if( m_DrawElement == -1 )
		//{
		//	break;	//未知场景
		//}

		if( m_CurrentScene != m_DrawElement.scene_id )
		{
			continue;
		}

		int mesh_id		= m_DrawElement.mesh_id;
		float x			= m_DrawElement.x;
		float y			= m_DrawElement.y;
		float z			= m_DrawElement.z;

		float s_xyz     = m_DrawElement.s_xyz;
		float r_x       = m_DrawElement.r_x;
		float r_y       = m_DrawElement.r_y;
		float r_z       = m_DrawElement.r_z;

		D3DXMATRIX matWorld, matRot, matRotX, matRotY, matRotZ, matScal;
		D3DXMatrixTranslation( &matWorld, x,y,z );

		D3DXMatrixScaling( &matScal, s_xyz, s_xyz, s_xyz );

		D3DXMatrixRotationX( &matRotX, r_x );
		D3DXMatrixRotationY( &matRotY, r_y );
		D3DXMatrixRotationZ( &matRotZ, r_z );
		matRot = matRotX * matRotY * matRotZ;

		matWorld = matScal * matRot * matWorld;

		GetMeshLibrary()->Render( mesh_id,  &matWorld, NULL, m_DrawElement.url );
	}


	//if( !m_bIsLoad )
	//{
	//	return;
	//}

	//D3DXMATRIX kMat;
	//D3DXMatrixIdentity(&kMat);
	//m_pd3dDevice->SetTransform(D3DTS_WORLD, &kMat);					//world matrix

	//float fSpeed = 10.0f;
	//D3DXMATRIX kWorldMat, kRotXWorld, kRotYWorld, kTransWorld, kTransWorldMovement;
	//D3DXMatrixIdentity(&kWorldMat);
	//D3DXMatrixIdentity(&kRotXWorld);
	//D3DXMatrixIdentity(&kRotYWorld);
	//D3DXMatrixIdentity(&kTransWorld);
	//D3DXMatrixIdentity(&kTransWorldMovement);

	//D3DXMatrixScaling(&kWorldMat, m_DrawElement.s_x, m_DrawElement.s_y, m_DrawElement.s_z);
	//D3DXMatrixRotationX(&kRotXWorld, m_DrawElement.r_x );
	//D3DXMatrixRotationY(&kRotYWorld, m_DrawElement.r_y );
	//D3DXMatrixTranslation(&kTransWorld, m_DrawElement.x, m_DrawElement.y, m_DrawElement.z);					
	//
	//D3DXMatrixMultiply(&kWorldMat, &kWorldMat, &kRotXWorld);
	//D3DXMatrixMultiply(&kWorldMat, &kWorldMat, &kRotYWorld);
	//D3DXMatrixMultiply(&kWorldMat, &kWorldMat, &kTransWorld);
	//D3DXMatrixMultiply(&kWorldMat, &kWorldMat, &kTransWorldMovement);

	//m_pkAnimInstance->Update(1.0f / 60.0f, &kWorldMat);
	//m_pkAnimInstance->Render();

	/*GetMeshLibrary()->Render( 5,  &kWorldMat, NULL );*/

	//GetDialog()->Render();
}

//---------------------------------
//绘制后的3D环境恢复
void IGA3DModule::PostIGA3DRender()
{
	//if( !m_bIsLoad )
	//{
	//	return;
	//}

	//GetUI()->Render();

	m_pd3dDevice->SetFVF(ulFVF);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, ulLighting);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, ulCullMode);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, ulZEnable);
	m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, ulVertexBlend);
	m_pd3dDevice->SetTexture(0, pkTexStage0);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, ulTexStage0ColorOp);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, ulTexStage0ColorArg1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, ulTexStage0ColorArg2);

	//结束绘制
	if( m_DrawPos == Per_Present )
	{
		m_pd3dDevice->EndScene();
	}

	//状态恢复
    if(m_pkOriginalStateBlock)
	{
		m_pkOriginalStateBlock->Apply();
	}

	//移除顶部消息
	if( m_bGetMsg )
	{
		GetMessageManager()->RemoveMsg();
		m_Msg.key = IGA3D_MSG_UNKNOW;
		m_bGetMsg = false;
	}

	////--------------------------------------------------
	//static bool _bLoad = true;
	//if( GetAsyncKeyState('U') )
	//{
	//	if( _bLoad )
	//	{
	//		m_bUpdate = true;
	//		_bLoad = false;
	//	}
	//}
	//else
	//{
	//	_bLoad = true;
	//}

	////2008-8-12
	////为了配合实时更新
	if( m_bUpdate )
	{
		ReLoad();
		m_bUpdate = false;
	}
}

void IGA3DModule::ReLoad()
{
	GetMeshLibrary()->Destory();	//释放加载的模型资源

	m_DrawLibrary.clear();
	//---------------------------------------------------------
	char acXFileName[1024], acXFilePath[1024], acXFileFullName[1024], acIniFileName[1024];
	strcpy( acXFilePath, "InterAdtive\\EMMOA\\3D\\" );
	
	//-----------------------------------------------------
	sprintf( acIniFileName,"%s%s", acXFilePath, "data.ini" );

	GetAbsPath( acIniFileName );

	//OutputDebugString(acIniFileName);
	GetIni()->Attach(acIniFileName);

	int  num = 0;
	TCHAR  szTmpBuf[256];
	GetIni()->GetInt("comm","count",num,0 );

	for( int i = 0; i < num; i++ )
	{
		TCHAR  szTmpBuf1[256];
		wsprintf(szTmpBuf1,"%u",i+1);
		GetIni()->GetString(szTmpBuf1,"mesh_id",szTmpBuf,256,"-1" );	m_DrawElement.mesh_id	= (int)atof(szTmpBuf);
		GetIni()->GetString(szTmpBuf1,"scene_id",szTmpBuf,256,"-1" );	m_DrawElement.scene_id	= (int)atof(szTmpBuf);
		
		GetIni()->GetString(szTmpBuf1,"s_xyz",szTmpBuf,256,"1.0" );		m_DrawElement.s_xyz		= (float)atof(szTmpBuf);
		
		GetIni()->GetString(szTmpBuf1,"r_x",szTmpBuf,256,"0.0" );		m_DrawElement.r_x		= (float)atof(szTmpBuf);
		GetIni()->GetString(szTmpBuf1,"r_y",szTmpBuf,256,"0.0" );		m_DrawElement.r_y		= (float)atof(szTmpBuf);
		GetIni()->GetString(szTmpBuf1,"r_z",szTmpBuf,256,"0.0" );		m_DrawElement.r_z		= (float)atof(szTmpBuf);

		GetIni()->GetString(szTmpBuf1,"x",szTmpBuf,256,"0.0" );			m_DrawElement.x			= (float)atof(szTmpBuf);
		GetIni()->GetString(szTmpBuf1,"y",szTmpBuf,256,"0.0" );			m_DrawElement.y			= (float)atof(szTmpBuf);
		GetIni()->GetString(szTmpBuf1,"z",szTmpBuf,256,"0.0" );			m_DrawElement.z			= (float)atof(szTmpBuf);

		GetIni()->GetString(szTmpBuf1,"url",m_DrawElement.url,256,"" );

		//char text[1024];
		//sprintf( text, "mesh_id: %d, x: %f, y: %f, z:%f, s_xyz:%f, r_x:%f, r_y:%f, r_z:%f, url:%s", m_DrawElement.mesh_id, m_DrawElement.x,m_DrawElement.y,m_DrawElement.z, m_DrawElement.s_xyz, m_DrawElement.r_x, m_DrawElement.r_y, m_DrawElement.r_z, m_DrawElement.url );
		//OutputDebugString(text);
		m_DrawLibrary.push_back(m_DrawElement);
	}
}

//--------------------------------------------------
//输入状态
void IGA3DModule::IGA3DInputState()
{
	GetMessageManager()->UpdataMessage();
	
	////MSG msg;
	////GetMessage( &msg, NULL,  WM_KEYFIRST, WM_MOUSELAST);

	////if( msg.message == WM_KEYDOWN )
	//{
	//	GetAsyncKeyState(VK_F1)? InputState[b_F1] = true : InputState[b_F1] = false;
	//	GetAsyncKeyState(VK_F2)? InputState[b_F2] = true : InputState[b_F2] = false;
	//	GetAsyncKeyState(VK_F3)? InputState[b_F3] = true : InputState[b_F3] = false;
	//	GetAsyncKeyState(VK_F4)? InputState[b_F4] = true : InputState[b_F4] = false;
	//	GetAsyncKeyState(VK_F5)? InputState[b_F5] = true : InputState[b_F5] = false;
	//	GetAsyncKeyState(VK_F6)? InputState[b_F6] = true : InputState[b_F6] = false;
	//	GetAsyncKeyState(VK_F7)? InputState[b_F7] = true : InputState[b_F7] = false;
	//	GetAsyncKeyState(VK_F8)? InputState[b_F8] = true : InputState[b_F8] = false;
	//	GetAsyncKeyState(VK_F9)? InputState[b_F9] = true : InputState[b_F9] = false;
	//	GetAsyncKeyState(VK_F10)? InputState[b_F10] = true : InputState[b_F10] = false;
	//	GetAsyncKeyState(VK_F11)? InputState[b_F11] = true : InputState[b_F11] = false;
	//	GetAsyncKeyState(VK_F12)? InputState[b_F12] = true : InputState[b_F12] = false;

	//	GetAsyncKeyState('L')? InputState[b_L] = true : InputState[b_L] = false;
	//	GetAsyncKeyState('W')? InputState[b_W] = true : InputState[b_W] = false;
	//	GetAsyncKeyState('H')? InputState[b_H] = true : InputState[b_H] = false;
	//	GetAsyncKeyState('R')? InputState[b_R] = true : InputState[b_R] = false;
	//	GetAsyncKeyState('S')? InputState[b_S] = true : InputState[b_S] = false;
	//	GetAsyncKeyState('A')? InputState[b_A] = true : InputState[b_A] = false;
	//	GetAsyncKeyState('N')? InputState[b_N] = true : InputState[b_N] = false;

	//	GetAsyncKeyState(VK_UP)? InputState[b_UP] = true : InputState[b_UP] = false;
	//	GetAsyncKeyState(VK_DOWN)? InputState[b_DOWN] = true : InputState[b_DOWN] = false;
	//	GetAsyncKeyState(VK_LEFT)? InputState[b_LEFT] = true : InputState[b_LEFT] = false;
	//	GetAsyncKeyState(VK_RIGHT)? InputState[b_RIGHT] = true : InputState[b_RIGHT] = false;

	//	GetAsyncKeyState(VK_CONTROL)? InputState[b_CTRL] = true : InputState[b_CTRL] = false;
	//	GetAsyncKeyState(VK_SHIFT)? InputState[b_SHIFT] = true : InputState[b_SHIFT] = false;
	//	GetAsyncKeyState(VK_OEM_PLUS)? InputState[b_ADD] = true : InputState[b_ADD] = false;
	//	GetAsyncKeyState(VK_OEM_MINUS)? InputState[b_SUB] = true : InputState[b_SUB] = false;

	//	GetAsyncKeyState(VK_DELETE)? InputState[b_DELETE] = true : InputState[b_DELETE] = false;
	//	GetAsyncKeyState(VK_ESCAPE)? InputState[b_ESC] = true : InputState[b_ESC] = false;

	//	GetAsyncKeyState(VK_OEM_COMMA)? InputState[b_MAX] = true : InputState[b_MAX] = false;
	//	GetAsyncKeyState(VK_OEM_PERIOD)? InputState[b_MIN] = true : InputState[b_MIN] = false;
	//	GetAsyncKeyState('T')? InputState[b_T] = true : InputState[b_T] = false;
	//	GetAsyncKeyState('M')? InputState[b_M] = true : InputState[b_M] = false;
	//}

	////if( msg.message == WM_MOUSEACTIVATE )
	//{
	//	GetAsyncKeyState(VK_LBUTTON)? InputState[b_LBUTTON] = true : InputState[b_LBUTTON] = false;
	//	GetAsyncKeyState(VK_RBUTTON)? InputState[b_RBUTTON] = true : InputState[b_RBUTTON] = false;
	//}
	
}

//IGA3D脚本处理
void IGA3DModule::RunScript()
{	
	//------------------------------------------------------
	//运行脚本
	GetScript()->RunScript();

	if( GetAsyncKeyState('C') )
	{
		GetCameraPos();
	}
}

//-----------------------------------------------------------
//
int CGetState( lua_State *L )
{
	if( GetIGA3DModule()->m_PickID >= 0 )
	{
		lua_pushnumber( L, 0 );		//0:表示有对象被选中
		lua_pushnumber( L, GetIGA3DModule()->m_PickID );
		return 2;
	}

	//int num = lua_gettop(L);
	//if( num != 0 )
	//{
	//	for( int i = 0; i < num; i++ )
	//	{
	//		lua_pop(L, i);
	//	}
	//}
	//lua_pushboolean( L, b );
	//lua_pushboolean( L, GetIGA3DModule()->InputState[b_S] );
	//lua_pushstring( L, "asas" );

	//lua_pushboolean( L, false );
	//lua_pushnumber( L, 150 );
	//lua_pushstring( L, "hello world......" );

	return 0; //返回值的数目 
}

int CSetState( lua_State *L )
{
	int num = lua_gettop(L);

	if( num == 0 )
	{
		return 0;
	}
	
	if( !lua_isnumber(L, 1) )
	{
		return 0;
	}

	int operate_id = (int)lua_tonumber(L, 1);
	if( operate_id == 0 )	//清空被选择的ID
	{
		GetIGA3DModule()->m_PickID = -1;
	}

	if( operate_id == 1 )	//绘制模型
	{
		DrawMesh( L );
	}

	if( operate_id == 2 )	//绘制文本
	{
		DrawText( L );
	}
	
	if( operate_id == 3 )
	{
		OpenUrl( L );
	}
	
	if( operate_id == 4 )
	{
		DrawDlg( L );
	}

	return 0; //返回值的数目 
}

void DrawMesh( lua_State *L )
{
	if( !GetIGA3DModule()->m_bShow )
	{
		return;
	}

	int mesh_id = (int)lua_tonumber(L, 2);
	float x       = (float)lua_tonumber(L, 3);
	float y       = (float)lua_tonumber(L, 4);
	float z       = (float)lua_tonumber(L, 5);

	float s_xyz     = (float)lua_tonumber(L, 6);
	float r_x       = (float)lua_tonumber(L, 7);
	float r_y       = (float)lua_tonumber(L, 8);
	float r_z       = (float)lua_tonumber(L, 9);

	int tex_id  = (int)lua_tonumber(L, 10);

	D3DXMATRIX matWorld, matRot, matRotX, matRotY, matRotZ, matScal;
	D3DXMatrixTranslation( &matWorld, x,y,z );

	D3DXMatrixScaling( &matScal, s_xyz, s_xyz, s_xyz );

	D3DXMatrixRotationX( &matRotX, r_x );
	D3DXMatrixRotationY( &matRotY, r_y );
	D3DXMatrixRotationZ( &matRotZ, r_z );
	matRot = matRotX * matRotY * matRotZ;

	matWorld = matScal * matRot * matWorld;

	LPDIRECT3DTEXTURE9  pTex = GetTexLibrary()->GetTexture( tex_id );
	GetMeshLibrary()->Render( mesh_id,  &matWorld, pTex );

}

void DrawText( lua_State *L )
{
	const char* txt = lua_tostring( L, 2 );
	int	   x		= (int)lua_tonumber( L, 3 );
	int    y		= (int)lua_tonumber( L, 4 );

	WCHAR wtext[256];
	::MultiByteToWideChar( CP_ACP, 0, txt, -1, wtext, MAX_PATH );
	GetIGA3DFont()->ShowText( wtext, x, y );

}

void OpenUrl( lua_State *L )
{
	const char* url = lua_tostring( L, 2 );

	HWND hWnd = GetDesktopWindow();
	ShellExecute( hWnd, "open", url, NULL, NULL, SW_SHOW );
}

void DrawDlg( lua_State *L )
{
	GetDialog()->Render();
}

void GetCameraPos()
{
	D3DXMATRIX matView,matProj, matCombViewProj, matInv;
	D3DXVECTOR3 vCamera(0.0f,0.0f,0.0f);
	//D3DXMatrixIdentity(&matView);
	//D3DXMatrixIdentity(&matProj);

	matView = GetIGA3DModule()->m_matView;
	matProj = GetIGA3DModule()->m_matProj;

	matCombViewProj = matView * matProj;

	D3DXMatrixInverse(&matInv, NULL, &matCombViewProj );

	D3DXVec3TransformCoord( &vCamera, &vCamera, &matInv );

	char text[256];
	sprintf( text, "Camera x:%3.2f, y:%3.2f, z:%3.2f", vCamera.x, vCamera.y, vCamera.z );
	OutputDebugString(text);
}