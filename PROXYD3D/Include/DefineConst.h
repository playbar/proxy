/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : defineCon.h
*	工程使用常量定义，包括字符串常量，枚举常量, add by hgl
*	修改SHOWOUTPUTDEBUG宏定义，用于调试版本中显示游戏执行的位置
*	当前版本：2, 0, 9, 0304
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1226
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once

#ifndef _DEFINECONST_H_
#define _DEFINECONST_H_


#include "stdio.h"

#define ADINFO_EMMOA_3D (TEXT("InterAdtive\\EMMOA\\3D\\"))
#define	ADINFO_EMMOA_RES_FLASH	"InterAdtive\\EMMOA\\res\\flash\\" 

#define _IGA_DATA (TEXT("F43436CA-F337-410a-8BDB-F470040A81C2"))

const float EPSINON = 0.000001f;  //浮点数比较值 
#define FLOAT_EQAUL_ZERO(fdate)  (((fdate) >= - EPSINON) && ((fdate) <= EPSINON))

#define D3DRM_XTEMPLATE_BYTES   3278
#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }

#ifndef SAFE_DELETE
#define SAFE_DELETE( T ) {if( T ) { delete T; T = NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( T ) {if( T ) { delete[] T; T = NULL; }}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( T ) if( T ) { T->Release(); T = NULL; }
#endif


#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

//////////////////////////////////////////////////////////////////////////

// Put up an assertion failure message box.
inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
{
	char szOutputString[ sizeof(char) *MAX_PATH];
	memset(szOutputString, 0, sizeof(char)  * MAX_PATH );
	SYSTEMTIME st;
	GetLocalTime( &st );
	char szDate[100];
	sprintf( szDate, "%04d-%02d-%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	sprintf(szOutputString, "%s,  File %s, line %d ,Date:%s ",expr, file, line, szDate );
	OutputDebugStringA(szOutputString);
}


//在debug下显示输出信息宏,使用前要先定义存放字符串的数组
# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# else
#	define	SHOWOUTPUTDEBUG(szOutputString)  InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# endif
/*
定义是工具还是客户端
0:表示是客户端，将不具有坐标寻找、重新加载等手动功能；同时也不会输出任何信息。
1:表示是工具模式，可以进行坐标寻找、测试3D模型，还会输出相关的信息。
*/
#define IS_3D_TOOL	0


# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	PROTECT(s)		{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
#	define	UNPROTECT()		}
# else
#	define	PROTECT(s)		{			\
							try			\
							{			\
								static	TCHAR *__FUNC_NAME__= TEXT(#s);	
								
#	define	UNPROTECT()		}			\
							catch(...)	\
							{			\
							}			\
						}
# endif


enum 
{
	b_ESC, b_F1, b_F2, b_F3, b_F4, b_F5, b_F6, b_F7, b_F8, b_F9, b_F10, b_F11, b_F12, //功能键
	b_L, b_W, b_H, b_S, b_R, b_T, b_M, b_A, b_N,//字母键
	b_UP, b_DOWN, b_LEFT, b_RIGHT, //方向键
	b_LBUTTON, b_RBUTTON, //鼠标左键
	b_CTRL, b_SHIFT, //CTRL、SHIFT键
	b_ADD, b_SUB, //“+”、“-”键
	b_DELETE, //DELETE键
	b_MAX, b_MIN, //“,”、“.”或“<”、“>”键
};

enum GAME_STATE
{
	STATE_START,	//开始
	STATE_IN_SCENE, //进入了场景
	STATE_AFTER_DLG,	//玩家选择了一个场景入口，弹出IGA对话框,表示玩家看过IGA活动说明
	STATE_CHOICE_IGA,	//玩家选择了IGA活动
	STATE_IGA_START,	//IGA活动开始
	STATE_IGA_COMPLETE, //完成IGA活动
	STATE_IGA_END 		//退出IGA活动，可能的情况是完成后退出、中途退出、一开始就退出
};

enum IGA_STATE
{
	IGA_START,	//IGA任务开始
	IGA_END,	//IGA任务结束
	IGA_1,	//完成第一个任务
	IGA_2,	//
	IGA_3,
	IGA_OK,	//完成全部任务
	IGA_DESIGN //设计模式
};

enum RenderPos
{
	Per_SetTransform,	//设置矩阵前
	Post_SetTransform,	//设置矩阵后
	Per_Present,		//翻转前
	Post_Present		//翻转后
};

enum ENUM_IGA_GAME_LIST
{
	IGA_NO_GAME = 0,
	IGA_FENGYUN,			//风云
	IGA_GONGFUSHIJIE,		//功夫世界
	IGA_GUANGZHIGUODU,		//光之国度
	IGA_SHENQI,				//神泣
	IGA_TEZHONGBUDUI,		//特种部队
	IGA_HANGHAISHIJI,		//航海世纪
	IGA_JIJIASHIJI,			//机甲世纪
	IGA_WUJIEQU	,			//舞街区
	IGA_HANGHAISHIJI_EX	,	//航海世纪革新版
	IGA_JINGTIANDONGDI,		//惊天动地
	IGA_LONGHUMEN,			//龙虎门
	IGA_HAIZEIWANG	,		//海贼王
	IGA_TIANSHANGRENJIAN,	//天上人间
	IGA_JUREN,				//巨人
	IGA_KANGZHAN,			//抗战
	IGA_LUOQI,				//洛奇
	IGA_PAOPAOKADINGCHE,	//跑跑卡丁车
	IGA_REXUEJIANGHU,		//热血江湖
	IGA_SILUCHUANSHUO,		//丝路传说
	IGA_TIANJI,				//天机
	IGA_TIANLONGBABU,		//天龙八部  
	IGA_WANMEISHIJIE,		//完美世界	
	IGA_ZHUXIAN	,			//诛仙	
	IGA_WEIWUDUZUN,			//唯舞独尊	
	IGA_ZHUOYUEZHIJIAN,		//卓越之剑	
	IGA_CHUNQIU_Q_ZHUAN, 	//春秋Q传	
	IGA_FENGHUOZHILV,		//风火之旅	
	IGA_MENGJIANG,			//猛将	
	IGA_ZHENGFU	,			//征服
	IGA_MOYU,				//征服
	IGA_XINYANG,			//信仰--百年战争	
	IGA_ZHANHUO,			//战火	
	IGA_JIETOULANQIU,		//街头篮球	
	IGA_TIANTANG2,			//天堂2
	IGA_SUN,				//奇迹世界
	IGA_TIANZUN,			//天尊OnLine
	IGA_WLOL,				//武林群侠传 
	IGA_GV,					//大航海时代Online
	IGA_RELIPAIQIU,			//热力排球	

	IGA_XIANJIAN=41,		//仙剑
	IGA_YINGTAOCHENG = 42, // 樱桃城   
	IGA_FENGYICHUANSHUO = 43,	//封印传说
	IGA_SHUSHAN = 44,			//蜀山
	IGA_ZHENSANGUO = 45,		//真三国
	IGA_MOLINGONLINE= 46,		//魔灵Online
	IGA_QICHENG = 47,			//启程

	IGA_WUJIEQU_MLXY=155,	//舞街区(马来西亚)
	IGA_WUJIEQU_XJP,		//舞街区(新加坡)
	IGA_WUJIEQU_FLB = 158,	//舞街区(菲律宾)

	IGA_TEST = 1000,		//测试游戏使用ID
};


#endif

