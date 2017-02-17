//////////////////////////////////
//Game.cpp
//////////////////////////////////


//#include "NetApp.h"
#include "Game.h"


CGame * g_pGame;


bool CGame::Init(HINSTANCE hHinstance,HWND hWND,WORD wScreenWidth,WORD wScreenHeight,WORD wBPP,bool bFullScreen)
{
	m_hHinstance	= hHinstance;
	m_hWND			= hWND;
	m_wScreenWidth	= wScreenWidth;
	m_wScreenHeight	= wScreenHeight;
	m_wBPP			= wBPP;
	m_bFullScreen	= bFullScreen;

	m_bWindowClosed = false;

	m_pUI = new CUI;
	g_pUI = m_pUI;

	m_pCamera = NULL;
	m_pTerrain = NULL;
	m_pSkyBox = NULL;

	RECT rect;
	GetClientRect( m_hWND, &rect );
	m_pCamera  = g_pCamera	= new CCamera( rect.right / (float)rect.bottom );
	m_pCamera->Init( 0.5f, 6.f, -D3DX_PI / 4 + 0.1f, D3DX_PI / 2 - 0.1f );


	m_dwFrame		= 0;
	m_dwStartTime  = 0;

	m_wGameControl	= GAME_INIT_LOADING;
	m_wLoadingFlag  = LOADING_UI;

	m_hFont[0] = CreateFont(16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,L"隶书");

	m_hFont[1] = CreateFont(10,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,L"宋体");
	
	m_dwStartTime = GetTickCount();

	return true;
}


/////////////////////////////////////////////////////////////////////////////


bool CGame::MainLoop()
{
	if( m_bWindowClosed )
		return true;

	g_pDInput->ReadKeyboard();			//读取键盘


	//主游戏循环
	switch( m_wGameControl )
	{
	case GAME_LOGIN:
		{
			if(!GameLogin())
				return false;
		}
		break;
	case GAME_NEW:
		{
			if(!GameNew())
				return false;
		}
		break;
	case GAME_LOAD:
		{
			if(!GameLoad())
				return false;
		}
		break;
	case GAME_RUN:
		{
			if(!GameRun())
				return false;
		}
		break;
	case GAME_OVER:
		{
			if(!GameOver())
				return false;
		}
		break;
	case GAME_STORY:
		{
			if(!GameStory())
				return false;
		}
		break;
	case GAME_INIT_LOADING:
		{
			if(!GameInitLoading())
				return false;
		}
		break;
	case GAME_ENTER_LOADING:
		{
			if(!GameEnterLoading())
				return false;
		}
		break;
	case GAME_RETURN_LOGIN:
		{
			if (!GameReturnLogin())
			{
				return false;
			}
		}
		break;
	}
	
	UpdateFrame();

	return true;
}


///////////////////////////////////////////////////////////////


void CGame::Release()
{
	SAFE_DELETE( m_pCamera );
	SAFE_DELETE( m_pTerrain );
	SAFE_DELETE( m_pSkyBox );

	SAFE_RELEASE( m_pUI );
	SAFE_DELETE( m_pUI );
	
	SAFE_RELEASE( m_pPlayerManager );
	SAFE_DELETE( m_pPlayerManager );
	SAFE_RELEASE(m_pItemManager);
	SAFE_DELETE(m_pItemManager);
 //	SAFE_RELEASE(m_pSkillManager);
 	SAFE_DELETE(m_pSkillManager);
}


//////////////////////////////////////////////////////////////////


void CGame::UpdateFrame()
{
	//process
	SetMousePos();
	Process();

	m_pCamera->UpdateCamera();

	//3D Render
	g_pMyD3D->Render();

	//UI Render
	g_pMyD3D->BeginScene();
	g_pMyD3D->GetDevice()->SetRenderState(D3DRS_FOGENABLE,false);
	m_pUI->Render();
	g_pMyD3D->EndScene();

//*
	//Frame
	DWORD dwTime = GetTickCount();
	while( dwTime - m_dwFrameTime < 10 )
	{
		Sleep( 1 );
		dwTime = GetTickCount();
	}
	
	//test info
	TCHAR szTmp1[50] = L"";
	TCHAR szTmp3[] = L" 帧/秒"; 
	TCHAR szNum[10];
	APP_Tools::appIntToChar( 1000 / ( dwTime - m_dwFrameTime ), szNum );
	_tcscat( szTmp1, szNum );
	_tcscat( szTmp1, szTmp3 );
	RECT rect = { 10, 20, 200, 600 };

	g_pMyD3D->BeginScene();
	g_pMyD3D->m_pVFont[0]->m_pFont->DrawTextW( NULL, szTmp1, (INT)_tcslen( szTmp1 ), &rect, DT_WORDBREAK, 0xff0000ff );
	g_pMyD3D->EndScene();

	char szPos[100];
	if( m_wGameControl == GAME_RUN )
	{
		D3DXVECTOR3 vPos; 
		m_pPlayerManager->GetUser()->GetPos(vPos);
		sprintf( szPos, "Player: %f,%f,%f", vPos.x, vPos.y, vPos.z );
		rect.left = 200;
		rect.right = 800;
		rect.top = 20;
		rect.bottom = 100;

		g_pMyD3D->BeginScene();
		g_pMyD3D->m_pVFont[0]->m_pFont->DrawTextA( NULL, szPos, (INT)strlen( szPos ), &rect, DT_WORDBREAK, 0xffffffff );
		g_pMyD3D->EndScene();
	}

	rect.left = 600;
	rect.right = 800;
	sprintf( szPos, "MousePos: %d, %d", m_pUI->m_pMouseUI->m_MousePos.x, m_pUI->m_pMouseUI->m_MousePos.y );
	g_pMyD3D->BeginScene();
	g_pMyD3D->m_pVFont[0]->m_pFont->DrawTextA( NULL, szPos, (INT)strlen( szPos ), &rect, DT_WORDBREAK, 0xff0000ff );
	g_pMyD3D->EndScene();

	m_dwFrameTime = dwTime; 
//*/
	//flip
	g_pMyD3D->Flip();
	m_dwFrame ++;

	
}


/////////////////////////////////////////////////////////////////////

void CGame::Process()
{
	POINT point;
	GetMousePos( &point );

	//Process here
	m_pUI->Process( (float)point.x, (float)point.y );
}

/////////////////////////////////////////////////////////////////////

bool CGame::GameNew()
{
	//g_pDSound->DMusicLoadMIDI("midi/home.mid");
	//g_pDSound->DMusicPlay(0);

	return true;
}

////////////////////////////////////////////////////////////////////


bool CGame::GameRun()
{
	m_pPlayerManager->GetUser()->process();
	for (int i = 0; i < MONSTER_NUM; i++)
	{
		m_pPlayerManager->GetPlayer(i)->process();
	}
	m_pItemManager->RemoveItem();
	m_pSkillManager->RemoveSkill();
	static bool bKey = true;
	if ((g_pDInput->m_bKeyboard[DIK_B] & 0x80) && (m_pUI->m_pGameUI->m_pItemUI->GetActivate()))
	{
		if (bKey)
		{
			m_pUI->m_pGameUI->m_pItemUI->SetActivate(false);
			bKey = false;
		}
	}
	else if ((g_pDInput->m_bKeyboard[DIK_B] & 0x80))
	{
		if (bKey)
		{
			m_pUI->m_pGameUI->m_pItemUI->SetActivate(true);
			bKey = false;
		}
	}
	else 
	{
		bKey = true;
	}
		
	static bool bKey2 = true;
	if ((g_pDInput->m_bKeyboard[DIK_V] & 0x80) && (m_pUI->m_pGameUI->m_pFlxUI->GetActivate()))
	{
		if (bKey2)
		{
			m_pUI->m_pGameUI->m_pFlxUI->SetActivate(false);
			bKey2 = false;
		}
	}
	else if ((g_pDInput->m_bKeyboard[DIK_V] & 0x80))
	{
		if (bKey2)
		{
			m_pUI->m_pGameUI->m_pFlxUI->SetActivate(true);
			bKey2 = false;
		}
	}
	else bKey2 = true;

	static bool bKey3 = true;
	if ((g_pDInput->m_bKeyboard[DIK_P] & 0x80) && (m_pUI->m_pGameUI->m_pSkillBookUI->GetActivate()))
	{
		if (bKey3)
		{
			m_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(false);
			bKey3 = false;
		}
	}
	else if ((g_pDInput->m_bKeyboard[DIK_P] & 0x80))
	{
		if (bKey3)
		{
			m_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(true);
			bKey3 = false;
		}
	}
	else bKey3 = true;

 	D3DXVECTOR3 vPos;
 	vPos  = g_pGame->m_pCamera->GetPos();
 
 	g_pGame->m_pTerrain->LineCheck(vPos - D3DXVECTOR3(0.f,1000,0.f),vPos + D3DXVECTOR3(0.f,1000.f,0.f),vPos);
 	if (g_pGame->m_pCamera->GetPos().y < vPos.y)
 	{
 		g_pGame->m_pCamera->SetPos(vPos);
 	}
	return true;
}


////////////////////////////////////////////////////////////////////


bool CGame::GameOver()
{
	return true;
}


////////////////////////////////////////////////////////////////////


bool CGame::GameLogin()
{	
	return true;
}

///////////////////////////////////////////////////////////////////////

bool CGame::GameLoad()
{
	m_pSkyBox = new CSkyBox( 6000 );
	m_pSkyBox->LoadSkyBox(L"bin/sky.cfg",L"SkyBox");
	m_pSkyBox->SetPos( D3DXVECTOR3(0,0,0) );
	g_pMyD3D->AddToWorld( m_pSkyBox );

	m_pTerrain = new CTerrain( D3DXVECTOR3(0,0,0) );
	m_pTerrain->CreateTerrain(L"Bin/Terrain_128_RGB.bmp",L"Bin/terr_dirt-grass.jpg",20,200);
	m_pTerrain->InitialiseLight( D3DLIGHT_DIRECTIONAL );
	g_pMyD3D->AddToWorld( m_pTerrain );
		
	CPlayer * pPlayer = new CPlayer(D3DXVECTOR3(1200.f,10.f,1200.f),1.f,0.05f);
	pPlayer->SetMesh(m_pPlayerManager->m_pSkinMesh[0]);
	pPlayer->SetAction("idle");
	m_pPlayerManager->m_pPlayerList->InsertPlayer(30,pPlayer);
	m_pTerrain->LineCheck(*(pPlayer->GetPos()) - D3DXVECTOR3(0.f,1000,0.f),*(pPlayer->GetPos()) + D3DXVECTOR3(0.f,1000.f,0.f),*(pPlayer->GetPos()));

	CNpc * pNPC = new CNpc(D3DXVECTOR3(1200.f,10.f,1200.f),1.f,0.07f);
	pNPC->SetMesh(m_pPlayerManager->m_pSkinMesh[0]);
	pNPC->SetAction("idle");
	m_pTerrain->LineCheck(*(pNPC->GetPos()) - D3DXVECTOR3(0.f,1000,0.f),*(pNPC->GetPos()) + D3DXVECTOR3(0.f,1000.f,0.f),*(pNPC->GetPos()));
	m_pPlayerManager->m_pPlayerList->InsertPlayer(40,pNPC);
	
	CPlayer * pMonster;
	for (int i = 0; i < MONSTER_NUM; i++)
	{
		pMonster = new CMonster(D3DXVECTOR3((float)(rand()%300)+1000,10.f,(float)(rand()%300)+1200),0.f,1.f);
		m_pTerrain->LineCheck(*(pMonster->GetPos()) - D3DXVECTOR3(0.f,1000,0.f),*(pMonster->GetPos()) + D3DXVECTOR3(0.f,1000.f,0.f),*(pMonster->GetPos()));
		pMonster->SetMesh(m_pPlayerManager->m_pSkinMesh[1]);
		m_pPlayerManager->m_pPlayerList->InsertPlayer(i,pMonster);
	}
	m_wGameControl = GAME_RUN;
	m_pUI->m_pLoadingUI->SetActivate( false );
	return true;
}


/////////////////////////////////////////////////////////////////////////

bool CGame::GameStory()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////

bool CGame::GameInitLoading()
{
	if( m_pUI->Init() )
	{
		m_wGameControl = GAME_LOGIN;
		m_pPlayerManager = new CPlayerManager;
		m_pPlayerManager->Init();
		m_pItemManager = new CItemManager;
		m_pItemManager->Init();
		m_pSkillManager = new CSkillManager;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

bool CGame::GameEnterLoading()
{
	if( m_pUI->SetActivateId( BASEUI_GAME ) )
	{
	}
	else
	{
		return true;
	}
	m_wGameControl = GAME_LOAD;
	return true;
}

bool CGame::GameReturnLogin()
{
	static int i = 0;
	switch(i)
	{
	case 0:
		g_pMyD3D->RemoveAllFromWorld();
		i++;
		m_pUI->m_pLoadingUI->UpdateLoading(100.f/5.f);
		return true;
		break;
	case 1:
		SAFE_DELETE(m_pTerrain);
		i++;
		m_pUI->m_pLoadingUI->UpdateLoading(100.f/5.f);
		return true;
		break;
	case 2:
		SAFE_DELETE(m_pSkyBox);
		i++;
		m_pUI->m_pLoadingUI->UpdateLoading(100.f/5.f);
		return true;
		break;
	case 3:
		m_pItemManager->m_pItemList->ClearAll();
		i++;
		m_pUI->m_pLoadingUI->UpdateLoading(100.f/5.f);
		return true;
		break;
	case 4:
		m_pPlayerManager->m_pPlayerList->ClearAll();
		i++;
		m_pUI->m_pLoadingUI->UpdateLoading(100.f/5.f);
		return true;
		break;
	}
	if (m_pUI->SetActivateId(BASEUI_LOGIN))
	{
		i = 0;
		SetGameControl(GAME_LOGIN);
		m_pUI->m_pLoadingUI->SetActivate(false);
	}
	return true;
}