/**
 * Copyright 2007 by smallfool,
 * Room 303, Building 41, No 51, Boshan Road, Pudong District, Shanghai, China
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of smallfool. 
 * Use is subject to license terms.
 *
 * Thanks to Dandan Huang.
 */
// Window.h: interface for the KWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOW_H__34A43422_A484_401D_84A9_D8042790CD15__INCLUDED_)
#define AFX_WINDOW_H__34A43422_A484_401D_84A9_D8042790CD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-------------------------------------------------------------------------
*
*	Class wrapped for Window object 
* 
*------------------------------------------------------------------------*/
class  KWindow 
{
public:
	KWindow();
	virtual ~KWindow();
	
	BOOL IsValid() const	{	return	NULL != m_hWnd ;	}

	virtual void Close();
	virtual BOOL Create(HWND hParent);
	
	virtual BOOL Send(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL Post(UINT uMsg, WPARAM wParam, LPARAM lParam);

	
	operator HWND()							{	return	this->m_hWnd;	}
	HWND	GetHwnd()						{	return	this->m_hWnd;	}

	virtual BOOL HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	
	virtual BOOL OnMessage(WPARAM wParam, LPARAM lParam);	//	for S_MESSAGEID
protected:
	HWND				m_hWnd;			// Handle of inner window object
};


BOOL IGAWinInit();

#endif