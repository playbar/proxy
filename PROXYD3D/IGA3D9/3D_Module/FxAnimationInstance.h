/************************************************************************
*	Copyright(c) 2008-2009 互友财富（北京）因特网技术有限公司
*	All rights reserved
*	FileName : FxAnimationInstance.h
*	功能描述：IGA3D, modify by hgl
*	当前版本：2, 0, 8, 1226
*	作者/修改者：hgl
*	完成日期：2008-12-26
*	取代版本: 2, 0, 8, 1222
*	修改人：
*	完成日期：                                                      
/************************************************************************/
#pragma  once 

#include <d3d9.h>
#include <d3dx9.h>


class FxAllocateHierarchy;

class FxAnimInstance
{
public:
	FxAnimInstance(IDirect3DDevice9 *pkD3DDevice);
	~FxAnimInstance();

	
	HRESULT InitialiseResource(LPCSTR pcFileFullName, LPCSTR pcFilePath);
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
	bool Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, LPD3DXFRAME pFrame = NULL, float* pfDist = NULL );
	bool PickMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, float* pfDist = NULL);


private:
	CComPtr<IDirect3DDevice9>	m_spD3DDevice;
	LPD3DXFRAME			m_pFrameRoot;

	CComPtr<ID3DXMesh>		m_pD3DMesh;
	DWORD			m_dwNumMaterial;
	D3DMATERIAL9	m_D3DMaterialWhite;

	CComPtr<ID3DXAnimationController> m_spAnimController;
	FxAllocateHierarchy			*m_pFxAllocateHierarchy;

};

