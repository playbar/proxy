// kEventLock.cpp: implementation of the CLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EventLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventLock::CEventLock(LPCTSTR   szName)
{
	m_hEvent = ::CreateEvent(NULL,TRUE,TRUE,szName);
			
}

CEventLock::~CEventLock()
{
	if (m_hEvent!=NULL)
	{
		::CloseHandle(m_hEvent);	
		m_hEvent	= NULL;
	}
}

