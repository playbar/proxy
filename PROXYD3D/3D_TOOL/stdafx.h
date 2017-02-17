// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0410 //为 Windows Me 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0400	//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS


#pragma warning( disable : 4996 )

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdhtml.h>

#include "z_FileIni.h"
#include "z_Memory.h"
#define _IGA_DATA _T("F43436CA-F337-410a-8BDB-F470040A81C2")
struct _SIGASTATEDATA
{
	float x;
	float y;
	float z;

	bool bIsReload;

	UINT nPickObjID;
	
	BOOL bIsShowPickMesh;

	_SIGASTATEDATA()
	{
		x = 0;
		y = 0;
		z = 0;
		bIsReload = false;
		nPickObjID = -1;
		bIsShowPickMesh = false;
	}

};

typedef CShareMemory<_SIGASTATEDATA,1> IGA_DATA_MEM;



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
	IGA_CHAOJIWUZHE,			//超级舞者

	IGA_XIANJIAN=41,		//仙剑
	IGA_YINGTAOCHENG = 42, // 樱桃城   
	IGA_FENGYICHUANSHUO = 43,	//封印传说
	IGA_SHUSHAN = 44,			//蜀山

	IGA_WUJIEQU_MLXY=155,	//舞街区(马来西亚)
	IGA_WUJIEQU_XJP,		//舞街区(新加坡)
	IGA_WUJIEQU_FLB = 158,	//舞街区(菲律宾)

	IGA_TEST = 1000,		//测试游戏使用ID
};

