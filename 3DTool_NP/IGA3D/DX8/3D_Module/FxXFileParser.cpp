#include <guiddef.h>
#include <rmxftmpl.h>

#include "stdafx.h"

bool cXParser::parse(const char* filename, void** data)
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

	if(begin_parse(data))
	{
		IDirectXFileData* xfile_data;
		while(SUCCEEDED(xfile_enum->GetNextDataObject(&xfile_data)))
		{
			bool parse_result = parse_objects(xfile_data, NULL, 0, data, false);
			release_com(xfile_data);

			if(parse_result == false)
				break;
		}

		end_parse(data);
	}

	release_com(xfile_enum);
	release_com(xfile);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool cXParser::parse_child_objects(IDirectXFileData* xfile_data, DWORD depth, void** data, bool force_ref)
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

			bRes = parse_objects(child_xfile_data, xfile_data, depth + 1, data, force_ref);
			release_com(child_xfile_data);
			release_com(pRef);
		}

		if(SUCCEEDED(child_xfile_obj->QueryInterface(IID_IDirectXFileData, (void**)&child_xfile_data)))
		{
			bRes = parse_objects(child_xfile_data, xfile_data, depth + 1, data, force_ref);			
			release_com(child_xfile_data);
		}

		release_com(child_xfile_obj);
	}

	if(!bRes)
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

char* cXParser::get_object_name(IDirectXFileData* xfile_data)
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
