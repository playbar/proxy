
#include <windows.h>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cl_entity.h"
#include "defs.h"
#include "recoil.h"
#include "random.h"
#include "client.h"
#include "cvar.h"
#include "aimbot.h"
#include "attack.h"
#include "playeritems.h"
#pragma optimize("gtp",on)


float GetVecSpread(float speed)
{
	int id;
	float spread;

	id = playerItems.CurrentID();

	switch (id)
	{
		case WEAPONLIST_DEAGLE: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.25 * (1.0f - me.spread.spreadvar); 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.115 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.13 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 1.5 * (1.0f - me.spread.spreadvar); 

         break; 
      case WEAPONLIST_ELITE: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.175 * (1.0f - me.spread.spreadvar); 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.08 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.1 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 1.3 * (1.0f - me.spread.spreadvar); 

         break; 
      case WEAPONLIST_FIVESEVEN: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.255 * (1.0f - me.spread.spreadvar); 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.075 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.15 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 1.5 * (1.0f - me.spread.spreadvar); 

         break; 
      case WEAPONLIST_GLOCK18: 
         if (!(me.spread.WeaponState & GLOCK18_BURST)) 
         { 
            if (me.spread.prcflags & FL_ONGROUND) 
            { 
               if (speed) 
                  spread = 0.165 * (1.0f - me.spread.spreadvar); 
               else if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0.075 * (1.0f - me.spread.spreadvar); 
               else 
                  spread = 0.1 * (1.0f - me.spread.spreadvar); 
            } 
            else 
               spread = 1.0f - me.spread.spreadvar; 
         } 
         else 
         { 
            if (me.spread.prcflags & FL_ONGROUND) 
            { 
               if (speed) 
                  spread = 0.185 * (1.0f - me.spread.spreadvar); 
               else if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0.095 * (1.0f - me.spread.spreadvar); 
               else 
                  spread = 0.3 * (1.0f - me.spread.spreadvar); 
            } 
            else 
               spread = 1.2 * (1.0f - me.spread.spreadvar); 
         } 

         break; 
      case WEAPONLIST_P228: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.255 * (1.0f - me.spread.spreadvar); 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.075 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.15 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 1.5 * (1.0f - me.spread.spreadvar); 

         break; 
      case WEAPONLIST_G3SG1: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.15f; 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.035 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.055 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 0.45 * (1.0f - me.spread.spreadvar); 

         if (!(me.iFOV < 90.0f)) 
            spread += 0.025f; 

         break; 
      case WEAPONLIST_SG550: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed) 
               spread = 0.15f; 
            else if (me.spread.prcflags & FL_DUCKING) 
               spread = 0.04 * (1.0f - me.spread.spreadvar); 
            else 
               spread = 0.05 * (1.0f - me.spread.spreadvar); 
         } 
         else 
            spread = 0.45 * (1.0f - me.spread.spreadvar); 

         if (!(me.iFOV < 90.0f)) 
            spread += 0.025f; 

         break; 
      case WEAPONLIST_USP: 
         if (!(me.spread.WeaponState & USP_SILENCER)) 
         { 
            if (me.spread.prcflags & FL_ONGROUND) 
            { 
               if (speed) 
                  spread = 0.225 * (1.0f - me.spread.spreadvar); 
               else if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0.08 * (1.0f - me.spread.spreadvar); 
               else 
                  spread = 0.1 * (1.0f - me.spread.spreadvar); 
            } 
            else 
               spread = 1.2 * (1.0f - me.spread.spreadvar); 
         } 
         else 
         { 
            if (me.spread.prcflags & FL_ONGROUND) 
            { 
               if (speed) 
                  spread = 0.25 * (1.0f - me.spread.spreadvar); 
               else if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0.125 * (1.0f - me.spread.spreadvar); 
               else 
                  spread = 0.15 * (1.0f - me.spread.spreadvar); 
            } 
            else 
               spread = 1.3 * (1.0f - me.spread.spreadvar); 
         } 

         break; 
      case WEAPONLIST_AK47: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed > 140.0f) 
               spread = me.spread.spreadvar * 0.07 + 0.04; 
            else 
               spread = me.spread.spreadvar * 0.0275; 
         } 
         else 
            spread = me.spread.spreadvar * 0.4 + .04; 

         break; 
      case WEAPONLIST_SG552: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed > 140.0f) 
               spread = me.spread.spreadvar * 0.07 + 0.035; 
            else 
               spread = me.spread.spreadvar * 0.02; 
         } 
         else 
            spread = me.spread.spreadvar * 0.4 + .035; 

         break; 
      case WEAPONLIST_AUG: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed > 140.0f) 
               spread = me.spread.spreadvar * 0.07 + 0.035; 
            else 
               spread = me.spread.spreadvar * 0.02; 
         } 
         else 
            spread = me.spread.spreadvar * 0.4 + .035; 

         break; 
      case WEAPONLIST_M249: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed > 140.0f) 
               spread = me.spread.spreadvar * 0.095 + 0.045; 
            else 
               spread = me.spread.spreadvar * 0.03; 
         } 
         else 
            spread = me.spread.spreadvar * 0.5 + .045; 

         break; 
      case WEAPONLIST_M4A1: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (!(me.spread.WeaponState & M4A1_SILENCER)) 
            { 
               if (speed > 140.0f) 
                  spread = me.spread.spreadvar * 0.07 + 0.035; 
               else 
                  spread = me.spread.spreadvar * 0.02; 
            } 
            else 
            { 
               if (speed > 140.0f) 
                  spread = me.spread.spreadvar * 0.07 + 0.035; 
               else 
                  spread = me.spread.spreadvar * 0.025; 
            } 
         } 
         else 
            spread = me.spread.spreadvar * 0.4 + .035; 

         break; 
      case WEAPONLIST_MP5: 
         if (me.spread.prcflags & FL_ONGROUND) 
            spread = 0.04 * me.spread.spreadvar; 
         else 
            spread = 0.2 * me.spread.spreadvar; 

         break; 
      case WEAPONLIST_MAC10: 
         if (me.spread.prcflags & FL_ONGROUND) 
            spread = 0.03 * me.spread.spreadvar; 
         else 
            spread = 0.375 * me.spread.spreadvar; 

         break; 
      case WEAPONLIST_P90: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed > 170.0f) 
               spread = me.spread.spreadvar * 0.115; 
            else 
               spread = me.spread.spreadvar * 0.045; 
         } 
         else 
            spread = me.spread.spreadvar * 0.3; 

         break; 
      case WEAPONLIST_TMP: 
         if (me.spread.prcflags & FL_ONGROUND) 
            spread = 0.03 * me.spread.spreadvar; 
         else 
            spread = 0.25 * me.spread.spreadvar; 

         break; 
      case WEAPONLIST_UMP45: 
         if (me.spread.prcflags & FL_ONGROUND) 
            spread = 0.04 * me.spread.spreadvar; 
         else 
            spread = 0.24 * me.spread.spreadvar; 

         break; 
      case WEAPONLIST_AWP: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed < 10.0f) 
            { 
               if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0; 
               else 
                  spread = 0.001f; 
            } 
            else if (speed < 140.0f) 
            { 
               spread = 0.1f; 
            } 
            else 
               spread = 0.25f; 
         } 
         else 
            spread = 0.85f; 

         if (!(me.iFOV < 90.0f)) 
            spread += 0.08f; 

         break; 
      case WEAPONLIST_SCOUT: 
         if (me.spread.prcflags & FL_ONGROUND) 
         { 
            if (speed < 170.0f) 
            { 
               if (me.spread.prcflags & FL_DUCKING) 
                  spread = 0; 
               else 
                  spread = 0.007f; 
            } 
            else 
               spread = 0.075f; 
         } 
         else 
            spread = 0.2f; 

         if (!(me.iFOV < 90.0f)) 
            spread += 0.025f; 

         break; 
      default: 
         spread = 0; 
         break; 
   } 

   return spread; 
} 


void GetSpreadXY(unsigned int seed, int future, float *velocity, float *vec)
{
	float vecspread, speed;

	vec[0] = UTIL_SharedRandomFloat(seed + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 1 + future, -0.5, 0.5);

	vec[1] = UTIL_SharedRandomFloat(seed + 2 + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 3 + future, -0.5, 0.5);

	speed = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);

	vecspread = GetVecSpread(speed);

	vec[0] *= vecspread;
	vec[1] *= vecspread;

	return;
}


void GetRecoilOffset(unsigned int seed, int future, float *inangles, float *velocity, float *outangles)
{
	float forward[3], right[3], up[3], vecDir[3];
	float view[3], dest[3], spread[2];

	gEngfuncs.pfnAngleVectors(inangles, forward, right, up);

	GetSpreadXY(seed, future, velocity, spread);

	vecDir[0] = forward[0] + spread[0] * right[0] + spread[1] * up[0];
	view[0] = 8192 * vecDir[0];

	vecDir[1] = forward[1] + spread[0] * right[1] + spread[1] * up[1];
	view[1] = 8192 * vecDir[1];

	vecDir[2] = forward[2] + spread[0] * right[2] + spread[1] * up[2];
	view[2] = 8192 * vecDir[2];

	VectorAngles(view, dest);
	dest[0] *= -1;

	outangles[0] = inangles[0] - dest[0];
	outangles[1] = inangles[1] - dest[1];
	outangles[2] = 0;
}