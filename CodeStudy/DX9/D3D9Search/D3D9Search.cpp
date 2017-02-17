// gui9.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



#define IDC_BUTTON  1
#define IDC_EDITBOX 2
#define IDC_BAIDUBUTTON	3

CDXUTDialogResourceManager g_kDialogResourceManager;
CDXUTDialog g_kDx9UIDialog;

CMyButton g_MyButtom;

char *g_pcSettingFilePath = "settings.ini";		//setting file path

//define ui controls' positions abd sizes
int iButtonX = 445, iButtonY = 0, iButtonWidth = 80, iButtonHeight = 35;
int iEditBoxX = 242, iEditBoxY = 0, iEditBoxWidth = 200, iEditBoxHeight = 32;

void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl *pkControl, void *pUserContext);
void SaveEditBoxText(const char *pcFilePath);

HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );

LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPDIRECT3DTEXTURE9 g_pTex      = NULL;
LPDIRECT3DTEXTURE9 g_pTex1      = NULL;
LPDIRECT3DTEXTURE9 g_pTex2      = NULL;

HWND g_hWnd = NULL;
HandleBaiduSearch();

typedef LRESULT( WINAPI *LPMSGPROC)( HWND, UINT, WPARAM, LPARAM );
LPMSGPROC OldMsgProc = 0;
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool _bsShow= false;
	bool bNoFurtherProcessing = g_MyButtom.MsgProc(hWnd, msg, wParam, lParam);
	if(bNoFurtherProcessing)
	{
		_bsShow = !_bsShow;
		g_kDx9UIDialog.GetButton(IDC_BUTTON)->SetVisible(_bsShow);
		g_kDx9UIDialog.GetEditBox(IDC_EDITBOX)->SetVisible(_bsShow);
	}

	bNoFurtherProcessing = g_kDialogResourceManager.MsgProc(hWnd, msg, wParam, lParam);
	if(bNoFurtherProcessing)
		return 0;

	bNoFurtherProcessing = g_kDx9UIDialog.MsgProc(hWnd, msg, wParam, lParam);
	if(bNoFurtherProcessing)
		return 0;


	if( OldMsgProc != 0 )
	{
		return OldMsgProc( hWnd, msg, wParam, lParam );
	}
	else
	{
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	
}

HRESULT OnGUIInit( HWND hWnd, LPDIRECT3DDEVICE9 pd3dDevice )
{
	OutputDebugString(L"OnGUIInit.....");

	g_hWnd = hWnd;
	g_pd3dDevice = pd3dDevice;

	if(IsWindowUnicode(hWnd))
	{
		OldMsgProc = (LPMSGPROC)::SetWindowLongW( hWnd, GWL_WNDPROC, (LONG)MsgProc );
		if( OldMsgProc != 0 )
		{
			OutputDebugString(L"OldMsgProc != 0");
			WCHAR text[256];
			wsprintf(text,L"IGA3D of OldMsgProc:%x", OldMsgProc );
			OutputDebugString(text);
		}
	}
	else
	{
		OldMsgProc = (LPMSGPROC)::SetWindowLongA( hWnd, GWL_WNDPROC, (LONG)MsgProc );
		if( OldMsgProc != 0 )
		{
			OutputDebugString(L"OldMsgProc != 0");
			WCHAR text[256];
			wsprintf(text,L"IGA3D of OldMsgProc:%x", OldMsgProc );
			OutputDebugString(text);
		}
	}

	DXUTSetCallbackD3D9DeviceReset(OnResetDevice);
	DXUTSetCallbackD3D9DeviceLost(OnLostDevice);

	g_kDialogResourceManager.OnD3D9CreateDevice(pd3dDevice);
	g_kDialogResourceManager.OnD3D9ResetDevice();
	//DXUT: ui dialog initialization
	g_kDx9UIDialog.Init(&g_kDialogResourceManager);
	g_kDx9UIDialog.SetCallback(OnGUIEvent);

	//define edit box's text
	const int iMaxSize = 1024;
	WCHAR acEditBoxText[iMaxSize];
	wcscpy(acEditBoxText, L"Edit Box");

	//DXUT: add ui controls (a button and an edit box)
	g_kDx9UIDialog.AddButton(IDC_BUTTON, L"OK", iButtonX, iButtonY, iButtonWidth, iButtonHeight);
	g_kDx9UIDialog.AddEditBox(IDC_EDITBOX, L"请输入内容....", iEditBoxX, iEditBoxY, iEditBoxWidth, iEditBoxHeight);

	g_kDx9UIDialog.GetButton(IDC_BUTTON)->SetVisible(false);
	g_kDx9UIDialog.GetEditBox(IDC_EDITBOX)->SetVisible(false);

	//DXUT: dxut initialization
	DXUTInit(true, true);
	DXUTSetHotkeyHandling(true, true, true);

	//DXUT: tell dxut that window is created
	DXUTNotifyWinCreated(NULL, g_hWnd);

	//DXUTSetD3D9Device(pd3dDevice);
	D3DXCreateTextureFromFile( pd3dDevice, L"google.jpg", &g_pTex );
	D3DXCreateTextureFromFile( pd3dDevice, L"google1.jpg", &g_pTex1 );
	D3DXCreateTextureFromFile( pd3dDevice, L"google2.jpg", &g_pTex2 );

	g_MyButtom.AddResource(0,g_pTex);
	g_MyButtom.AddResource(1,g_pTex1);
	g_MyButtom.AddResource(2,g_pTex2);

	g_MyButtom.Set( 150,0,90,32 );


	return S_OK;
}

void OnGUIRender()
{
	//OutputDebugString(L"OnGUIRender.....");

	g_kDx9UIDialog.OnRender( 1.0f / 60.0f );

	g_MyButtom.Draw();


	//RECT rc;
	//SetRect( &rc, 0, 250, 120, 330 );
	//DrawRect( g_pd3dDevice, rc, g_pTex );


}

void OnGUIDestory()
{
	OutputDebugString(L"OnGUIDestory.....");
	if( OldMsgProc != 0 )
	{
		if(IsWindowUnicode(g_hWnd))
		{
			::SetWindowLongW( g_hWnd, GWL_WNDPROC, (LONG)OldMsgProc );
		}
		else
		{
			::SetWindowLongA( g_hWnd, GWL_WNDPROC, (LONG)OldMsgProc );
		}
	}
	g_kDialogResourceManager.OnD3D9LostDevice();
	g_kDialogResourceManager.OnD3D9DestroyDevice();

	if(g_pTex)
	{
		g_pTex->Release();
		g_pTex = NULL;
	}

	if(g_pTex1)
	{
		g_pTex1->Release();
		g_pTex1 = NULL;
	}

	if(g_pTex2)
	{
		g_pTex2->Release();
		g_pTex2 = NULL;
	}


	DXUTNotifyWinClosed();
}



//------------------------------------------------------------------------------
//DXUT: ui event process, it's called by "bool CDXUTDialog::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)"
void CALLBACK OnGUIEvent(UINT uiEvent, int iControlID, CDXUTControl *pkControl, void *pUserContext)
{

	switch(iControlID)
	{
		case IDC_BUTTON:
			{
				HandleBaiduSearch();
				//MessageBox( NULL, L"Button",L"asdads",MB_OK);
				break;
			}
		case IDC_EDITBOX:
			switch(uiEvent)
			{
			case EVENT_EDITBOX_STRING:
				{
					//press Enter to save text in edit box
					//SaveEditBoxText(g_pcSettingFilePath);
					break;
				}

			case EVENT_EDITBOX_CHANGE:
				{
					//MessageBox( NULL, L"CHANGE",L"asdads",MB_OK);
					break;
				}
			}

			break;
	}
}

//DXUT: callback of device reset. you can resize and relocated your controls here 
HRESULT CALLBACK OnResetDevice(IDirect3DDevice9 *pkD3dDevice, const D3DSURFACE_DESC *pkBackBufferSurfaceDesc, void *pUserContext)
{
	HRESULT hr;

	hr = g_kDialogResourceManager.OnD3D9ResetDevice();
	if(FAILED(hr))
		return hr;

	//DXUT: resize and relocated controls
	g_kDx9UIDialog.GetControl(IDC_BUTTON)->SetLocation(iButtonX, iButtonY);
	g_kDx9UIDialog.GetControl(IDC_BUTTON)->SetSize(iButtonWidth, iButtonHeight);
	g_kDx9UIDialog.GetControl(IDC_EDITBOX)->SetLocation(iEditBoxX, iEditBoxY);
	g_kDx9UIDialog.GetControl(IDC_EDITBOX)->SetSize(iEditBoxWidth, iEditBoxHeight);

	return S_OK;
}

//DXUT: callback of device lost 
void CALLBACK OnLostDevice(void* pUserContext)
{
	g_kDialogResourceManager.OnD3D9DestroyDevice();
}

//DXUT: save text to a file
void SaveEditBoxText(const char *pcFilePath)
{


}

//打开连接
BOOL HandleBaiduSearch()
{
	WCHAR text[256];
	g_kDx9UIDialog.GetEditBox(IDC_EDITBOX)->GetTextCopy( text, 256);
	CHAR* txt = "http://www.baidu.com/s?wd=";

	CHAR	szText[512]	= { 0 };
	int	nRet	= ::WideCharToMultiByte(CP_ACP, 0, text, -1, szText, sizeof(szText), NULL, NULL);
	szText[nRet]	= 0;
	CHAR	szFix[4]	= { 0 };
	CHAR	szBaseCodeText[512]	= { 0 };
	for(int nIndex = 0; nIndex < nRet; nIndex++)
	{
		StringCchPrintfA(szFix, 4, "%s%02X", "%", (BYTE)szText[nIndex]);
		StringCchCatA(szBaseCodeText, 512, szFix);
	}
	StringCchPrintfA(szText, 512, "%s%s", txt, szBaseCodeText);
	
	ShellExecuteA( NULL, "open", szText, NULL, NULL, SW_SHOWDEFAULT );
	
	return FALSE;
}



void DrawRect( RECT& rc, LPDIRECT3DTEXTURE9 pTex )
{
	//定义顶点
	SVertex verteices[] = 
	{
		{ rc.left,	rc.top,		0.0f, 1.0f,		0.0f, 0.0f },
		{ rc.right, rc.top,		0.0f, 1.0f,		1.0f, 0.0f },
		{ rc.right, rc.bottom,	0.0f, 1.0f,		1.0f, 1.0f },
		{ rc.left,	rc.bottom,	0.0f, 1.0f,		0.0f, 1.0f }
	};

	//设置材质


	//设置纹理
	g_pd3dDevice->SetTexture( 0, pTex );

	//绘制
	IDirect3DVertexDeclaration9 *pDecl = NULL;
    g_pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl

	g_pd3dDevice->SetFVF(SVertex::_fvf);
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, verteices, sizeof(SVertex) );

    g_pd3dDevice->SetVertexDeclaration( pDecl );
    pDecl->Release();

}