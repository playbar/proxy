
#pragma once

#include "D3D.h"


struct D3DXFRAME_DERIVED : public D3DXFRAME               //扩展帧
{
    D3DXMATRIXA16        CombinedTransformationMatrix;//组合变换矩阵
};


struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER            //扩展Mesh容器
{
    LPDIRECT3DTEXTURE9*  ppTextures;       
                                
    // SkinMesh信息            
    LPD3DXMESH           pOrigMesh;
    LPD3DXATTRIBUTERANGE pAttributeTable;
    DWORD                NumAttributeGroups; 
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;
    D3DXMATRIX**         ppBoneMatrixPtrs;
    D3DXMATRIX*          pBoneOffsetMatrices;
    DWORD                NumPaletteEntries;
    bool                 UseSoftwareVP;
    DWORD                iAttributeSW;     //SW   HW
};

//skin模式
enum METHOD 
{
	D3DINDEXEDVS,
	D3DINDEXEDHLSLVS,
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    NONE
};


class CSkinMesh;
////////////////////////////////////////////////////////////////////////////////////////////


class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
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

    CAllocateHierarchy( CSkinMesh * p) 
	{
		m_pSkinMesh = p;
	}

	CSkinMesh * m_pSkinMesh;
};



////////////////////////////////////////////////////////////////////////////////////////////////////

class CSkinMesh  
{
public:

	CSkinMesh();


	VOID SetAnim( BOOL bAnim );
	BOOL InterSect( D3DVECTOR * pRayOrig, D3DVECTOR * pRayDir,D3DVECTOR * pVRet );
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );


	LPD3DXANIMATIONCONTROLLER GetAnimController()	//控制变换的动作
	{
		return m_pAnimController;
	}

	BOOL SetAnimationName( CHAR * strAnimName, LPD3DXANIMATIONCONTROLLER pAnimController );		//设置动作的名字
//	void SetPosition(D3DXVECTOR3 &vPos)			{m_vPos=vPos; };
//	void SetPosition(float x,float y,float z)	{m_vPos.x=x;m_vPos.y=y;m_vPos.z=z;};

	VOID	Render( LPD3DXANIMATIONCONTROLLER pAnimController, float fElapsedAppTime, D3DXVECTOR3 vPos, float angle, float scale = 1.0f );
	HRESULT LoadFromXFile( TCHAR * strFileName );
	HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_DERIVED * pMeshContainer );


	bool SetMaterial( D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower );
	//void CalculateBondingBox( LPD3DXFRAME pFrameParent, D3DXVECTOR3 * pVmin, D3DXVECTOR3 * pVmax );

	virtual ~CSkinMesh();

protected:

//	FLOAT						m_fRotateAngle;
//	BOOL						m_bMoving;
	LPDIRECT3DDEVICE9			m_pDevice;
	LPD3DXFRAME					m_pFrameRoot;
	

	LPD3DXMESH				m_pMesh;


	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	FLOAT						m_fElapsedTime;      //经过的时间，直到最后一帧


	VOID DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
	VOID DrawFrame( LPD3DXFRAME pFrame );
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );

public:

	LPDIRECT3DVERTEXSHADER9		m_pIndexedVertexShader[4];
	D3DXMATRIXA16 *				m_pBoneMatrices;

	UINT						m_NumBoneMatricesMax;
	METHOD						m_SkinningMethod;
	D3DCAPS9 					m_d3dCaps;
    D3DXVECTOR3                 m_vObjectCenter;    // 物体的范围中心
    FLOAT                       m_fObjectRadius;    // 物体的范围半径
	D3DXVECTOR3                 m_vMin;				// 物体的范围中心的半径的最小值
	D3DXVECTOR3                 m_vMax;				//物体的范围中心的半径的最大值

	D3DMATERIAL9				m_matMaterial;


	BOOL						m_bUseSoftwareVP;

};






///////////////////////////////////////////////////////////////////////////

class CSkinMeshObj : public CBase
{
public:
	CSkinMeshObj( CSkinMesh * pSkinMesh, D3DXVECTOR3 vPos, float fAngleY, float fScaling )
	{
		m_pSkinMesh = pSkinMesh;
		LPD3DXANIMATIONCONTROLLER p = pSkinMesh->GetAnimController();
		p->CloneAnimationController( p->GetMaxNumAnimationOutputs(), p->GetMaxNumAnimationSets(), p->GetMaxNumTracks(),
									p->GetMaxNumEvents(), &m_pAnimController );

		m_fTimeLast = APP_Tools::appGetTime();


		m_vPos = vPos;
		m_fAngle = fAngleY;
		m_fScaling = fScaling;

		m_dwCustomColor = 0xff000000;

		m_dwBaseType = OBJECT_TYEP_SKIN_MESH;
		m_pDevice = g_pMyD3D->GetDevice();

		SetCustomRenderState( COLOR_MODULATE_TEXTURE_CURRENT, ALPHA_SELECT_TEXTURE, ALPHABLEND_SRCALPHA_INVSRCALPHA,
			FILTER_LINEAR, CULL_CCW, TRUE, true, 0 );

		
		
	}
	CSkinMeshObj( D3DXVECTOR3 vPos, float fAngleY, float fScaling )
	{
		
		m_fTimeLast = APP_Tools::appGetTime();


		m_pSkinMesh = NULL;
		m_pAnimController = NULL;
		m_vPos = vPos;
		m_fAngle = fAngleY;
		m_fScaling = fScaling;

		m_dwCustomColor = 0xff000000;

		m_dwBaseType = OBJECT_TYEP_SKIN_MESH;
		m_pDevice = g_pMyD3D->GetDevice();

		SetCustomRenderState( COLOR_MODULATE_TEXTURE_CURRENT, ALPHA_SELECT_TEXTURE, ALPHABLEND_SRCALPHA_INVSRCALPHA,
			FILTER_LINEAR, CULL_CCW, TRUE,true, 0 );

		
	}
	~CSkinMeshObj()
	{
		SAFE_RELEASE( m_pAnimController );
	}


	void CSkinMeshObj::Render()
	{
		double fTimeNow = APP_Tools::appGetTime();

		if( !m_pSkinMesh )
			return;

		UseRenderState();

		m_pSkinMesh->Render( m_pAnimController, (float)(fTimeNow - m_fTimeLast), m_vPos, m_fAngle, m_fScaling );
		m_fTimeLast = fTimeNow;

	}

	
	void SetAction( CHAR * szName )
	{
		if( m_pSkinMesh )
			m_pSkinMesh->SetAnimationName( szName, m_pAnimController );
		//m_fTimeLast = APP::appGetTime();
	}

	void SetMesh( CSkinMesh * pSkinMesh )
	{
		m_pSkinMesh = pSkinMesh;
		SAFE_RELEASE( m_pAnimController );
		LPD3DXANIMATIONCONTROLLER p = pSkinMesh->GetAnimController();
		p->CloneAnimationController( p->GetMaxNumAnimationOutputs(), p->GetMaxNumAnimationSets(), p->GetMaxNumTracks(),
									p->GetMaxNumEvents(), &m_pAnimController );
	}

	bool RayCheck( D3DXVECTOR3 vPos, D3DXVECTOR3 vDir);
protected:
	
	CSkinMesh *				m_pSkinMesh;

	DWORD					m_dwCustomColor;

	double					m_fTimeLast;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;

};
