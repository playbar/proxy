/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
* 
* GNU license doesn't cover Engine directory.
* Content of Engine directory is copyrighted (c) 1999, 2000, by Valve LLC
* and it is licensed under Valve's proprietary license (see original HL SDK).
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include "gpatch.h"

using namespace std;

//===================================================================================
inline static bool datacompare(const BYTE* data1, const BYTE* data2, const char* mask)
{
	for(;*mask;++mask,++data1,++data2)
	{
		if(*mask=='x' && *data1!=*data2 ) return false;
	}
	return (*mask)==0;
}

//===================================================================================
bool GenericPatch::find_location(BYTE* hint,const BYTE* sig_bytes,const char* sig_mask)
{
	assert(sig_mask&&strlen(sig_mask)&&sig_bytes&&hint);
	assert(!patchloc&&!patchsize);
	DWORD len = strlen(sig_mask);
	assert(len<PATCH_MAX);
	if( !datacompare(hint,sig_bytes,sig_mask) ) { return false; }
	patchloc  = hint;
	patchsize = len;
	assert(patchsize<PATCH_MAX);
	memcpy(backup ,patchloc,patchsize);
	memcpy(patchdata,patchloc,patchsize);
	return true;
}

//===================================================================================
void GenericPatch::set_location (BYTE* loc,int length)
{
	assert(!patchloc&&!patchsize);
	assert(loc&&length);
	assert(length<PATCH_MAX);
	patchloc  = loc;
	patchsize =length;
	memcpy(backup ,patchloc,patchsize);
	memcpy(patchdata,patchloc,patchsize);
}

//===================================================================================
void GenericPatch::apply()
{
	assert(patchloc&&patchsize);
	assert(!memcmp(patchdata+patchsize,backup+patchsize,PATCH_MAX-patchsize));
	Protect(patchloc,patchsize);
	memcpy(patchloc,patchdata,patchsize);
	Protect(patchloc);
}

//===================================================================================
void GenericPatch::restore()
{
	assert(patchloc&&patchsize);
	Protect(patchloc,patchsize);
	memcpy(patchloc,backup,patchsize);
	Protect(patchloc);
}

//===================================================================================
void GenericPatch::Protect(void* addr, int size /*=-1*/)
{
	if (size!=-1)
	{
		protect_size = size;
		VirtualProtect(addr,protect_size,PAGE_EXECUTE_READWRITE,&protect_old);
	}
	else 
	{   
		DWORD dummy;
		VirtualProtect(addr,protect_size,protect_old,&dummy);
	}
}
