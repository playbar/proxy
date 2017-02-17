#ifndef FXSKELETALMESH_H
#define FXSKELETALMESH_H

#include <stdio.h>
#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dxfile.h>

class D3DXFRAME;
class D3DXMESHCONTAINER;
class FxAnimationCollection;

class FxSkeletalMesh
{
public:
	FxSkeletalMesh(IDirect3DDevice8 *pkD3DDevice);
	~FxSkeletalMesh();

	bool LoadFromXFile(char* filename, char* texture_path, DWORD new_fvf, DWORD load_flags);

	void Update(unsigned long ulElapsedTime, bool bLoop = true);

	void Render( LPDIRECT3DTEXTURE8 pTex );

	//helper function
	float GetBoundingSphereRadius() const;

	//------------------------
	//拾取相关的代码
	//2008-7-23
	void GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const;
	void GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const;		//note: get a loose bounding box
	bool Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, D3DXFRAME* pFrame = NULL);
	bool PickMeshContainer(D3DXMESHCONTAINER* pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir);
	//2008-7-23
	//------------------

protected:
	IDirect3DDevice8 *m_pkD3DDevice;

	D3DXFRAME *m_pkRootFrame;
	D3DXMESHCONTAINER *m_pkMeshContainer;
	FxAnimationCollection *m_pkAnimCollection;

};


#endif