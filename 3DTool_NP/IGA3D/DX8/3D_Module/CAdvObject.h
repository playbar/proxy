#pragma once
#include "d3d8.h"
#include "d3dx8.h"
#include <list>
#include <iostream>
#include <fstream>

#include <queue>
#include <algorithm>

using namespace std;
#define PLANE_EPSILON	0.5f

#include "MeshLibrary.h"
#include "TexLibrary.h"

//------------------------------------------------------------
//
struct BaseDescription
{	
	DWORD			AdvObjectDescriptionID;	//对象ID
	int				num_Pack;				//被点击的次数
	DWORD			e_time;					//显示时间
	int				mesh_id;
	int				texture_id;
	//int				skin_mesh_id;
	BaseDescription(): AdvObjectDescriptionID(0),e_time(0),num_Pack(0)
	{
		mesh_id		= -1;
		texture_id	= -1;
		//skin_mesh_id = -1;
	}
};

struct BaseAdvObjectDescription : public BaseDescription
{
	D3DXVECTOR3		MinPoint, MaxPoint, CenterPoint;//计算的碰撞盒的对角点
	float           RotationY;
	bool			is_Active;				//是否是活动的
	BaseAdvObjectDescription()
	{
		MinPoint.x = -1;
		MinPoint.y = -1;
		MinPoint.z = -1;
		MaxPoint.x = 1;
		MaxPoint.y = 1;
		MaxPoint.z = 1;
		CenterPoint.x = 0;
		CenterPoint.y = 0;
		CenterPoint.z = 0;
		RotationY = 0;
		is_Active = false;
	}
};

struct AdvObjectDescription : public BaseAdvObjectDescription
{
	BOOL			b_isPack;				//是否被拾取上
	DWORD			time;					//开始显示时间
	bool			is_Show;				//显示计时的开始、重设标志
	
	AdvObjectDescription():b_isPack(true),time(0)
	{
		is_Show  = false;
	}
};

class CAdvObject
{
public:
	CAdvObject();
	~CAdvObject();

public:
	HRESULT InitAdvObject( LPDIRECT3DDEVICE8 pd3dDevice );	//初始化广告对象容器
	void Release();				//释放广告对象容器

	//为切换场景提供的
	HRESULT LoadDDF( char* FileName );	//加载DDF文件
	HRESULT UnloadDDF();				//卸载DDF文件
	D3DXVECTOR3 GetViewPos();			//获取摄影机的位置

	//根据游戏的单位设置转换矩阵
	void	SetMatrixTranslation( D3DXMATRIX matTran );	//

	//设置工具的参考变量
	void SetOffsetParam( float tran, float rot, float scal );

	void Render( D3DXMATRIX matView, D3DXMATRIX matProj );				//"绘制"所有广告对象
	//注意：这里的绘制是检索所有在视椎范围内的广告对象，并判断是否有对象被选中
	
	void OutputToFile( char* pathName );

	DWORD GetSelectID(){ return m_SelectID; };
	void  ClearSelectID(){ m_SelectID = 0xffffffff; };

private:
	void RunScript( int meshID, int* ptexID );

protected:
	bool ViewCentrumClip( D3DXVECTOR3* pv );		//视椎裁减
	void UpdataViewCentrum( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj );	//更新视椎体
	BOOL Pick(	D3DXVECTOR3		  * pPosition,	//碰撞盒的位置
				CONST D3DXVECTOR3 * pMin,		//碰撞盒的对角点
				CONST D3DXVECTOR3 * pMax );		//拾取计算
	virtual void KeyProcessor(){};

protected:
	virtual HRESULT InitSub(){ return S_OK; };
	virtual void RenderSub( AdvObjectDescription *pAdvObjectDescription ){}; //真正的绘制在这里
	virtual void ReleaseSub(){};
	virtual void OutputToFileSub( char* pathName ){};

	//修改函数的参考变量
	float					m_OffsetTran, m_OffsetRot, m_OffsetScal;

	//对象替换描述队列
	typedef map<int,int>		DES_SCR;	//场景脚本描述
	typedef pair<int,int>		NODE;
	map<int,int>::iterator		m_pNode;   

	DES_SCR	m_Des;

	//添加脚本描述
	void AddDes( int mesh_id, int tex_id );


protected:
	list<AdvObjectDescription*>				m_AdvObjectContainer; //广告对象容器
	list<AdvObjectDescription*>::iterator   m_Iterator;			//指向被选中的节点
	AdvObjectDescription					*m_pPackObject;		//指向被拾取的碰撞盒
	D3DXVECTOR3								m_vShowPos;
	bool									m_bIsDesignMode;			//是否是设计模式

	LPDIRECT3DDEVICE8		m_pd3dDevice;	//绘图设备
	DWORD					m_SelectID;		//被选种对象的ID
	DWORD					m_MaxObjectID;	//当前场景中最大ID对象

	int          iWidth, iHeight;

private:	//用于进行视椎切割
    D3DXVECTOR3	m_vtx[9];	/// 构成平截头体的8个顶点
	D3DXPLANE	m_plane[6];	/// 构成平截头体的6个平面
	D3DXVECTOR3	m_vPos;		/// 当前摄像机的世界坐标
	float       m_nearLength; //当前帧距摄影机最近的一个选中对象的距离
	D3DXMATRIX  m_matView, m_matProj;

protected:
	D3DXMATRIX  m_MatrixTranslate;	//转换矩阵

	D3DXMATRIX  _m_MatrixTranslate; //内部的转换矩阵，让模型在自身空间内做变换
	
};

//------------------------------------------------------------
class CAdvObjectTool : public CAdvObject
{
public:
	CAdvObjectTool();
	~CAdvObjectTool();

public:
	HRESULT CreateBoundBox();	//在摄影机近平面创建一个盒子，它的空间区域就是碰撞和的计算区域
	HRESULT DeleteBoundBox();	//删除当前的碰撞盒，从节点容器中移除当前对象

public:
	void OutputToFileSub( char* pathName );

public:  //重载的成员函数
	void RenderSub( AdvObjectDescription *pAdvObjectDescription );
	void ReleaseSub();
	HRESULT InitSub();
	void SetMaxMeshID( int id ){ m_MaxMeshID = id; };
	void SetMaxTexID( int id ){ m_MaxTexID = id; };

private:
	void	KeyProcessor();	  //按键处理
	void    ModifyBoundBox(); //修改碰撞盒数据

private:	//用于创建绘制广告对象
	LPD3DXMESH				m_pCurrentMesh;
	D3DMATERIAL8			m_WhiteMaterial,m_RedMaterial;

	float					x, y, z, s_x, s_y, s_z, r_y;	//选中碰撞盒的中心位置盒尺寸，这里作为一个临时存储地
		
	AdvObjectDescription    *m_pAdvObjectDescription;	//当前碰撞盒描述对象，用于创建新的碰撞盒

private:	//广告对象仓库
	int						m_CurrentMeshID, m_MaxMeshID;
	int						m_CurrentTexID, m_MaxTexID;
//----------------------------------------------
//子类接口
protected:
	virtual void ComputeBoundBox( LPD3DXMESH pMesh ){};			//计算碰撞盒并计算碰撞盒对角点
	virtual void SetPosition( float x, float y, float z ){};	//设置碰撞盒位置
	virtual void AdjustBoundBox(){};	//通过键盘调整碰撞盒的属性
};

CAdvObject* GetAdvObject();

CAdvObjectTool* GetAdTool();

