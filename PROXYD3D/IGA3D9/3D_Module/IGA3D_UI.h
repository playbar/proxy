/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGA3D_UI.h
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/

#pragma  once 

//#include "d3d9.h"
//#include "d3dx9.h"

#include <queue>
using namespace std;

#include <functional>
#include "DefineConst.h"


class CIGA3DMessageManager
{
public:
	CIGA3DMessageManager();
	~CIGA3DMessageManager();

public:
	HRESULT Init();
	void    Destory();
	void	UpdataMessage();
	bool    GetIGA3DMessage( STIGA3DMSG* msg );	//从消息队列里得到一个消息
	void	SendIGA3DMessage( STIGA3DMSG msg );	//把消息发送到消息队列里
	void	RemoveMsg();//移除顶部消息
	void	SetMsgManagerState( DWORD state );	//设置管理器状态

private:

	queue<STIGA3DMSG>	m_queueMsg;

public:
	int		m_xCursor;
	int		m_yCursor;

};

extern CIGA3DMessageManager g_CIGA3DMessageManager;





