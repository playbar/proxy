
#include "stdafx.h"
#include "guiddef.h"
#include "tchar.h"
#include "define.h"
#include "z_FileIni.h"
#include "FxSkeletalAnim.h"

#pragma warning(disable : 4996)

CD3DXFrame::CD3DXFrame()
{
	m_lpName = NULL;
	m_pMeshContainer = NULL;
	m_pCD3DXFrameSibling =NULL;
	m_pCD3DXFrameFirstChild = NULL;

	D3DXMatrixIdentity(&m_MatrixCombined);
	D3DXMatrixIdentity(&m_MatrixOriginal);
	D3DXMatrixIdentity(&m_TransformationMatrix);
}

CD3DXFrame::~CD3DXFrame()
{
	if ( NULL != m_lpName )
	{
		delete[] m_lpName;				
		m_lpName = NULL;
	}
	if ( NULL != m_pCD3DXFrameSibling)
	{
		delete m_pCD3DXFrameSibling;		
		m_pCD3DXFrameSibling = NULL;
	}
	if ( NULL != m_pCD3DXFrameFirstChild )
	{
		delete m_pCD3DXFrameFirstChild;	
		m_pCD3DXFrameFirstChild = NULL;
	}
}

// Function to scan hierarchy for matching frame name
CD3DXFrame* CD3DXFrame::FindFrame(const char* frame_name)
{
	// return this frame instance if name matched
	if(m_lpName && frame_name && !strcmp(frame_name, m_lpName))
		return this;

	if(m_pCD3DXFrameSibling)	// scan siblings
	{
		CD3DXFrame* frame = ((CD3DXFrame*) m_pCD3DXFrameSibling)->FindFrame(frame_name);

		if(frame)		// If found then return, if no found then search child frames continuously.
			return frame;			
	}

	if(m_pCD3DXFrameFirstChild)	// scan children
		return ((CD3DXFrame*) m_pCD3DXFrameFirstChild)->FindFrame(frame_name);

	return NULL;	// no found		
}

// reset transformation matrices to originals
void CD3DXFrame::Reset()
{
	m_TransformationMatrix = m_MatrixOriginal;

	if(m_pCD3DXFrameSibling)
		((CD3DXFrame*) m_pCD3DXFrameSibling)->Reset();

	if(m_pCD3DXFrameFirstChild)
		((CD3DXFrame*) m_pCD3DXFrameFirstChild)->Reset();
}

// function to combine matrices in frame hierarchy
void CD3DXFrame::UpdateHierarchy(D3DXMATRIX* mat_trans)
{
	D3DXMATRIX mat_identity;

	// use an identity matrix if none passed
	if(mat_trans == NULL)
	{			
		D3DXMatrixIdentity(&mat_identity);
		mat_trans = &mat_identity;
	}

	// combine matrices with supplied transformation matrix
	m_MatrixCombined = m_TransformationMatrix * (*mat_trans);

	// combine with sibling frames
	if(m_pCD3DXFrameSibling)
		((CD3DXFrame*) m_pCD3DXFrameSibling)->UpdateHierarchy(mat_trans);

	// combine with child frames
	if(m_pCD3DXFrameFirstChild)
		((CD3DXFrame*) m_pCD3DXFrameFirstChild)->UpdateHierarchy(&m_MatrixCombined);
}

void CD3DXFrame::CountFrame(DWORD* num)
{
	if(num == NULL)	// error checking
		return;

	(*num) += 1;	// increase count of frames

	// process sibling frames
	if(m_pCD3DXFrameSibling)
		((CD3DXFrame*) m_pCD3DXFrameSibling)->CountFrame(num);

	// process child frames
	if(m_pCD3DXFrameFirstChild)
		((CD3DXFrame*) m_pCD3DXFrameFirstChild)->CountFrame(num);
}


D3DXMeshContainer::D3DXMeshContainer()
{
	ZeroMemory(this, sizeof(*this));
}

D3DXMeshContainer::~D3DXMeshContainer()
{
	if(m_ppTextures && m_dwNumMaterials)
	{
		for(DWORD i = 0; i < m_dwNumMaterials; i++)
			RELEASE_COM(m_ppTextures[i]);
	}

	delete[] m_ppTextures;					m_ppTextures = NULL;
	m_dwNumMaterials = 0;

	delete[] m_lpName;						m_lpName = NULL;
	delete[] m_pMaterials;				m_pMaterials = NULL;

	delete[] m_pdwAdjacency;				m_pdwAdjacency = NULL;
	delete[] m_ppMatrixFrameCombined;	m_ppMatrixFrameCombined = NULL;
	delete[] m_pMatricxTrans;		m_pMatricxTrans = NULL;

	delete[] m_lpBoneNames;
	delete[] m_pBoneTransforms;

	RELEASE_COM(m_pD3DMesh);
	RELEASE_COM(m_pSkinMesh);

	delete m_pNextMeshContainer;	
	m_pNextMeshContainer = NULL;
}

D3DXMeshContainer* D3DXMeshContainer::find(const char* mesh_name)
{
	// return this mesh instance if name matched
	if(m_lpName && mesh_name && !strcmp(mesh_name, m_lpName))
		return this;

	// scan next in list
	if(m_pNextMeshContainer)
		return ((D3DXMeshContainer*) m_pNextMeshContainer)->find(mesh_name);

	return NULL;
}


