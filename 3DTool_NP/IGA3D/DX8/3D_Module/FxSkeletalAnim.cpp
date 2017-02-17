#include "guiddef.h"
#include "stdafx.h"

#pragma warning(disable : 4996)


D3DXFRAME::D3DXFRAME()
{
	Name = NULL;
	pMeshContainer = NULL;
	pFrameSibling = pFrameFirstChild = NULL;

	D3DXMatrixIdentity(&mat_combined);
	D3DXMatrixIdentity(&mat_original);
	D3DXMatrixIdentity(&TransformationMatrix);
}

D3DXFRAME::~D3DXFRAME()
{
	delete[] Name;				Name = NULL;
	delete pFrameSibling;		pFrameSibling = NULL;
	delete pFrameFirstChild;	pFrameFirstChild = NULL;
}

// Function to scan hierarchy for matching frame name
D3DXFRAME* D3DXFRAME::find(const char* frame_name)
{
	// return this frame instance if name matched
	if(Name && frame_name && !strcmp(frame_name, Name))
		return this;

	if(pFrameSibling)	// scan siblings
	{
		D3DXFRAME* frame = ((D3DXFRAME*) pFrameSibling)->find(frame_name);

		if(frame)		// If found then return, if no found then search child frames continuously.
			return frame;			
	}

	if(pFrameFirstChild)	// scan children
		return ((D3DXFRAME*) pFrameFirstChild)->find(frame_name);

	return NULL;	// no found		
}

// reset transformation matrices to originals
void D3DXFRAME::reset()
{
	TransformationMatrix = mat_original;

	if(pFrameSibling)
		((D3DXFRAME*) pFrameSibling)->reset();

	if(pFrameFirstChild)
		((D3DXFRAME*) pFrameFirstChild)->reset();
}

// function to combine matrices in frame hierarchy
void D3DXFRAME::update_hierarchy(D3DXMATRIX* mat_trans)
{
	D3DXMATRIX mat_identity;

	// use an identity matrix if none passed
	if(mat_trans == NULL)
	{			
		D3DXMatrixIdentity(&mat_identity);
		mat_trans = &mat_identity;
	}

	// combine matrices with supplied transformation matrix
	mat_combined = TransformationMatrix * (*mat_trans);

	// combine with sibling frames
	if(pFrameSibling)
		((D3DXFRAME*) pFrameSibling)->update_hierarchy(mat_trans);

	// combine with child frames
	if(pFrameFirstChild)
		((D3DXFRAME*) pFrameFirstChild)->update_hierarchy(&mat_combined);
}

void D3DXFRAME::count(DWORD* num)
{
	if(num == NULL)	// error checking
		return;

	(*num) += 1;	// increase count of frames

	// process sibling frames
	if(pFrameSibling)
		((D3DXFRAME*) pFrameSibling)->count(num);

	// process child frames
	if(pFrameFirstChild)
		((D3DXFRAME*) pFrameFirstChild)->count(num);
}


D3DXMESHCONTAINER::D3DXMESHCONTAINER()
{
	ZeroMemory(this, sizeof(*this));
}

D3DXMESHCONTAINER::~D3DXMESHCONTAINER()
{
	if(textures && NumMaterials)
	{
		for(DWORD i = 0; i < NumMaterials; i++)
			release_com(textures[i]);
	}

	delete[] textures;					textures = NULL;
	NumMaterials = 0;

	delete[] Name;						Name = NULL;
	delete[] pMaterials;				pMaterials = NULL;

	delete[] pAdjacency;				pAdjacency = NULL;
	delete[] frame_combined_matrices;	frame_combined_matrices = NULL;
	delete[] bone_trans_matrices;		bone_trans_matrices = NULL;

	delete[] ppBoneNames;
	delete[] pBoneTransforms;

	release_com(pMesh);
	release_com(skin_mesh);

	delete pNextMeshContainer;	
	pNextMeshContainer = NULL;
}

D3DXMESHCONTAINER* D3DXMESHCONTAINER::find(const char* mesh_name)
{
	// return this mesh instance if name matched
	if(Name && mesh_name && !strcmp(mesh_name, Name))
		return this;

	// scan next in list
	if(pNextMeshContainer)
		return ((D3DXMESHCONTAINER*) pNextMeshContainer)->find(mesh_name);

	return NULL;
}


bool FxAnimationCollection::parse_objects(IDirectXFileData* xfile_data, IDirectXFileData* parent_xfile_data, DWORD depth, void** data, bool force_ref)
{
	const GUID *type;
	xfile_data->GetType(&type);

	char *pObjectName = get_object_name(xfile_data);

	if(*type == TID_D3DRMAnimationSet)	
	{
		// create and link in a sAnimationSet object
		sAnimationSet* anim_set = new sAnimationSet;
		anim_set->next = m_anim_sets;
		m_anim_sets = anim_set;

		m_num_anim_sets++;

		anim_set->name = get_object_name(xfile_data);
	}
	else if(*type == TID_D3DRMAnimation && m_anim_sets)
	{
		// add a sAnimation to top-level sAnimationSet
		sAnimation* anim = new sAnimation;
		anim->next = m_anim_sets->anims;
		m_anim_sets->anims = anim;

		m_anim_sets->num_anims++;
	}
	else if(*type == TID_D3DRMFrame && force_ref == true && m_anim_sets && m_anim_sets->anims)
	{
		// a frame reference inside animation template

		if(parent_xfile_data)
		{
			//GUID parent_type;
			//get_object_guid(parent_xfile_data, &parent_type);
			const GUID *parent_type;
			parent_xfile_data->GetType(&parent_type);

			// make sure parent object is an animation template
			if(*parent_type == TID_D3DRMAnimation)
				m_anim_sets->anims->bone_name = get_object_name(xfile_data);
		}

		return true;	// do not process child of reference frames
	}
	else if(*type == TID_D3DRMAnimationKey && m_anim_sets && m_anim_sets->anims)
	{
		sAnimation* anim = m_anim_sets->anims;

		//SIZE_T size;
		DWORD* data_ptr;
		char *DataPtr;
		DWORD DataSize;
		xfile_data->GetData(NULL, &DataSize, (void**)&DataPtr);
		//xfile_data->Lock(&size, (LPCVOID*) &data_ptr);
		
		data_ptr = (DWORD*)DataPtr;
		DWORD type	   = *data_ptr++;
		DWORD num_keys = *data_ptr++;

		// branch based on key type
		switch(type)
		{
		case 0:	// rotation
			delete[] anim->rotation_keys;
			anim->num_rotation_keys = num_keys;
			anim->rotation_keys		= new sAnimQuatKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->rotation_keys[i].time = *data_ptr++;

				if(anim->rotation_keys[i].time > m_anim_sets->length)
					m_anim_sets->length = anim->rotation_keys[i].time;

				data_ptr++;		// skip number of keys to follow (should be 4)	

				// quaternion data stored with w,x,y,z order in xfile, so can not cast directly to assigned!
				
				float* float_ptr = (float*) data_ptr;

				anim->rotation_keys[i].quat.w = *float_ptr++;
				anim->rotation_keys[i].quat.x = *float_ptr++;
				anim->rotation_keys[i].quat.y = *float_ptr++;
				anim->rotation_keys[i].quat.z = *float_ptr++;

				data_ptr += 4;
			}

			break;

		case 1:	// scaling
			delete[] anim->scale_keys;
			anim->num_scale_keys = num_keys;
			anim->scale_keys = new sAnimVectorKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->scale_keys[i].time = *data_ptr++;

				if(anim->scale_keys[i].time > m_anim_sets->length)
					m_anim_sets->length = anim->scale_keys[i].time;

				data_ptr++;		// skip number of keys to follow (should be 3)			
				anim->scale_keys[i].vec = *((D3DXVECTOR3*) data_ptr);
				data_ptr += 3;
			}

			break;

		case 2:	// translation
			delete[] anim->translation_keys;
			anim->num_translation_keys = num_keys;
			anim->translation_keys = new sAnimVectorKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->translation_keys[i].time = *data_ptr++;

				if(anim->translation_keys[i].time > m_anim_sets->length)
					m_anim_sets->length = anim->translation_keys[i].time;

				data_ptr++;		// skip number of keys to follow (should be 3)			
				anim->translation_keys[i].vec = *((D3DXVECTOR3*) data_ptr);
				data_ptr += 3;
			}

			break;

		case 4:	// transformation matrix
			delete[] anim->matrix_keys;
			anim->num_matrix_keys = num_keys;
			anim->matrix_keys = new sAnimMatrixKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->matrix_keys[i].time = *data_ptr++;

				if(anim->matrix_keys[i].time > m_anim_sets->length)
					m_anim_sets->length = anim->matrix_keys[i].time;

				data_ptr++;	// skip number of keys to follow (should be 16)				
				anim->matrix_keys[i].matrix = *((D3DXMATRIX*) data_ptr);
				data_ptr += 16;
			}

			break;
		}

		//xfile_data->Unlock();
	}

	return parse_child_objects(xfile_data, depth, data, force_ref);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void FxAnimationCollection::map_frames(D3DXFRAME* root_frame)
{
	for(sAnimationSet* anim_set = m_anim_sets; anim_set != NULL; anim_set = anim_set->next)
	{
		for(sAnimation* anim = anim_set->anims; anim != NULL; anim = anim->next)
			anim->bone = root_frame->find(anim->bone_name);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void FxAnimationCollection::update(const char* anim_set_name, unsigned long time, bool is_loop)
{
	sAnimationSet* anim_set = m_anim_sets;

	// look for matching animation set if name used
	if(anim_set_name)
	{
		// find matching animation set name
		while(anim_set != NULL)
		{
			// break when match found
			if(! stricmp(anim_set->name, anim_set_name))
				break;

			anim_set = anim_set->next;
		}
	}

	if(anim_set == NULL)	// no animation set found
		return;

	// bounds time to animation length
	if(time > anim_set->length)
		time = is_loop ? (time % (anim_set->length + 1)) : anim_set->length;

	for(sAnimation* anim = anim_set->anims; anim != NULL; anim = anim->next)
	{
		if(anim->bone == NULL)	// only process if it is attached to a bone
			continue;
	
		// reset transformation
		D3DXMatrixIdentity(&anim->bone->TransformationMatrix);

		// apply various matrices to transformation

		// scaling
		if(anim->num_scale_keys && anim->scale_keys)
		{
			DWORD key1 = 0, key2 = 0;

			// loop for matching scale key
			for(DWORD i = 0; i < anim->num_scale_keys; i++)
			{
				if(time >= anim->scale_keys[i].time)
					key1 = i;
			}

			key2 = (key1 >= (anim->num_scale_keys - 1)) ? key1 : key1+1;

			DWORD time_diff = anim->scale_keys[key2].time - anim->scale_keys[key1].time;

			if(time_diff == 0)
				time_diff = 1;

			float scalar = (float)(time - anim->scale_keys[key1].time) / time_diff;

			// calculate interpolated scale values
			D3DXVECTOR3 scale_vec = anim->scale_keys[key2].vec - anim->scale_keys[key1].vec;
			scale_vec *= scalar;
			scale_vec += anim->scale_keys[key1].vec;

			// create scale matrix and combine with transformation
			D3DXMATRIX scale_matrix;
			D3DXMatrixScaling(&scale_matrix, scale_vec.x, scale_vec.y, scale_vec.z);
			anim->bone->TransformationMatrix *= scale_matrix;
		}

		// rotation
		if(anim->num_rotation_keys && anim->rotation_keys)
		{
			DWORD key1 = 0, key2 = 0;

			// loop for matching rotation key
			for(DWORD i = 0; i < anim->num_rotation_keys; i++)
			{
				if(time >= anim->rotation_keys[i].time)
					key1 = i;
			}

			key2 = (key1 >= (anim->num_rotation_keys - 1)) ? key1 : key1+1;

			DWORD time_diff = anim->rotation_keys[key2].time - anim->rotation_keys[key1].time;

			if(time_diff == 0)
				time_diff = 1;

			float scalar = (float)(time - anim->rotation_keys[key1].time) / time_diff;

			// slerp rotation values
			D3DXQUATERNION rot_quat;
			D3DXQuaternionSlerp(&rot_quat, &anim->rotation_keys[key1].quat, &anim->rotation_keys[key2].quat, scalar);

			// create rotation matrix and combine with transformation
			D3DXMATRIX rot_matrix;
			D3DXMatrixRotationQuaternion(&rot_matrix, &rot_quat);
			anim->bone->TransformationMatrix *= rot_matrix;
		}

		// translation
		if(anim->num_translation_keys && anim->translation_keys)
		{
			DWORD key1 = 0, key2 = 0;

			// loop for matching translation key
			for(DWORD i = 0; i < anim->num_translation_keys; i++)
			{
				if(time >= anim->translation_keys[i].time)
					key1 = i;
			}

			key2 = (key1 >= (anim->num_matrix_keys - 1)) ? key1 : key1+1;

			DWORD time_diff = anim->translation_keys[key2].time - anim->translation_keys[key1].time;

			if(time_diff == 0)
				time_diff = 1;

			float scalar = (float)(time - anim->translation_keys[key1].time) / time_diff;

			// calculate interpolated vector values
			D3DXVECTOR3 pos_vec = anim->translation_keys[key2].vec - anim->translation_keys[key1].vec;
			pos_vec *= scalar;
			pos_vec += anim->translation_keys[key1].vec;

			// create translation matrix and combine with transformation
			D3DXMATRIX translation_matrix;
			D3DXMatrixTranslation(&translation_matrix, pos_vec.x, pos_vec.y, pos_vec.z);
			anim->bone->TransformationMatrix *= translation_matrix;
		}

		// matrix
		if(anim->num_matrix_keys && anim->matrix_keys)
		{
			DWORD key1 = 0, key2 = 0;

			// loop for matching matrix key
			for(DWORD i = 0; i < anim->num_matrix_keys; i++)
			{
				if(time >= anim->matrix_keys[i].time)
					key1 = i;
			}

			key2 = (key1 >= (anim->num_matrix_keys - 1)) ? key1 : key1+1;

			DWORD time_diff = anim->matrix_keys[key2].time - anim->matrix_keys[key1].time;

			if(time_diff == 0)
				time_diff = 1;

			float scalar = (float)(time - anim->matrix_keys[key1].time) / time_diff;

			// calculate interpolated matrix
			D3DXMATRIX diff_matrix = anim->matrix_keys[key2].matrix - anim->matrix_keys[key1].matrix;
			diff_matrix *= scalar;
			diff_matrix += anim->matrix_keys[key1].matrix;

			// combine with transformation
			anim->bone->TransformationMatrix *= diff_matrix;
		}
	}
}
