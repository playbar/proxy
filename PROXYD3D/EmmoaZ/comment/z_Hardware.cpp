#include "StdAfx.h"
#include "z_Hardware.h"
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include "HardDisk.h"

ZHardwareInfo::ZHardwareInfo()
{

}
ZHardwareInfo::~ZHardwareInfo()
{

}
VOID    ZHardwareInfo::Load(DWORD   dwType )
{
	START_LOG;
	LOG  << " ZHardwareInfo::Load  Start \r\n"   ;

	if  ( dwType & _hardware_macid ) 
	{		
        GetLocalMAC(m_szMacID,32);	
	}

	if  ( dwType & _hardware_hardid ) 
	{		
        GetLocalHARD(m_szHardID,32);	
	}
	
	if  ( dwType & _hardware_localip ) 
	{		
		GetLocalIP(m_szLocalIP,32);		
	}

	if  ( dwType & _hardware_cpu ) 
	{			
		GetCPUInfo(m_szCPU,127);	
	}

	if  ( dwType & _hardware_system ) 
	{		
		DWORD  dwType = 0;
		GetOperatingSystem(m_szSystem,dwType,127);		
	}
	if  ( dwType & _hardware_dispaly ) 
	{	
		GetDisplayInfo(m_szDisplay,127);		
	}
	if  ( dwType & _hardware_memory ) 
	{		
		MEMORYSTATUS ms			= {0};
		ms.dwLength				= sizeof(ms);
		::GlobalMemoryStatus (&ms);
		m_dwMemorySize	= (DWORD)ms.dwTotalPhys/1024/1024;		
		
	}

	LOG  << " ZHardwareInfo::Load  End \r\n"   ;
	END_LOG;

	return;
}

LPCTSTR ZHardwareInfo::GetOnlyID()
{
	if(m_szHardID[0])
		return m_szHardID;
	return m_szMacID;
}
#include "Iphlpapi.h" 
#pragma comment(lib, "Iphlpapi.lib") 

void ZHardwareInfo::GetLocalMAC(LPSTR buf,DWORD dwSize)
{
 	IP_ADAPTER_INFO *IpAdaptersInfo =NULL;
 	IP_ADAPTER_INFO *IpAdaptersInfoHead =NULL;
	
	IpAdaptersInfo = (IP_ADAPTER_INFO *) GlobalAlloc(GPTR, sizeof(IP_ADAPTER_INFO ));
	
	if (IpAdaptersInfo == NULL)
	{
		return;
	}
	
	DWORD dwDataSize = sizeof( IP_ADAPTER_INFO );
	DWORD dwRetVal = GetAdaptersInfo(IpAdaptersInfo,&dwDataSize);
	
	if ( ERROR_SUCCESS != dwRetVal)
	{
		GlobalFree( IpAdaptersInfo );
		IpAdaptersInfo = NULL;
		
		if( ERROR_BUFFER_OVERFLOW == dwRetVal)
		{
			IpAdaptersInfo =(IP_ADAPTER_INFO *) GlobalAlloc( GPTR, dwDataSize );
			if (IpAdaptersInfo == NULL)
			{
				return;
			}
			if ( ERROR_SUCCESS != GetAdaptersInfo( IpAdaptersInfo, &dwDataSize ))
			{
				GlobalFree( IpAdaptersInfo );
				return;
			}
			
		}
		else
		{
			return;
		}
		
	}
	
	//Save the head pointer of IP_ADAPTER_INFO structures list.
	IpAdaptersInfoHead = IpAdaptersInfo;
	
// 	do{
// 		//if (IsLocalAdapter(IpAdaptersInfo->AdapterName))
// 		{
// 			sprintf(buf,"%02x-%02x-%02x-%02x-%02x-%02x",
// 				IpAdaptersInfo->Address[0],
// 				IpAdaptersInfo->Address[1],
// 				IpAdaptersInfo->Address[2],
// 				IpAdaptersInfo->Address[3],
// 				IpAdaptersInfo->Address[4],
// 				IpAdaptersInfo->Address[5]);
// 			
// 			break;
// 		}
// 		
// 		IpAdaptersInfo = IpAdaptersInfo->Next;
// 		
// 	}while (IpAdaptersInfo);
	

	int   i = 0;
	if  ( NULL != IpAdaptersInfo )
	{
		//while(IpAdaptersInfo->Next)
		//{   
		//	 IpAdaptersInfo = IpAdaptersInfo->Next;			 
		//}    
		
        if  (	IpAdaptersInfo->Address[0] == 0 &&
			    IpAdaptersInfo->Address[1] == 0 &&
			    IpAdaptersInfo->Address[2] == 0 &&
			    IpAdaptersInfo->Address[3] == 0 && 
			    IpAdaptersInfo->Address[4] == 0 && 
				IpAdaptersInfo->Address[5] == 0 )
				sprintf(buf,"09-08-07-06-05-04");
		else
			sprintf(buf,"%02x-%02x-%02x-%02x-%02x-%02x",
					IpAdaptersInfo->Address[0],
					IpAdaptersInfo->Address[1],
					IpAdaptersInfo->Address[2],
					IpAdaptersInfo->Address[3],
					IpAdaptersInfo->Address[4],
					IpAdaptersInfo->Address[5]);
	}
	else
	{
        sprintf(buf,"08-07-06-05-04-03");
	}

	if (IpAdaptersInfoHead != NULL)
	{
		GlobalFree( IpAdaptersInfoHead );
	}
}
void ZHardwareInfo::GetLocalHARD(LPSTR buf,DWORD dwSize)
{
	buf[0]=0;
	CHardDisk hd;
	if(!hd.getHardDriveComputerID(buf))
	{
		OutputDebugString("Can not Get HardDisk ID");
		buf[0]=0;
		return;
	}
	CString strTmp = buf;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	lstrcpy(buf,(char*)(LPCSTR)strTmp);
}
//针对xp有效
// BOOL ZHardwareInfo::IsLocalAdapter(LPSTR pAdapterName)
// {
// 	BOOL ret_value = FALSE;
// 	
// #define NET_CARD_KEY _T("System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}")
// 	char szDataBuf[MAX_PATH+1];
// 	DWORD dwDataLen = MAX_PATH;
// 	DWORD dwType = REG_SZ;
// 	HKEY hNetKey = NULL;
// 	HKEY hLocalNet = NULL;
// 	
// 	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NET_CARD_KEY, 0, KEY_READ, &hNetKey))
// 		return FALSE;
// 	wsprintf(szDataBuf, "%s\\Connection", pAdapterName);
// 	if(ERROR_SUCCESS != RegOpenKeyEx(hNetKey ,szDataBuf ,0 ,KEY_READ, &hLocalNet))
// 	{
// 		RegCloseKey(hNetKey);
// 		return FALSE;
// 	}
// 	if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "MediaSubType", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen))
// 	{
// 		goto ret;
// 	}
// 	if (*((DWORD *)szDataBuf)!=0x01)
// 		goto ret;
// 	dwDataLen = MAX_PATH;
// 	if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "PnpInstanceID", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen))
// 	{
// 		goto ret;
// 	}
// 	if (strncmp(szDataBuf, "PCI", strlen("PCI")))
// 		goto ret;
// 	
// 	ret_value = TRUE;
// 	
// ret:
// 	RegCloseKey(hLocalNet);
// 	RegCloseKey(hNetKey);
// 	
// 	return ret_value;
// 	
// }

BOOL ZHardwareInfo::GetLocalIP(LPSTR pszLocalIP,DWORD dwSize)
{
	BOOL	 bRet			= TRUE;
	PHOSTENT hostinfo; 
	char     szHost[255] = {0}; 
	char*    szBuffer;
	WORD     wVersionRequested;   
	WSADATA  wsaData;     
	wVersionRequested   =   MAKEWORD(1,1);   
	if( WSAStartup( wVersionRequested,&wsaData ) == 0 ) 
	{
		if( gethostname ( szHost, sizeof(szHost)) == 0)  
		{   
			if((hostinfo = gethostbyname(szHost)) != NULL)  
			{    
				szBuffer = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);				
				lstrcpyA(pszLocalIP,szBuffer);

				//取最后一个ip地址								
// 				int  i = 0;
// 				while(hostinfo->h_addr_list[i+1]);				
// 				        
// 				szBuffer = inet_ntoa(*(struct in_addr*)(hostinfo->h_addr_list[i]));				
// 				lstrcpyA(pszLocalIP,szBuffer);

			}   
		}   
	}	
	WSACleanup();
	//	检查一下 IP 信息中的非法字符
	if( bRet )
	{
		CHAR	*p	= pszLocalIP;
		while (*p)
		{
			if ( *p == '\'' || *p == '\\' || *p == '\"' )
				*p	= '.';
			p++;
		}
	}
	
	return bRet;
}


BOOL ZHardwareInfo::GetCPUInfo(LPSTR pszFamily,DWORD dwSize)
{
	HKEY	hKey	= NULL;
	const TCHAR szSubKey[] = _T("Hardware\\Description\\System\\CentralProcessor");  
	if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey, 0, KEY_READ, &hKey  ) )
		return	FALSE;
	
	BOOL	bRet			= TRUE;
	HKEY	hKey2			= NULL;    
	CHAR	szBuffer[512]	= { 0 };
	DWORD	dwLength		= sizeof(szBuffer);
	const TCHAR szSubKey2[] = _T("Hardware\\Description\\System\\CentralProcessor\\0");  
	if ( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKey2, 0, KEY_READ, &hKey2  ))
	{  
		DWORD	dwType			= REG_SZ;

		if ( ERROR_SUCCESS != ::RegQueryValueEx( hKey2, _T("ProcessorNameString"), NULL, (LPDWORD)&dwType,	(LPBYTE)szBuffer , (LPDWORD) &dwLength ) )
		{
			if (ERROR_SUCCESS != ::RegQueryValueEx( hKey, _T("VendorIdentifier"), NULL, (LPDWORD)&dwType,	(LPBYTE)szBuffer ,  (LPDWORD) &dwLength ) )
			{
				ASSERT(FALSE);
				bRet	= FALSE;
			}
		}
		RegCloseKey(hKey2);
	}
	::RegCloseKey(hKey); 
	
	//	检查一下 CPU 信息中的非法字符
	if( bRet )
	{
		CHAR	*p	= szBuffer;
		while (*p)
		{
			if ( *p == '\'' || *p == '\\' || *p == '\"' )
				*p	= '.';
			p++;
		}
	}

	if  ( ::_tcslen(szBuffer ) > 128 )
	{
       memcpy(pszFamily,szBuffer,127)  ;
	   pszFamily[127]    =  0;
	}
	else
	   lstrcpyA(pszFamily, szBuffer);
	pszFamily		+= dwLength;
	//dwSize			+= dwLength;
	return	bRet;
}

BOOL ZHardwareInfo::GetOperatingSystem(LPSTR pszOperatingSystem,DWORD&  dwType, DWORD dwSize)
{
	CHAR	szBuffer[256]	= { 0 };
	OSVERSIONINFOEX osvi	= { 0 };
	DWORD	*pdwType	= &dwType;
	*pdwType	= OS_UNKNOWN;
	wsprintf(pszOperatingSystem,_T("%s"),_T("Unknow"));

	memset(&osvi,0, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	BOOL bOsVersionInfoEx	= FALSE;
	if( !(bOsVersionInfoEx = ::GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! ::GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}

	switch (osvi.dwPlatformId)
	{
		
	case VER_PLATFORM_WIN32_NT:
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
		{
			*pdwType = OS_VISTA;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows vista"));
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			*pdwType	= OS_SVR2003;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows 2003"));
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			*pdwType	= OS_XP;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows XP"));

		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			*pdwType	= OS_2K;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows 2000"));

		}
		else if ( osvi.dwMajorVersion <= 4 )
		{
			*pdwType	= OS_NT;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows nt"));
		}
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			*pdwType	= OS_95;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows 95"));
		}

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			*pdwType	= OS_98;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows 98"));
		}
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			*pdwType	= OS_ME;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Windows me"));
		}
		break;
	case VER_PLATFORM_WIN32s:
		{
		    *pdwType	= OS_W32S;
			wsprintf(pszOperatingSystem,_T("%s"),_T("Unknow"));
		}
		break;
	}

	return TRUE; 
}


/*****************************************************************************************
*
* 函数名称 : ZHardwareInfo::GetDisplayInfo
* 说    明 : 获取显示卡信息
* 返 回 值 : BOOL 
* 参 数 表 : LPSTR pszDisplay
* 参 数 表 : DWORD dwSize
*
* 作    者 : smallfool
* 版    本 : V 1.0 
* 创建日期 : 
*
******************************************************************************************/
BOOL ZHardwareInfo::GetDisplayInfo(LPSTR pszDisplay, DWORD dwSize)
{
	typedef BOOL (WINAPI* FNENUMDISPLAYDEVICES)(LPCTSTR,DWORD,PDISPLAY_DEVICE,DWORD);
	FNENUMDISPLAYDEVICES EnumDisplayDevices;
    DISPLAY_DEVICE DispDev		= {0}; 
	CHAR szSaveDeviceName[32]	= {0};
    BOOL bRet = FALSE;
    
	HMODULE hDll = ::LoadLibrary( _T("User32.dll") );
	if( NULL == hDll )	return FALSE; 
    
    // Get the address of the EnumDisplayDevices function
	EnumDisplayDevices = (FNENUMDISPLAYDEVICES) GetProcAddress(hDll,  _T("EnumDisplayDevicesA" ));
    if (!EnumDisplayDevices) 
	{
        ::FreeLibrary(hDll);
        return FALSE;
	}
	
	memset(&DispDev,0, sizeof(DISPLAY_DEVICE));
    DispDev.cb = sizeof(DISPLAY_DEVICE); 
    
    // After the first call to EnumDisplayDevices, 
    // DispDev.DeviceString is the adapter name
    if (EnumDisplayDevices(NULL, NULL, &DispDev, 0)) 
	{  
		lstrcpyA(szSaveDeviceName, (CHAR *)DispDev.DeviceName);
        
        // After second call, DispDev.DeviceString is the 
        // monitor name for that device 
		if ( EnumDisplayDevices(NULL, NULL, &DispDev, 0) )
		{
			//	检查相应的字符串中，是否含有导致插入数据库失败的非法字符
			lstrcpyA(pszDisplay, (CHAR*)DispDev.DeviceString);
			if  ( ::_tcslen((CHAR*)DispDev.DeviceString ) > 128 )
			{
				memcpy(pszDisplay,(CHAR*)DispDev.DeviceString,127)  ;
				pszDisplay[127]    =  0;
			}


			CHAR	*p	= pszDisplay;
			while (*p)
			{
				if ( *p ==  '\'' || *p == '\\' || *p == '\"' )
					*p	= '.';
				p++;
			}
			bRet	= TRUE;
		}
    } 

	::FreeLibrary(hDll);
	return bRet;
}

DWORD   ZHardwareInfo::GetDWORD_IP    (int  )  
{
	 return ::inet_addr(m_szLocalIP);
}


