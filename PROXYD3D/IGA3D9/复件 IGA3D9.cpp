/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : IGA3D9.cpp
*	IGA3D9.dll导入文件实现
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：
/************************************************************************/

#include "stdafx.h"
#include "IGA3DModule.h"
#include "IGA3D9.h"


int g_bDubug = 0;		//	全局变量，控制是否显示调试信息，从emmoa.ini文件中读取


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//OutputDebugString("IGA3D9  DllMain");
    return TRUE;
}
//初始化3D模块
DWORD IGA3DInit(HWND hGameWnd, LPDIRECT3DDEVICE9 pd3dDeviceOrig, LPDIRECT3DDEVICE9 pd3dDeviceProxy, LPCallBack3DFocus lpfocus, LPCallBackDeviceRefCount lpRefCount )
{
	//
	//防止重复初始化
	//
	static bool s_bLoad = false;
	if ( ! s_bLoad )
	{
		s_bLoad = true;
	}
	else
	{
		IGA3DDestory();
	}
	setlocale(LC_ALL,"Chinese-simplified");
	char szNameINI[MAX_PATH];
	strcpy( szNameINI, "emmoa.ini" );
	GetModulePath(szNameINI);
	g_bDubug = GetPrivateProfileInt("Common", "debug", 0, (LPCTSTR)szNameINI);

	DWORD draw_pos = 0;
	__try
	{
		g_IGA3DModule.SetGameWnd( hGameWnd );
		g_IGA3DModule.m_spD3DDevice = pd3dDeviceOrig;
		g_IGA3DModule.m_spD3D9DeviceProxy = pd3dDeviceProxy;
		g_IGA3DModule.m_pFun3DFocus = lpfocus;
		g_IGA3DModule.m_pFunDeviceRefCount = lpRefCount;
		draw_pos = g_IGA3DModule.Init();
		g_IGA3DModule.m_iDrawPos = draw_pos;
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}

	return draw_pos;
}

//释放3D模块
void IGA3DDestory()
{
	__try
	{
		g_IGA3DModule.Destory();
	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}
}

//获取矩阵数据
void IGA3DGetMatrixData( D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix )
{
	__try
	{
		if( pMatrix == NULL )
		{
			return;
		}

		if( D3DTS_WORLD == State )
		{
			/*char szOutputString[256];
			sprintf(szOutputString, "_11= %f, _12= %f, _13= %f, _14=%f, _21=%f, _22=%f, _23=%f, _24=%f, \
			_31=%f, _32=%f, _33=%f, _34=%f, _41= %f, _42=%f, _43= %f, _44=%f", \
			pMatrix->_11, pMatrix->_12, pMatrix->_13, pMatrix->_14, pMatrix->_21, pMatrix->_22, pMatrix->_23, pMatrix->_24,\
			pMatrix->_31, pMatrix->_32, pMatrix->_33, pMatrix->_34, pMatrix->_41, pMatrix->_42, pMatrix->_43, pMatrix->_44);
			OutputDebugString(szOutputString);*/
			memcpy( &g_IGA3DModule.m_matWorld, pMatrix, sizeof(D3DXMATRIX) );
		}
		else if( (D3DTS_VIEW == State)  &&(!g_IGA3DModule.m_bInitView) )
		{
			if( g_IGA3DModule.m_dwGameID == IGA_WLOL )	 	//《武林群侠传》
			{	
				static int istaticWLCount = 0;
				if( fabsf(pMatrix->_41) > 1.0f )
				{
					if( istaticWLCount == 1 )
					{
						memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
						g_IGA3DModule.m_bInitView = true;
						istaticWLCount = 0;
					}
					else
					{
						istaticWLCount++;
					}
				}
			}
			else  //其他游戏
			{
				if( fabsf(pMatrix->_41) > 0.0f )
				{
					memcpy( &g_IGA3DModule.m_matView, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitView = true;
				}
			}


		}
		else if( ( D3DTS_PROJECTION == State) &&( !g_IGA3DModule.m_bInitProj ) )
		{

			switch (g_IGA3DModule.m_dwGameID)
			{
			case IGA_FENGYICHUANSHUO:  //封印传说
				/*if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				SHOWOUTPUTDEBUG(szOutputString);
				}*/
				//sprintf(szOutputString, "_11= %f, _12= %f, _13= %f, _14=%f, _21=%f, _22=%f, _23=%f, _24=%f, \
				_31=%f, _32=%f, _33=%f, _34=%f, _41= %f, _42=%f, _43= %f, _44=%f", \
				pMatrix->_11, pMatrix->_12, pMatrix->_13, pMatrix->_14, pMatrix->_21, pMatrix->_22, pMatrix->_23, pMatrix->_24,\
				pMatrix->_31, pMatrix->_32, pMatrix->_33, pMatrix->_34, pMatrix->_41, pMatrix->_42, pMatrix->_43, pMatrix->_44);
				//OutputDebugString(szOutputString);
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				break;
			case IGA_SHUSHAN:   //蜀山
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
			case IGA_YINGTAOCHENG:   //婴淘城
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//《神泣》的代码，这个游戏能够显示3D模型、支持模型点击、支持裁剪，因此能够支持数据统计，在重新加载和退出时，会将模型的显示信息保存到文件。
				//模型会遮挡住游戏UI界面。
			case IGA_SHENQI:
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && ( pMatrix->_22 - pMatrix->_11 > EPSINON ) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//《大航海时代》的代码，支持3D模型的显示、点击，支持裁剪，因此能够支持数据统计。在重新加载和退出时，会将模型的显示信息保存到文件。
				//模型会遮挡住游戏UI界面。
			case IGA_GV:
				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

				//《武林群侠传》的代码，支持3D模型显示、点击，支持裁剪，因此能够支持数据统计。在获取视口矩阵时，有特殊要求。
				//注意：这个游戏的3D模型显示不是很稳定，在特定情况下会导致模型显示不出来，例如，当点击了游戏中的公告牌就可能导致模型显示不出来，但离开一段距离又可以显示。
				//模型会遮挡住游戏UI界面。
			case IGA_WLOL:

				if( FLOAT_EQAUL_ZERO(pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON ) && pMatrix->_33 >= 1.0f)
				{
					memcpy(&g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DMATRIX));
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

				//《奇迹世界》的代码，支持3D模型显示、点击，支持裁剪，因此能够支持数据统计。这个游戏在显示时，有点问题（感觉上不是跟游戏场景在统一坐标系下），但不会影响游戏。
				//模型会遮挡住游戏UI界面。
			case IGA_SUN:	
				if( FLOAT_EQAUL_ZERO( pMatrix->_44)  && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}	
				break;

				//《热力排球》的代码，支持3D模型显示、点击，支持裁剪，因此能够支持数据统计。这个游戏在显示时，游戏的ＵＩ边界会遮挡住添加的３Ｄ模型，影响不大。
				//这个游戏是分场景的，对于不同的的场景很可能会使用用一个世界坐标，因此，在一个场景里添加的模型，可能在另一个场景中也能看到。
				//就需要有效的场景识别功能。
			case IGA_RELIPAIQIU:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}

				break;

				//《跑跑卡丁车》的代码，支持3D模型显示、点击，支持裁剪，因此能够支持数据统计。
				//模型会遮挡住游戏，就是说，不管什么视角都会显示在最前面，并不是跟游戏的场景在一个空间中。
				//这个游戏是分场景的，对于不同的的场景很可能会使用用一个世界坐标，因此，在一个场景里添加的模型，可能在另一个场景中也能看到。就需要有效的场景识别功能。
			case IGA_PAOPAOKADINGCHE:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11 > EPSINON) && pMatrix->_33 >= 1.0f )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}

				break;

				//在《唯舞独尊》......
			case IGA_WEIWUDUZUN:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;
				//未知的游戏,这是默认的情况，为了那些带测试的游戏，对于需要具体对待的游戏，上面的代码已经做了具体解释。
				//当然，以后经测试后的游戏，也需要按名称ID添加到上面的代码里，并把支持情况说明。

			case IGA_TEST:
				if( FLOAT_EQAUL_ZERO( pMatrix->_44) && (pMatrix->_22 - pMatrix->_11  > EPSINON) )
				{
					memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
					g_IGA3DModule.m_bInitProj = true;
				}
				break;

			default:
				memcpy( &g_IGA3DModule.m_matProj, pMatrix, sizeof(D3DXMATRIX) );
				g_IGA3DModule.m_bInitProj = true;
				break;
			}	
		}

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}
}

//绘制
void IGA3DRender()
{
	__try
	{
		/*
		* 延迟加载3D模块，延迟30秒钟,调试状态下不延迟加载，
		*/
#ifndef _DEBUG
		static long s_time = GetTickCount();
		static bool s_bStartDraw = false;
		if( !s_bStartDraw )
		{
			if( GetTickCount() - s_time > 1*1000 )
			{
				s_bStartDraw = true;
			}
			else
			{
				return;
			}
		}
#endif

		if( ! g_IGA3DModule.m_bFrist  )
		{
			return;
		}

		g_IGA3DModule.m_bFrist = false;

		//获取输入状态
		g_IGA3DModule.IGA3DInputState();

		//绘制
		g_IGA3DModule.PerIGA3DRender();

		//运行IGA3D脚本
		//g_IGA3DModule.RunScript();

		g_IGA3DModule.OnIGA3DRender();

		g_IGA3DModule.PostIGA3DRender();

	}
	__except( ExceptFilter( GetExceptionInformation() ))
	{
		ExceptDispose(__FILE__, __LINE__ );
	}


}

void SetFristDraw()
{
	g_IGA3DModule.m_bFrist = true;
	g_IGA3DModule.m_bInitView	= false;
	g_IGA3DModule.m_bInitProj	= false;
	g_IGA3DModule.m_bInitWprld	= false;
}

void SetUpdate()
{
	g_IGA3DModule.m_bUpdate = true;
}


