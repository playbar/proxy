
#pragma  once 
#ifndef DXUT_GLOBALFUNCTION_H
#define DXUT_GLOBALFUNCTION_H

#include <d3d9.h>
#include <d3dx9.h>

HRESULT DXUTCreateGUITextureFromInternalArray( LPDIRECT3DDEVICE9 pd3dDevice, IDirect3DTexture9** ppTexture, D3DXIMAGE_INFO* pInfo );
HRESULT DXUTCreateArrowMeshFromInternalArray( LPDIRECT3DDEVICE9 pd3dDevice, ID3DXMesh** ppMesh );

bool DXUTFindMediaSearchTypicalDirs( WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf, WCHAR* strExePath, WCHAR* strExeName );
bool DXUTFindMediaSearchParentDirs( WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt, WCHAR* strLeafName );

HRESULT DXUTFindDXSDKMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );


#endif

