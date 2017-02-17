#pragma once
//日期处理类，提供日期格式转换，得到昨天日期，前天日期等功能

#include <Windows.h>

class CSystemTime
{
public:
	CSystemTime(void);
	~CSystemTime(void);
	
	// 获取当天的日期
	bool GetTodayDate(TCHAR * pTodayDate, int ilen);
	// 获取昨天的日期
	bool GetYesterDayDate(TCHAR * pYesterdayDate, int ilen);
	
private:
	SYSTEMTIME m_st;
};
