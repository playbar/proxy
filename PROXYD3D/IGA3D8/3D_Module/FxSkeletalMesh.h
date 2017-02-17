#pragma  once 

class CD3DXFrame;
class D3DXMeshContainer;
class FxAnimationCollection;

class CXInternalParser
{
public:
	// information passed from calling function
	IDirect3DDevice8*		m_device;
	/*const*/ char*				m_texture_path;
	DWORD					m_new_fvf;
	DWORD					m_load_flags;

	DWORD					m_flags;	// flags for which data to load: 1 = mesh, 2 = frames, 3 = both.

	// hierarchies used during loading
	D3DXMeshContainer*	m_root_mesh_container;
	CD3DXFrame*			m_root_frame;

protected:
	void parse_object(IDirectXFileData* xfile_data,
		IDirectXFileData* parent_xfile_data,
		DWORD   depth,
		void**  data,
		bool	  force_ref);

	void parse_child_object(IDirectXFileData* xfile_data, DWORD depth, void** data, bool force_ref);

	bool load_mesh(D3DXMeshContainer** ret_mesh_container,
		IDirect3DDevice8* device,
		IDirectXFileData* xfile_data,				  
		char* texture_path,
		DWORD new_fvf,
		DWORD load_flags);

public:
	CXInternalParser();
	~CXInternalParser();

	bool  parse(char* filename, void** data);
	char* get_name(IDirectXFileData* xfile_data);
};

class CFxSkeletalMesh
{
public:
	CFxSkeletalMesh(IDirect3DDevice8 *pkD3DDevice);
	~CFxSkeletalMesh();
	bool LoadFromXFile(char* filename, char* texture_path, DWORD new_fvf, DWORD load_flags);
	void Update(unsigned long ulElapsedTime, bool bLoop = true);
	void Render( LPDIRECT3DTEXTURE8 pTex );
	float GetBoundingSphereRadius() const;

	//------------------------
	//拾取相关的代码
	//2008-7-23
	void GetBoundingSphereRadius(LPD3DXVECTOR3 pObjectCenter, float *pfObjectRadius) const;
	void GetBoundingBox(D3DXVECTOR3 *pObjMin, D3DXVECTOR3 *pObjMax) const;		//note: get a loose bounding box
	bool Pick(const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir, CD3DXFrame* pFrame = NULL, float* pfDist = NULL );
	bool PickMeshContainer(D3DXMeshContainer* pMeshContainer, const D3DXVECTOR3 *pRayPos, const D3DXVECTOR3 *pRayDir,  float* pfDist = NULL );
	//2008-7-23
	//------------------

protected:
	IDirect3DDevice8		*m_pkD3DDevice;
	CD3DXFrame				*m_pkRootFrame;
	D3DXMeshContainer		*m_pkMeshContainer;
	FxAnimationCollection	*m_pkAnimCollection;
	LPD3DXMESH				 m_pPickMesh;

};
