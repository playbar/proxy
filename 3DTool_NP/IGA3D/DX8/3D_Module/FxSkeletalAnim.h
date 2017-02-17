#ifndef FXSKELETALANIM_H
#define FXSKELETALANIM_H

#include <windows.h>
#include <d3dx8.h>
#include "FxXFileParser.h"


class D3DXMESHCONTAINER;

class D3DXFRAME
{
public:
	D3DXFRAME();
	~D3DXFRAME();
	D3DXFRAME* find(const char* frame_name);
	void reset();
	void update_hierarchy(D3DXMATRIX* mat_trans);
	void count(DWORD* num);

public:
	LPSTR                Name;
	D3DXMATRIX           TransformationMatrix;

	D3DXMESHCONTAINER    *pMeshContainer;

	class D3DXFRAME     *pFrameSibling;
	class D3DXFRAME     *pFrameFirstChild;

	D3DXMATRIX mat_combined;
	D3DXMATRIX mat_original;
};

class D3DXMESHCONTAINER
{
public:
	D3DXMESHCONTAINER();
	~D3DXMESHCONTAINER();

	D3DXMESHCONTAINER* find(const char* mesh_name);

public:
	LPSTR                    Name;

	IDirect3DTexture8**		 textures;
	LPD3DXMATERIAL           pMaterials;
	DWORD                    NumMaterials;
	DWORD                    *pAdjacency;

	LPSTR					 *ppBoneNames;
	D3DXMATRIX				 *pBoneTransforms;

	ID3DXSkinMesh*			 skin_mesh;
	LPD3DXMESH               pMesh;

	class D3DXMESHCONTAINER *pNextMeshContainer;

	D3DXMATRIX**			 frame_combined_matrices;
	D3DXMATRIX*				 bone_trans_matrices;
};


//=====================================================================================
// Animation key type data structures
//=====================================================================================
struct sAnimVectorKey
{
	DWORD		time;
	D3DXVECTOR3	vec;
};

struct sAnimQuatKey
{
	DWORD			time;
	D3DXQUATERNION	quat;
};

struct sAnimMatrixKey
{
	DWORD		time;
	D3DXMATRIX	matrix;
};


//=====================================================================================
// Animation structures
//=====================================================================================
struct sAnimation
{
	char*			bone_name;
	D3DXFRAME*		bone;		// pointer to bone frame
	sAnimation*		next;		// next animation object in list

	// each key type and array of each type's keys

	DWORD			num_translation_keys;
	sAnimVectorKey*	translation_keys;

	DWORD			num_scale_keys;
	sAnimVectorKey*	scale_keys;

	DWORD			num_rotation_keys;
	sAnimQuatKey*	rotation_keys;

	DWORD			num_matrix_keys;
	sAnimMatrixKey*	matrix_keys;

	sAnimation()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~sAnimation()
	{
		delete[] bone_name;	bone_name = NULL;
		delete[] translation_keys;
		delete[] scale_keys;
		delete[] rotation_keys;
		delete[] matrix_keys;	

		delete next;		next = NULL;		
	}
};

//=====================================================================================
// Animation set is container of animation.
//=====================================================================================
struct sAnimationSet
{
	char*			name;		// name of animation set
	DWORD			length;		// length of animation
	sAnimationSet*	next;

	DWORD			num_anims;
	sAnimation*		anims;

	sAnimationSet()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~sAnimationSet()
	{
		delete[] name;	name  = NULL;
		delete anims;	anims = NULL;
		delete next;	next  = NULL;
	}
};

//=====================================================================================
// Parse animation data from X file.
//=====================================================================================
class FxAnimationCollection : public cXParser
{
protected:
	DWORD			m_num_anim_sets;
	sAnimationSet*	m_anim_sets;

protected:
	virtual bool parse_objects(IDirectXFileData* xfile_data,
							   IDirectXFileData* parent_xfile_data,
							   DWORD  depth,
							   void** data,
							   bool   force_ref);

public:
	FxAnimationCollection()
	{
		m_num_anim_sets = 0;
		m_anim_sets		= NULL;
	}

	~FxAnimationCollection()
	{
		free();
	}

	void free()
	{
		m_num_anim_sets = 0;
		delete m_anim_sets; m_anim_sets = NULL;
	}

	bool load(const char* filename)
	{
		free();		// free a prior loaded collection

		return parse(filename, NULL);
	}

	void map_frames(D3DXFRAME* root_frame);
	void update(const char* anim_set_name, unsigned long time, bool is_loop);
};

#endif