
#pragma  once

#define PATH_WORK "InterAdtive\\EMMOA\\3D\\"
#define _IGA_DATA _T("F43436CA-F337-410a-8BDB-F470040A81C2")

#define D3DRM_XTEMPLATE_BYTES 3278
#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }


/*
定义是工具还是客户端
0:表示是客户端，将不具有坐标寻找、重新加载等手动功能；同时也不会输出任何信息。
1:表示是工具模式，可以进行坐标寻找、测试3D模型，还会输出相关的信息。
*/
#define IS_3D_TOOL	1

//button状态
enum BUTTON_MOUSE_FLAG
{
	BUTTON_MOUSE_NORMAL = 0,
	BUTTON_MOUSE_ON,
	BUTTON_MOUSE_DOWN,
};

enum GAMEUI_TEXTURE_ID
{
	GAMEUI_TEX_ITEM_STATIC_BACK,
	GAMEUI_TEX_ITEM_STATIC_0,
	GAMEUI_TEX_ITEM_STATIC_1,
	GAMEUI_TEX_ITEM_STATIC_2,
	GAMEUI_TEX_ITEM_STATIC_3,
	GAMEUI_TEX_ITEM_STATIC_4,
	GAMEUI_TEX_ITEM_STATIC_5,
	GAMEUI_TEX_ITEM_STATIC_6,

	GAMEUI_TEX_PLAYER_STATIC_BACK,
	GAMEUI_TEX_PLAYER_STATIC_PLAYER,
	GAMEUI_TEX_PLAYER_STATIC_HP,
	GAMEUI_TEX_PLAYER_STATIC_MP,

	GAMEUI_TEX_SYSTEM_BACK,

	GAMEUI_TEX_SYSTEM_BUTTON_NORMAL,
	GAMEUI_TEX_SYSTEM_BUTTON_DOWN,
	GAMEUI_TEX_SYSTEM_BUTTON_ON,

	GAMEUI_TEX_ITEM_BUTTON_NORMAL,
	GAMEUI_TEX_ITEM_BUTTON_DOWN,
	GAMEUI_TEX_ITEM_BUTTON_ON,

	GAMEUI_TEX_SHOP_STATIC_BACK,

	GAMEUI_TEX_FLX_STATIC_ITEM,

	GAMEUI_TEX_SKILL_STATIC_BACK,
	GAMEUI_TEX_SKILL_STATIC_ALPHASKILL,

	GAMEUI_TEX_FLX_STATIC_LOAD,
	GAMEUI_TEX_FLX_STATIC_LOADING,

	GAMEUI_TEX_SKILL_STATIC_1,
	GAMEUI_TEX_SKILLBOOKUI_STATIC_BACK,
	GAMEUI_TEX_MOUSEMESSAGE_STATIC_BACK,

	GAMEUI_TEX_SKILL_STATIC_2,

	GAMEUI_TEX_MINIMAP_STATIC_BACK,
	GAMEUI_TEX_MINIMAP_STATIC_MAP,
	GAMEUI_TEX_MINIMAP_STATIC_ROLE,

	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_NORMAL,
	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_DOWN,
	GAMEUI_TEX_ITEM_BUTTON_ITEMBUTTON_ON,

	GAMEUI_TEX_SKILL_STATIC_BUTTONRECT,

	GAMEUI_TEX_SKILL_BUTTON_FIXDOWN,
	GAMEUI_TEX_SKILL_BUTTON_NULLDOWN,
	GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKDOWN,
	GAMEUI_TEX_SKILL_BUTTON_SKILLBOOKNORMAL,


	MAX_GAMEUI_TEX_NUM,
};


//UI消息     
struct UI_MSG
{
	DWORD dwDialogId;
	DWORD dwControlId;
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
};

enum IGA3D_MSG_ENUM
{
	IGA3D_MSG_UNKNOW = -1,
	IGA3D_MSG_LBUTTONDOWN,
	IGA3D_MSG_LBUTTONUP,
	IGA3D_MSG_RBUTTONDOWN,
	IGA3D_MSG_RBUTTONUP,

};

struct SIGA3DMSG
{
	bool	bIsProc;	//是否被处理过
	DWORD	dwType;		//消息的类型
	int     iKey;	    //键值
	int		ixPoint;		//鼠标的位置
	int		iyPoint;

	SIGA3DMSG()
	{
		bIsProc = false;
		dwType	= IGA3D_MSG_UNKNOW;
		iKey		= IGA3D_MSG_UNKNOW;
		ixPoint		= 0;
		iyPoint       = 0;
	}

	SIGA3DMSG( const SIGA3DMSG& obj )
	{
		bIsProc = obj.bIsProc;
		dwType	= obj.dwType;
		iKey		= obj.iKey;
		ixPoint		= obj.ixPoint;
		iyPoint       = obj.iyPoint;
	}
	~SIGA3DMSG() {}
};

//-----------------------------------------------------
//
struct SUIElement
{
	DWORD	m_dwID;
	int		m_iTexID;	//使用的纹理ID
	RECT	m_rect;

	SUIElement*  pFirstChildNode; //子节点
	SUIElement*  pSiblingNode;    //兄弟节点

	SUIElement()
	{
		m_dwID		= 0;
		m_iTexID	= 0;
		m_rect.top		= 0;
		m_rect.left		= 0;
		m_rect.right		= 30;
		m_rect.bottom		= 50;

		pFirstChildNode = NULL;
		pSiblingNode	= NULL;
	}

};


struct _S3DXML
{
	int		id;
	int		Close;
};


struct STDrawElement
{
	int		m_iObjID;	 //对象id
	bool	m_bIsShow;
	int		m_iMeshID;  //模型ID
	int		scene_id;   //场景ID
	int		tex_Num;	//纹理数量
	int		tex_Index;  //纹理索引
	int		tex[100];	//播放列表
	int		tex_id;
	//2008-10-13
	//网格组，对于比较复杂模型，使用网格组的形式进行描述
	int     mesh_sub_num;
	int     mesh_array[100];

	float	x, y, z;
	float	s_xyz;
	float	r_x, r_y, r_z;
	char	szURL[MAX_PATH];
	UINT	ShowTime;		//总共显示的时间
	UINT	StartShowTime;	//开始显示的时间
	bool    bStartShow;		//是否开始显示
	UINT	PickCount;		//点击的次数

	STDrawElement()
	{
		m_iObjID  = -1;
		m_iMeshID = -1;
		scene_id = -1;
		tex_id   = -1;

		m_bIsShow = false;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szURL[0] = '\0';

		tex_Num		= 0;
		tex_Index	= 0;
		tex[0]		= -1;

		mesh_sub_num = 0;
		mesh_array[0] = -1;

		ShowTime		= 0;		
		StartShowTime	= 0;	
		bStartShow		= false;
		PickCount		= 0;

	}

	STDrawElement( const STDrawElement& obj )
	{
		m_iObjID = obj.m_iObjID;

		m_iMeshID = obj.m_iMeshID;
		scene_id = obj.scene_id;
		tex_id   = obj.tex_id;

		m_bIsShow = obj.m_bIsShow;

		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;

		strcpy( szURL, obj.szURL );

		tex_Num		= obj.tex_Num;
		tex_Index	= obj.tex_Index;
		for( int i = 0; i < tex_Num; i++ )
		{
			tex[i]		= obj.tex[i];
		}

		mesh_sub_num = obj.mesh_sub_num;
		for( int i = 0; i < mesh_sub_num; i++ )
		{
			mesh_array[i] = obj.mesh_array[i];
		}

		ShowTime		= obj.ShowTime;		
		StartShowTime	= obj.StartShowTime;	
		bStartShow		= obj.bStartShow;
		PickCount		= obj.PickCount;
	}

	void Clear()
	{
		m_iObjID = -1;

		m_iMeshID = -1;
		scene_id = -1;
		tex_id   = -1;

		m_bIsShow = false;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		szURL[0] = '\0';

		tex_Num		= 0;
		tex_Index	= 0;
		tex[0]		= -1;

		mesh_sub_num = 0;
		mesh_array[0] = -1;

		ShowTime		= 0;		
		StartShowTime	= 0;	
		bStartShow		= false;
		PickCount		= 0;
	}
};

//用于保存的结构体
struct STShowInfoHead
{
	UINT	year;
	char	month;
	char	day;

	char	hour;
	char	minute;
	char	sec;

	UINT	number;	//文件中记录的数量
	struct ShowInfo
	{
		UINT	id;
		UINT	PickCount;
		float	ShowTime;
	};
};

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
	IGA_PAOPAOKADINGCHE,		//跑跑卡丁车
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
	IGA_XINYANG,				//信仰--百年战争	
	IGA_ZHANHUO,				//战火	
	IGA_JIETOULANQIU,		//街头篮球	
	IGA_TIANTANG2,			//天堂2
	IGA_SUN,					//奇迹世界
	IGA_TIANZUN,				//天尊OnLine
	IGA_WLOL,				//武林群侠传 
	IGA_GV,					//大航海时代Online
	IGA_RELIPAIQIU,			//热力排球	

	IGA_WUJIEQU_MLXY	=155,	//舞街区(马来西亚)
	IGA_WUJIEQU_XJP,				//舞街区(新加坡)
	IGA_WUJIEQU_FLB = 158,				//舞街区(菲律宾)
};

