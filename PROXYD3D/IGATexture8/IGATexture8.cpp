// IGATexture8.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "BasePlay.h"
#include "TexturePaly.h"
#include "IGATexture.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


HRESULT IGATextureInit( LPDIRECT3DDEVICE8 pd3dDevice )
{
	HRESULT hr = g_CIGATexture.Init(pd3dDevice);
	if( FAILED(hr) )
	{
		OutputDebugString("IGATextureInit");
	}
	return hr;
}

LPDIRECT3DTEXTURE8 IGAGetTexture( int id, bool* bIsPlayEnd , float* w, float* h )
{
	LPDIRECT3DTEXTURE8 pIGATex = g_CIGATexture.GetTexture(id, bIsPlayEnd, w, h );
	if( pIGATex != NULL )
	{
		return pIGATex;
	}

	return NULL;
}


void IGATextureDestory()
{
	//GetTexPlay()->Stop();
	//GetTexPlay()->Destroy();
	//CBasePlay::GetPlayerInstance()->Destory();

	g_CIGATexture.ReleaseResource();
}

int DrawPlay()
{
	//CBasePlay::GetPlayerInstance()->FrameMove();
	return 0;//GetTexPlay()->ShowPlayer(0);
}

void PlayAVI( LPDIRECT3DDEVICE8 pd3dDevice, char* name )
{
	//CBasePlay::GetPlayerInstance()->PlayAVIFile( pd3dDevice, name );
	//GetTexPlay()->PlayAVIFile(name);
}

void PlayFLASH( LPDIRECT3DDEVICE8 pd3dDevice, char* name )
{
	//CBasePlay::GetPlayerInstance()->PlayFlashFile( pd3dDevice, name );
	//GetTexPlay()->PlayFlashFile(name);
}

void PausePlayer()
{
	//GetTexPlay()->Pause();
}

void StopPlayer( DWORD dwID)
{
	g_CIGATexture.StopPlay( dwID);
	//g_CIGATexture.Destory();
	//GetTexPlay()->Stop();
}