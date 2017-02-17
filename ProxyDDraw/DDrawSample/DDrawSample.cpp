#include <windows.h>
#include <ddraw.h>
#include "Surface.h"

const char *g_pcFileName = "car.bmp";

Surface g_kSurf;
LPDIRECTDRAW7 g_pkDD = NULL;
LPDIRECTDRAWSURFACE7 g_pkDDSFront = NULL;
LPDIRECTDRAWSURFACE7 g_pkDDSBack = NULL;


int InitDirectDraw(HWND hWnd)
{
	DDSURFACEDESC2 kDDSurfDesc;
	DDSCAPS2 kDDScaps;

	HRESULT hRet = DirectDrawCreateEx(NULL, (VOID**)&g_pkDD, IID_IDirectDraw7, NULL);
	if(hRet != DD_OK)
		return -1;

	hRet = g_pkDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if(hRet != DD_OK)
		return -2;

	hRet = g_pkDD->SetDisplayMode(640, 480, 16, 0, 0);
	if(hRet != DD_OK)
		return -3;

	ZeroMemory(&kDDSurfDesc, sizeof(kDDSurfDesc));
	kDDSurfDesc.dwSize = sizeof(kDDSurfDesc);
	kDDSurfDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	kDDSurfDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	kDDSurfDesc.dwBackBufferCount = 1;

	hRet = g_pkDD->CreateSurface(&kDDSurfDesc, &g_pkDDSFront, NULL);
	if(hRet != DD_OK)
		return -1;

	ZeroMemory(&kDDScaps, sizeof(kDDScaps));
	kDDScaps.dwCaps = DDSCAPS_BACKBUFFER;
	hRet = g_pkDDSFront->GetAttachedSurface(&kDDScaps, &g_pkDDSBack);
	if(hRet != DD_OK)
		return -1;

	return 0;
} 

void ProcessIdle()
{
	static int iX = 0, iY = 0;
	static int iLastBlit;

	if(GetTickCount() - iLastBlit < 50)
		return;

	g_kSurf.Draw(g_pkDDSBack, 245, 170, iX, iY, 150, 140);

	while(1)
	{
		HRESULT hRet = g_pkDDSFront->Flip(NULL, 0);
		if(hRet == DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
			g_pkDDSFront->Restore();

		if(hRet != DDERR_WASSTILLDRAWING)
			break;
	}

	iX += 150;
	if(iX >= 1500)
	{
		iX = 0;
		iY += 140;
		if(iY >= 280)
			iY = 0;
	}

	iLastBlit = GetTickCount();
}

void CleanUp()
{
	g_kSurf.Destroy();

	if(g_pkDDSBack)
		g_pkDDSBack->Release();

	if(g_pkDDSFront)
		g_pkDDSFront->Release();

	if(g_pkDD)
		g_pkDD->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
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
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND InitWindow(HINSTANCE hInstance, int iCmdShow)
{
	HWND hWnd;
	WNDCLASS kWndClass;

	kWndClass.style = CS_HREDRAW | CS_VREDRAW;
	kWndClass.lpfnWndProc = WndProc;
	kWndClass.cbClsExtra = 0;
	kWndClass.cbWndExtra = 0;
	kWndClass.hInstance = hInstance;
	kWndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	kWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	kWndClass.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	kWndClass.lpszMenuName = TEXT("");
	kWndClass.lpszClassName = TEXT("DDrawSample");
	RegisterClass(&kWndClass);

	hWnd = CreateWindowEx(WS_EX_TOPMOST, TEXT("DDrawSample"), TEXT("DDrawSample"), WS_POPUP, 0, 0,
		GetSystemMetrics(SM_CXSCREEN),  GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	return hWnd;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	HWND hWnd = InitWindow(hInstance, iCmdShow); 

	if(!hWnd)
		return -1;

	if(InitDirectDraw(hWnd) < 0)
	{
		CleanUp();
		MessageBox(hWnd, "Could start DirectX engine in your computer. Make sure you have at least version 7 of DirectX installed.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	g_kSurf.Create(g_pkDD, 1500, 280);
	g_kSurf.LoadBitmap(hInstance, g_pcFileName, 0, 0, 1500, 280);
	
    while(1)
    {
        MSG kMsg;

        if( PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
        {
            if(kMsg.message == WM_QUIT)
                break;

            TranslateMessage(&kMsg);
            DispatchMessage(&kMsg);
        }
        else
            ProcessIdle();
    }

	CleanUp();

	return 0;
}
