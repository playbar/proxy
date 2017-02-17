// stdafx.h 
#pragma once

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "stdio.h"

#include "IGA_IDirect3D9.h"
#include "IGA_IDirect3DDevice9.h"
#include "IGA_IDirect3DSwapChain9.h"
#include "IGA_IDirect3DTexture9.h"

// Put up an assertion failure message box.
inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
{
	char szOutputString[ sizeof(char) *MAX_PATH];
	memset(szOutputString, 0, sizeof(char)  * MAX_PATH );
	sprintf(szOutputString, "%s,  File %s, line %d ,Date:%s, Time:%s",expr, file, line,__DATE__, __TIME__ );
	OutputDebugStringA(szOutputString);
}

//在debug下显示输出信息宏,使用前要先定义存放字符串的数组
# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# else
#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
# endif

