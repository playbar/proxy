///////////////////////////////////////////
//DInput.h
///////////////////////////////////////////

#pragma once

#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <dinput.h>


class CDInput
{
public:
	CDInput(){}
	~CDInput(){}

public:
	LPDIRECTINPUT8       m_pDInput;    //主输入对象
	LPDIRECTINPUTDEVICE8 m_pDIKey;    //主输入键盘设备

	UCHAR m_bKeyboard[256];

public:
	bool Init(HINSTANCE hHinstance);
	void Release();

	bool InitKeyboard(HWND hWND);
	bool ReadKeyboard();
	void ReleaseKeyboard();

};

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


extern CDInput * g_pDInput;