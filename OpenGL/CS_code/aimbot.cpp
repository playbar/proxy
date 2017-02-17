#define WIN32_LEAN_AND_MEAN
#pragma warning (disable:4786)
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#undef NDEBUG
#include <assert.h>
#include <memory.h>
#include <map>
#include <vector>
#include <fstream>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cdll_int.h"
#include "engine/util_vector.h"
#include "engine/cl_entity.h"
#include "common/com_model.h"
#include "common/event_api.h"
#include "engine/pmtrace.h" 
#include "engine/pm_defs.h"
#include "aimbot.h"
#include "cvar.h"
#include "client.h"
#include "interpreter.h"
#include "trace.h"
#include "attack.h"
#include "defs.h"
#include "playeritems.h"
#include "perfectwall.h"

#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
using namespace std;
OriginAimbot gAimbot;
#define gAimbot
extern float mainViewOrigin[3],mainViewAngles[3];

void VectorAngles( const float *forward, float *angles )
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0) 
			pitch = 90; 
		else
			pitch = 270;
	}
	else
		yaw = float(atan2(forward[1],forward[0]) * 180/3.141592);
	if (yaw < 0) yaw += 360;
		tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = float(atan2(forward[2], tmp) * 180/3.141592);
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
static void PredictTarget(int index,float *pred)
{
	if (cvar.pred)
	{
		vec3_t vPredictedOrigin = vPlayers[index].getEnt()->origin;
		double fTargetTime = gEngfuncs.GetClientTime() - cvar.predtime;
		int iHistIndex = vPlayers[index].getEnt()->current_position + HISTORY_MAX;
		int iMaxIndex = iHistIndex - HISTORY_MAX;;
		int iFoundHistIndex = 0;
		bool bFoundInHistory = false;
		while ( !bFoundInHistory  && iHistIndex > iMaxIndex)
		{
			if ((fTargetTime < (vPlayers[index].getEnt()->ph[iHistIndex % HISTORY_MAX].animtime)) &&  (fTargetTime > (vPlayers[index].getEnt()->ph[(iHistIndex-1) % HISTORY_MAX].animtime)) )
			{
				iFoundHistIndex = (iHistIndex-1) % HISTORY_MAX;
				bFoundInHistory = true;
			}
			iHistIndex--;
		}
		vec3_t vFromOrigin , vToOrigin , vDeltaOrigin;
		float fFromTime,fToTime,fTimeFraction;
		if (bFoundInHistory) 
		{
			int iFoundHistIndexNext = (iFoundHistIndex+1) % HISTORY_MAX;
			vFromOrigin = vPlayers[index].getEnt()->ph[iFoundHistIndex].origin;
			vToOrigin = vPlayers[index].getEnt()->ph[iFoundHistIndexNext].origin;
			fFromTime = vPlayers[index].getEnt()->ph[iFoundHistIndex].animtime;
			fToTime = vPlayers[index].getEnt()->ph[iFoundHistIndexNext].animtime;
			fTimeFraction = 0;
			if ( (fToTime - fFromTime) != 0 ) 
				fTimeFraction = (fTargetTime - fFromTime) / (fToTime - fFromTime);
			vDeltaOrigin = vToOrigin - vFromOrigin;
			vPredictedOrigin = vFromOrigin + (vDeltaOrigin * fTimeFraction);
		}
		else 
		{
			int iCurHInd = vPlayers[index].getEnt()->current_position;
			int iPrevHInd = ((iCurHInd - 1) + HISTORY_MAX) % HISTORY_MAX;
			vFromOrigin = vPlayers[index].getEnt()->ph[iCurHInd].origin;
			vToOrigin = vFromOrigin + (vFromOrigin - vPlayers[index].getEnt()->ph[iPrevHInd].origin);
			fFromTime = vPlayers[index].getEnt()->ph[iCurHInd].animtime;
			fToTime = fFromTime + (fFromTime - vPlayers[index].getEnt()->ph[iPrevHInd].animtime);
			fTimeFraction = 0;
			if ( (fToTime - fFromTime) != 0 ) 
				fTimeFraction = (fTargetTime - fFromTime) / (fToTime - fFromTime);
			if (fTimeFraction > cvar.predmax)
				fTimeFraction = cvar.predmax;
			vDeltaOrigin = vToOrigin - vFromOrigin;
			vPredictedOrigin = vFromOrigin + (vDeltaOrigin * fTimeFraction);
		}
		VectorCopy(vPredictedOrigin,pred);
	}
	else 
	{
		VectorCopy(vPlayers[index].getEnt()->origin,pred);
	}
}
double OriginAimbot::GetAimingFov(int ax)
{
	float vec[3], view[3];
	float dot;	
	float* origin = vPlayers[ax].getEnt()->origin;
	view[0] = origin[0] - me.pmEyePos[0];
	view[1] = origin[1] - me.pmEyePos[1];
	view[2] = origin[2] - me.pmEyePos[2];
	dot = sqrt(view[0] * view[0] + view[1] * view[1] + view[2] * view[2]);
	dot = 1/dot;
	vec[0] = view[0] * dot;
	vec[1] = view[1] * dot;
	vec[2] = view[2] * dot;
	{
		view[0] = sin((me.viewAngles[1] + 90) * 3.141592/180 );
		view[1] = -cos((me.viewAngles[1] + 90) * 3.141592/180 );
		view[2] = -sin(me.viewAngles[0] * 3.141592/180 );
	}		
	dot = view[0] * vec[0] + view[1] * vec[1] + view[2] * vec[2];
	return dot;
}
bool PathFree(float* from,float* to)
{
	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( from, to, PM_GLASS_IGNORE, me.ent->index, &tr );
	return (tr.fraction == 1.0);
}
void OriginAimbot::CalcFovangleAndVisibility(int ax)
{
	int damage;
	float targetspot[3];
	PlayerInfo& r = vPlayers[ax];

	r.fovangle = ((1.0-GetAimingFov(ax))*360.0); 

	if (!GetAimingTeam(ax))
	{
		r.visible = 0;
		r.damage = 0;

		r.damagespot[0] = 0;
		r.damagespot[1] = 0;
		r.damagespot[2] = 0;
	}
	else
	{
		damage = GetDamageVec(ax, targetspot, true);
		if (damage)
		{
			r.visible = 2;
		}
		else
		{
			damage = GetDamageVec(ax, targetspot, false);
			if (damage)
				r.visible = 1;
			else
				r.visible = 0;
		}
		r.damage = damage;
		r.damagespot[0] = targetspot[0];
		r.damagespot[1] = targetspot[1];
		r.damagespot[2] = targetspot[2];
	}
}
void OriginAimbot::FindTarget()
{
	if(target!=-1 && vPlayers[target].getPVS() && GetPlayerVisX(target) )
	{
		return;
	}
	target=-1;
	for (int ax=0;ax<vPlayers.size();ax++)
	if  ( vPlayers[ax].getAlive() && vPlayers[ax].getPVS() && GetPlayerVisX(ax) && GetAimingTeam(ax) && vPlayers[ax].fovangle<cvar.fov)
	{

		if( target == -1 ) { target = ax; continue; }
			if( vPlayers[ax].visible  && !vPlayers[target].visible )  { target = ax; break; }
			if( vPlayers[ax].distance <  vPlayers[target].distance )  { target = ax; break; }
	}
}

void OriginAimbot::Draw()
{
	for (int ax=0;ax<vPlayers.size();ax++) 
		if (vPlayers[ax].getPVS() ) Draw_Player_Vecs(ax);
}
void OriginAimbot::Draw_Player_Vecs(int ax)
{
	vec3_t aim_location, target_origin;
	vec3_t up, right, forward, playerAngles;
	vector<aimvec>::iterator pos, end;
	if (vPlayers[ax].getEnt()->curstate.usehull == 0)
	{
			pos = AimVecsS.begin();
			end = AimVecsS.end();
	} else {
			pos = AimVecsD.begin();
			end = AimVecsD.end();
	}
	if (vPlayers[ax].getEnt()->curstate.gaitsequence == 6)
	{
         pos = AimVecsJ.begin();
         end = AimVecsJ.end();
	}
	PredictTarget(ax,target_origin);
	playerAngles[0]=0;
	playerAngles[1]=vPlayers[ax].getEnt()->angles[1];
	playerAngles[2]=0;
	gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	forward[2] = -forward[2];
	register DWORD color = 0xFF1111FF;
	for (;pos!=end;++pos) 
	{
		VectorCopy(target_origin,aim_location);
		aim_location = aim_location + forward * pos->f;
		aim_location = aim_location + up * pos->h;
		aim_location = aim_location + right * pos->r;
		gDrawFilledBoxAtLocation(aim_location,color,1);
		color = 0xFFFFFFFF;
	}
}
aimvec* OriginAimbot::TargetRegion(int ax)
{
	vec3_t vecEnd, up, right, forward, EntViewOrg,PredOrg, playerAngles;
	PredictTarget(ax,PredOrg);
	vector<aimvec>::iterator si, end;
	if (vPlayers[ax].getEnt()->curstate.usehull == 0)
	{
		si = AimVecsS.begin(); end = AimVecsS.end();
	} else {
		si = AimVecsD.begin(); end = AimVecsD.end();
	}
	if (vPlayers[ax].getEnt()->curstate.gaitsequence == 6)
	{
      si = AimVecsJ.begin(); end = AimVecsJ.end();
	}
	playerAngles[0]=0;
	playerAngles[1]=vPlayers[ax].getEnt()->angles[1];
	playerAngles[2]=0;
    gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	forward[2] = -forward[2];
	for (;si!=end;++si)
	{
		VectorCopy(PredOrg,EntViewOrg);
		EntViewOrg = EntViewOrg + forward * si->f;
		EntViewOrg = EntViewOrg + up * si->h;
        EntViewOrg = EntViewOrg + right * si->r;
		if(PathFree(me.pmEyePos,EntViewOrg)) return &(*si);
	}
	return NULL;
}
int OriginAimbot::GetDamageVec(int ax, float *targetspot, bool onlyvis)
{
	int hitdamage, penetration = WALL_PEN0, bullettype, damage;
	vec3_t vecEnd, up, right, forward, EntViewOrg, PlayerOrigin, playerAngles;
	float distance, wallpierce, defaultvec[3] = {0,0,0};
	vector<aimvec>::iterator si, end;
	PredictTarget(ax,PlayerOrigin);
	if (vPlayers[ax].getEnt()->curstate.usehull == 0)
	{
		if (AimVecsS.size() > 0)
		{
			si = AimVecsS.begin();
			end = AimVecsS.end();
			defaultvec[0] = AimVecsS[0].h;
			defaultvec[1] = AimVecsS[0].f;
			defaultvec[2] = AimVecsS[0].r;
		}
	}
	else
	{
		if (AimVecsD.size() > 0)
		{
			si = AimVecsD.begin();
			end = AimVecsD.end();
			defaultvec[0] = AimVecsD[0].h;
			defaultvec[1] = AimVecsD[0].f;
			defaultvec[2] = AimVecsD[0].r;
		}
	}
	if (vPlayers[ax].getEnt()->curstate.gaitsequence == 6)
	{
		if (AimVecsJ.size() > 0)
		{
			si = AimVecsJ.begin();
			end = AimVecsJ.end();

			defaultvec[0] = AimVecsJ[0].h;
			defaultvec[1] = AimVecsJ[0].f;
			defaultvec[2] = AimVecsJ[0].r;
		}
	}
	playerAngles[0]=0;
	playerAngles[1]=vPlayers[ax].getEnt()->angles[1];
	playerAngles[2]=0;
    gEngfuncs.pfnAngleVectors(playerAngles, forward, right, up);
	forward[2] = -forward[2];
	if (!onlyvis && cvar.autowall)
	penetration = playerItems.CurPenetration();
	bullettype = playerItems.CurBulletType();
	damage = playerItems.CurDamage();
	distance = playerItems.CurDistance();
	wallpierce = playerItems.CurWallPierce();
	for (;si!=end;++si)
	{
		targetspot[0] = PlayerOrigin[0] + up[0] * si->h + forward[0] * si->f + right[0] * si->r;
		targetspot[1] = PlayerOrigin[1] + up[1] * si->h + forward[1] * si->f + right[1] * si->r;
		targetspot[2] = PlayerOrigin[2] + up[2] * si->h + forward[2] * si->f + right[2] * si->r;

		hitdamage = CanPenetrate(me.pmEyePos, targetspot, distance, penetration, bullettype, damage, wallpierce);
		if (hitdamage > 0)
			return hitdamage;
	}

	targetspot[0] = PlayerOrigin[0] + up[0] * defaultvec[0] + forward[0] * defaultvec[1] + right[0] * defaultvec[2];
	targetspot[1] = PlayerOrigin[1] + up[1] * defaultvec[0] + forward[1] * defaultvec[1] + right[1] * defaultvec[2];
	targetspot[2] = PlayerOrigin[2] + up[2] * defaultvec[0] + forward[2] * defaultvec[1] + right[2] * defaultvec[2];

	return CanPenetrate(me.pmEyePos, targetspot, distance, penetration, bullettype, damage, wallpierce);
}
void OriginAimbot::CalcTargetSpot(float *out)
{
	vec3_t vecEnd, up, right, forward, EntViewOrg, playerAngles;
	cl_entity_s* ent = vPlayers[target].getEnt();
	PredictTarget(target,out);
	playerAngles[0]=0;
	playerAngles[1]=ent->angles[1];
	playerAngles[2]=0;
	gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	aimvec* si = TargetRegion(target);
	float h=0,r=0,f=0;
	if (si)
	{
		f = si->f;
		h = si->h;
		r = si->r;
	}
	else 
	{
		if (ent->curstate.usehull==0)
		{
			if (AimVecsS.size() > 0)
			{
				f = AimVecsS[0].f;
				h = AimVecsS[0].h;
				r = AimVecsS[0].r;
			}
		}
		else
		{
			if (AimVecsD.size() > 0)
			{
				f = AimVecsD[0].f;
				h = AimVecsD[0].h;
				r = AimVecsD[0].r;
			}
		}
	if (ent->curstate.gaitsequence==6 && AimVecsJ.size()>0) 
      { 
         f = AimVecsJ[0].f; 
         h = AimVecsJ[0].h; 
         r = AimVecsJ[0].r; 
      } 
   }
	// add aim vecs. (out=predicted origin)
	out[0] = out[0] + forward[0]*f + up[0]*h + right[0]*r;
	out[1] = out[1] + forward[1]*f + up[1]*h + right[1]*r;
	out[2] = out[2] + forward[2]*f + up[2]*h + right[2]*r;
}
void OriginAimbot::CalcVec_MeToTarget(float *view)
{
	float EntViewOrg[3];
	CalcTargetSpot(EntViewOrg);
	VectorCopy(EntViewOrg,aim_spot);
	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];
}
void OriginAimbot::CalcViewAngles()
{
	float vec_to_target[3];
	vec_to_target[0] = vPlayers[target].damagespot[0] - me.pmEyePos[0];
	vec_to_target[1] = vPlayers[target].damagespot[1] - me.pmEyePos[1];
	vec_to_target[2] = vPlayers[target].damagespot[2] - me.pmEyePos[2];
	VectorAngles(vec_to_target,aim_viewangles);
	aim_viewangles[0] *= -1;
	if (aim_viewangles[0]>180) 
		aim_viewangles[0]-=360;
	if (aim_viewangles[1]>180) 
		aim_viewangles[1]-=360;
}

