#pragma  once

#include "FxSkeletalAnim.h"

class CXFileParser
{
protected:
	// functions called when parsing begins and ends
	virtual bool BeginParseXFile(void* data)	{ return true; }
	virtual bool EndParseXFile(void* data)		{ return true; }

	// function called for every template found
	virtual bool ParseObjects(IDirectXFileData* xfile_data,
							   IDirectXFileData* parent_xfile_data,
							   DWORD  depth,
							   void** data,
							   bool   force_ref)
	{
		return ParseChildObjects(xfile_data, depth, data, force_ref);
	}

	// function called to enumerate child objects
	bool ParseChildObjects(IDirectXFileData* xfile_data,
							 DWORD depth,
							 void** data,
							 bool   force_ref);

public:
	// function to start parsing an .X file
	bool Parse(const char* filename, void** data);

	// functions to help retrieve object information
	char* GetObjectName(IDirectXFileData* xfile_data);

};

//=====================================================================================
// Animation key type data structures
//=====================================================================================
struct SAnimVectorKey
{
	DWORD		time;
	D3DXVECTOR3	vec;
};

struct SAnimQuatKey
{
	DWORD			time;
	D3DXQUATERNION	quat;
};

struct SAnimMatrixKey
{
	DWORD		time;
	D3DXMATRIX	matrix;
};


//=====================================================================================
// Animation structures
//=====================================================================================
struct SAnimation
{
	char*			bone_name;
	CD3DXFrame*		bone;		// pointer to bone frame
	SAnimation*		next;		// next animation object in list

	// each key type and array of each type's keys

	DWORD			num_translation_keys;
	SAnimVectorKey*	translation_keys;

	DWORD			num_scale_keys;
	SAnimVectorKey*	scale_keys;

	DWORD			num_rotation_keys;
	SAnimQuatKey*	rotation_keys;

	DWORD			num_matrix_keys;
	SAnimMatrixKey*	matrix_keys;

	SAnimation()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~SAnimation()
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
struct SAnimationSet
{
	char*			name;		// name of animation set
	DWORD			length;		// length of animation
	SAnimationSet*	next;

	DWORD			num_anims;
	SAnimation*		anims;

	SAnimationSet()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~SAnimationSet()
	{
		delete[] name;	name  = NULL;
		delete anims;	anims = NULL;
		delete next;	next  = NULL;
	}
};

//=====================================================================================
// Parse animation data from X file.
//=====================================================================================
class FxAnimationCollection : public CXFileParser
{

protected:
	virtual bool ParseObjects(IDirectXFileData* xfile_data,
		IDirectXFileData* parent_xfile_data,
		DWORD  depth,
		void** data,
		bool   force_ref);

public:
	FxAnimationCollection()
	{
		m_dwNumAnimSet = 0;
		m_AnimationSet		= NULL;
	}

	~FxAnimationCollection()
	{
		FreeAnimtionSet();
	}

	void FreeAnimtionSet()
	{
		m_dwNumAnimSet = 0;
		if (NULL != m_AnimationSet)
		{
			delete m_AnimationSet;
			m_AnimationSet = NULL;
		}
		
	}

	bool LoadXFile(const char* filename)
	{
		FreeAnimtionSet();		// free a prior loaded collection

		return Parse(filename, NULL);
	}

	void MapFrames(CD3DXFrame* root_frame);
	void UpdateAnimationSet(const char* anim_set_name, unsigned long time, bool is_loop);

protected:
	DWORD			m_dwNumAnimSet;
	SAnimationSet*	m_AnimationSet;
};

