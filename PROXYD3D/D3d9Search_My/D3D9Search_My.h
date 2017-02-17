

#include "WinInet.h"

#pragma  once


#pragma warning(disable:4996)


#define IDC_LABEL_TIPS	         5   // 标签控件, 显示用户操作说明
#define IDC_BUTTON_ENABLE_IME   18   // 启用IME按钮
#define IDC_BUTTON_DISABLE_IME	19   // 禁用IME按钮
#define IDC_IME_EDITBOX	        21   // IME编辑框
#define	IDC_BUTTON_SEARCH		22	 // 搜索按钮
#define	IDC_BUTTON_CLOSE		23	 // 关闭按钮

#define	ADINFO_EMMOA_RES_FLASH	"InterAdtive\\EMMOA\\res\\flash\\" 

typedef DECLSPEC_IMPORT UINT (WINAPI* LPTIMEBEGINPERIOD)( UINT uPeriod );

//消息接管函数指针定义
typedef LONG (CALLBACK *LPWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);


HRESULT WINAPI D3D9SearchInit(LPDIRECT3DDEVICE9 pDevice, LPARAM lParam);
void WINAPI D3D9SearchRender();
void WINAPI D3D9SearchDestory();

D3DCOLOR ColorFromText(LPCSTR lpszText);
BOOL LoadGUIDefaultFromConfig();

DWORD WINAPI OpenIEBrowserThreadProc(void *p);
BOOL HandleSearchButtonMsg();
BOOL IsWindows(LPDIRECT3DDEVICE9 pDevice);


typedef struct tagD3DSearchParam
{
	union
	{
		DWORD	dwAdID;
		DWORD	dwEventID;
	};
	DWORD	dwGameID;
	int		nSearchType;
	LPCallBack3DFocus pFun3DFocus;	//	统计点击次数的回调函数
	HWND	hWndFocus;
	HWND	hWndDeviceFullscreen;
	HWND	hWndDeviceWindowed;
	WPARAM	wParam;
	LPARAM	lParam;

}D3DSEARCH_PARAM, *LPD3DSEARCH_PARAM;


typedef	struct tagD3D9SearchSettings
{
	DWORD		dwSearchType;
	DWORD		dwGameID;
	DWORD		dwAdID;

	WCHAR		szFontName1[256];
	UINT		nFontSize1;
	RECT		rectSearchEdit;
	D3DCOLOR	clrTexture;
	D3DCOLOR	clrText;
	D3DCOLOR	clrCaret;
	D3DCOLOR	clrTextSelected;
	D3DCOLOR	clrBackSelected;
	RECT		rectSearchButton;
	RECT		rectShowSearchButton;

	int iSearchBoxWidth;
	int iSearchBoxHeight;
	int iSearchBtnWidth;
	int iSearchBtnHeight;
	WPARAM		wKey;

	tagD3D9SearchSettings()
	{
		ZeroMemory(this, sizeof(*this));
	}

}D3D9SEARCHSETTINGS, *LPD3D9SEARCHSETTINGS;


typedef struct tagThreadParam
{
	HWND	hWndToMonitor;
	char	szURL[INTERNET_MAX_URL_LENGTH];
	WPARAM	wParam;
	LPARAM	lParam;
}THREADPARAM, *LPTHREADPARAM;

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK HookGetMsg(int code, WPARAM wParam, LPARAM lParam);

class CHookerMsg
{
public:
	CHookerMsg():m_hHook(NULL), m_hWndHook( NULL ){}
	~CHookerMsg()
	{
		if(m_hHook != NULL)
		{
			UnhookWindowsHookEx(m_hHook);
			m_hHook	= NULL;
		}
	}
	void SetHook(HHOOK h){ m_hHook	= h; }
	HHOOK GetHook()      { return m_hHook; }

	void SetWndHook( HWND hWnd )		{ m_hWndHook = hWnd; }
	HWND GetWndHook()					{ return m_hWndHook; }
	LRESULT CALLBACK HookWindowProc(int code, WPARAM wParam, LPARAM lParam);

private:
	HHOOK	m_hHook;
	HWND	m_hWndHook;
};

extern CHookerMsg g_HookerMsg;

//////////////////////////////////////////////////////////////////////////
