///////////////////////////////////////////////
//main.cpp
///////////////////////////////////////////////



#define WIN32_LEAN_AND_MEAN  

#define INITGUID


//#include "NetApp.h"
#include "main.h"
#include "Game.h"


///////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	PAINTSTRUCT		ps;		//用于WM_PAINT
	HDC				hdc;	//指向设备

	if( g_pGame && g_pGame->m_bInitOver && g_pUI->m_bInitOver )
	{
		if( g_pGame->m_pUI->WndProc( msg, wparam, lparam ) )
			return NULL;
	}
	//主消息循环
	switch(msg)
	{	
	case WM_CREATE: 
		{
			return 0;
		}
		break;

	case WM_PAINT: 
		{
			hdc = BeginPaint(hwnd,&ps);	 
			EndPaint(hwnd,&ps);
			return 0;
		}
		break;

	case WM_DESTROY:
		{
			g_pGame->m_bWindowClosed = true;
			PostQuitMessage(0);
			return 0;
		}
		break;
	case WM_SIZE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD( lparam );
			int y = HIWORD( lparam );

			RAY ray;
			g_pGame->m_pCamera->GetRay( x, y, &ray );

			CItem * pItem = g_pGame->m_pItemManager->ItemRayCheck();
			if (pItem && pItem->m_bPickUp == false)
			{
				D3DXVECTOR3 vItemPos;
				D3DXVECTOR3 vPlayerPos;
				pItem->GetPos(vItemPos);
				CPlayer * pPlayer = g_pGame->m_pPlayerManager->GetUser();
				if (pPlayer)
				{
					pPlayer->GetPos(vPlayerPos);
					D3DXVECTOR3 vPos = vItemPos - vPlayerPos;
					float temp = D3DXVec3Length(&vPos);
					if (D3DXVec3Length(&vPos) < 40)
					{
						if (g_pUI->m_pGameUI->m_pItemUI->AddItem(pItem->m_iType,pItem->m_iNum))
						{
							pItem->m_bPickUp = true;
						}
					}
					else 
					{
						g_pGame->m_pPlayerManager->GetUser()->PickUpItem(pItem);
					}
				}
			}
			CPlayer * pMonster = g_pGame->m_pPlayerManager->MonsterRayCheck();
			if (pMonster)
			{
				if (pMonster == g_pGame->m_pPlayerManager->GetPlayer(40))
				{
					D3DXVECTOR3 vPos;
					pMonster->GetPos(vPos);

					g_pUI->m_pGameUI->m_pShopUI->SetActivate(true);
					g_pGame->m_pPlayerManager->m_pMonster = pMonster;
					g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster = pMonster;
					g_pUI->m_pGameUI->m_pMonsterUI->SetActivate(true);
					g_pGame->m_pPlayerManager->m_pMonster->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_ONE_ONE,FILTER_LINEAR,CULL_CCW,true,true,0);
					g_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(false);
					g_pUI->m_pGameUI->m_pFlxUI->SetActivate(false);
				}
				else
				{
					g_pGame->m_pPlayerManager->m_pMonster = pMonster;
					g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster = pMonster;
					g_pUI->m_pGameUI->m_pMonsterUI->SetActivate(true);
					g_pGame->m_pPlayerManager->m_pMonster->SetCustomRenderState(COLOR_SELECT_TEXTURE,ALPHA_SELECT_TEXTURE,ALPHABLEND_ONE_ONE,FILTER_LINEAR,CULL_CCW,true,true,0);
				}
			}
			else if(g_pGame->m_pPlayerManager->m_pMonster != NULL)
			{
				g_pUI->m_pGameUI->m_pMonsterUI->SetActivate(false);
				g_pGame->m_pPlayerManager->m_pMonster = NULL;
				g_pUI->m_pGameUI->m_pMonsterUI->m_pMonster = NULL;
			}
			g_pGame->m_pPlayerManager->MonsterRayCheck();
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( g_pGame->GetGameUI() )
			{
				short iWheel = (short)(wparam >> 16);
				g_pGame->m_pCamera->Translate( (float)iWheel / 10 );
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
		}
		break;
	case WM_RBUTTONUP:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
			if( g_pGame->GetGameUI() )
			{
				CUIMouse * pUI = g_pUI->m_pMouseUI;
				int x = LOWORD( lparam );
				int y = HIWORD( lparam );
				if( pUI->m_iRDown >= 0 )
				{
					g_pGame->m_pCamera->Rotate( ( x - pUI->m_LastPos.x ) / 100.0f, ( y - pUI->m_LastPos.y ) / 100.0f );
					pUI->m_LastPos.x = x;
					pUI->m_LastPos.y = y;
				}
				g_pGame->m_pItemManager->ItemRayCheck();
				g_pGame->m_pPlayerManager->MonsterRayCheck();
			}
		}
		break;
	case WM_KEYDOWN:
		{
			switch( wparam )
			{
			case VK_RETURN:
				{
				}
				break;
			case VK_ESCAPE:
				{
					if (g_pUI->m_pGameUI->m_pItemUI->GetActivate()||g_pUI->m_pGameUI->m_pMonsterUI->GetActivate()||g_pUI->m_pGameUI->m_pShopUI->GetActivate()||
						g_pUI->m_pGameUI->m_pFlxUI->GetActivate()||g_pUI->m_pGameUI->m_pSkillBookUI->GetActivate())
					{
						g_pUI->m_pGameUI->m_pItemUI->SetActivate(false);
						g_pUI->m_pGameUI->m_pMonsterUI->SetActivate(false);
						g_pUI->m_pGameUI->m_pShopUI->SetActivate(false);
						g_pUI->m_pGameUI->m_pFlxUI->SetActivate(false);
						g_pUI->m_pGameUI->m_pSkillBookUI->SetActivate(false);
					}
					else
					{
						g_pUI->m_pGameUI->m_pSystemUI->SetActivate(true);
					}
				}
				break;
			}
		}
		break;
	case WM_CHAR:
		{
		}
		break;

	default:
		break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}


// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lpcmdline,int ncmdshow)
{
	WNDCLASSEX winclass;
	HWND	   hwnd;	 
	MSG		   msg;		  

	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

	//注册窗口
	if (!RegisterClassEx(&winclass))
		return 0;

	DWORD dwStyle;
	int width,height;

	if( FULLSCREEN )
	{
		dwStyle = WS_POPUP;
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}
	else
	{
		dwStyle = WS_SYSMENU | WS_CAPTION;
		width = SCREEN_WIDTH + 2 * GetSystemMetrics(SM_CXFRAME) - 4;
		height = SCREEN_HEIGHT + 2 * GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CYCAPTION) - 4;
	}
	//创建窗口
	if (!(hwnd = CreateWindowEx(NULL,WINDOW_CLASS_NAME,
		L"3D GAME TEST",
		dwStyle,
		(GetSystemMetrics(SM_CXSCREEN) - (SCREEN_WIDTH))>>1,(GetSystemMetrics(SM_CYSCREEN) - (SCREEN_HEIGHT))>>1,
		width,height,
		NULL,	  
		NULL,	  
		hinstance,
		NULL)))	
		return 0;


	UpdateWindow( hwnd );
	ShowWindow( hwnd, true );

	//图形引擎初始化
	g_pMyD3D		= new CMyD3D;
	if( !g_pMyD3D->Init( hwnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN ) )
		return 0;


	//键盘设备初始化
	g_pDInput	= new CDInput;
	if( !g_pDInput->Init( hinstance ) )
		return 0;
	if( !g_pDInput->InitKeyboard( hwnd ) )
		return 0;

	//声音设备初始化
	g_pDSound	= new CDSound;
	if( !g_pDSound->Init( hwnd ) )
		return 0;
	//if( !g_pDSound->DMusicInit( hwnd ) )
	//	return 0;


	//Game类初始化
	g_pGame		= new CGame;
	if( !g_pGame->Init( hinstance, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, FULLSCREEN ) )
		return 0;



	//读取声音文件
	//g_pDSound->DMusicLoadMIDI("midi/menu.mid");


	//g_pDSound->LoadWAV(L"sound/pop1.wav");
	//g_pDSound->LoadWAV(L"sound/button1.wav");
	//g_pDSound->LoadWAV(L"sound/getitem.wav");

	// 随机种子
	srand(GetTickCount());
	//播放音乐
	//g_pDSound->DMusicPlay(0);
	g_pGame->m_bInitOver = true;
	//主事件循环
	while(TRUE)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			if (msg.message == WM_QUIT)
				break;
			if(msg.message == WM_KEYDOWN && ( msg.lParam & 0x40000000 ) )
			{
			}
			else
			{
				TranslateMessage(&msg);

				DispatchMessage(&msg);
			}
		} 
		if(!g_pGame->MainLoop())
			break;
	} 

	SAFE_RELEASE( g_pMyD3D );


	//释放键盘
	g_pDInput->ReleaseKeyboard();
	//释放输入设备对象
	SAFE_RELEASE( g_pDInput );

	//释放声音设备
	/*g_pDSound->DMusicRelease();
	SAFE_RELEASE( g_pDSound );*/
	SAFE_RELEASE( g_pGame );


	delete g_pGame;
	//delete g_pDSound;
	delete g_pDInput;
	delete g_pMyD3D;


	UnregisterClass( WINDOW_CLASS_NAME, winclass.hInstance );

	return 1;//(int)msg.wParam;

} 

///////////////////////////////////////////////////////////

