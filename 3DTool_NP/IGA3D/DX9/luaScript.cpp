#include "stdafx.h"

CLuaScript* GetScript()
{
	static CLuaScript lua;
	return &lua;
}


//---------------------------------------------
CLuaScript::CLuaScript()
{
	L = lua_open();
	lua_cpcall(L, luaopen_base, 0); 
	lua_cpcall(L, luaopen_io, 0);   
	lua_cpcall(L, luaopen_math, 0); 
	lua_cpcall(L, luaopen_string, 0);
}

CLuaScript::~CLuaScript()
{
	lua_close(L);
}

int CLuaScript::LoadLuaFile( char* fileName )
{
	if( L != NULL )
	{
		lua_close(L);
		L = lua_open();
		lua_cpcall(L, luaopen_base, 0); 
		lua_cpcall(L, luaopen_io, 0);   
		lua_cpcall(L, luaopen_math, 0); 
		lua_cpcall(L, luaopen_string, 0);
	}

	int s = luaL_loadfile( L, fileName );
    if ( s==0 )
	{
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
		if( s == 0 )
		{
			lua_getglobal(L,"DrawPos");
			if( lua_isnumber(L,1) )
			{
				int DrawPos = (int)lua_tonumber(L,1);
				return DrawPos;
			}
		}
		else if ( s == LUA_ERRERR )
		{
			OutputDebugString("CLuaScript: 运行错误处理函数时发生的错误！");
		}
		else if( s == LUA_ERRMEM )
		{
			OutputDebugString("CLuaScript: 内存分配错误！");		
		}
		else if( s == LUA_ERRRUN )
		{
			OutputDebugString("CLuaScript: 运行时错误！");
		}
	}
	OutputDebugString("CLuaScript: 脚本文件加载失败！");
	return Per_Present;
}

void CLuaScript::RegFunction( char* funName, lua_CFunction pfun )
{
	lua_register( L, funName, pfun );
}

void CLuaScript::RunScript()
{
	//获取lua函数
	lua_getglobal(L,"Run");

	//给lua函数传递参数

    //lua_pushnumber(L, 0 );


	//执行lua函数
	lua_pcall( L,	//lua虚拟机
			   0,   //入栈的参数数目
			   0,   //返回值的数目
			   0 ); //错误信息

	//char text[256];

	//for( int i = 1; ; i++ )
	//{
	//	//返回NULL值就结束
	//	if( lua_isnil(L,-i) )
	//	{
	//		break;
	//	}

	//	//返回字符串
	//	if( lua_isstring(L, -i) )
	//	{
	//		sprintf(text, "%d : %s\n", i, lua_tostring(L, -i) );
	//		continue;
	//	}

	//	//返回数字
	//	if( lua_isnumber(L, -i) )
	//	{
	//		sprintf(text, "%d : %s\n", i, lua_tonumber(L, -i) );
	//		continue;
	//	}
	//	OutputDebugString(text);
	//}

	////清理虚拟堆栈
	//lua_pop(L, 1);

}