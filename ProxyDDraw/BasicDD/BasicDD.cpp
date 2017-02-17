// BasicDD.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <ddraw.h>
#include <windowsx.h> 
#include "csurface.h"
#include "resource.h"

HWND					g_hMainWnd;
HINSTANCE				g_hInst;

LPDIRECTDRAW7			g_pDD = NULL;          // DirectDraw object
LPDIRECTDRAWSURFACE7	g_pDDSFront = NULL;  // DirectDraw frontbuffer surface
LPDIRECTDRAWSURFACE7	g_pDDSBack = NULL;     // DirectDraw backbuffer surface
cSurface				g_surfCar;

HWND InitWindow(int iCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ProcessIdle();
int InitDirectDraw();
void CleanUp();


#define WINDOW_WIDTH      640   // size of window
#define WINDOW_HEIGHT     480
#define WINDOW_BPP        16    // bitdepth of window (8,16,24 etc.)
#define WINDOWED_APP      0     // 0 not windowed, 1 windowed

int window_client_x0   = 0;   // used to track the starting (x,y) client area for
int window_client_y0   = 0;   // for windowed mode directdraw operations


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	g_hInst = hInstance;
	g_hMainWnd = InitWindow(nCmdShow); 

	if(!g_hMainWnd)
		return -1;

	if(InitDirectDraw() < 0)
	{
		CleanUp();
		MessageBox(g_hMainWnd, "Could start DirectX engine in your computer. Make sure you have at least version 7 of DirectX installed.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}


	g_surfCar.Create(g_pDD, 1500, 280);
	g_surfCar.LoadBitmap(g_hInst, IDB_GREENCAR, 0, 0, 1500, 280);
	
    while( TRUE )
    {
        MSG msg;

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            // Check for a quit message
            if( msg.message == WM_QUIT )
                break;

            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            ProcessIdle();
        }
    }

	CleanUp();

	return 0;
}

void ProcessIdle()
{
	HRESULT hRet;
	static int iX = 0, iY = 0;
	static int iLastBlit = 0;

	if(GetTickCount() - iLastBlit < 50)
	{
		return;
	}

	g_surfCar.Draw(g_pDDSBack, 245, 170, iX, iY, 150, 140);

	while( 1 )
	{
		hRet = g_pDDSFront->Flip(NULL, 0 );
		if( hRet == DD_OK )
		{
			break;
		}
		if( hRet == DDERR_SURFACELOST )
		{
			g_pDDSFront->Restore();
		}
		if( hRet != DDERR_WASSTILLDRAWING )
		{
			break;
		}
	}

	iX += 150;
	if(iX >= 1500)
	{
		iX = 0;
		iY += 140;
		if(iY >= 280)
		{
			iY = 0;
		}
	}

	iLastBlit = GetTickCount();
}

HWND InitWindow(int iCmdShow)
{

   HWND      hWnd;
   WNDCLASS  wc;

   wc.style = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = WndProc;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hInstance = g_hInst;
   wc.hIcon = LoadIcon(g_hInst, IDI_APPLICATION);
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
   wc.lpszMenuName = TEXT("");
   wc.lpszClassName = TEXT("Basic DD");
   RegisterClass(&wc);

   hWnd = CreateWindowEx(
      WS_EX_TOPMOST,
      TEXT("Basic DD"),
      TEXT("Basic DD"),
      (WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)), 
      0,
      0,
      WINDOW_WIDTH,//GetSystemMetrics(SM_CXSCREEN),
      WINDOW_HEIGHT,//GetSystemMetrics(SM_CYSCREEN),
      NULL,
      NULL,
      g_hInst,
      NULL);

   if (WINDOWED_APP)
   {
	   // now resize the window, so the client area is the actual size requested
	   // since there may be borders and controls if this is going to be a windowed app
	   // if the app is not windowed then it won't matter
	   RECT window_rect = {0,0,WINDOW_WIDTH-1,WINDOW_HEIGHT-1};

	   // make the call to adjust window_rect
	   AdjustWindowRectEx(&window_rect,
		   GetWindowStyle(hWnd),
		   GetMenu(hWnd) != NULL,
		   GetWindowExStyle(hWnd));

	   // save the global client offsets, they are needed in DDraw_Flip()
	   window_client_x0 = -window_rect.left;
	   window_client_y0 = -window_rect.top;

	   // now resize the window with a call to MoveWindow()
	   MoveWindow(hWnd,
		   0, // x position
		   0, // y position
		   window_rect.right - window_rect.left, // width
		   window_rect.bottom - window_rect.top, // height
		   FALSE);

	   // show the window, so there's no garbage on first render
	   ShowWindow(hWnd, SW_SHOW);
   } // end if windowed


   //::CreateWindow( "BUTTON","asdasdsad", WS_CHILD|WS_VISIBLE,0,0,300,80,hWnd,0,::GetModuleHandle(NULL), 0);

   ShowWindow(hWnd, iCmdShow);
   UpdateWindow(hWnd);
   SetFocus(hWnd);

   return hWnd;
	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_KEYDOWN:
	   if(wParam == VK_ESCAPE)
	   {
			PostQuitMessage(0);
			return 0;
	   }
	   break;
   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

   } // switch

   return DefWindowProc(hWnd, message, wParam, lParam);
} // WndProc

int InitDirectDraw()
{
   DDSURFACEDESC2 ddsd;
   DDSCAPS2       ddscaps;
   HRESULT		  hRet;

   // Create the main DirectDraw object.
   hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
   if( hRet != DD_OK )
       return -1;

   // Get exclusive mode.
   hRet = g_pDD->SetCooperativeLevel(g_hMainWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
   //hRet = g_pDD->SetCooperativeLevel(g_hMainWnd, DDSCL_NOWINDOWCHANGES);
   if( hRet != DD_OK )
       return -2;

   // Set the video mode to 640x480x16.
   hRet = g_pDD->SetDisplayMode(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, 0, 0);
   if( hRet != DD_OK )
       return -3;

   // Prepare to create the primary surface by initializing
   // the fields of a DDSURFACEDESC2 structure.
   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);
  /* ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
   ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |DDSCAPS_COMPLEX;
   ddsd.dwBackBufferCount = 1;*/

   if (!WINDOWED_APP)
   {
	   // fullscreen mode
	   ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	   ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	   ddsd.dwBackBufferCount = 1;
   } // end if
   else
   {
	   ddsd.dwFlags = DDSD_CAPS| DDSD_BACKBUFFERCOUNT;
	   //ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	   ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	   ddsd.dwBackBufferCount = 1;
   } // end else


   // Create the primary surface.
   hRet = g_pDD->CreateSurface(&ddsd, &g_pDDSFront, NULL);
   if( hRet != DD_OK )
       return -1;

   // Get a pointer to the back buffer.
   ZeroMemory(&ddscaps, sizeof(ddscaps));
   ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
   hRet = g_pDDSFront->GetAttachedSurface(&ddscaps, &g_pDDSBack);
   if( hRet != DD_OK )
       return -1;

   return 0;

} 

void CleanUp()
{
	g_surfCar.Destroy();

	if(g_pDDSBack)
		g_pDDSBack->Release();

	if(g_pDDSFront)
		g_pDDSFront->Release();

	if(g_pDD)
		g_pDD->Release();
}