/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : defineST.h
*	工程使用的结构体定义, add by hgl
*	删除多余的结构定义
*	当前版本：2, 0, 9, 0304
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/
#pragma  once

#include "IGATexture9.h"


enum IGA3D_MSG_ENUM
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};


typedef struct tagD3DSearchParam
{
	typedef VOID(WINAPI *LPCallBack3DFocus)(DWORD,LPVOID);
	union
	{
		DWORD	dwAdID;
		DWORD	dwEventID;
	};

	DWORD	dwGameID;
	int		nSearchType;

	LPCallBack3DFocus pFun3DFocus;	//	统计点击次数的回调函数
	HWND	hWndFocus;
	HWND	hWndDeviceFullscreen;
	HWND	hWndDeviceWindowed;

	WPARAM	wParam;
	LPARAM	lParam;

}D3DSEARCH_PARAM, *LPD3DSEARCH_PARAM;



struct STIGAStateData
{
	float fx;
	float fy;
	float fz;
	bool bIsReload;
	UINT nPickObjID;
	BOOL bIsShowPickMesh;

	STIGAStateData()
	{
		fx = 0;
		fy = 0;
		fz = 0;
		bIsReload = false;
		nPickObjID = 0;
		bIsShowPickMesh = false;
	}

};



struct _S3DXML
{
	int		id;
	int		Close;
};

struct STDrawElement
{
	int		iObjID;				//对象id
	int		iMeshID;			//模型ID
	int		iSceneID;			//场景ID
	int		iTextureNum;		//纹理数量
	int		iTextureIndex;		//纹理索引
	int		iArrTexture[100];	//播放列表
	int		iTextureID;
	//2008-10-13
	//网格组，对于比较复杂模型，使用网格组的形式进行描述
	int     iMeshSubNum;
	int     iArrMesh[100];

	//显示时间
	long	lStart;
	bool	bIsShow;
	float	x;
	float   y;
	float	z;
	float	s_xyz;
	float	r_x;
	float	r_y;
	float	r_z;

	char	szUrl[MAX_PATH];

	UINT	nTotalShowTime;	//总共显示的时间
	UINT	nStartShowTime;	//开始显示的时间
	bool    bStartShow;		//是否开始显示
	UINT	nPickCount;		//点击的次数

	//一个模型对应一个纹理对象，add by hgl
	bool bIsClip;							// 是否裁剪 , false 代表没有被裁剪， ture是被裁剪
	bool bIsPlay;							//是否正在播放，false 没有播放或暂停，ture是正在播放
	char szTextureFilePath[MAX_PATH];		//纹理文件的全路径
	CD3D9ManageTexture  st_D3D9ManageTexture; //纹理对象管理类，由IGATexture9.dll导入

	STDrawElement()
	{
		memset(szUrl, 0, MAX_PATH );
		memset(szTextureFilePath, 0, MAX_PATH );
		lStart  = 0L;
		bIsShow = false;
		bIsPlay = false;
		bIsClip = false;

		iObjID  = -1;
		iMeshID = -1;
		iSceneID = -1;
		iTextureID   = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		iTextureNum		= 0;
		iTextureIndex	= 0;
		iArrTexture[0]		= -1;

		iMeshSubNum = 0;
		iArrMesh[0] = -1;

		nTotalShowTime		= 0;		
		nStartShowTime	= 0;	
		bStartShow		= false;
		nPickCount		= 0;

	}

	STDrawElement( const STDrawElement& obj )
	{
		strcpy( szUrl, obj.szUrl );
		strcpy( szTextureFilePath, obj.szTextureFilePath );

		lStart  = obj.lStart;
		bIsShow = obj.bIsShow;
		iObjID  = obj.iObjID;
		iMeshID = obj.iMeshID;
		iSceneID = obj.iSceneID;
		iTextureID   = obj.iTextureID;
		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;
		//------------------------------------
		iTextureNum		= obj.iTextureNum;
		iTextureIndex	= obj.iTextureIndex;
		for( int i = 0; i < iTextureNum; i++ )
		{
			iArrTexture[i]		= obj.iArrTexture[i];
		}
		//--------------------------------------
		iMeshSubNum = obj.iMeshSubNum;
		for( int i = 0; i < iMeshSubNum; i++ )
		{
			iArrMesh[i] = obj.iArrMesh[i];
		}
		nTotalShowTime		= obj.nTotalShowTime;		
		nStartShowTime	= obj.nStartShowTime;	
		bStartShow		= obj.bStartShow;
		nPickCount		= obj.nPickCount;

	}

	void Clear()
	{
		iMeshID = -1;
		iSceneID = -1;
		iTextureID   = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szUrl[0] = '\0';
		szTextureFilePath[0] = '\0';

		iTextureNum		= 0;
		iTextureIndex	= 0;
		iArrTexture[0]		= -1;

		iMeshSubNum = 0;
		iArrMesh[0] = -1;

		nTotalShowTime		= 0;		
		nStartShowTime	= 0;	
		bStartShow		= false;
		nPickCount		= 0;
	}
};


struct STIGA3DMSG
{
	bool	bIsProc;	//是否被处理过
	DWORD	dwType;		//消息的类型
	int     iKey;	    //键值
	int		ixPosition;		//鼠标的位置
	int		iyPosition;

	STIGA3DMSG()
	{
		bIsProc = false;
		dwType	= IGA3D_MSG_UNKNOW;
		iKey		= IGA3D_MSG_UNKNOW;
		ixPosition		= 0;
		iyPosition      = 0;
	}

	STIGA3DMSG( const STIGA3DMSG& obj )
	{
		bIsProc		= obj.bIsProc;
		dwType		= obj.dwType;
		iKey		= obj.iKey;
		ixPosition	= obj.ixPosition;
		iyPosition  = obj.iyPosition;
	}

	~STIGA3DMSG()
	{}
};

