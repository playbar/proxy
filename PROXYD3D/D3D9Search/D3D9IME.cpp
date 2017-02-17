
#include "dxstdafx.h"
#include "D3D9IME.h"
#include <strsafe.h>

CD3D9IME::CD3D9IME(void)
{
	m_lpCandList = NULL;
//	DisableIme();
	m_bIme = true;
	m_wImeName[ 0 ] = 0;
	m_wCompStr[ 0 ] = 0;
	m_wCandList[ 0 ] = 0;
	m_wImeStr[0] = 0;
}

CD3D9IME::~CD3D9IME(void)
{
	DisableIme();
	if( m_lpCandList )
	{
		GlobalFree( (HANDLE)m_lpCandList );
		m_lpCandList = NULL;
	}
}

bool CD3D9IME::GetImeState()
{
	return m_bIme;
}

void CD3D9IME::DisableIme()
{
	while( ImmIsIME( GetKeyboardLayout( 0 )))
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );//如果ime打开通过循环切换到下一个关闭
	m_bIme = false;
	m_wImeName[ 0 ] = 0;
	m_wCompStr[ 0 ] = 0;
	m_wCandList[ 0 ] = 0;
	m_wImeStr[0] = 0;
}

void CD3D9IME::EnableIme()
{
	m_bIme = true;
}

void CD3D9IME::NextIme()
{
	if( m_bIme )
	{
		ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
	}
}

void CD3D9IME::SharpIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SHAPE_TOGGLE );
}

void CD3D9IME::SymbolIme( HWND hWnd )
{
	ImmSimulateHotKey( hWnd, IME_CHOTKEY_SYMBOL_TOGGLE );
}

//转换CandidateList到一个串，空格分隔每一项
void CD3D9IME::ConvertCandList()
{
	CANDIDATELIST *pCandList = m_lpCandList;
	WCHAR *pszCandList = m_wCandList;

	if( pCandList->dwCount < pCandList->dwSelection )
	{
		pszCandList[ 0 ] = 0;
		return;
	}
		//待选字序号超出总数，微软拼音第二次到选字表最后一页后再按PageDown会出现这种情况，并且会退出选字状态，开始一个新的输入
		//但微软拼音自己的ime窗口可以解决这个问题，估计微软拼音实现了更多的接口，所以使用了这种不太标准的数据
		//我现在无法解决这个问题，而且实际使用中也很少遇到这种事，而且其它标准输入法不会引起这种bug
		//非标准输入法估计实现的接口比较少，所以应该也不会引起这种bug
	unsigned int i;
	char * a;
	WCHAR * b;
	for( i = 0; ( i < pCandList->dwCount - pCandList->dwSelection )&&( i < pCandList->dwPageSize ); i++ )
	{
		*pszCandList++ = ( i % 10 != 9 )? i % 10 + L'1' : L'0';//每项对应的数字键
		*pszCandList++ = L'.';//用'.'分隔
		//  根据偏移找到字符串的地址
		a = (char*)pCandList + pCandList->dwOffset[ pCandList->dwSelection + i ];
		//　将指针转换成宽字
		b = (WCHAR*)a;
		wcscpy( pszCandList, b );//每项实际的内容
		pszCandList += wcslen( pszCandList );
		*pszCandList++ = L' ';//项之间以' '分隔
	}
	*( pszCandList - 1 )= 0;//串尾，并覆盖最后一个' '
}

//使用Ctrl+Space或Ctrl+Shift切换输入法时会产生该消息。
//如果我们禁止了ime，则返回false，此时窗口函数应返回0，防止DefWindowProc打开输入法
bool CD3D9IME::OnWM_INPUTLANGCHANGEREQUEST()
{
	return !m_bIme;
}

bool CD3D9IME::OnWM_INPUTLANGCHANGE( HWND hWnd )
{
	//ime改变
	HKL hKL = GetKeyboardLayout( 0 );
	if( ImmIsIME( hKL )){
		HIMC hIMC = ImmGetContext( hWnd );
		ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, m_wImeName );//取得新输入法名字
		DWORD dwConversion, dwSentence;
		ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
		m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;//取得全角标志
		m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;//取得中文标点标志
		ImmReleaseContext( hWnd, hIMC );
	}
	else//英文输入
		wcscpy( m_wImeName, L"英文" );

	ConvertImeStr();
	return false;//总是返回false，因为需要窗口函数调用DefWindowProc继续处理
}

bool CD3D9IME::OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam )
{
	HIMC hIMC;
	DWORD dwSize;
	DWORD dwConversion, dwSentence;
	switch( wParam ){
		case IMN_SETCONVERSIONMODE://全角/半角，中/英文标点改变
			hIMC = ImmGetContext( hWnd );
			ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
			m_bImeSharp = ( dwConversion & IME_CMODE_FULLSHAPE )? true : false;
			m_bImeSymbol = ( dwConversion & IME_CMODE_SYMBOL )? true : false;
			ImmReleaseContext( hWnd, hIMC );
			break;
		case IMN_OPENCANDIDATE://进入选字状态
		case IMN_CHANGECANDIDATE://选字表翻页
			hIMC = ImmGetContext( hWnd );
			if( m_lpCandList )
			{
				//释放以前的选字表
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}
			if( dwSize = ImmGetCandidateList( hIMC, 0, NULL, 0 ))//得到新的选字表
			{
				m_lpCandList = (LPCANDIDATELIST)GlobalAlloc( GPTR, dwSize );
				if( m_lpCandList )
					ImmGetCandidateList( hIMC, 0, m_lpCandList, dwSize );
			}
			ImmReleaseContext( hWnd, hIMC );
			if( m_lpCandList )ConvertCandList();//选字表整理成串
			break;
		case IMN_CLOSECANDIDATE://关闭选字表
			if( m_lpCandList )//释放
			{
				GlobalFree( (HANDLE)m_lpCandList );
				m_lpCandList = NULL;
			}
			m_wCandList[ 0 ] = 0;
			break;
		default:
			return true;
	}
	ConvertImeStr();
	return true;//总是返回true，防止ime窗口打开
}

//输入改变
bool CD3D9IME::OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam, TCHAR * pStr )
{
	HIMC hIMC;
	DWORD dwSize;
	hIMC = ImmGetContext( hWnd );
	// 取得用户输入的编码
	if( lParam & GCS_COMPSTR )
	{
		dwSize = ImmGetCompositionString( hIMC, GCS_COMPSTR, (void*)m_wCompStr, sizeof( m_wCompStr ));
		// 该函数返回的长度为字节数，宽字环境要除以2
		m_wCompStr[ dwSize / 2 ] = 0;
	}
	// 获得输入的中文字串
	if( lParam & GCS_RESULTSTR ){
		WCHAR str[ MAX_PATH ];
		dwSize = ImmGetCompositionString( hIMC, GCS_RESULTSTR, (void*)str, sizeof( str ));//取得汉字输入串
		str[ dwSize / 2 ] = 0;
		// 将输入的文字加入到字串中
		if( pStr )
		{
			StringCchPrintf( pStr, MAX_PATH, L"%s%s", pStr, str );
		}
	}
	ImmReleaseContext( hWnd, hIMC );
	ConvertImeStr();
	return true;//总是返回true，防止ime窗口打开
}

bool CD3D9IME::IfImeSharp()
{//是否全角
	return m_bImeSharp;
}

bool CD3D9IME::IfImeSymbol()
{//是否中文标点
	return m_bImeSymbol;
}

void CD3D9IME::ConvertImeStr()
{
	StringCchPrintf( m_wImeStr, MAX_PATH, L"[%s - %s] %s", m_wImeName, m_wCompStr, m_wCandList );
}

WCHAR * CD3D9IME::GetImeStr()
{
	return m_wImeStr;
}

//由于微软拼音实现了很多自己的东西，CIme和它的兼容性有些问题
//1、在函数ConvertCandList中所说的选字表的问题
//2、函数GetImeInput返回的szCompReadStr显然经过了加工而不是最初的键盘输入
//   它的每个可组合的输入占以空格补足的8byte，且新的不可组合的输入存为0xa1
//   我们可以在输入法名字中有子串"微软拼音"时，只显示末尾的一组8byte，如果有0xa1就什么都不显示，也可以直接用TextOut显示所有的
