
#ifndef CLIENT_H
#define CLIENT_H

#include <windows.h>
#include <string>
#include <vector>
#undef NDEBUG
#include <assert.h>
#include "cvar.h"
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/util_vector.h"

class VACThread
{
public:
	DWORD dwThreadId;
	HANDLE hThread;
};

BOOL InsertThread(VACThread cNewThread);
void CheckForClientHook(FARPROC* pProc,LPCTSTR lpProcName);
int WallActive(void);
bool LambertActive(void);
void StatusDump(void);
void ParseDump(void);
std::string getOgcDirFile(const char* basename);
extern char ogcdir [256];
extern char hldir  [256];
extern struct engine_studio_api_s* pIEngineStudio;
void tintArea(int x,int y,int w,int h, struct ColorEntry* clr);
void gDrawFilledBoxAtLocation( float* origin, DWORD color = 0xFFFFFFFF, int radius=1);
void gSetHudMessage2(const char* message);
void gSetHudMessage(const char* message);
void Con_Echo(const char *fmt, ... );
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
void tintArea2(int x,int y,int w,int h, ColorEntry* clr1,ColorEntry* clr2);
void consoleBorder(int x,int y,int w,int h,int r=128,int g=128,int b=128);

int CanPenetrate( float *start, float *end, int power );
#ifndef CDLL_INT_H

typedef unsigned char 		byte;
typedef unsigned short 		word;
typedef struct
{
	char *name;
	short ping;
	byte thisplayer;
	byte spectator;
} hud_player_info_t;

#endif
#define USP_SILENCER (1 << 0)
#define GLOCK18_BURST (1 << 1)
#define M4A1_SILENCER (1 << 2)
#define ELITE_LEFT (1 << 4)

struct spread_info
{
	unsigned int random_seed;
	int recoil;
	float gtime;
	float prevtime;
	float brokentime; 
	float spreadvar;
	float recoiltime;
	bool firing;
	int WeaponState;
	int prcflags;
};


struct local_player_info
{
	local_player_info():team(0),iClip(0),iFOV(0)
		,sin_yaw(0),minus_cos_yaw(-1),inZoomMode(false)
		,alive(false),ent(0)
	{}
	int team;
	struct cl_entity_s *ent;
	int iClip;
	int iFOV;
	bool inZoomMode;
	bool alive;
	float punchangle[3];
	int pmFlags; 
	int pmMoveType;
	float pmVelocity[3];
	float pmEyePos[3];
	float viewAngles[3];
	float sin_yaw, minus_cos_yaw;
	float recoil;
	struct spread_info spread;
		int headshots;
			float hspercent;


};
extern local_player_info me;
enum    { MAX_TARGET_SPOTS=10 };

class PlayerInfo 
{
public:
	int index;
	void init( int _entindex) 
	{ 
		team=0;
		iInfo=0;
		entinfo.name="\\missing-name\\";
		entinfo.ping=0;
		entinfo.thisplayer=0;
		entinfo.spectator=0;
		numTargetSpots=0;
		inpvs=0;
		alive=false;
		entindex = _entindex;
		distance = 1000000.0;
		damage = 0;
		frags=0;
		ratio=0;
		deaths=0;
		headshots=0;
		bestplayer = false;
	}
	hud_player_info_t entinfo;
	int    team;
	int    iInfo;
	float  distance;
	int   visible;
	float  fovangle;
	bool 	bestplayer;
	int     numTargetSpots;
	vec3_t  TargetSpots[MAX_TARGET_SPOTS];
	bool getPVS()   { return inpvs!=0;   }
	void clearPVS() { inpvs = 0;         }
	void setPVS(int i=cvar.inpvs)   { inpvs = alive ? i : 0; }
	void decrPVS()  { if(inpvs) --inpvs; }
	void setAlive( bool state=true ) { alive = state; }
	bool getAlive()                  { return  alive; }
	struct cl_entity_s * getEnt() { return gEngfuncs.GetEntityByIndex(entindex); }
	float lastOrg[3];
	float lastMsg;
	int damage;
		int frags;
		int headshots;
	int deaths;
	int roundhs;
	int roundKills;
	int ratio;
	float damagespot[3];
private:
	int inpvs;
	bool alive;
	int entindex;
public:
	PlayerInfo() { init(0); }
};
enum{  MAX_VPLAYERS =36 };

class VecPlayers
	: public std::vector<PlayerInfo>
{
	typedef std::vector<PlayerInfo> STL_VecPlayers;
public:
	VecPlayers()
		: STL_VecPlayers (MAX_VPLAYERS)
	{
		for(int i=0;i<(int)size();i++) operator [] (i).init( i );
	}
	PlayerInfo& operator [] (unsigned int i)
	{
		if(i>=MAX_VPLAYERS) return STL_VecPlayers::operator[](0);
		return STL_VecPlayers::operator[](i);
	}
};
extern VecPlayers vPlayers;
extern int currentWeaponID;

#endif
