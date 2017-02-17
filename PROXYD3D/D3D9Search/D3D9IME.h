#pragma once

#pragma comment ( lib, "imm32.lib" )
#include <windows.h>
#include <imm.h>

class CD3D9IME
{
public:
	CD3D9IME(void);
public:
	~CD3D9IME(void);

	//控制函数
	void DisableIme();//关闭并禁止输入法，如ime已经打开则关闭，此后玩家不能用热键呼出ime
	void EnableIme();//允许输入法，此后玩家可以用热键呼出ime
	void NextIme();//切换到下一种输入法，必须EnableIme后才有效
	void SharpIme( HWND hWnd );//切换全角/半角
	void SymbolIme( HWND hWnd );//切换中/英文标点

	//状态函数
	bool	GetImeState();
	WCHAR*	GetImeStr();	// 返回完整的输入法名称与选字列表
	bool	IfImeSharp();	//是否全角
	bool	IfImeSymbol();	//是否中文标点

	//必须在消息中调用的函数，如果返回是true，则窗口函数应直接返回0，否则应传递给DefWindowProc
	bool OnWM_INPUTLANGCHANGEREQUEST();
	bool OnWM_INPUTLANGCHANGE( HWND hWnd );
	bool OnWM_IME_NOTIFY( HWND hWnd, WPARAM wParam );
	bool OnWM_IME_COMPOSITION( HWND hWnd, LPARAM lParam, TCHAR * pStr );

private:

	bool	m_bIme;						//ime允许标志
	WCHAR	m_wCompStr[ MAX_PATH ];		//存储转换后的串
	WCHAR	m_wCandList[ MAX_PATH ];	//存储整理成字符串选字表
	WCHAR	m_wImeStr[ MAX_PATH ];		// 完整的输入法字串
	int		m_nImeCursor;			//存储转换后的串中的光标位置
	CANDIDATELIST *m_lpCandList;	//存储标准的选字表
	WCHAR	m_wImeName[ 64 ];		//存储输入法的名字
	bool	m_bImeSharp;			//全角标志
	bool	m_bImeSymbol;			//中文标点标志

	void	ConvertCandList();//将选字表整理成串
	void	ConvertImeStr();		// 整理完整输入法字串

};
