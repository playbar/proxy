#ifndef _ZHARDWARE_H
#define _ZHARDWARE_H

typedef enum eOSTYPE
{
	OS_VISTA    = 0 ,		//	VISTA或更高的系统	
	OS_SVR2003,		//	Windows 2003
	OS_XP,			//	XP操作
	OS_2K,
	OS_NT,
	OS_98,
	OS_95,
	OS_ME,
	OS_W32S,
	OS_UNKNOWN
}OSTYPE;



class  ZHardwareInfo
{
	enum
	{
		_hardware_macid   =  0x00000001 ,
		_hardware_localip =  0x00000002 ,
		_hardware_cpu     =  0x00000004 ,
		_hardware_system  =  0x00000008 ,

		_hardware_dispaly =  0x00000010 ,
		_hardware_memory  =  0x00000020 , 
		_hardware_hardid  =  0x00000040 , 
		_hardware_all     =  0x0000007F
	};
public:
	ZHardwareInfo();
	~ZHardwareInfo();
	
	VOID    Load(DWORD   dwType = _hardware_all);

	DWORD   GetDWORD_IP    (int  )  ; 

    inline LPCTSTR GetMacID          (int  )  { return m_szMacID;      }
	inline LPCTSTR GetIP             (int  )  { return m_szLocalIP;    }

	inline LPCTSTR GetCPU            ()       { return m_szCPU;        }
	inline LPCTSTR GetOperatorSystem ()       { return m_szSystem;     }
	inline LPCTSTR GetDisplay        ()       { return m_szDisplay;    }
	inline LPCTSTR GetHardID        ()       { return m_szHardID;    }
	inline DWORD   GetMemorySize     ()       { return m_dwMemorySize; }
	

public:	
	static void GetLocalMAC(LPSTR buf,DWORD dwSize);
	static void GetLocalHARD(LPSTR buf,DWORD dwSize);
	static BOOL GetLocalIP(LPSTR pszLocalIP,DWORD dwSize);	
	static BOOL GetCPUInfo(LPSTR pszFamily, DWORD dwSize);
	static BOOL GetOperatingSystem(LPSTR pszOperatingSystem,DWORD&  dwType ,DWORD dwSize);
	static BOOL GetDisplayInfo(LPSTR pszDisplay, DWORD dwSize);
	LPCTSTR GetOnlyID();
private:
	DWORD	m_dwMemorySize;	

	TCHAR	m_szMacID  [  32  ];
	TCHAR   m_szLocalIP[  32  ];

	TCHAR	m_szCPU    [  128 ];
	TCHAR	m_szDisplay[  128 ];
	TCHAR   m_szSystem [  128 ];	
	TCHAR	m_szHardID [  32  ];
};

#endif