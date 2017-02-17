#include "stdafx.h"
#include <rmxftmpl.h>
#pragma warning(disable : 4996)


#define PARSE_NONE			0
#define PARSE_MESH			1
#define PARSE_FRAME			2
#define PARSE_MESH_FRAME	3

//-------------------------------------------------------------------------------------------
// Declare an internal .X file parser class for loading meshes and frames.
//-------------------------------------------------------------------------------------------
class cXInternalParser
{
public:
	// information passed from calling function
	IDirect3DDevice8*		m_device;
	/*const*/ char*				m_texture_path;
	DWORD					m_new_fvf;
	DWORD					m_load_flags;

	DWORD					m_flags;	// flags for which data to load: 1 = mesh, 2 = frames, 3 = both.

	// hierarchies used during loading
	D3DXMESHCONTAINER*	m_root_mesh_container;
	D3DXFRAME*			m_root_frame;

protected:
	void parse_object(IDirectXFileData* xfile_data,
		IDirectXFileData* parent_xfile_data,
		DWORD   depth,
		void**  data,
		bool	  force_ref);

	void parse_child_object(IDirectXFileData* xfile_data, DWORD depth, void** data, bool force_ref);

	bool load_mesh(D3DXMESHCONTAINER** ret_mesh_container,
		IDirect3DDevice8* device,
		IDirectXFileData* xfile_data,				  
		char* texture_path,
		DWORD new_fvf,
		DWORD load_flags);

public:
	cXInternalParser();
	~cXInternalParser();

	bool  parse(char* filename, void** data);
	char* get_name(IDirectXFileData* xfile_data);
};

cXInternalParser::cXInternalParser()
{
	ZeroMemory(this, sizeof(*this));
}

cXInternalParser::~cXInternalParser()
{
	delete m_root_mesh_container;	m_root_mesh_container = NULL;
	delete m_root_frame;			m_root_frame = NULL;
}

bool cXInternalParser::parse(char* filename, void** data)
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
		release_com(xfile_data);
	}

	release_com(xfile_enum);
	release_com(xfile);

	return true;
}

void cXInternalParser::parse_object(IDirectXFileData* xfile_data, 
									IDirectXFileData* parent_xfile_data, 
									DWORD depth, void** data, bool force_ref)
{
	const GUID *type;
	xfile_data->GetType(&type);

	// process templates based on their type
	if(*type == TID_D3DRMFrame && (m_flags & PARSE_FRAME) && force_ref == false)
	{
		D3DXFRAME* frame = new D3DXFRAME;
		frame->Name = get_name(xfile_data);

		// link frame into hierarchy
		if(data == NULL)
		{
			// link as sibling of root
			frame->pFrameSibling = m_root_frame;
			m_root_frame = frame;
			frame = NULL;

			data = (void**) &m_root_frame;
		}
		else
		{
			// link as child of supplied frame
			D3DXFRAME* frame_ptr		= (D3DXFRAME*)*data;
			frame->pFrameSibling		= frame_ptr->pFrameFirstChild;
			frame_ptr->pFrameFirstChild = frame;
			frame = NULL;

			data = (void**)&frame_ptr->pFrameFirstChild;
		}
	}
	else if(*type == TID_D3DRMFrameTransformMatrix && (m_flags & PARSE_FRAME) && data && force_ref == false)
	{
		D3DXFRAME* frame = (D3DXFRAME*)*data;

		if(frame)
		{
			char *DataPtr;
			DWORD DataSize;
			xfile_data->GetData(NULL, &DataSize, (void**)&DataPtr);

			frame->TransformationMatrix = *((const D3DXMATRIX*) DataPtr);
			frame->mat_original			= frame->TransformationMatrix;
		}
	}
	else if(*type == TID_D3DRMMesh && (m_flags & PARSE_MESH))	// load a mesh (skinned or regular)
	{
		if(force_ref == false)
		{
			D3DXMESHCONTAINER* mesh_container;
			load_mesh(&mesh_container, m_device, xfile_data, m_texture_path, m_new_fvf, m_load_flags);

			// link mesh to head of list of meshes0...
			if(mesh_container)
			{
				mesh_container->pNextMeshContainer = m_root_mesh_container;
				m_root_mesh_container = mesh_container;
				mesh_container = NULL;

				// link mesh to frame if needed
				if(data)
				{
					D3DXFRAME* frame = (D3DXFRAME*) *data;

					if((m_flags & PARSE_FRAME) && frame)
						frame->pMeshContainer = m_root_mesh_container;
				}
			}
		}
		else	// referenced, then check if wanting to link to frame.
		{
			if(data)
			{
				D3DXFRAME* frame = (D3DXFRAME*) *data;

				if((m_flags & PARSE_FRAME) && m_root_mesh_container && frame)
				{
					char* name = get_name(xfile_data);

					if(name)
					{
						frame->pMeshContainer = m_root_mesh_container->find(name);

						delete[] name;
						name = NULL;
					}
				}
			}
		}
	}
	
	parse_child_object(xfile_data, depth, data, force_ref);
}

void cXInternalParser::parse_child_object(IDirectXFileData* xfile_data, 
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
			release_com(child_xfile_data);
			release_com(pRef);
		}

		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileData, (void**)&child_xfile_data)))
		{
			parse_object(child_xfile_data, xfile_data, depth + 1, data, force_ref);			
			release_com(child_xfile_data);
		}

		release_com(child_xfile_obj);
	}
}

bool cXInternalParser::load_mesh(D3DXMESHCONTAINER** ret_mesh_container,
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

	D3DXMESHCONTAINER* mesh_container = new D3DXMESHCONTAINER;
	*ret_mesh_container = mesh_container;

	DWORD size;
	xfile_data->GetName(NULL, &size);
	if(size)
	{
		mesh_container->Name = new char[size];
		xfile_data->GetName(mesh_container->Name, &size);
	}

	DWORD adj_buffer_size = adj_buffer->GetBufferSize();
	if(adj_buffer_size)
	{
		mesh_container->pAdjacency = (DWORD*) new char[adj_buffer_size];
		memcpy(mesh_container->pAdjacency, adj_buffer->GetBufferPointer(), adj_buffer_size);
	}
	release_com(adj_buffer);

	mesh_container->NumMaterials = num_materials;
	if(num_materials == 0)
	{
		mesh_container->NumMaterials = 1;
		mesh_container->pMaterials	 = new D3DXMATERIAL[1];
		mesh_container->textures	 = new IDirect3DTexture8*[1];

		ZeroMemory(mesh_container->pMaterials, sizeof(D3DXMATERIAL));

		mesh_container->pMaterials[0].MatD3D.Diffuse.r = 1.0f;
		mesh_container->pMaterials[0].MatD3D.Diffuse.g = 1.0f;
		mesh_container->pMaterials[0].MatD3D.Diffuse.b = 1.0f;
		mesh_container->pMaterials[0].MatD3D.Diffuse.a = 1.0f;
		mesh_container->pMaterials[0].MatD3D.Ambient   = mesh_container->pMaterials[0].MatD3D.Diffuse;
		mesh_container->pMaterials[0].MatD3D.Specular  = mesh_container->pMaterials[0].MatD3D.Diffuse;
		mesh_container->pMaterials[0].pTextureFilename = NULL;

		mesh_container->textures[0]					   = NULL;
	}
	else
	{
		D3DXMATERIAL* xmaterials   = (D3DXMATERIAL*) material_buffer->GetBufferPointer();
		mesh_container->pMaterials = new D3DXMATERIAL[mesh_container->NumMaterials];
		mesh_container->textures   = new IDirect3DTexture8*[mesh_container->NumMaterials];

		for(DWORD i = 0; i < mesh_container->NumMaterials; i++)
		{
			mesh_container->pMaterials[i].MatD3D = xmaterials[i].MatD3D;
			mesh_container->pMaterials[i].MatD3D.Ambient = mesh_container->pMaterials[i].MatD3D.Diffuse;

			mesh_container->textures[i] = NULL;

			if(xmaterials[i].pTextureFilename)
			{
				char texture_file[MAX_PATH];
				sprintf(texture_file, "%s%s", texture_path, xmaterials[i].pTextureFilename);
				D3DXCreateTextureFromFile(device, texture_file, &mesh_container->textures[i]);
			}
		}
	}
	release_com(material_buffer);
	
	DWORD nBones = mesh->GetNumBones();
	if( nBones > 0 )
	{
		mesh_container->skin_mesh = mesh;
		mesh_container->skin_mesh->GenerateSkinnedMesh(load_flags, 0.0f, mesh_container->pAdjacency, NULL, NULL, NULL, &mesh_container->pMesh);

		

		LPSTR *ppBoneNames = (LPSTR*)bone_names->GetBufferPointer();
		mesh_container->ppBoneNames = new LPSTR[nBones];
		memcpy(mesh_container->ppBoneNames, ppBoneNames, nBones * sizeof(LPSTR));

		D3DXMATRIX *pBoneTransforms = (D3DXMATRIX*)bone_transforms->GetBufferPointer();
		mesh_container->pBoneTransforms = new D3DXMATRIX[nBones];
		memcpy(mesh_container->pBoneTransforms, pBoneTransforms, nBones * sizeof(D3DXMATRIX));
	}
	else
	{
		mesh->GetOriginalMesh(&mesh_container->pMesh);
		release_com(mesh);

		mesh_container->ppBoneNames = NULL;
		mesh_container->pBoneTransforms = NULL;
	}

	release_com(bone_transforms);


	mesh_container = NULL;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

char* cXInternalParser::get_name(IDirectXFileData* xfile_data)
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
FxSkeletalMesh::FxSkeletalMesh(IDirect3DDevice8 *pkD3DDevice)
{
	m_pkD3DDevice = pkD3DDevice;

	m_pkRootFrame = NULL;
	m_pkMeshContainer = NULL;
	m_pkAnimCollection = new FxAnimationCollection;
}

FxSkeletalMesh::~FxSkeletalMesh()
{
	delete m_pkRootFrame;
	delete m_pkMeshContainer;
	delete m_pkAnimCollection;
}

bool FxSkeletalMesh::LoadFromXFile(char* filename, char* texture_path, DWORD new_fvf, DWORD load_flags)
{
	if(m_pkD3DDevice == NULL || filename == NULL || texture_path == NULL)
	{
		OutputDebugString("filename == NULL || texture_path == NULL...");
		return false;
	}

	cXInternalParser parser;

	parser.m_device			= m_pkD3DDevice;
	parser.m_texture_path	= texture_path;
	parser.m_new_fvf		= new_fvf;
	parser.m_load_flags		= load_flags;
	parser.m_flags			= PARSE_MESH | PARSE_FRAME;

	parser.m_root_frame = NULL;
	parser.m_root_mesh_container = NULL;

	if(! parser.parse(filename, NULL))
	{
		OutputDebugString("解析文件错误...");
		OutputDebugString(filename);
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
			if(m_pkMeshContainer->skin_mesh)
			{
				DWORD num_bones = m_pkMeshContainer->skin_mesh->GetNumBones();

				// allocate the matrix pointers and bone matrices
				m_pkMeshContainer->frame_combined_matrices = new D3DXMATRIX*[num_bones];
				m_pkMeshContainer->bone_trans_matrices		= new D3DXMATRIX[num_bones];

				// match matrix poiners to frames
				for(DWORD i = 0; i < num_bones; i++)
				{
					const char* bone_name = m_pkMeshContainer->ppBoneNames[i];
					D3DXFRAME* frame   = parser.m_root_frame->find(bone_name);

					// match frame to bone
					if(frame)
						m_pkMeshContainer->frame_combined_matrices[i] = &frame->mat_combined;
					else
						m_pkMeshContainer->frame_combined_matrices[i] = NULL;
				}
			}

			// go to next mesh
			m_pkMeshContainer = m_pkMeshContainer->pNextMeshContainer;
		}
	}

	m_pkMeshContainer = parser.m_root_mesh_container;
	parser.m_root_mesh_container = NULL;

	m_pkRootFrame = parser.m_root_frame;
	parser.m_root_frame = NULL;

	if(!m_pkAnimCollection->load(filename))
		return false;
	m_pkAnimCollection->map_frames(m_pkRootFrame);

	return true;
}

void FxSkeletalMesh::Update(unsigned long ulElapsedTime, bool bLoop /* = true */)
{
	if(m_pkMeshContainer == NULL)
		return;

	if(m_pkMeshContainer->skin_mesh == NULL)
		return;

	if(m_pkMeshContainer->bone_trans_matrices == NULL || m_pkMeshContainer->frame_combined_matrices == NULL)
		return;

	m_pkAnimCollection->update(NULL, ulElapsedTime, bLoop);

	if(m_pkRootFrame)
		m_pkRootFrame->update_hierarchy(NULL);

	for(DWORD i = 0; i < m_pkMeshContainer->skin_mesh->GetNumBones(); i++)
	{
		m_pkMeshContainer->bone_trans_matrices[i] = m_pkMeshContainer->pBoneTransforms[i];

		if(m_pkMeshContainer->frame_combined_matrices[i])
			m_pkMeshContainer->bone_trans_matrices[i] *= (*m_pkMeshContainer->frame_combined_matrices[i]);
	}

	m_pkMeshContainer->skin_mesh->UpdateSkinnedMesh(m_pkMeshContainer->bone_trans_matrices, NULL, m_pkMeshContainer->pMesh);
}

void FxSkeletalMesh::Render( LPDIRECT3DTEXTURE8 pTex )
{
	if(m_pkMeshContainer == NULL)
		return;

	if(m_pkMeshContainer->pMesh == NULL)
		return;

	if(m_pkMeshContainer->NumMaterials == 0 || m_pkMeshContainer->pMaterials == NULL)
		return;

	DWORD fvf = m_pkMeshContainer->pMesh->GetFVF();
	m_pkD3DDevice->SetVertexShader(fvf);

	DWORD last_alpha_blend, old_alpha_blend, old_src_blend, old_dest_blend;

	m_pkD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE,	&old_alpha_blend);
	m_pkD3DDevice->GetRenderState(D3DRS_SRCBLEND,			&old_src_blend);
	m_pkD3DDevice->GetRenderState(D3DRS_DESTBLEND,			&old_dest_blend);
	last_alpha_blend = old_alpha_blend;

	for(DWORD i = 0; i < m_pkMeshContainer->NumMaterials; i++)
	{
		m_pkD3DDevice->SetMaterial(&m_pkMeshContainer->pMaterials[i].MatD3D);

		//----------------------------------------------------
		//模型纹理替换
		if( pTex == NULL )
		{
			m_pkD3DDevice->SetTexture(0, m_pkMeshContainer->textures[i]);
		}
		else
		{
			if( i == 0 )
			{
				m_pkD3DDevice->SetTexture(0, pTex );
			}
			else
			{
				m_pkD3DDevice->SetTexture(0, m_pkMeshContainer->textures[i]);
			}
		}



		if(m_pkMeshContainer->pMaterials[i].MatD3D.Diffuse.a != 1.0f)
		{
			if(last_alpha_blend != TRUE)
			{
				last_alpha_blend = TRUE;

				m_pkD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	TRUE);
				m_pkD3DDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);	// src color
				m_pkD3DDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_DESTCOLOR);
			}
		}
		else
		{
			if(last_alpha_blend != FALSE)
			{
				last_alpha_blend = FALSE;
				m_pkD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
		}

		m_pkMeshContainer->pMesh->DrawSubset(i);
	}

	if(last_alpha_blend != old_alpha_blend)
	{
		m_pkD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alpha_blend);
		m_pkD3DDevice->SetRenderState(D3DRS_SRCBLEND,		   old_src_blend);
		m_pkD3DDevice->SetRenderState(D3DRS_DESTBLEND,		   old_dest_blend);
	}
}

float FxSkeletalMesh::GetBoundingSphereRadius() const
{
	float fRadius = 0.0f;

	D3DXMESHCONTAINER *pkTempMeshContainer = m_pkMeshContainer;
	while(pkTempMeshContainer)
	{
		ID3DXMesh *pkMesh = pkTempMeshContainer->pMesh;

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

		pkTempMeshContainer = pkTempMeshContainer->pNextMeshContainer;
	}

	return fRadius;
}


//-----------------------------------------------------------------------------------------------------
//2008-7-22: 添加的有关拾取的代码，（射线拾取）
void FxSkeletalMesh::GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const
{ 
	//D3DXFrameCalculateBoundingSphere(m_pkRootFrame, pObjectCenter, pfObjectRadius); 
}

void FxSkeletalMesh::GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const
{
	D3DXVECTOR3 kCenter;
	float fRadius;
	GetBoundingSphereRadius(&kCenter, &fRadius);

	*pObjMin = kCenter - D3DXVECTOR3(fRadius, fRadius, fRadius);
	*pObjMax = kCenter + D3DXVECTOR3(fRadius, fRadius, fRadius);
}

bool FxSkeletalMesh::Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, D3DXFRAME* pFrame)
{
	if(!pFrame)
		pFrame = m_pkRootFrame;

	bool bRet = false;
	D3DXMESHCONTAINER* pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		bRet = PickMeshContainer(pMeshContainer, pRayPos, pRayDir);

		if(bRet)
			return true;
		else
			pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		bRet = Pick(pRayPos, pRayDir, pFrame->pFrameSibling);
		if(bRet)
			return true;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		bRet = Pick(pRayPos, pRayDir, pFrame->pFrameFirstChild);
		if(bRet)
			return true;
	}

	return false;
}

bool FxSkeletalMesh::PickMeshContainer(D3DXMESHCONTAINER* pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir)
{
	BOOL bHit = 0;
	HRESULT hRes = D3DXIntersect(pMeshContainer->pMesh, pRayPos, pRayDir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);

	return bHit != 0; 
}
//2008-7-22  ↑
//-------------------------------------------------------------------------------
