
#include "stdafx.h"
#include "tchar.h"
#include "z_FileIni.h"
#include <guiddef.h>
#include "define.h"
#include <d3d8.h>
#include <d3dx8.h>
#include <dxfile.h>
#include <rmxfguid.h>
#include <rmxftmpl.h>
#include "FxXFileParser.h"




bool CXFileParser::Parse(const char* filename, void** data)
{
	if(filename == NULL)
		return false;

	IDirectXFile* xfile;

	if(FAILED(DirectXFileCreate(&xfile)))
		return false;

	// register standard templates
	if(FAILED(xfile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		xfile->Release();
		return false;
	}

	IDirectXFileEnumObject* xfile_enum;

	if(FAILED(xfile->CreateEnumObject((LPVOID) filename, DXFILELOAD_FROMFILE, &xfile_enum)))
	{
		xfile->Release();
		return false;
	}

	if(BeginParseXFile(data))
	{
		IDirectXFileData* xfile_data;
		while(SUCCEEDED(xfile_enum->GetNextDataObject(&xfile_data)))
		{
			bool parse_result = ParseObjects(xfile_data, NULL, 0, data, false);
			RELEASE_COM(xfile_data);

			if(parse_result == false)
				break;
		}

		EndParseXFile(data);
	}

	RELEASE_COM(xfile_enum);
	RELEASE_COM(xfile);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool CXFileParser::ParseChildObjects(IDirectXFileData* xfile_data, DWORD depth, void** data, bool force_ref)
{
	bool bRes = true;
	IDirectXFileData* child_xfile_data;
	IDirectXFileObject* child_xfile_obj;
	while(SUCCEEDED(xfile_data->GetNextObject(&child_xfile_obj)))
	{
		IDirectXFileDataReference *pRef;
		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileDataReference, (void**)&pRef)))
		{
			pRef->Resolve(&child_xfile_data);
			force_ref = true;

			bRes = ParseObjects(child_xfile_data, xfile_data, depth + 1, data, force_ref);
			RELEASE_COM(child_xfile_data);
			RELEASE_COM(pRef);
		}

		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileData, (void**)&child_xfile_data)))
		{
			bRes = ParseObjects(child_xfile_data, xfile_data, depth + 1, data, force_ref);			
			RELEASE_COM(child_xfile_data);
		}

		RELEASE_COM(child_xfile_obj);
	}

	if(!bRes)
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

char* CXFileParser::GetObjectName(IDirectXFileData* xfile_data)
{
	if(xfile_data == NULL)
		return NULL;

	DWORD size;
	xfile_data->GetName(NULL, &size);	

	char* name = NULL;

	if(size > 1)
	{
		name = new char[size];
		xfile_data->GetName(name, &size);
	}

	return name;
}

bool FxAnimationCollection::ParseObjects(IDirectXFileData* xfile_data, IDirectXFileData* parent_xfile_data, DWORD depth, void** data, bool force_ref)
{
	const GUID *type;
	xfile_data->GetType(&type);

	char *pObjectName = GetObjectName(xfile_data);

	if(*type == TID_D3DRMAnimationSet)	
	{
		// create and link in a sAnimationSet object
		SAnimationSet* anim_set = new SAnimationSet;
		anim_set->next = m_AnimationSet;
		m_AnimationSet = anim_set;

		m_dwNumAnimSet++;

		anim_set->name = GetObjectName(xfile_data);
	}
	else if(*type == TID_D3DRMAnimation && m_AnimationSet)
	{
		// add a sAnimation to top-level sAnimationSet
		SAnimation* anim = new SAnimation;
		anim->next = m_AnimationSet->anims;
		m_AnimationSet->anims = anim;

		m_AnimationSet->num_anims++;
	}
	else if(*type == TID_D3DRMFrame && force_ref == true && m_AnimationSet && m_AnimationSet->anims)
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
				m_AnimationSet->anims->bone_name = GetObjectName(xfile_data);
		}

		return true;	// do not process child of reference frames
	}
	else if(*type == TID_D3DRMAnimationKey && m_AnimationSet && m_AnimationSet->anims)
	{
		SAnimation* anim = m_AnimationSet->anims;

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
			anim->rotation_keys		= new SAnimQuatKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->rotation_keys[i].time = *data_ptr++;

				if(anim->rotation_keys[i].time > m_AnimationSet->length)
					m_AnimationSet->length = anim->rotation_keys[i].time;

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
			anim->scale_keys = new SAnimVectorKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->scale_keys[i].time = *data_ptr++;

				if(anim->scale_keys[i].time > m_AnimationSet->length)
					m_AnimationSet->length = anim->scale_keys[i].time;

				data_ptr++;		// skip number of keys to follow (should be 3)			
				anim->scale_keys[i].vec = *((D3DXVECTOR3*) data_ptr);
				data_ptr += 3;
			}

			break;

		case 2:	// translation
			delete[] anim->translation_keys;
			anim->num_translation_keys = num_keys;
			anim->translation_keys = new SAnimVectorKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->translation_keys[i].time = *data_ptr++;

				if(anim->translation_keys[i].time > m_AnimationSet->length)
					m_AnimationSet->length = anim->translation_keys[i].time;

				data_ptr++;		// skip number of keys to follow (should be 3)			
				anim->translation_keys[i].vec = *((D3DXVECTOR3*) data_ptr);
				data_ptr += 3;
			}

			break;

		case 4:	// transformation matrix
			delete[] anim->matrix_keys;
			anim->num_matrix_keys = num_keys;
			anim->matrix_keys = new SAnimMatrixKey[num_keys];

			for(DWORD i = 0; i < num_keys; i++)
			{
				anim->matrix_keys[i].time = *data_ptr++;

				if(anim->matrix_keys[i].time > m_AnimationSet->length)
					m_AnimationSet->length = anim->matrix_keys[i].time;

				data_ptr++;	// skip number of keys to follow (should be 16)				
				anim->matrix_keys[i].matrix = *((D3DXMATRIX*) data_ptr);
				data_ptr += 16;
			}

			break;
		}

		//xfile_data->Unlock();
	}

	return ParseChildObjects(xfile_data, depth, data, force_ref);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void FxAnimationCollection::MapFrames(CD3DXFrame* root_frame)
{
	for(SAnimationSet* anim_set = m_AnimationSet; anim_set != NULL; anim_set = anim_set->next)
	{
		for(SAnimation* anim = anim_set->anims; anim != NULL; anim = anim->next)
			anim->bone = root_frame->FindFrame(anim->bone_name);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void FxAnimationCollection::UpdateAnimationSet(const char* anim_set_name, unsigned long time, bool is_loop)
{
	SAnimationSet* anim_set = m_AnimationSet;

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

	for(SAnimation* anim = anim_set->anims; anim != NULL; anim = anim->next)
	{
		if(anim->bone == NULL)	// only process if it is attached to a bone
			continue;

		// reset transformation
		D3DXMatrixIdentity(&anim->bone->m_TransformationMatrix);

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
			anim->bone->m_TransformationMatrix *= scale_matrix;
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
			anim->bone->m_TransformationMatrix *= rot_matrix;
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
			anim->bone->m_TransformationMatrix *= translation_matrix;
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
			anim->bone->m_TransformationMatrix *= diff_matrix;
		}
	}
}
