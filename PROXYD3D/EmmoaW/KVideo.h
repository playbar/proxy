// KVideo.h: interface for the KVideo class.
//
//************************************************************************************************************
// class      :  
// purpose    :  ÏÔÊ¾ÊÓÆµ
// Date       :  2008-7-2  13:37:02
// author     :  3o3afe1
//************************************************************************************************************

#if !defined(AFX_KVIDEO_H__60C5E7A4_63F7_41FB_A44B_18AF035B7A6F__INCLUDED_)
#define AFX_KVIDEO_H__60C5E7A4_63F7_41FB_A44B_18AF035B7A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "wmp.tlh"
#include "KWindowContainer.h"


class KVideo  : public KContainerCtl
{
public:
	KVideo();
	virtual ~KVideo();

	BOOL   Create(HWND hParent);
	BOOL   Play(LPCTSTR lpszName,BOOL bLoopPaly);
	VOID   Stop();
private:

	CComQIPtr< IGAMEDIA::IWMPPlayer4>   IMediaPalyer;

};

#endif // !defined(AFX_KVIDEO_H__60C5E7A4_63F7_41FB_A44B_18AF035B7A6F__INCLUDED_)
