
#ifndef AIMBOT_H
#define AIMBOT_H

#include "client.h"
#include "engine/r_studioint.h"
#include "engine/studio.h"
#include <map>

using namespace std;

struct aimvec
{
	float h;
	float f;
	float r;
};

#define WALL_PEN0 0
#define WALL_PEN1 1
#define WALL_PEN2 2

bool PathFree(float* from,float* to);
bool IsAimingAtMe( int ax );
void VectorAngles( const float *forward, float *angles );

class OriginAimbot
{
public:
	virtual void FindTarget();
	virtual void CalcViewAngles();
	vec3_t aim_viewangles, aim_spot;
	vector<aimvec> AimVecsS;
	vector<aimvec> AimVecsD;
	vector<aimvec> AimVecsJ;
	vector<aimvec> AimVecsR;
	virtual void Draw();
	int target;
	int firstaimspot;
	virtual void CalcFovangleAndVisibility (int ax);
protected:
	int GetDamageVec(int ax, float *targetspot, bool onlyvis); 
	virtual void CalcVec_MeToTarget ( float* out );
	virtual void CalcTargetSpot     ( float *out );
	virtual aimvec* TargetRegion    ( int ax );
	virtual void Draw_Player_Vecs   ( int ax );
	virtual double GetAimingFov     (int ax);
	virtual bool GetPlayerVisX      (int ax){ return (cvar.aimthru || vPlayers[ax].visible); }
	virtual bool GetAimingTeam      (int ax)
	{
		if (cvar.team<=0 && (me.team != vPlayers[ax].team)) return true; 
		if (cvar.team==1) return true;
		if (cvar.team>1 &&  (me.team == vPlayers[ax].team)) return true; 
		return false;
	}
public:
	OriginAimbot() : target(-1), firstaimspot(0) {}
	virtual ~OriginAimbot(){}
};
extern OriginAimbot gAimbot;

#endif
