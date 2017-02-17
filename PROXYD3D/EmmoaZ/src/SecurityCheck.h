/********************************************************************************************************************
 *  2008-09-02  安全检测类 仅扫描进程标题名称
 *********************************************************************************************************************/
#if !defined(AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_)
#define AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_


class SecurityCheck  
{
public:
	SecurityCheck();
	~SecurityCheck();

public:	
	BOOL    Start ();
	void    Stop  ();

	BOOL    IsSecurity()  ;   //当前是否安全的
public:	
    static SecurityCheck* CreateInstance();
private:
    static DWORD WINAPI ThreadProc(void *p);
	
protected:
	HANDLE    m_hWorkingThread;       //工作线程句柄
	DWORD     m_dwWorkingThreadID;    //工作线程id    
	HANDLE    m_hEventStop;           //停止线程事件

	DWORD     m_dwTimeOut;
};

#endif // !defined(AFX_SECURITYCHECK_H__FD88C52C_AC2D_48BF_8922_BD1B2E8EE9FC__INCLUDED_)
