// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <d3d8.h>
#include <d3d8types.h>
#include <d3dx8tex.h>
#include <string>

#include "Hideme.h"
#include "IGAWin.h"


//-----------------------------------------------
//3D模块
typedef DWORD (*FUN_IGA3DINIT)( LPDIRECT3DDEVICE8  );
typedef void  (*FUN_IGA3DGetMatrixData)( D3DTRANSFORMSTATETYPE ,CONST D3DMATRIX*  );
typedef void  (*FUN_IGA3DRender)();
typedef void  (*FUN_IGA3DDestory)();
typedef void  (*FUN_ISetFristDraw)();

enum RenderPos
{
	Per_SetTransform,	//设置矩阵前
	Post_SetTransform,	//设置矩阵后
	Per_Present,		//翻转前
	Post_Present		//翻转后
};
//-------------------------------------------------