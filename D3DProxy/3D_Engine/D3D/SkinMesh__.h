//////////////////////////////////////////////////////////////////////
// SkinMesh.h: interface for the CSkinMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKINMESH_H)
#define AFX_SKINMESH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include <d3dx9.h>
#include <d3dx9mesh.h>

#include "D3D.h"

//-----------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//-----------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;//组合变换矩阵
};

//-----------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//-----------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;       // array of textures, entries are NULL if no texture specified    
                                
    // SkinMesh info             
    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups; 
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                NumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;     // used to denote the split between SW and HW if necessary for non-indexed skinning
};

// enum for various skinning modes possible
enum METHOD 
{
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    NONE
};

class CSkinMesh;

//-----------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//-----------------------------------------------------------------------------

//这里是Directx9.0a的
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);

//#if (D3D_SDK_VERSION & 31)	//这里是Directx9.0b的
    /*STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR Name, 
							LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, 
							LPD3DXEFFECTINSTANCE pEffectInstances, 
							DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer);
#else	*/					//这里是Directx9.0c的
    STDMETHOD(CreateMeshContainer)(
		THIS_ LPCTSTR Name, 
        CONST D3DXMESHDATA *pMeshData,
        CONST D3DXMATERIAL *pMaterials, 
        CONST D3DXEFFECTINSTANCE *pEffectInstances, 
        DWORD NumMaterials, 
        CONST DWORD *pAdjacency, 
        LPD3DXSKININFO pSkinInfo, 
        LPD3DXMESHCONTAINER *ppNewMeshContainer);
//#endif

    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
    CAllocateHierarchy(CSkinMesh *pSkinMesh) :m_pSkinMesh(pSkinMesh) {}

public:
	CSkinMesh*		m_pSkinMesh;
};

//这里是DirectX9.0C的



//蒙皮动画
class CSkinMesh
{
public:
	VOID SetAnim(BOOL bAnim);
	BOOL CSkinMesh::InterSect( D3DVECTOR *pRayOrig,
		 D3DVECTOR *pRayDir,D3DVECTOR* pVRet);
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

	LPDIRECT3DVERTEXSHADER9 m_pIndexedVertexShader[4];
	D3DXMATRIXA16* m_pBoneMatrices;

	UINT		m_NumBoneMatricesMax;
	METHOD		m_SkinningMethod;
	D3DCAPS9 					m_d3dCaps;
    D3DXVECTOR3                 m_vObjectCenter;    // 物体的范围中心
    FLOAT                       m_fObjectRadius;    // 物体的范围半径
	D3DXVECTOR3                 m_vMin;    // 物体的范围中心的半径的最小值
	D3DXVECTOR3                 m_vMax;    //物体的范围中心的半径的最大值


	LPD3DXANIMATIONCONTROLLER GetAnimController()//控制变换的动作
		{return m_pAnimController;};

	BOOL SetAnimationName(char *strAnimName);//设置动作的名字
//	void SetPosition(D3DXVECTOR3 &vPos)			{m_vPos=vPos; };
//	void SetPosition(float x,float y,float z)	{m_vPos.x=x;m_vPos.y=y;m_vPos.z=z;};

	VOID	Render(float fTimeFromLastFrame,D3DXVECTOR3 vPos,float angle,float scale=1.0f);//渲染
	HRESULT LoadFromXFile(TCHAR* strFileName);
	HRESULT GenerateSkinnedMesh(D3DXMESHCONTAINER_DERIVED *pMeshContainer);

	//CSkinMesh(LPDIRECT3DDEVICE9 pD3DDevice);
	CSkinMesh(LPDIRECT3DDEVICE9 pD3DDevice,char* strFileName);

	HRESULT CalculateBondingBox(LPD3DXFRAME pFrameParent, D3DXVECTOR3 *pVmin,D3DXVECTOR3 *pVmax)
	{
		static LPVOID pV; 
		static LPD3DXMESH pMesh;
		static LPD3DXMATRIX pMat;
		static D3DXVECTOR3 vMin,vMax; 
		static D3DXVECTOR3 vTransFormed;

		if(pFrameParent->pMeshContainer)
		{
			pMesh=pFrameParent->pMeshContainer->MeshData.pMesh;    
			pMat=&(((D3DXFRAME_DERIVED*)pFrameParent)->CombinedTransformationMatrix);

			pMesh->LockVertexBuffer(0,&pV); 
			D3DXComputeBoundingBox((LPD3DXVECTOR3)pV,pMesh->GetNumVertices(),
				pMesh->GetNumBytesPerVertex(),&vMin,&vMax); 
			
			vTransFormed.x=pMat->_11*vMin.x+pMat->_21*vMin.y+pMat->_31*vMin.z+pMat->_41;
			vTransFormed.y=pMat->_12*vMin.x+pMat->_22*vMin.y+pMat->_32*vMin.z+pMat->_42;
			vTransFormed.z=pMat->_13*vMin.x+pMat->_23*vMin.y+pMat->_33*vMin.z+pMat->_43;

			if(vTransFormed.x<pVmin->x) pVmin->x=vTransFormed.x;
			if(vTransFormed.y<pVmin->y) pVmin->y=vTransFormed.y;
			if(vTransFormed.z<pVmin->z) pVmin->z=vTransFormed.z;

			vTransFormed.x=pMat->_11*vMax.x+pMat->_21*vMax.y+pMat->_31*vMax.z+pMat->_41;
			vTransFormed.y=pMat->_12*vMax.x+pMat->_22*vMax.y+pMat->_32*vMax.z+pMat->_42;
			vTransFormed.z=pMat->_13*vMax.x+pMat->_23*vMax.y+pMat->_33*vMax.z+pMat->_43;

			if(vTransFormed.x>pVmax->x) pVmax->x=vTransFormed.x;
			if(vTransFormed.y>pVmax->y) pVmax->y=vTransFormed.y;
			if(vTransFormed.z>pVmax->z) pVmax->z=vTransFormed.z;
			pMesh->UnlockVertexBuffer(); 
		}
		//else return S_OK;
		
		if(pFrameParent->pFrameSibling)
			CalculateBondingBox(pFrameParent->pFrameSibling,pVmin,pVmax);
		
		if(pFrameParent->pFrameFirstChild)
			CalculateBondingBox(pFrameParent->pFrameFirstChild,pVmin,pVmax);
		return S_OK;
	
	}
	virtual ~CSkinMesh();
protected:
//	FLOAT						m_fRotateAngle;
	BOOL						m_bMoving;
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	LPD3DXFRAME					m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
    FLOAT						m_fElapsedTime;      // Time elapsed since last frame


	VOID DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	VOID DrawFrame(LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
};




class CSkinMeshObj : public CBase
{
public:
	CSkinMeshObj( CSkinMesh * pSkinMesh, D3DXVECTOR3 vPos, float fAngleY, float fScaling )
	{
		m_pSkinMesh = pSkinMesh;
		m_vPos = vPos;
		m_fAngle = fAngleY;
		m_fScaling = fScaling;

		m_dwType = OBJECT_TYEP_SKIN_MESH;
	}
	~CSkinMeshObj()
	{
	}


	void Render();

	void SetPos( D3DXVECTOR3 vPos )
	{
		m_vPos = vPos;
	}
	void SetPos( float x, float y, float z )
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void GetPos( D3DXVECTOR3 & vPos )
	{
		vPos = m_vPos;
	}
	D3DXVECTOR3 * GetPos()
	{
		return &m_vPos;
	}

	void SetRotationY( float fAngleY )
	{
		m_fAngle = fAngleY;
	}
	void RotationY( float fAngleY )
	{
		m_fAngle += fAngleY;
	}
	
	void SetScaling( float fScaling )
	{
		m_fScaling = fScaling;
	}

	void SetAction( CHAR * szName )
	{
		m_pSkinMesh->SetAnimationName( szName );
	}

protected:

	CSkinMesh *				m_pSkinMesh;

	D3DXVECTOR3				m_vPos;
	float					m_fAngle;
	float					m_fScaling;

};

#endif // !defined(AFX_SKINMESH_H__AE3ADAAF_4BE2_4B23_9A1D_9379FA1BAFD9__INCLUDED_)
