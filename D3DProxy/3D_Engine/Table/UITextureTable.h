

#pragma once

#include <tchar.h>


struct UI_TEXTURE_TABLE
{
	TCHAR szPath[50];
};

extern TCHAR * GetCommonTexturePath( int id );
extern TCHAR * GetLoginUITexturePath( int id );
extern TCHAR * GetGameUITexturePath( int id );
