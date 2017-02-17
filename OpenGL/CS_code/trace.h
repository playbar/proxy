
#ifndef TRACE_H
#define TRACE_H

#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "common/com_model.h"

struct strace_t
{
	bool finished;
	bool allsolid;
	bool startsolid;	
	float dist;
	float fraction;		
	float endpos[3];	
	bool hitsky;
};

mleaf_t *GetLeafFromPoint(float *point);
mnode_t *GetNodeFromPoint(float *point);
void TraceThickness(float *start, float *end, float thickness, strace_t *tr);

#endif
