#include "stdafx.h"

#include <stdio.h>
#include "define.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <dxfile.h>
#include <rmxfguid.h>
#include "IGA3D8.h"
#include "FxXFileParser.h"
#include "FxSkeletalAnim.h"
#include "FxSkeletalMesh.h"

#pragma warning(disable : 4996)


#define PARSE_NONE			0
#define PARSE_MESH			1
#define PARSE_FRAME			2
#define PARSE_MESH_FRAME	3

//-------------------------------------------------------------------------------------------
// Declare an internal .X file parser class for loading meshes and frames.
//-------------------------------------------------------------------------------------------
extern unsigned char D3DRM_XTEMPLATES[];

CXInternalParser::CXInternalParser()
{
	ZeroMemory(this, sizeof(*this));
}

CXInternalParser::~CXInternalParser()
{
	delete m_root_mesh_container;	
	m_root_mesh_container = NULL;
	delete m_root_frame;			
	m_root_frame = NULL;
}

bool CXInternalParser::parse(char* filename, void** data)
{
	if(filename == NULL)
	{
		return false;
	}

	IDirectXFile* xfile;

	if(FAILED(DirectXFileCreate(&xfile)))
		return false;

	// register standard templates
	if(FAILED(xfile->RegisterTemplates((LPVOID) D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		xfile->Release();
		return false;
	}

	IDirectXFileEnumObject* xfile_enum;

	if(FAILED(xfile->CreateEnumObject(filename, DXFILELOAD_FROMFILE, &xfile_enum)))
	{
		xfile->Release();
		return false;
	}

	IDirectXFileData* xfile_data;
	while(SUCCEEDED(xfile_enum->GetNextDataObject(&xfile_data)))
	{
		parse_object(xfile_data, NULL, 0, data, false);
		RELEASE_COM(xfile_data);
	}

	RELEASE_COM(xfile_enum);
	RELEASE_COM(xfile);

	return true;
}

void CXInternalParser::parse_object(IDirectXFileData* xfile_data, 
									IDirectXFileData* parent_xfile_data, 
									DWORD depth, void** data, bool force_ref)
{
	const GUID *type;
	xfile_data->GetType(&type);

	// process templates based on their type
	if(*type == TID_D3DRMFrame && (m_flags & PARSE_FRAME) && force_ref == false)
	{
		CD3DXFrame* frame = new CD3DXFrame;
		frame->m_lpName = get_name(xfile_data);

		// link frame into hierarchy
		if(data == NULL)
		{
			// link as sibling of root
			frame->m_pCD3DXFrameSibling = m_root_frame;
			m_root_frame = frame;
			frame = NULL;

			data = (void**) &m_root_frame;
		}
		else
		{
			// link as child of supplied frame
			CD3DXFrame* frame_ptr		= (CD3DXFrame*)*data;
			frame->m_pCD3DXFrameSibling		= frame_ptr->m_pCD3DXFrameFirstChild;
			frame_ptr->m_pCD3DXFrameFirstChild = frame;
			frame = NULL;

			data = (void**)&frame_ptr->m_pCD3DXFrameFirstChild;
		}
	}
	else if(*type == TID_D3DRMFrameTransformMatrix && (m_flags & PARSE_FRAME) && data && force_ref == false)
	{
		CD3DXFrame* frame = (CD3DXFrame*)*data;

		if(frame)
		{
			char *DataPtr;
			DWORD DataSize;
			xfile_data->GetData(NULL, &DataSize, (void**)&DataPtr);

			frame->m_TransformationMatrix = *((const D3DXMATRIX*) DataPtr);
			frame->m_MatrixOriginal			= frame->m_TransformationMatrix;
		}
	}
	else if(*type == TID_D3DRMMesh && (m_flags & PARSE_MESH))	// load a mesh (skinned or regular)
	{
		if(force_ref == false)
		{
			D3DXMeshContainer* mesh_container;
			load_mesh(&mesh_container, m_device, xfile_data, m_texture_path, m_new_fvf, m_load_flags);

			// link mesh to head of list of meshes0...
			if(mesh_container)
			{
				mesh_container->m_pNextMeshContainer = m_root_mesh_container;
				m_root_mesh_container = mesh_container;
				mesh_container = NULL;

				// link mesh to frame if needed
				if(data)
				{
					CD3DXFrame* frame = (CD3DXFrame*) *data;

					if((m_flags & PARSE_FRAME) && frame)
						frame->m_pMeshContainer = m_root_mesh_container;
				}
			}
		}
		else	// referenced, then check if wanting to link to frame.
		{
			if(data)
			{
				CD3DXFrame* frame = (CD3DXFrame*) *data;

				if((m_flags & PARSE_FRAME) && m_root_mesh_container && frame)
				{
					char* name = get_name(xfile_data);

					if(name)
					{
						frame->m_pMeshContainer = m_root_mesh_container->find(name);

						delete[] name;
						name = NULL;
					}
				}
			}
		}
	}
	
	parse_child_object(xfile_data, depth, data, force_ref);
}

void CXInternalParser::parse_child_object(IDirectXFileData* xfile_data, 
										  DWORD depth, void** data, bool force_ref)
{
	IDirectXFileData* child_xfile_data;
	IDirectXFileObject* child_xfile_obj;
	while(SUCCEEDED(xfile_data->GetNextObject(&child_xfile_obj)))
	{
		IDirectXFileDataReference *pRef;
		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileDataReference, (void**)&pRef)))
		{
			pRef->Resolve(&child_xfile_data);
			force_ref = true;

			parse_object(child_xfile_data, xfile_data, depth + 1, data, force_ref);
			RELEASE_COM(child_xfile_data);
			RELEASE_COM(pRef);
		}

		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileData, (void**)&child_xfile_data)))
		{
			parse_object(child_xfile_data, xfile_data, depth + 1, data, force_ref);			
			RELEASE_COM(child_xfile_data);
		}

		RELEASE_COM(child_xfile_obj);
	}
}

bool CXInternalParser::load_mesh(D3DXMeshContainer** ret_mesh_container,
				  IDirect3DDevice8* device,
				  IDirectXFileData* xfile_data,				  
				  char* texture_path,
				  DWORD new_fvf,
				  DWORD load_flags)
{
	if(ret_mesh_container == NULL || device == NULL || xfile_data == NULL || texture_path == NULL)
		return false;

	*ret_mesh_container = NULL;

	ID3DXSkinMesh*	mesh = NULL;
	ID3DXBuffer*	material_buffer;
	ID3DXBuffer*	adj_buffer;
	ID3DXBuffer*    bone_names;
	ID3DXBuffer*    bone_transforms;
	DWORD			num_materials;	

	HRESULT hr =D3DXLoadSkinMeshFromXof(xfile_data, load_flags, device, 
										&adj_buffer, &material_buffer, &num_materials,
										&bone_names, &bone_transforms,
										&mesh);

	if(FAILED(hr))
		return false;

	//if(mesh && mesh->GetNumBones() == 0)
	//	release_com(mesh);

	D3DXMeshContainer* mesh_container = new D3DXMeshContainer;
	*ret_mesh_container = mesh_container;

	DWORD size;
	xfile_data->GetName(NULL, &size);
	if(size)
	{
		mesh_container->m_lpName = new char[size];
		xfile_data->GetName(mesh_container->m_lpName, &size);
	}

	DWORD adj_buffer_size = adj_buffer->GetBufferSize();
	if(adj_buffer_size)
	{
		mesh_container->m_pdwAdjacency = (DWORD*) new char[adj_buffer_size];
		memcpy(mesh_container->m_pdwAdjacency, adj_buffer->GetBufferPointer(), adj_buffer_size);
	}
	RELEASE_COM(adj_buffer);

	mesh_container->m_dwNumMaterials = num_materials;
	if(num_materials == 0)
	{
		mesh_container->m_dwNumMaterials = 1;
		mesh_container->m_pMaterials	 = new D3DXMATERIAL[1];
		mesh_container->m_ppTextures	 = new IDirect3DTexture8*[1];

		ZeroMemory(mesh_container->m_pMaterials, sizeof(D3DXMATERIAL));

		mesh_container->m_pMaterials[0].MatD3D.Diffuse.r = 1.0f;
		mesh_container->m_pMaterials[0].MatD3D.Diffuse.g = 1.0f;
		mesh_container->m_pMaterials[0].MatD3D.Diffuse.b = 1.0f;
		mesh_container->m_pMaterials[0].MatD3D.Diffuse.a = 1.0f;
		mesh_container->m_pMaterials[0].MatD3D.Ambient   = mesh_container->m_pMaterials[0].MatD3D.Diffuse;
		mesh_container->m_pMaterials[0].MatD3D.Specular  = mesh_container->m_pMaterials[0].MatD3D.Diffuse;
		mesh_container->m_pMaterials[0].pTextureFilename = NULL;

		mesh_container->m_ppTextures[0]					   = NULL;
	}
	else
	{
		D3DXMATERIAL* xmaterials   = (D3DXMATERIAL*) material_buffer->GetBufferPointer();
		mesh_container->m_pMaterials = new D3DXMATERIAL[mesh_container->m_dwNumMaterials];
		mesh_container->m_ppTextures   = new IDirect3DTexture8*[mesh_container->m_dwNumMaterials];

		for(DWORD i = 0; i < mesh_container->m_dwNumMaterials; i++)
		{
			mesh_container->m_pMaterials[i].MatD3D = xmaterials[i].MatD3D;
			mesh_container->m_pMaterials[i].MatD3D.Ambient = mesh_container->m_pMaterials[i].MatD3D.Diffuse;

			mesh_container->m_ppTextures[i] = NULL;

			if(xmaterials[i].pTextureFilename)
			{
				char texture_file[MAX_PATH];
				sprintf(texture_file, "%s%s", texture_path, xmaterials[i].pTextureFilename);
				GetModulePath(texture_file);
				D3DXCreateTextureFromFile(device, texture_file, &mesh_container->m_ppTextures[i]);
			}
		}
	}
	RELEASE_COM(material_buffer);

	DWORD nBones = mesh->GetNumBones();
	if( nBones > 0 )
	{
		mesh_container->m_pSkinMesh = mesh;
		mesh_container->m_pSkinMesh->GenerateSkinnedMesh(load_flags, 0.0f, mesh_container->m_pdwAdjacency, NULL, NULL, NULL, &mesh_container->m_pD3DMesh);

		

		LPSTR *ppBoneNames = (LPSTR*)bone_names->GetBufferPointer();
		mesh_container->m_lpBoneNames = new LPSTR[nBones];
		memcpy(mesh_container->m_lpBoneNames, ppBoneNames, nBones * sizeof(LPSTR));

		D3DXMATRIX *pBoneTransforms = (D3DXMATRIX*)bone_transforms->GetBufferPointer();
		mesh_container->m_pBoneTransforms = new D3DXMATRIX[nBones];
		memcpy(mesh_container->m_pBoneTransforms, pBoneTransforms, nBones * sizeof(D3DXMATRIX));
	}
	else
	{
		mesh->GetOriginalMesh(&mesh_container->m_pD3DMesh);
		RELEASE_COM(mesh);

		mesh_container->m_lpBoneNames = NULL;
		mesh_container->m_pBoneTransforms = NULL;
	}

	RELEASE_COM(bone_transforms);

	mesh_container = NULL;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

char* CXInternalParser::get_name(IDirectXFileData* xfile_data)
{
	if(xfile_data == NULL)
		return NULL;

	DWORD size;

	if(FAILED(xfile_data->GetName(NULL, &size)))
		return NULL;

	char* name = NULL;

	if(size > 1)
	{
		name = new char[size];
		xfile_data->GetName(name, &size);
	}

	return name;
}



//FxSkeletalMesh
CFxSkeletalMesh::CFxSkeletalMesh(IDirect3DDevice8 *pkD3DDevice)
{
	m_pkD3DDevice = pkD3DDevice;

	m_pkRootFrame = NULL;
	m_pkMeshContainer = NULL;
	m_pkAnimCollection = new FxAnimationCollection;

	m_pPickMesh = NULL;
}

CFxSkeletalMesh::~CFxSkeletalMesh()
{
	if( m_pPickMesh != NULL )
	{
		m_pPickMesh->Release();
		m_pPickMesh = NULL;
	}

	if( m_pkRootFrame != NULL )
	{
		delete m_pkRootFrame;
		m_pkRootFrame = NULL;
	}

	if( m_pkMeshContainer != NULL )
	{
		delete m_pkMeshContainer;
		m_pkMeshContainer = NULL;
	}

	if( m_pkAnimCollection != NULL )
	{
		delete m_pkAnimCollection;
		m_pkAnimCollection = NULL;
	}
}

bool CFxSkeletalMesh::LoadFromXFile(char* filename, char* texture_path, DWORD new_fvf, DWORD load_flags)
{
	if(m_pkD3DDevice == NULL || filename == NULL || texture_path == NULL)
	{
		OutputDebugString("filename == NULL || texture_path == NULL...");
		return false;
	}

	//先以普通网格的形式加载
	HRESULT hr = D3DXLoadMeshFromX( filename, D3DXMESH_MANAGED, m_pkD3DDevice, NULL, NULL, NULL, &m_pPickMesh );
	if( SUCCEEDED(hr) )
	{
		//OutputDebugString("普通网格加载成功！");
		LPD3DXMESH	pMesh = 0;
		hr = m_pPickMesh->CloneMeshFVF( D3DXMESH_MANAGED, D3DFVF_XYZ, m_pkD3DDevice, &pMesh );
		if( SUCCEEDED(hr) )
		{
			m_pPickMesh->Release();
			m_pPickMesh = pMesh;
			//OutputDebugString("克隆网格成功！");
		}
	}
	else
	{
		if( IS_3D_TOOL )
		{
			OutputDebugString("普通网格加载失败！");
		}
	}

	CXInternalParser parser;

	parser.m_device			= m_pkD3DDevice;
	parser.m_texture_path	= texture_path;
	parser.m_new_fvf		= new_fvf;
	parser.m_load_flags		= load_flags;
	parser.m_flags			= PARSE_MESH | PARSE_FRAME;

	parser.m_root_frame = NULL;
	parser.m_root_mesh_container = NULL;

	if(! parser.parse(filename, NULL))
	{
		if( IS_3D_TOOL )
		{
			OutputDebugString("解析文件错误...");
			OutputDebugString(filename);
		}
		return false;
	}


	// Map the matrices to the frames and create an array of bone matrices,
	// but only if user passed pointers to receive and the loader found some meshes and frames.
	if(parser.m_root_mesh_container && parser.m_root_frame)
	{
		m_pkMeshContainer = parser.m_root_mesh_container;

		while(m_pkMeshContainer)
		{
			// does this mesh use skinning?
			if(m_pkMeshContainer->m_pSkinMesh)
			{
				DWORD num_bones = m_pkMeshContainer->m_pSkinMesh->GetNumBones();

				// allocate the matrix pointers and bone matrices
				m_pkMeshContainer->m_ppMatrixFrameCombined = new D3DXMATRIX*[num_bones];
				m_pkMeshContainer->m_pMatricxTrans		= new D3DXMATRIX[num_bones];

				// match matrix poiners to frames
				for(DWORD i = 0; i < num_bones; i++)
				{
					const char* bone_name = m_pkMeshContainer->m_lpBoneNames[i];
					CD3DXFrame* frame   = parser.m_root_frame->FindFrame(bone_name);

					// match frame to bone
					if(frame)
						m_pkMeshContainer->m_ppMatrixFrameCombined[i] = &frame->m_MatrixCombined;
					else
						m_pkMeshContainer->m_ppMatrixFrameCombined[i] = NULL;
				}
			}

			// go to next mesh
			m_pkMeshContainer = m_pkMeshContainer->m_pNextMeshContainer;
		}
	}

	m_pkMeshContainer = parser.m_root_mesh_container;
	parser.m_root_mesh_container = NULL;

	m_pkRootFrame = parser.m_root_frame;
	parser.m_root_frame = NULL;

	if(!m_pkAnimCollection->LoadXFile(filename))
		return false;
	m_pkAnimCollection->MapFrames(m_pkRootFrame);


	return true;
}

void CFxSkeletalMesh::Update(unsigned long ulElapsedTime, bool bLoop /* = true */)
{
	if(m_pkMeshContainer == NULL)
		return;

	if(m_pkMeshContainer->m_pSkinMesh == NULL)
		return;

	if(m_pkMeshContainer->m_pMatricxTrans == NULL || m_pkMeshContainer->m_ppMatrixFrameCombined == NULL)
		return;

	m_pkAnimCollection->UpdateAnimationSet(NULL, ulElapsedTime, bLoop);

	if(m_pkRootFrame)
		m_pkRootFrame->UpdateHierarchy(NULL);

	for(DWORD i = 0; i < m_pkMeshContainer->m_pSkinMesh->GetNumBones(); i++)
	{
		m_pkMeshContainer->m_pMatricxTrans[i] = m_pkMeshContainer->m_pBoneTransforms[i];

		if(m_pkMeshContainer->m_ppMatrixFrameCombined[i])
			m_pkMeshContainer->m_pMatricxTrans[i] *= (*m_pkMeshContainer->m_ppMatrixFrameCombined[i]);
	}

	m_pkMeshContainer->m_pSkinMesh->UpdateSkinnedMesh(m_pkMeshContainer->m_pMatricxTrans, NULL, m_pkMeshContainer->m_pD3DMesh);
}


extern bool  g_bShowPickMesh;
void CFxSkeletalMesh::Render( LPDIRECT3DTEXTURE8 pTex )
{
	if(m_pkMeshContainer == NULL)
		return;

	if(m_pkMeshContainer->m_pD3DMesh == NULL)
		return;

	if(m_pkMeshContainer->m_dwNumMaterials == 0 || m_pkMeshContainer->m_pMaterials == NULL)
		return;

	DWORD fvf = m_pkMeshContainer->m_pD3DMesh->GetFVF();
	m_pkD3DDevice->SetVertexShader(fvf);

	//DWORD last_alpha_blend, old_alpha_blend, old_src_blend, old_dest_blend;

	//m_pkD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE,	&old_alpha_blend);
	//m_pkD3DDevice->GetRenderState(D3DRS_SRCBLEND,			&old_src_blend);
	//m_pkD3DDevice->GetRenderState(D3DRS_DESTBLEND,			&old_dest_blend);
	//last_alpha_blend = old_alpha_blend;

	for(DWORD i = 0; i < m_pkMeshContainer->m_dwNumMaterials; i++)
	{
		m_pkD3DDevice->SetMaterial(&m_pkMeshContainer->m_pMaterials[i].MatD3D);

		//----------------------------------------------------
		//模型纹理替换
		if( pTex == NULL )
		{
			m_pkD3DDevice->SetTexture(0, m_pkMeshContainer->m_ppTextures[i]);
		}
		else
		{
			if( i == 0 )
			{
				m_pkD3DDevice->SetTexture(0, pTex );
			}
			else
			{
				m_pkD3DDevice->SetTexture(0, m_pkMeshContainer->m_ppTextures[i]);
			}
		}

		m_pkMeshContainer->m_pD3DMesh->DrawSubset(i);

		if( g_bShowPickMesh && m_pPickMesh != NULL )
		{
			//OutputDebugString(".....");
			m_pkD3DDevice->SetTexture(0,0);
			m_pkD3DDevice->SetVertexShader(D3DFVF_XYZ);
			m_pPickMesh->DrawSubset(i);
		}

	}
}

float CFxSkeletalMesh::GetBoundingSphereRadius() const
{
	float fRadius = 0.0f;

	D3DXMeshContainer *pkTempMeshContainer = m_pkMeshContainer;
	while(pkTempMeshContainer)
	{
		ID3DXMesh *pkMesh = pkTempMeshContainer->m_pD3DMesh;

		if(pkMesh)
		{
			D3DXVECTOR3* vertices;
			D3DXVECTOR3  center;
			float		 radius;

			pkMesh->LockVertexBuffer(D3DLOCK_READONLY, (BYTE**)&vertices);

			D3DXComputeBoundingSphere(vertices, pkMesh->GetNumVertices(), D3DXGetFVFVertexSize(pkMesh->GetFVF()), &center, &radius);

			pkMesh->UnlockVertexBuffer();

			if(radius > fRadius)
				fRadius = radius;
		}

		pkTempMeshContainer = pkTempMeshContainer->m_pNextMeshContainer;
	}

	return fRadius;
}


//-----------------------------------------------------------------------------------------------------
//2008-7-22: 添加的有关拾取的代码，（射线拾取）
void CFxSkeletalMesh::GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const
{ 
	//D3DXFrameCalculateBoundingSphere(m_pkRootFrame, pObjectCenter, pfObjectRadius); 
}

void CFxSkeletalMesh::GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const
{
	D3DXVECTOR3 kCenter;
	float fRadius;
	GetBoundingSphereRadius(&kCenter, &fRadius);

	*pObjMin = kCenter - D3DXVECTOR3(fRadius, fRadius, fRadius);
	*pObjMax = kCenter + D3DXVECTOR3(fRadius, fRadius, fRadius);
}

bool CFxSkeletalMesh::Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, CD3DXFrame* pFrame, float* pfDist )
{
	BOOL bHit = 0;
	float _fDist = 0.0f;
	HRESULT hRes = D3DXIntersect(m_pPickMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, &_fDist, NULL, NULL);
	if( SUCCEEDED(hRes) )
	{
		//OutputDebugString("Pick operator is succeeded!");
		if( bHit && pfDist != NULL )
		{
			//OutputDebugString("Pick......");
			if( _fDist < *pfDist )
			{
				*pfDist = _fDist;
				return true;
			}
		}
	}
	return false;


	//if(!pFrame)
	//	pFrame = m_pkRootFrame;

	//bool bRet = false;
	//D3DXMESHCONTAINER* pMeshContainer = pFrame->pMeshContainer;
	//while (pMeshContainer != NULL)
	//{
	//	bRet = PickMeshContainer(pMeshContainer, pRayPos, pRayDir, pfDist);

	//	if(bRet)
	//		return true;
	//	else
	//		pMeshContainer = pMeshContainer->pNextMeshContainer;
	//}

	//if (pFrame->pFrameSibling != NULL)
	//{
	//	bRet = Pick(pRayPos, pRayDir, pFrame->pFrameSibling, pfDist);
	//	if(bRet)
	//		return true;
	//}

	//if (pFrame->pFrameFirstChild != NULL)
	//{
	//	bRet = Pick(pRayPos, pRayDir, pFrame->pFrameFirstChild);
	//	if(bRet)
	//		return true;
	//}

	//return false;
}

bool CFxSkeletalMesh::PickMeshContainer(D3DXMeshContainer* pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, float* pfDist)
{
	BOOL bHit = 0;
	float _fDist = 0.0f;
	HRESULT hRes = D3DXIntersect(pMeshContainer->m_pD3DMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, &_fDist, NULL, NULL);
	if( SUCCEEDED(hRes) )
	{
		//OutputDebugString("pick1..........");
		if( bHit && pfDist != NULL )
		{
			//OutputDebugString("pick..........");
			if( _fDist < *pfDist )
			{
				*pfDist = _fDist;
				return true;
			}
		}
	}
	return false; 
}
//2008-7-22  ↑
//-------------------------------------------------------------------------------
