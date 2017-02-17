////////////////////////////////
//main.h
////////////////////////////////

#pragma once


#include <windows.h>   
//#include <windowsx.h> 
//#include <mmsystem.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <string.h>
//#include <stdarg.h>
//#include <stdio.h> 
#include <math.h>
//#include <io.h>
//#include <fcntl.h>
#include <zmouse.h>



#include "DInputSound/DSound.h"
#include "DInputSound/DInput.h"
#include "ToolFunction.h"

#include "D3D/D3D.h"
#include "D3D/Camera.h"
#include "D3D/Terrain.h"
#include "D3D/D3DStruct.h"
#include "D3D/Sky.h"

#include "resource.h"
#include "itemManager.h"
#include "item.h"

//#pragma comment( lib, "winmm.lib" )
//#pragma comment( lib, "dsound.lib" )
//#pragma comment( lib, "dinput.lib" )
//#pragma comment( lib, "dinput8.lib" )

//#pragma comment( lib, "d3dx9.lib" )
//#pragma comment( lib, "d3d9.lib" )



#define WINDOW_CLASS_NAME L"D3D9GAME"

#define SCREEN_WIDTH			1024
#define SCREEN_HEIGHT			768
#define SCREEN_BPP				32

#define FULLSCREEN				0



enum DIR_CONTROL
{
	DOWN = 0,
	LEFT,
	RIGHT,
	UP,
	LEFT_DOWN,
	RIGHT_DOWN,
	LEFT_UP,
	RIGHT_UP,
};


//ÓÎÏ·×´Ì¬¿ØÖÆ
enum GAME_CONTROL
{
	GAME_NONE = 0,
	GAME_LOGIN,
	GAME_LOAD,
	GAME_NEW,
	GAME_RUN,
	GAME_OVER,
	GAME_STORY,
	GAME_LOADGAME,
	GAME_INIT_LOADING,
	GAME_ENTER_LOADING,
	GAME_RETURN_LOGIN,
};

enum LOADING_FLAG
{
	LOADING_UI = 0,
	LOADING_TERRAIN,
};

enum CURSOR_ID
{
	CURSOR_NORMAL,
	CURSOR_ATTACK,
	CURSOR_SPEAK,

	MAX_CURSOR_NUM,
};


enum ACTION_CONTROL
{
	AC_STAY = 0,
	AC_WALK,
	AC_RUN,
};


enum MIDI_ID
{
	MIDI_MENU = 0,
	MIDI_HOME,
};

enum SOUND_ID
{
	SOUND_POP = 0,
	SOUND_BUTTON1,
	SOUND_GETITEM,
};


enum MAP_ID
{
	MAP_HOME = 0,
	MAP_HOME_YARD,
	MAP_HOME1,
	MAP_HOME_FIELD1,
	MAP_HOME_SHOP1,
	MAP_HOME_SHOP2,
	MAP_HOME_FIELD2,

	MAP_NUM,
	
};



enum STORY_ID
{
	STORY_SPEAKFIRST = 0,
	STORY_SENDMEDICINE,
	STORY_SPEAK_01,
};