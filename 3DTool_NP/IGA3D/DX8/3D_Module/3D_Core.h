#include "d3d8.h"
#include "d3dx8.h"
#include <list>
using namespace std;

#include "luaScript.h"
#include "MeshLibrary.h"
#include "TexLibrary.h"

#include "IGA3D_UI.h"
#include "shellapi.h"

//#include "Font.h"

//回调函数
typedef void(*LPIGAFUN)(int id);

class IGA3DModule;
//初始化3D模块
DWORD IGA3DInit( LPDIRECT3DDEVICE8 pd3dDeviceOrig );

//释放3D模块
void IGA3DDestory();

//获取矩阵数据
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix );

//绘制
DWORD SetRenderPos();
void IGA3DRender();

//设置更新
void SetFristDraw();
void SetUpdate();	//这个函数的作用是通知系统重新加载资源

//----------------------------
struct SDrawElement
{
	int		mesh_id;  //模型ID
	int		scene_id; //场景ID
	
	float	x, y, z;
	float	s_xyz;
	float	r_x, r_y, r_z;

	char	url[1024];

	SDrawElement()
	{
		mesh_id = -1;
		scene_id = -1;

		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		s_xyz = 1.0f;
		r_x  = 0.0f;
		r_y  = 0.0f;
		r_z  = 0.0f;

		url[0] = '\0';
		
	}

	SDrawElement( const SDrawElement& obj )
	{
		mesh_id = obj.mesh_id;
		scene_id = obj.scene_id;

		x = obj.x;
		y = obj.y;
		z = obj.z;
		s_xyz = obj.s_xyz;
		r_x  = obj.r_x;
		r_y  = obj.r_y;
		r_z  = obj.r_z;

		strcpy( url, obj.url );
		
	}
};


class IGA3DModule
{
public:
	IGA3DModule(){ m_pkFxSkelMesh = NULL; };
	~IGA3DModule(){};

public:
	int Init();
	void Destory();

	void PerIGA3DRender();
	void OnIGA3DRender();
	void PostIGA3DRender();

	void IGA3DInputState();
	void RunScript();

	void ReLoad();

//--------------------------------------
//2008-7-24添加：
//逻辑资源: 用来保存被拾取上的对象ID。
public:
	int		m_PickID;	//被拾取上模型的ID,如果小于0表示没有拾取到任何3D对象
	SIGA3DMSG	m_Msg;	//当前消息
	bool		m_bGetMsg;	//

	//是否显示模型
	bool		m_bShow;

	//更新系统
	bool		m_bUpdate;

//-------------------------------------
//系统资源
public:
	LPDIRECT3DDEVICE8	m_pd3dDevice;
	D3DXMATRIX			m_matView, m_matProj, m_matWorld;
	bool				m_bInitView, m_bInitProj, m_bInitWprld;

	IDirect3DBaseTexture8 *pkTexStage0;
	DWORD ulFVF, ulLighting, ulCullMode, ulZEnable, ulVertexBlend, ulTexStage0ColorOp, ulTexStage0ColorArg1, ulTexStage0ColorArg2;


//3D环境状态
public:
	DWORD					m_Lighting, 
							m_CullMode, 
							m_ZEnable, 
							m_Alpha,
							m_TexStage0ColorOp,		//*
							m_TexStage0ColorArg1,	//*
							m_TexStage0ColorArg2;	//*

	IDirect3DBaseTexture8*	m_pTexStage0;	//*

	bool InputState[128];	//输入状态
	
public:
	BOOL		m_bFrist;
	long		m_ElapseTime;	//当前相同运行时间
	long		m_Start;	
	bool		m_bIsLoad;		//模型是否加载成功

	DWORD		m_State;

	int			m_DrawPos;	//在Core里绘制的位置

private:
	FxSkeletalMesh *m_pkFxSkelMesh;
	SDrawElement	m_DrawElement;

//2008-8-5添加
private:
	typedef  list<SDrawElement>		_DE;
	typedef  list<SDrawElement>::iterator _DE_NODE;

	_DE			m_DrawLibrary;
	_DE_NODE	m_pDrawNode;

	int			m_CurrentScene;

public:
	LPIGAFUN			m_pFun;
	
};

IGA3DModule* GetIGA3DModule();

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

#define PATH_WORK "InterAdtive\\EMMOA\\3D\\"

void GetAbsPath( char fullPathName[256] );

//----------------------------------
//脚本函数
int CGetState( lua_State *L );
int CSetState( lua_State *L );

void DrawMesh( lua_State *L );

//获取摄影机位置
void GetCameraPos();