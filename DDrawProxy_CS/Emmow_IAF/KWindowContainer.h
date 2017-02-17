#if !defined(AFX_KContainerCtl_H__829BA231_6C3A_4C25_B4C0_14E69D9C6835__INCLUDED_)
#define AFX_KContainerCtl_H__829BA231_6C3A_4C25_B4C0_14E69D9C6835__INCLUDED_

#pragma once

#include "StdAfx.h"
#include "Window.h"

#pragma warning(disable : 4099 4192)
#include <map>
#include <list>


enum 
{
	K_WINDOW_FLASH = 0,
	K_WINDOW_VIDEO = 1,
	K_WINDOW_IE    = 2
};
class KContainerCtl  :public KWindow
{
	enum { BTN_CLOSE_WIDTH = 10 ,
	       BTN_CLOSE_HIGHT = 10 };
public:
	typedef std::list< KContainerCtl* >              Container_VEC;
    typedef std::list< KContainerCtl* >::iterator    Container_VEC_IT;
	
	/*父窗体下的子窗体列链表*/
	typedef struct
	{
		Container_VEC    ContainerArr;	
		WNDPROC          pFunProc;

	} CHIELD_VEC;   

	
	//父窗口句柄与子窗口链表的map
	typedef std::map< HWND,CHIELD_VEC >              PARENT_MAP;
    typedef std::map< HWND,CHIELD_VEC >::iterator    PARENT_MAP_IT;
	typedef std::map< HWND,CHIELD_VEC >::value_type  PARENT_MAP_VALUE;

public:
	KContainerCtl();
	virtual ~KContainerCtl();

    //创建函数
	virtual BOOL Create(HWND hParent);

	//消息执行函数
    virtual BOOL HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	virtual BOOL OnSize();

	/*设置α值 */
	BOOL    SetAlpah(BYTE  nAlpah);

	//显示窗体
	BOOL    ShowWindow();
    BOOL    ShowWindow(int nLeft,int nTop,int nRight,int nBottom);
	
	//销毁窗体
	BOOL    DestroyWindow();	


public:
	//取窗体类型
	inline DWORD   GetWindowType()    { return  m_dwWindwoType; }
	
private:
	VOID    HookParent   (HWND hParent); 
    VOID    UnHookParent (HWND hParent);
public:
    // 父窗体函数指针
	WNDPROC      m_pFunParentCallProc;
protected:   
	
	//控件容器
    CAxWindow    m_container;     

	//整个窗体位置坐标
	int          m_nLeft;
	int          m_nTop;
	
	int          m_nRight;
	int          m_nBottom;

	//父窗体句柄vin
	HWND         m_hParent;

	//关闭按钮句柄
    HWND         m_hBtnClose;

    
	//窗体类型
	DWORD        m_dwWindwoType;

public:
    static  PARENT_MAP    s_ParentContainer; 	
    /*动态申请的Container窗机*/
	static std::map< KContainerCtl* , BOOL  >   s_DynamicContainer;

public:

	template <class T>
	static KContainerCtl* New(T*& pReturn)
	{
		pReturn = new T;
		
		KContainerCtl*   pWnd = pReturn;
		s_DynamicContainer.insert(std::map< KContainerCtl* , BOOL  >::value_type(pWnd,TRUE));
		
		
		return   pWnd;
	}

	
	static  VOID           Delete(KContainerCtl*);
};

#endif // !defined(AFX_KIGAWINDOW_H__829BA231_6C3A_4C25_B4C0_14E69D9C6835__INCLUDED_)

