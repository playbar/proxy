//************************************************************************************************************
// class      :  
// purpose    :  显示Flash
// Date       :  2008-7-2  13:37:28
// author     :  3o3afe1
//************************************************************************************************************
#if !defined(AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_)
#define AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Flash9f.tlh"

#include "Window.h"
#include <atlbase.h> 
extern CComModule _Module;
#include <atlwin.h>
#pragma comment(lib,"atl")

class KFlash : public KWindow
{
public:
	KFlash();
	virtual ~KFlash();

	//创建函数
	virtual BOOL Create(HWND hParent);
	//消息执行函数
    virtual BOOL HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);	
	
	/*设置α值 */
	BOOL    SetAlpah(BYTE  nAlpah);
	
	//显示窗体	    
	BOOL    CenterWindow(int  nWidht , int nHeight);

	//销毁窗体
	BOOL    DestroyWindow();	


	

	BOOL    Play(LPCTSTR lpszName,BOOL bLoopPaly);
	VOID    Stop();

	BOOL                                    m_bStop;
private:
	
	int                                     m_nMinute;
	CComQIPtr< IGAFlash::IShockwaveFlash>   IFlashPlayer; 		
    CAxWindow                               m_container;     
};






#endif // !defined(AFX_KFLASH_H__B3F0F9C0_0613_4D1E_AF6D_5A17BC3FB8E6__INCLUDED_)
