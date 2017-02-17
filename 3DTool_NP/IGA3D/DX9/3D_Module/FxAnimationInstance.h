#ifndef FXANIMATIONINSTANCE_H
#define FXANIMATIONINSTANCE_H

#include <d3d9.h>
#include <d3dx9.h>


class FxAllocateHierarchy;

class FxAnimInstance
{
public:
	FxAnimInstance(IDirect3DDevice9 *pkD3DDevice);
	~FxAnimInstance();

	HRESULT InitialiseResource(const char *pcFileFullName, const char *pcFilePath);
	HRESULT ReleaseResource();
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainer);

	void ReleaseAttributeTable(LPD3DXFRAME pFrameBase);

	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const LPD3DXMATRIX pParentMatrix);
	void Update(float fElapsedTime, const LPD3DXMATRIX pParentMatrix);

	void RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, LPDIRECT3DTEXTURE9 pTex );
	void Render(  LPDIRECT3DTEXTURE9 pTex, LPD3DXFRAME pFrame = NULL);

	//更新的有关拾取的代码：2008-7-22
	void GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const;
	void GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const;		//note: get a loose bounding box
	bool Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, LPD3DXFRAME pFrame = NULL);
	bool PickMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir);


private:
	IDirect3DDevice9 *m_pkD3DDevice;
	LPD3DXFRAME m_pkFrameRoot;
	ID3DXAnimationController* m_pkAnimController;

	FxAllocateHierarchy *m_pkAH;
};


#endif