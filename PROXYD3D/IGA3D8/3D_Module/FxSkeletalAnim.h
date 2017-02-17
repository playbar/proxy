#ifndef FXSKELETALANIM_H
#define FXSKELETALANIM_H

//#include <windows.h>
#include <d3dx8.h>
//#include "FxXFileParser.h"


class D3DXMeshContainer;

class CD3DXFrame
{
public:
	CD3DXFrame();
	~CD3DXFrame();
	CD3DXFrame* FindFrame(const char* frame_name);
	void Reset();
	void UpdateHierarchy(D3DXMATRIX* mat_trans);
	void CountFrame(DWORD* num);

public:
	LPSTR				m_lpName;
	D3DXMATRIX			m_TransformationMatrix;
	D3DXMATRIX			m_MatrixCombined;
	D3DXMATRIX			m_MatrixOriginal;

	D3DXMeshContainer   *m_pMeshContainer;
	CD3DXFrame			*m_pCD3DXFrameSibling;
	CD3DXFrame			*m_pCD3DXFrameFirstChild;
};

class D3DXMeshContainer
{
public:
	D3DXMeshContainer();
	~D3DXMeshContainer();

	D3DXMeshContainer* find(const char* mesh_name);

public:
	LPSTR                    m_lpName;

	IDirect3DTexture8	   **m_ppTextures;
	LPD3DXMATERIAL           m_pMaterials;
	DWORD                    m_dwNumMaterials;
	DWORD                   *m_pdwAdjacency;

	LPSTR					*m_lpBoneNames;
	D3DXMATRIX				*m_pBoneTransforms;

	ID3DXSkinMesh*			 m_pSkinMesh;
	LPD3DXMESH               m_pD3DMesh;

	D3DXMeshContainer	    *m_pNextMeshContainer;

	D3DXMATRIX			  **m_ppMatrixFrameCombined;
	D3DXMATRIX			    *m_pMatricxTrans;
};

#endif