#ifndef FXXFILEPARSER_H
#define FXXFILEPARSER_H

#include <windows.h>
#include <dxfile.h>
#include <rmxfguid.h>

extern unsigned char D3DRM_XTEMPLATES[];

#define D3DRM_XTEMPLATE_BYTES 3278

#define release_com(x)	{ if(x) { (x)->Release(); (x) = NULL; } }

class cXParser
{
protected:
	// functions called when parsing begins and ends
	virtual bool begin_parse(void* data)	{ return true; }
	virtual bool end_parse(void* data)		{ return true; }

	// function called for every template found
	virtual bool parse_objects(IDirectXFileData* xfile_data,
							   IDirectXFileData* parent_xfile_data,
							   DWORD  depth,
							   void** data,
							   bool   force_ref)
	{
		return parse_child_objects(xfile_data, depth, data, force_ref);
	}

	// function called to enumerate child objects
	bool parse_child_objects(IDirectXFileData* xfile_data,
							 DWORD depth,
							 void** data,
							 bool   force_ref);

public:
	// function to start parsing an .X file
	bool parse(const char* filename, void** data);

	// functions to help retrieve object information
	char* get_object_name(IDirectXFileData* xfile_data);

};

#endif
