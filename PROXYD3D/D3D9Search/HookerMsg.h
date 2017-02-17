#pragma once

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

