
#include "SystemTime.h"
#include <strsafe.h>
#include <assert.h>


CSystemTime::CSystemTime(void)
{
	//GetSystemTime(&m_st);  GetSystemTime返回的是格林威志标准时间
	GetLocalTime(&m_st);  //返回的是你所在地区的时间
}

CSystemTime::~CSystemTime(void)
{
}

bool CSystemTime::GetTodayDate(TCHAR * pTodayDate, int ilen)
{
	assert(NULL != pTodayDate );
	if ( NULL == pTodayDate  )
	{
		return false;
	}
	
	StringCchPrintf(pTodayDate, ilen,TEXT("%04d-%02d-%02d"),m_st.wYear, m_st.wMonth, m_st.wDay);
	return true;
}

bool CSystemTime::GetYesterDayDate(TCHAR * pYesterdayDate, int ilen)
{
	assert(NULL != pYesterdayDate );
	if ( NULL == pYesterdayDate  )
	{
		return false;
	}
	int iYear =0,iMonth = 0, iDay = 0; //为存储时间定义整型变量 
	iYear=m_st.wYear;    //分别提取时、分、秒 mm=gt.GetMinute(); 
	iMonth=m_st.wMonth; 
	iDay= m_st.wDay;
	//strTime.Format("%02d:%02d:%02d",hh,mm,ss);
	iDay = iDay - 1;
	if (iDay == 0) 
	{
		iMonth--;
		if (iMonth == 0) {
			iYear--;
			iMonth = 12;
			iDay = 31;
		} else {
			if (iMonth == 1 || iMonth == 3 || iMonth == 5 || iMonth == 7
					|| iMonth == 8 || iMonth == 10 || iMonth == 12) {
				iDay = 31;
			} else {
				if (iMonth == 2) {
					if (iYear % 4 == 0) {
						iDay = 29;
					} else {
						iDay = 28;
					}
				} else {
					iDay = 30;
				}
			}
		}
	}
	StringCchPrintf(pYesterdayDate, ilen,TEXT("%04d-%02d-%02d") ,iYear, iMonth, iDay);
	return true;
}


