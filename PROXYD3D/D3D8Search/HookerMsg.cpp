#include "dxstdafx.h"
#include "HookerMsg.h"

#include "defineCon.h"
extern DWORD g_dwGameID;

CHookerMsg g_HookerMsg;

extern HANDLE	g_hExitEvent;

LRESULT CALLBACK CHookerMsg::HookWindowProc(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT	*lpMsg	= (CWPSTRUCT*)lParam;
	if(m_hWndHook == lpMsg->hwnd )
	{
		DXUTStaticWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
	}

	return	::CallNextHookEx(m_hHook, code, wParam, lParam);
}

LRESULT CALLBACK HookGetMsg(int code, WPARAM wParam, LPARAM lParam)
{
	MSG	 *lpMsg	= (MSG*)lParam;
	switch ( g_dwGameID )
	{
	case IGA_SHENQI:

		if(g_HookerMsg.GetHookWnd() == lpMsg->hwnd )
		{
			DXUTStaticWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
		}
		break;

	default:

		if(code >= 0 && g_HookerMsg.GetHookWnd() == lpMsg->hwnd )
		{
			if(lpMsg->message == WM_DESTROY && g_hExitEvent)
			{
				::SetEvent(g_hExitEvent);
			}
			//	主要是需要过滤掉WM_KEYDOWN WM_KEYUP这两个消息中的一个
			//	以避免在输入汉字的时候出现重复输入, 这里保留WM_KEYUP，过滤掉WM_KEYDOWN
			if( lpMsg->message == WM_PAINT
				|| lpMsg->message == WM_ACTIVATEAPP
				|| lpMsg->message == WM_DESTROY 
				|| lpMsg->message == WM_KEYUP 
				|| lpMsg->wParam == WM_SYSKEYUP 
				|| lpMsg->message == WM_CHAR 
				|| lpMsg->message == WM_SYSCHAR 
				|| lpMsg->message == WM_DEADCHAR
				|| lpMsg->message == WM_SYSCOMMAND
				||  (lpMsg->message >= WM_MOUSEFIRST && lpMsg->message <= WM_MOUSELAST))
			{	
				LRESULT	lResult	= DXUTStaticWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
				if(lResult)
					return	1;
			}
		}
		break;
	}
	return	::CallNextHookEx(g_HookerMsg.GetHook(), code, wParam, lParam);
}