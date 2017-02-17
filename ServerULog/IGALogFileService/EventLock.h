// Lock.h: interface for the CLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCK_H__83CB6D79_C4F0_45A4_B0D5_F6021448869A__INCLUDED_)
#define AFX_LOCK_H__83CB6D79_C4F0_45A4_B0D5_F6021448869A__INCLUDED_
#pragma message("********Lock")
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEventLock  
{
public:
	VOID Lock()
	{
		::WaitForSingleObject(m_hEvent,INFINITE);
	}
	
	VOID UnLock()
	{
		::SetEvent(m_hEvent);	
	}

	CEventLock(LPCTSTR   szName);
	 ~CEventLock();
private:
	HANDLE		m_hEvent;
};

#endif // !defined(AFX_LOCK_H__83CB6D79_C4F0_45A4_B0D5_F6021448869A__INCLUDED_)
