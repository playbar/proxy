
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "timehandling.h"

float  ClientTime::mapTime    = 0;
double ClientTime::current    = 0;
DWORD  ClientTime::current_ms = 0;

//===================================================================================
void ClientTime::reportMapTime(float mapTimeNew)
{
	if( mapTimeNew < mapTime-10 )
	{
		void AtMapChange();
		AtMapChange();
	}
	mapTime = mapTimeNew;
	updateCurrentTime();
}

//===================================================================================
void ClientTime::updateCurrentTime()
{
	current_ms = timeGetTime();
	current = (double)current_ms / 1000.0;
}
