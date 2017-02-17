#ifndef FXALLOCATEHIERARCHY_H
#define FXALLOCATEHIERARCHY_H

#include <d3dx9.h>


class FxAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	FxAllocateHierarchy() {}

	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	void SetFilePath(const char *pcFilePath) { strcpy(m_acFilePath, pcFilePath); }

private:
	char m_acFilePath[256];
};


#endif