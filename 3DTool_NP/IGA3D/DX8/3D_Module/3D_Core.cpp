#include "stdafx.h"

//2008-8-7:测试函数
void fun( int id )
{
	char text[256];
	sprintf( text, "mesh_id: %d", id );
	OutputDebugString(text);
}

//-----------------------------------------------------------------------
//初始化3D模块
DWORD IGA3DInit( LPDIRECT3DDEVICE8 pd3dDeviceOrig )
{
	GetIGA3DModule()->m_pd3dDevice = pd3dDeviceOrig;
	int draw_pos = GetIGA3DModule()->Init();
    GetIGA3DModule()->m_DrawPos = draw_pos;

	GetIGA3DModule()->m_pFun = fun;

	return draw_pos;
}

//释放3D模块
void IGA3DDestory()
{
	GetIGA3DModule()->Destory();
}

//获取矩阵数据
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	if( pMatrix == NULL )
	{
		return;
	}

	if( D3DTS_WORLD == State )
	{
		//OutputDebugString("D3DTS_WORLD == State");
		memcpy( &GetIGA3DModule()->m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
	}

	if( D3DTS_VIEW == State )
	{
		//OutputDebugString("D3DTS_VIEW == State");
		if( !(GetIGA3DModule()->m_bInitView) )
		{
			memcpy( &GetIGA3DModule()->m_matView, pMatrix, sizeof(D3DXMATRIX) );
			GetIGA3DModule()->m_bInitView = true;
		}
	}

	if( D3DTS_PROJECTION == State )
	{
		//OutputDebugString("D3DTS_PROJECTION == State");
		if( !(GetIGA3DModule()->m_bInitProj) && pMatrix->_44 == 0.0f )				//perspective projection
		{
			if((pMatrix->_22 / pMatrix->_11 - 1.0f) > 0.0f && pMatrix->_33 >= 1.0f)
			{
				memcpy( &GetIGA3DModule()->m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				GetIGA3DModule()->m_bInitProj = true;
			}
		}
	}
}

//绘制
void IGA3DRender()
{
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
	GetIGA3DModule()->m_bFrist		= TRUE;
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

void GetAbsPath( char fullPathName[256] )
{
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;
    char path[256];
	sprintf(path,"%s%s",TmpPath,fullPathName );
	strcpy(fullPathName, path);

}


//---------------------------------------------------
int IGA3DModule::Init()
{	
	m_Start = GetTickCount();

	m_bIsLoad = false;
	m_bUpdate = false;
	
	//2008-8-5
	m_pFun = NULL;
	m_CurrentScene = -1;

	//2008-7-24添加
	m_PickID = -1;

	m_bIsLoad	= false;
	m_bGetMsg	= false;

	//加载绘制对象
	ReLoad();

	GetMessageManager()->Init();
	GetMeshLibrary()->Init(m_pd3dDevice);
	//GetTexLibrary()->Init(m_pd3dDevice);

	////------------------------------------------------
	////脚本初始化

	//char acXFileName[2048], acXFilePath[2048], acXFileFullName[2048];
	//strcpy(acXFileName, "test.lua");
	//strcpy(acXFilePath, "InterAdtive\\EMMOA\\3D\\");
	//strcpy(acXFileFullName, acXFilePath);
	//strcat(acXFileFullName, acXFileName);

	//GetAbsPath( acXFileFullName );

	//int drawPos = GetScript()->LoadLuaFile(acXFileFullName);
	//GetScript()->RegFunction( "CGetState", CGetState );
	//GetScript()->RegFunction( "CSetState", CSetState );

	return Per_Present;
}

void IGA3DModule::Destory()
{
	//if( m_pkFxSkelMesh != NULL )
	//{
	//	delete m_pkFxSkelMesh;
	//	m_pkFxSkelMesh = NULL;
	//}

	m_DrawLibrary.clear();

	GetMeshLibrary()->Destory();

	GetMessageManager()->Destory();
}


//绘制之前的3D环境状态收集
void IGA3DModule::PerIGA3DRender()
{
	m_ElapseTime = GetTickCount() - m_Start;

	//获得消息
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
			m_bIsLoad = true;
		}
		//return;
	}

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	//m_pd3dDevice->GetTransform( D3DTS_WORLD, &m_matWorld );

	if( m_DrawPos == Per_Present )
	{
		m_pd3dDevice->BeginScene();
	}
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
}

//---------------------------------
//绘制后的3D环境恢复
void IGA3DModule::PostIGA3DRender()
{
	//结束绘制
	if( m_DrawPos == Per_Present )
	{
		m_pd3dDevice->EndScene();
	}
	//m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	//m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

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
	//MSG msg;
	//GetMessage( &msg, NULL,  WM_KEYFIRST, WM_MOUSELAST);

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



//--------------------------------------------------------------------------
//lua脚本函数
int CGetState( lua_State *L )
{

	if( GetIGA3DModule()->m_PickID >= 0 )
	{
		lua_pushnumber( L, 0 );		//0:表示有对象被选中
		lua_pushnumber( L, GetIGA3DModule()->m_PickID );
		return 2;
	}

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

	if( operate_id == 1 )
	{
		DrawMesh( L );
	}

	if( operate_id == 2 )	
	{
		OutputDebugString("opre2.....");
	}
	
	if( operate_id == 3 )
	{
		OutputDebugString("opre3.....");
	}
	
	if( operate_id == 4 )
	{
		OutputDebugString("opre4.....");
	}

	return 0; //返回值的数目 
}

void DrawMesh( lua_State *L )
{
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

	LPDIRECT3DTEXTURE8  pTex = GetTexLibrary()->GetTexture( tex_id );
	GetMeshLibrary()->Render( mesh_id,  &matWorld, pTex );
}

void DrawDlg( lua_State *L )
{
	int tex_id  = (int)lua_tonumber(L, 6);

	POINT        ptCursor;  //鼠标位置

	//获取当前鼠标在窗口客户区中的位置
	GetCursorPos( &ptCursor );
	ScreenToClient( GetActiveWindow(), &ptCursor );
	
	//ShowDlg( &ptCursor, tex_id );
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
