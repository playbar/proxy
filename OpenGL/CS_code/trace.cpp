
#include <string.h>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "common/com_model.h"
#include "engine/cl_entity.h"
#include "trace.h"
#include "defs.h"
#undef DotProduct

//===================================================================================
mnode_t *GetNodeFromPoint(float *point)
{
	struct cl_entity_s *ent;
	mnode_t		*node;
	float		d;
	mplane_t	*plane;
	ent = gEngfuncs.GetEntityByIndex(0);
	if (!ent || !ent->model)
		return NULL;
	node = ent->model->nodes;
	while (node)
	{
		if (node->contents < 0)
		{
			if (!node->parent)
				return NULL;
			return node->parent;
		}
		plane = node->plane;
		if (!plane)
			return NULL;
		d = DotProd(point, plane->normal) - plane->dist;
		if (d > 0)
			node = node->children[0];
		else
			node = node->children[1];
	}
	return NULL;
}

//===================================================================================
mleaf_t *GetLeafFromPoint(float *point)
{
	struct cl_entity_s *ent;
	mnode_t		*node;
	float		d;
	mplane_t	*plane;
	ent = gEngfuncs.GetEntityByIndex(0);
	if (!ent || !ent->model)
		return NULL;
	node = ent->model->nodes;
	while (node)
	{
		if (node->contents < 0)
			return (mleaf_t *)node;
		plane = node->plane;
		d = DotProd(point, plane->normal) - plane->dist;
		if (d > 0)
			node = node->children[0];
		else
			node = node->children[1];
	}
	return NULL;
}

//===================================================================================
void TraceThickness(float *start, float *end, float thickness, strace_t *tr)
{
	mleaf_t *startleaf, *endleaf, *prevleaf;
	int numsteps, count = 0;
	float move[3], step[3], position[3];
	float stepdist, depth = 0;
	memset(tr, 0, sizeof(strace_t));
	if ((start[0] < -4095) || (start[0] > 4095) || (start[1] < -4095) || (start[1] > 4095) || (start[2] < -4095) || (start[2] > 4095))
	{
		tr->hitsky = true;
		tr->startsolid = true;
		tr->finished = false;
		tr->fraction = 0.0;
		return;
	}
	startleaf = GetLeafFromPoint(start);
	endleaf = GetLeafFromPoint(end);
	if (startleaf->contents == CONTENTS_SOLID)
	{
		tr->startsolid = true;
		VectorCopy(start, tr->endpos);
		tr->finished = false;
		if (endleaf->contents == CONTENTS_SOLID)
			tr->allsolid = true;
	}
	else
	{
		tr->startsolid = false;
		tr->allsolid = false;
	}
VectorSubtract(end, start, move);
	tr->dist = (float)VectorLength(move);
	if (tr->startsolid)
		return;
	if (startleaf == endleaf)
	{
		tr->finished = true;
		tr->fraction = 1.0f;
		VectorCopy(end, tr->endpos);
	}
	if (tr->dist > 1.0f)
		numsteps = (int)tr->dist;
	else
		numsteps = 1;
	VectorScale(move, 1.0f / (float)numsteps, step);
	stepdist = (float)VectorLength(step);
	VectorCopy(start, position);
	endleaf = NULL;
	for (;numsteps;numsteps--)
	{
		VectorAdd(position, step, position);
		prevleaf = endleaf;
		endleaf = GetLeafFromPoint(position);
		if (prevleaf != endleaf && endleaf->contents == CONTENTS_SOLID)
			count++;
		if (count == 1 && endleaf->contents == CONTENTS_SOLID)
			depth += stepdist;
		if (endleaf->contents == CONTENTS_SKY)
			tr->hitsky = true;
		if (count > 1 || depth > thickness || tr->hitsky)
		{
			VectorCopy(position, tr->endpos);
			VectorSubtract(position, start, move);
			tr->fraction = (float)VectorLength(move) / tr->dist;
			tr->finished = false;
			return;
		}
	}
	if (numsteps == 0)
	{
		tr->finished = true;
		tr->fraction = 1.0;
		VectorCopy(end, tr->endpos);
	}
}
