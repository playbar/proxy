// GarbageCollector.cpp: implementation of the CGarbageCollector class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "GarbageCollector.h"
#include ".\garbagecollector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGarbageCollector::CGarbageCollector()
{
	m_autoPtr.RemoveAll();
	m_autoTempBuffer.RemoveAll();
}

CGarbageCollector::~CGarbageCollector()
{
	try
	{
		int index = 0;
		for ( index =0;index<m_autoPtr.GetSize();index++)
		{
			void *ptr = m_autoPtr[index];
			SAFE_DELETE(ptr);
		}
		for (index = 0;index<m_autoTempBuffer.GetSize();index++)
		{
			void *TempBuffer = m_autoTempBuffer[index];
			SAFE_DELETE_ARRAY(TempBuffer);
		}
		m_autoPtr.RemoveAll();
		m_autoTempBuffer.RemoveAll();
	}
	catch (...)
	{
		
	}
	
}
void CGarbageCollector::AddPtr(void* ptr)
{
	try
	{
		m_autoPtr.Add(ptr);
	}
	catch (...)
	{
		
	}
	
}
void CGarbageCollector::	AddTempBuffer(void* TempBuffer)
{
	try
	{
		m_autoTempBuffer.Add(TempBuffer);
	}
	catch (...)
	{
		
	}
	
}
BOOL CGarbageCollector::RemoveAll(void)
{
	BOOL bRes=FALSE;
	try
	{
		int index = 0;
		for ( index =0;index<m_autoPtr.GetSize();index++)
		{
			void *ptr = m_autoPtr[index];
			SAFE_DELETE(ptr);
		}
		for (index = 0;index<m_autoTempBuffer.GetSize();index++)
		{
			void *TempBuffer = m_autoTempBuffer[index];
			SAFE_DELETE_ARRAY(TempBuffer);
		}
		m_autoPtr.RemoveAll();
		m_autoTempBuffer.RemoveAll();
		bRes=TRUE;
	}
	catch (...)
	{
		bRes=FALSE;
	}

	return bRes;
}
