#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma warning (disable:4786)
#pragma warning (disable:4244)
#pragma warning (disable:4731)
#include <map>
#include <vector>
#include <list>
#undef NDEBUG
#include <assert.h>
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cdll_int.h"
#include "engine/const.h"
#include "engine/progdefs.h"
#include "engine/eiface.h"
#include "engine/edict.h"
#include "engine/studio_event.h"
#include "engine/entity_types.h"
#include "engine/pmtrace.h" 
#include "common/ref_params.h"
#include "common/screenfade.h"
#include "common/event_api.h"
#include "common/com_model.h"
#include "parsemsg.h"
#include "engine/r_studioint.h"
#include "engine/triangleapi.h"
#include "engine/pm_defs.h"
#include "engine/r_efx.h"
#include "sprites.h" 
#include "common/net_api.h"
#include <time.h>
#include <gl/gl.h>
#include "stdafx.h"

using namespace std;

#include <tlhelp32.h> 
#include <fstream>
#include "cvar.h"
#include "calcscreen.h"
#include "timehandling.h"
#include "stringfinder.h"
#include "color.h"
#include "aimbot.h"
#include "attack.h"
#include "playeritems.h"
#include "bind.h"
#include "interpreter.h"
#include "client.h"
#include "menu.h"
#include "console.h"
#include "engine/studio.h"
#include "recoil.h"
#include "eventhook.h"
#include "gpatch.h"

extern GenericPatch soundPatch;
extern bool detour_S_DynamicSound();
#pragma warning (disable:4005)
#pragma warning (disable:4800)
#pragma optimize("gt",on)



using namespace std;
//===================================================================================
typedef void( *fCL_CreateMove )( float, struct usercmd_s *, int );
typedef int( *fHUD_AddEntity )( int, struct cl_entity_s *, const char * );
typedef int( *fHUD_DirectorEvent )( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
typedef void( *fHUD_Frame )( double time );
typedef int( *fHUD_Key_Event )( int, int, const char * );
typedef void( *fHUD_PlayerMove )( struct playermove_s *, int );
typedef void( *fHUD_PlayerMoveInit )( struct playermove_s * );
typedef char( *fHUD_PlayerMoveTexture )( char * );
typedef void( *fHUD_PostRunCmd )( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
typedef int( *fHUD_Redraw )( float, int );
typedef int( *fHUD_Init )( void );
typedef void( *fHUD_Shutdown )( void );
typedef void( *fHUD_TempEntUpdate )( double, double, double, struct tempent_s **, struct tempent_s **, int( *Callback_AddVisibleEntity )( struct cl_entity_s *fEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *fTemp, float damp ) );
typedef void( *fIN_MouseEvent )( int mstate );
typedef int( *fInitialize )( cl_enginefunc_t *, int );
typedef struct kbutton_s *( *fKB_Find )( const char *name );
typedef void( *fV_CalcRefdef )( struct ref_params_s *fparams ); 
typedef void (*fHUD_DirectorMessage)(unsigned char, unsigned int, unsigned int, unsigned int);
typedef int  (*fHUD_GetStudioModelInterface)( int, struct r_studio_interface_s **, struct engine_studio_api_s *);

void( *pCL_CreateMove )( float, struct usercmd_s *, int );
int( *pHUD_AddEntity )( int, struct cl_entity_s *, const char * );
int( *pHUD_DirectorEvent )( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
void( *pHUD_Frame )( double time );
int( *pHUD_Key_Event )( int, int, const char * );
void( *pHUD_PlayerMove )( struct playermove_s *, int );
void( *pHUD_PlayerMoveInit )( struct playermove_s * );
char( *pHUD_PlayerMoveTexture )( char * );

void( *pHUD_PostRunCmd )( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
int( *pHUD_Redraw )( float, int );
void( *pHUD_Shutdown )( void );
void( *pHUD_TempEntUpdate )( double, double, double, struct tempent_s **, struct tempent_s **, int( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
void( *pIN_MouseEvent )( int mstate );
int( *pInitialize )( cl_enginefunc_t *, int );
struct kbutton_s *( *pKB_Find )( const char *name );
void( *pV_CalcRefdef )( struct ref_params_s *pparams ); 
void (*pHUD_DirectorMessage)(unsigned char, unsigned int, unsigned int, unsigned int);
int  (*pHUD_GetStudioModelInterface)( int, struct r_studio_interface_s **, struct engine_studio_api_s *);
//Hook User Messages
DWORD HookUserMsgOrg;
pfnUserMsgHook TeamInfoOrg=NULL;
pfnUserMsgHook SetFOVOrg=NULL;
pfnUserMsgHook CurWeaponOrg=NULL;
pfnUserMsgHook ScoreAttribOrg=NULL;
pfnUserMsgHook TextMsgOrg=NULL;
pfnUserMsgHook ResetHUDOrg=NULL;
pfnUserMsgHook WeaponListOrg=NULL;
pfnUserMsgHook MoneyOrg=NULL;
pfnUserMsgHook BatteryOrg=NULL;
pfnUserMsgHook DeathMsgOrg=NULL;
pfnUserMsgHook ScoreInfoOrg=NULL;
pfnUserMsgHook SayMsgOrg=NULL;
pfnUserMsgHook DamageMsgOrg=NULL;
pfnUserMsgHook AmmoXOrg=NULL;
pfnUserMsgHook AmmoOrg=NULL;
pfnUserMsgHook HealthOrg=NULL;
int WallActive(void)
{ return cvar.wall; }
SCREENINFO screeninfo;
struct hostage_info
{ cl_entity_s *ent; };
struct cl_enginefuncs_s  gEngfuncs;
struct cl_enginefuncs_s  gHookedEngfuncs;
struct efx_api_s gHookEfxApi;
engine_studio_api_t  IEngineStudio;
engine_studio_api_t* pIEngineStudio=0;
cl_enginefuncs_s* pEngfuncs;
engine_studio_api_s* pstudio;
playermove_s *ppmove;
local_player_info me;
VecPlayers vPlayers;
vector<hostage_info> vEntity;
//StringFinder blockCommand;
//enum { CMDBLOCK_ALL, CMDBLOCK_RELPATH };
bool bNewFrame=false;
bool sniper=false;
bool bBombIsPlanted=false;
float fBombPlantedTime=0;
int iC4Timer=0;
int iRounds=0;
// Blinking for Radar/Esp
StopTimer blinkTimer;
bool blinkPhase = false;
PlayerItems playerItems;
//ofstream commandLogFile;
//bool commandLogFileOpened = false;
char  gHudMessage[256]="";
StopTimer gHudTimer;
char  gHudMessage2[256]="";
StopTimer gHudTimer2;
struct ScheduledCommand
{string cmd; RunningStopTimer timer; };
typedef list<ScheduledCommand> ScheduleList;
ScheduleList scheduleList;
static bool bGotHeadshot=false;
int currentWeaponID=0;
int lastWeaponID=0;
int iCurrentFOV=90;
float fCurrentFOV=90.0;
BurstInfo burstInfo;
HANDLE hProcess=0;
DWORD hw_dll_offset=0;
HMODULE hClientDll=0;
DWORD client_dll_offset=0;
DWORD client_dll_end=0;
int Cstrike_SequenceInfo[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
	1, 2, 0, 1, 1, 2, 0, 1, 1, 2,  
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
	1, 2, 0, 1, 2, 0, 1, 2, 0, 1,  
	2, 0, 1, 2, 0, 0, 0, 4, 0, 4, 
	0, 5, 0, 5, 0, 0, 1, 1, 2, 0, 
	1, 1, 2, 0, 1, 0, 1, 0, 1, 2, 
	0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
	3
};
enum { SEQUENCE_SHOOT=1, SEQUENCE_RELOAD=2, SEQUENCE_DIE=3, SEQUENCE_THROW=4, SEQUENCE_ARM_C4=5 };
char* Cstrike_infotext[] = {"", "(PenG)", "|| reloading ||", "", "|| throwing ||", "|| planting ||" };
bool Aimbot_active=false;
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
#define MAX_VALUE(a,b,c) ( a>b? (a>c?a:(b>c?b:c)) : (b>c?b:c) )
static ColorEntry dummycolor;
static bool bAttacking = false;
static bool bAttack2 = false;
class SoundMarker
{
public:
	SoundMarker() : color(&dummycolor),priority(0) {}
	float origin[3];
	char  description[16];
	int team;
	int index;
	StopTimer timer;
	ColorEntry* color;
	int priority;
};
typedef vector<SoundMarker> SoundMarkerVector;
SoundMarkerVector vecSoundMarker(65);
int soundMarkerCurrentIndex = 0;
char ogcdir [256]="";
char hldir  [256]="";
int beamindex = 0;
//===================================================================================
string getOgcDirFile(const char* basename)
{
	if(strstr(basename,"..")){ return ":*?\\/<>\""; }
	string ret = ogcdir;
	return (ret+basename);
}

//===================================================================================
inline float GetPseudoDistance(const float* const pos1, const float* const pos2)
{
	return
		fabs(pos1[0]-pos2[0])+
		fabs(pos1[1]-pos2[1])+
		fabs(pos1[2]-pos2[2]);
}

//===================================================================================
inline float GetDistanceFrom(float* pos)
{	
	register float a = pos[0] - me.pmEyePos[0];
	register float b = pos[1] - me.pmEyePos[1];
	register float c = pos[2] - me.pmEyePos[2];
	return sqrt(a*a + b*b + c*c);
}

//===================================================================================
int __fastcall CalcScreen(float *origin, float *vecScreen)
{
    if(!cvar.chase_cam)
    {
        return NewCalcScreen(origin, vecScreen);
    }
    else if(cvar.chase_cam)
    {
        int cResult = 0;
        if( gEngfuncs.GetLocalPlayer() != NULL )
            cResult = gEngfuncs.pTriAPI->WorldToScreen( origin, vecScreen );
        if( vecScreen[0] < 1 && vecScreen[1] < 1 && vecScreen[0] > -1 && vecScreen [1] > -1 && !cResult )
        {
            vecScreen[0] = vecScreen[0] * displayCenterX + displayCenterX;
            vecScreen[1] = -vecScreen[1] * displayCenterY + displayCenterY;
            return true;
        }
        return NULL;
    }
    return NULL;
}

//===================================================================================
void gDrawFilledBoxAtLocation( float* origin, DWORD color, int radius)
{
	float vecScreen[2]; 
	if( !CalcScreen(origin, vecScreen) ) { return; }
	int red   = (color>>24);
	int green = (color>>16)&0xFF;
	int blue  = (color>>8)&0xFF;
	int alpha = (color)&0xFF;
	int radius2 = radius<<1;
	gEngfuncs.pfnFillRGBA(vecScreen[0]-radius,vecScreen[1]-radius,radius2,radius2,red,green,blue,alpha);
}
//===================================================================================
void SPR_Draw (int frame, int x, int y, const wrect_t *prc)
{
	if (!sniper) gEngfuncs.pfnSPR_Draw (frame, x, y, prc);
}
//===================================================================================
void SPR_DrawHoles (int frame, int x, int y, const wrect_t *prc)
{
	if (!sniper) gEngfuncs.pfnSPR_DrawHoles (frame, x, y, prc);
}

//===================================================================================
HSPRITE sprset_hsprite = 0;
int     sprset_r, sprset_g, sprset_b;
extern bool oglSubtractive;
void SPR_DrawAdditive( int frame, int x, int y, const wrect_t *prc )
{
	if(cvar.hud) gEngfuncs.pfnSPR_Set (sprset_hsprite,sprset_r,sprset_g,sprset_b);
	gEngfuncs.pfnSPR_DrawAdditive(frame,x,y,prc);

}

//===================================================================================
void FillRGBA( int x, int y, int width, int height, int r, int g, int b, int a )
{
	if(cvar.hud)
	{
		register ColorEntry* color = colorList.get(5);
		gEngfuncs.pfnFillRGBA(x,y,width,height,color->r,color->g,color->b,a);
	} else {
		gEngfuncs.pfnFillRGBA(x,y,width,height,r,g,b,a);
	}
}
	
//===================================================================================
int	DrawCharacter( int x, int y, int number, int r, int g, int b )
{
	if( b==0 && g>0 && cvar.hud)
	{
		register double ratio = (double(r)/double(g));
		if( ratio>=1.5 && ratio<=1.7 )
		{
			register ColorEntry* color = colorList.get(5);
			return gEngfuncs.pfnDrawCharacter(x,y,number,color->r*r/255,color->g*r/255,color->b*r/255);
		}
	}
	return gEngfuncs.pfnDrawCharacter(x,y,number,r,g,b);
}

//===================================================================================
static void updateLocalPlayer()
{
	me.ent    = gEngfuncs.GetLocalPlayer();
	static cl_entity_s dummy;
	memset((char*)&dummy,0,sizeof(dummy));
	if(!me.ent){ me.ent = &dummy; }
}
//===================================================================================
void SPR_Set (HSPRITE hPic, int r, int g, int b)
{
	if(cvar.sniper)
	{
		const char * SPR_TOP        = "sprites/top";
		const char * SPR_BOTTOM     = "sprites/bottom";
		const char * SPR_LEFT       = "sprites/left";
		const char * SPR_RIGHT      = "sprites/right";
		const char * SPR_VERTICAL   = "sprites/vertical";
		const char * SPR_HORIZONTAL = "sprites/horizontal";
		const char * sprite_name    =  sprite_lookup(hPic);	
		if( !strncmp(sprite_name, SPR_TOP,      strlen(SPR_TOP)       )
		||  !strncmp(sprite_name, SPR_BOTTOM,   strlen(SPR_BOTTOM)    )
		||  !strncmp(sprite_name, SPR_LEFT,     strlen(SPR_LEFT)      )
		||  !strncmp(sprite_name, SPR_RIGHT,    strlen(SPR_RIGHT)     )
		||  !strncmp(sprite_name, SPR_VERTICAL, strlen(SPR_VERTICAL)  )
		||  !strncmp(sprite_name, SPR_RIGHT,    strlen(SPR_HORIZONTAL)))
		{
			sniper=true;
		} 
		else sniper = false;
	}
	else sniper = false;
	if(cvar.hud)
	{
		register ColorEntry* color = colorList.get(5);
		sprset_hsprite=hPic;
		int intensity = MAX_VALUE(r,g,b);
		sprset_r = color->r*intensity/255;
		sprset_g = color->g*intensity/255;
		sprset_b = color->b*intensity/255;
	}
	gEngfuncs.pfnSPR_Set (hPic,r,g,b);
}
//===================================================================================
void tintArea(int x,int y,int w,int h, ColorEntry* clr)
{
	gEngfuncs.pfnFillRGBA(x,y,w,h,clr->r,clr->g,clr->b,clr->a);
}
//===================================================================================

int DrawLen(char *fmt)
{
	int len=0;
	for ( char * p = fmt; *p; p++ ) len+=screeninfo.charWidths[*p]; 
	return len;
}

//===================================================================================
void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	if( cvar.confont ) {
		int length, height;
		gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
		x = x - length/2;
		gEngfuncs.pfnDrawSetTextColor(/*(float)*/r/255.0,/*(float)*/g/255.0,/*(float)*/b/255.0);
		gEngfuncs.pfnDrawConsoleString(x,y,buf);
	} else {
		int borderY = displayCenterY*2-18;
		if( y<0 || y>borderY ) { return; }
		int drawLen = DrawLen(buf);
		x = x-drawLen/2;
		int borderX = displayCenterX*2-11;
		int minX = x;
		int maxX = x+drawLen;
		bool needSingleCheck = (minX<1 || maxX>borderX);
		if( needSingleCheck )
		{
			for ( char * p = buf; *p; p++ )
			{
				int next = x + screeninfo.charWidths[*p];
				if( x>0 && x<borderX )
					gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
				x = next;
			}
		} else {
			for ( char * p = buf; *p; p++ )
			{
				int next = x + screeninfo.charWidths[*p];
				gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
				x = next;
			}
		}
	}
}

//===================================================================================
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	if(cvar.confont)
	{
		gEngfuncs.pfnDrawSetTextColor(/*(float)*/r/255.0,/*(float)*/g/255.0,/*(float)*/b/255.0);
		gEngfuncs.pfnDrawConsoleString(x,y,buf);
	} else {
		int borderY = displayCenterY*2-18;
		if( y<0 || y>borderY ) { return; }
		bool needSingleCheck=false;
		int borderX = displayCenterX*2-11;
		int drawLen = DrawLen(buf);
		if( x<1 ){ needSingleCheck=true; }
		else
		{
			int maxX = x+drawLen;
			needSingleCheck = (maxX>borderX);
		}
		if( needSingleCheck )
		{
			for ( char * p = buf; *p; p++ )
			{
				int next = x + screeninfo.charWidths[*p];
				if( x>0 && x<borderX )
					gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
				x = next;
			}
		}else {
			for ( char * p = buf; *p; p++ )
			{
				int next = x + screeninfo.charWidths[*p];
				gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
				x = next;
			}
		}
	}
}
//===================================================================================
char *getPlayerNameByIndex(int ax)
{
	char *name;
	if (ax == -1)
		return "";
	name = vPlayers[ax].entinfo.name;
	if (name)
		return name;
	else
		return "";
}
//===================================================================================
void Con_Echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	gConsole.echo("%s",buf);
}


static int TeamInfo (const char *pszName, int iSize, void *pbuf)
{
	const char PROC_NAME[50] = "TeamInfo\0";
	int retval;
	static bool         done=false;
	static StringFinder teamNumber;
	if(!done)
	{
		teamNumber.add("terrorist"  ,1);teamNumber.add("ct"         ,2);
		teamNumber.add("unassigned" ,0);teamNumber.add("spectator"  ,0);

		done = true;
	}
	BEGIN_READ(pbuf,iSize);
	int px = READ_BYTE();
	char * teamtext = READ_STRING();
	updateLocalPlayer();
	if(!*teamtext) { vPlayers[px].team = 0; }
	else
	{
		_strlwr(teamtext);
		if(teamNumber.find(teamtext))
		{
			vPlayers[px].team = teamNumber.num;
			if(px==me.ent->index){ me.team = teamNumber.num; }
		}
		else
		{
            Con_Echo("TeamInfo: unknown px=%d, team=%s",px,teamtext);
			vPlayers[px].team = -1;
			if(px==me.ent->index){ me.team = -1; }
		}
	}
	retval = (*TeamInfoOrg)(pszName, iSize, pbuf);
	return retval;
}

//===================================================================================
int CurWeapon(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "CurWeapon\0";
	int retval;
	BEGIN_READ( pbuf, iSize );
	int iState = READ_BYTE();
	int iID    = READ_CHAR();
	int iClip  = READ_CHAR();
	if (iState) me.iClip = iClip;
	playerItems.msgCurWeapon (iState,iID,iClip);
	if(iState)
	{
		char * name = playerItems.getNamebyId(iID);
		if(!strcmp(name,"mp5navy")) currentWeaponID = WEAPON_MP5;
		else currentWeaponID = burstInfo.getWeaponID( name );
	}
	retval = (*CurWeaponOrg)(pszName,iSize,pbuf);
	return retval;
}
//===================================================================================
int SayMsg(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "SayMsg\0";
	int retval;
	updateLocalPlayer();
	if(gConsole.mode == OgcConsole::MODE_CHAT)
	{
		BEGIN_READ( pbuf, iSize);
		int    client = READ_BYTE();
		char*  text   = READ_STRING();
		char* realtext = strstr(text," : ");
		if(realtext)
		{
			char* name = text; 
			*realtext=0;
			realtext+=3;
			while(*realtext==' ')realtext++;
			if( vPlayers[client].entinfo.name[0]!='\\' || client==me.ent->index ) 
			{
				cmd.exec("sigmessage");
				gConsole.say(realtext,name,vPlayers[client].team );
				return 1;
			} 
		}
	}
	retval = (*SayMsgOrg)(pszName,iSize,pbuf);

	return retval;
}


//===================================================================================
int DeathMsg(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "DeathMsg\0";
	int retval;
	updateLocalPlayer();
	BEGIN_READ( pbuf, iSize );
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = READ_BYTE();
    char* weaponName = READ_STRING();
	if(victim==me.ent->index)
	{	
		cvar.spec_fix = false;
	}
	if(cvar.alive_method==1)
	{
		vPlayers[victim].setAlive(false);
		if(victim==me.ent->index){ me.alive = false; }
	}
	if( killer==me.ent->index && headshot)
	{
		bGotHeadshot=true;
	}
	retval = (*DeathMsgOrg)(pszName,iSize,pbuf);

	return retval;
}

//===================================================================================
int MsgMoney(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "MsgMoney\0";
	int retval;

	BEGIN_READ( pbuf, iSize );
	playerItems.money = READ_SHORT();
	
	if (playerItems.money!=16000 && me.alive)
	{ 
		cmd.exec("#impulse 101");
	}
	retval =  (*MoneyOrg)(pszName,iSize,pbuf);

	return retval;
}
//===================================================================================
int MsgBattery(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "MsgBattery\0";
	int retval;

	BEGIN_READ( pbuf, iSize );
	playerItems.armor = READ_BYTE();
	retval = (*BatteryOrg)(pszName,iSize,pbuf);

	return retval;
}
//===================================================================================
int SetFOV(const char *pszName, int iSize, void *pbuf)
{	
	const char PROC_NAME[50] = "MsgHook_SetFOV\0";
	int retval;
	BEGIN_READ( pbuf, iSize );
	me.iFOV = READ_BYTE();
	if(!me.iFOV)
		me.iFOV=90;
	if(me.iFOV==90)
		me.inZoomMode=false;
	else
		me.inZoomMode=true;
	if (cvar.antizoom && me.iFOV != 90) 
	{ 
		me.iFOV = 89; 
		((unsigned char*)pbuf)[0] = 89;	
	}
	iCurrentFOV = me.iFOV;
	fCurrentFOV = me.iFOV;
	retval = (*SetFOVOrg)(pszName,iSize,pbuf);

	return retval;
}
//===================================================================================
int ScoreAttrib(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "ScoreAttrib\0";
	int retval;
	updateLocalPlayer();
	BEGIN_READ(pbuf, iSize);
    int idx  = READ_BYTE();
    int info = READ_BYTE();
    vPlayers[idx].iInfo = info;
	if( cvar.alive_method==0)
	{
		if(info&1) { vPlayers[idx].setAlive(false); vPlayers[idx].clearPVS(); }
		else       { vPlayers[idx].setAlive(); }
	}
	if(idx==me.ent->index) me.alive = ((info&1)==0);
	retval = (*ScoreAttribOrg)(pszName,iSize,pbuf);
	return retval;
}

//===================================================================================
int TextMsg(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "TextMsg\0";
	int retval;
	updateLocalPlayer();
	BEGIN_READ(pbuf, iSize);
	int id = READ_BYTE();
	char* msg = READ_STRING();
	if( *msg )
	{
		if (!strcmp(msg, "#Bomb_Planted"))
		{
			fBombPlantedTime = ClientTime::current;
			bBombIsPlanted=true;
		}
		else if (!strcmp(msg, "#Target_Bombed"))
		{
			if (bBombIsPlanted)
			{
				iC4Timer = (int) ((ClientTime::current-fBombPlantedTime)+0.4999999);
				if(iC4Timer<0 || iC4Timer>120)  {  iC4Timer=0;  } 
				bBombIsPlanted=false;
			}
		}
		else if (!strcmp(msg, "#Bomb_Defused"))
		{
			bBombIsPlanted=false;
		}
	}
	retval = (*TextMsgOrg)(pszName,iSize,pbuf);  
	return retval;
}

//===================================================================================
void AtRoundStart(void);


int ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "ResetHUD\0";
	int retval;
	updateLocalPlayer();
	bBombIsPlanted=false;
	cmd.exec("#say ( WWW.OGC.COM.CN )"); 


	if (iRounds) AtRoundStart();
	iRounds++;


	
	if (currentWeaponID == WEAPON_SG550 || currentWeaponID == WEAPON_G3SG1)
		me.spread.brokentime = 0; // This catches all cases except when you buy one and than shoot some bullets and that buy another

	retval = (*ResetHUDOrg)(pszName,iSize,pbuf);

	return retval;
}
//===================================================================================
int WeaponList(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "WeaponList\0";
	int retval;
	updateLocalPlayer();
	playerItems.msgWeaponList(iSize,pbuf);
	retval = (*WeaponListOrg)(pszName,iSize,pbuf);

	return retval;
}
//========================================================================================
int ScoreInfo(const char *pszName, int iSize, void *pbuf )
{
	const char PROC_NAME[50] = "ScoreInfo\0";
	int retval;
	updateLocalPlayer();

	BEGIN_READ(pbuf, iSize);
	int idx = READ_BYTE();
	PlayerInfo& r = vPlayers[idx];

	r.frags  = READ_SHORT();
	r.deaths = READ_SHORT();

	if(r.frags<=0 )   { r.ratio=0; }
	else              { r.ratio = (double)r.frags / ((r.deaths<=0)?1:r.deaths); }

	r.bestplayer = false;
	bool foundbetter = false;
	for(int i=0;i<vPlayers.size();i++)
	{
		if     ( r.ratio < vPlayers[i].ratio ) foundbetter = true;
		else if( r.ratio > vPlayers[i].ratio ) vPlayers[i].bestplayer=false;
	}
	if(!foundbetter) r.bestplayer = true;

	if(idx==me.ent->index)
	{
		static int lastfrags=0;
		static int lastdeaths=0;

		if(me.headshots>r.frags || r.frags==0) 
		{ 
			me.headshots = 0; 
			me.hspercent = 0;
			bGotHeadshot = false; 
			lastfrags    = r.frags;
			lastdeaths   = r.deaths;
		}
		else
		{
			if(bGotHeadshot)         { ++me.headshots; }

			double den = r.frags ? (double)r.frags : 1.0;
			me.hspercent = (double)me.headshots/den*100.0;

			if(cvar.saystats && (lastfrags!=r.frags || lastdeaths!=r.deaths ))
            {
                char tmp[128];
                switch(cvar.saystats)
                {
                case 1:    
                //    sprintf(tmp,"say 1VS16 HOOK  ~|Merked|~ %d ~|HeadShots|~ %d ~|Private|~",
                 //       r.frags,me.headshots);
					    sprintf(tmp,"say Oh Yeah ! %d个猪头~今天有猪头肉吃了,剩下的做猪肉罐头 ^_^ | ( WWW.OGC.COM.CN )  ",
                        r.frags);
                    gEngfuncs.pfnClientCmd(tmp);
                    break;
                }
            }

        }
		bGotHeadshot = false;
		lastfrags    = r.frags;
		lastdeaths   = r.deaths;
	}
	retval = (*ScoreInfoOrg)(pszName,iSize,pbuf);
	return retval;
}

//===================================================================================
int	HookUserMsg (char *szMsgName, pfnUserMsgHook pfn)
{
	int retval = gEngfuncs.pfnHookUserMsg (szMsgName, pfn);
	if (!strcmp(szMsgName,"TeamInfo")) 
	{
		TeamInfoOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, TeamInfo );
	}
	if (!strcmp(szMsgName,"CurWeapon")) 
	{
		CurWeaponOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, CurWeapon );
	}
	if (!strcmp(szMsgName,"ScoreAttrib")) 
	{
		ScoreAttribOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, ScoreAttrib );
		pEngfuncs->pfnHookUserMsg = gEngfuncs.pfnHookUserMsg;
	}
    if (!strcmp(szMsgName,"SetFOV")) 
	{
		SetFOVOrg= pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, SetFOV );
	}
	if (!strcmpi(szMsgName,"TextMsg")) 
	{
		TextMsgOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, TextMsg );
	}
	if (!strcmpi(szMsgName,"ResetHUD")) 
	{
		ResetHUDOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, ResetHUD );
	}
	if (!strcmpi(szMsgName,"WeaponList"))
	{
		WeaponListOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, WeaponList);
	}
	if (!strcmpi(szMsgName,"Money"))
	{
		MoneyOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, MsgMoney);
	}
	if (!strcmpi(szMsgName,"Battery"))
	{
		BatteryOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, MsgBattery);
	}
	if (!strcmpi(szMsgName,"DeathMsg"))
	{
		DeathMsgOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, DeathMsg);
	}
	if (!strcmpi(szMsgName,"SayText"))
	{
		SayMsgOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, SayMsg);
	}
	if (!strcmpi(szMsgName,"ScoreInfo"))
	{
		ScoreInfoOrg = pfn;
		retval = gEngfuncs.pfnHookUserMsg (szMsgName, ScoreInfo);
	}
	return retval;	
}
     
//===================================================================================
int	( *OrigStudioDrawPlayer	)( int flags, struct entity_state_s *pplayer );
int	 NewStudioDrawPlayer ( int flags, struct entity_state_s *pplayer )
{
	int ret = OrigStudioDrawPlayer ( flags, pplayer );
	return ret;
}

//===================================================================================
inline bool isEnemy(int ax)
{
	return (me.team != vPlayers[ax].team);
}

//===================================================================================
ColorEntry* PlayerColor(int ax)
{
	static ColorEntry colorDefault;
	if (vPlayers[ax].team == 1) { return colorList.get(1); }
	else                        { return colorList.get(2); }
}

//===================================================================================
HSPRITE ch_sniper=-1, sniper_scope=-1;
void drawCrosshair()
{
	if( me.inZoomMode ) { return; }
	int centerX = displayCenterX;
	int centerY = displayCenterY;
	ColorEntry* clr = colorList.get(0);
	int r=clr->r, g=clr->g, b=clr->b, a=clr->a;
}

//===================================================================================
char* gGetWeaponName( int weaponmodel )
{
	static char weapon[50];
	weapon[0]=0;
	model_s* mdl = IEngineStudio.GetModelByIndex( weaponmodel );
	if( !mdl ){ return weapon; }
	char* name = mdl->name;  if( !name )          { return weapon; }
	int len = strlen(name);  if( len>48 || len<10){ return weapon; }
	strcpy(weapon,name+9); len -=9;
	if(len>4)weapon[len-4]=(char)0;
	return weapon;
}
//===================================================================================
void calcRadarPoint(float* origin, int& screenx, int& screeny)
{
	float dx  = origin[0] - me.pmEyePos[0];
	float dy  = origin[1] - me.pmEyePos[1];
	float x =  dy*me.minus_cos_yaw + dx*me.sin_yaw;
	float y =  dx*me.minus_cos_yaw - dy*me.sin_yaw;
	float& range = cvar.radar_range;
	if(fabs(x)>range || fabs(y)>range)
	{ 
		if(y>x)
		{
			if(y>-x) {
				x = range*x/y;
				y = range;
			}  else  {
				y = -range*y/x; 
				x = -range; 
			}
		} else {
			if(y>-x) {
				y = range*y/x; 
				x = range; 
			}  else  {
				x = -range*x/y;
				y = -range;
			}
		}
	}
		screenx = cvar.radar_x+int(x/cvar.radar_range*float(cvar.radar_size));
	screeny = cvar.radar_y+int(y/cvar.radar_range*float(cvar.radar_size));

}
//===================================================================================
void drawRadarPoint(float* origin,int r,int g,int b,bool blink=false)
{
	int screenx,screeny;
	calcRadarPoint(origin,screenx,screeny);
	if(!blink || blinkPhase){ gEngfuncs.pfnFillRGBA(screenx,screeny,3,3,r,g,b,255);}
}
//===================================================================================
void drawRadarFrame()
{
	int& centerx = cvar.radar_x;
	int& centery = cvar.radar_y;
	int& size    = cvar.radar_size;
	ColorEntry* clr;
	clr = colorList.get(12);
	tintArea(centerx-size, centery-size, 2*size+2, 2*size+2, clr);
//	consoleBorder(centerx-size,centery-size,2*size,2*size);
	clr = colorList.get(11);
	gEngfuncs.pfnFillRGBA(centerx,centery-size,1,2*size,clr->r,clr->g,clr->b,clr->a);
	gEngfuncs.pfnFillRGBA(centerx-size,centery,2*size,1,clr->r,clr->g,clr->b,clr->a);
}


//===================================================================================
inline void gDrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius=1)
{
	int radius2 = radius<<1;
	gEngfuncs.pfnFillRGBA(x-radius+2, y-radius, radius2-2,2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x-radius, y-radius, 2,radius2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x-radius, y+radius, radius2,2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x+radius, y-radius, 2,radius2+2,r,g,b,alpha);
}
//======================================================================================
void consoleBorder(int x,int y,int w, int h,int r,int g,int b)
{
	oglSubtractive = true;
    
	gEngfuncs.pfnFillRGBA( x-6, y-6, w+11, 1 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x-6, y-6, 1, h+12 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x+w+5, y-6, 1, h+12 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x-6, y+h+6, w+12, 1 , 0, 0, 0, 255 );

/*	gEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x-1, y-1, 1, h+2 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x-1, y+h+1, w+2, 1 , 0, 0, 0, 255 );
	gEngfuncs.pfnFillRGBA( x+w, y-1, 1, h+2 , 0, 0, 0, 255 );

	gEngfuncs.pfnFillRGBA( x-2, y-2, w+4, 1 , r/2, g/2, b/2, 255 );
	gEngfuncs.pfnFillRGBA( x-2, y-2, 1, h+4 , r/2, g/2, b/2, 255 );
	gEngfuncs.pfnFillRGBA( x-5, y+h+5, w+10, 1 , r/2, g/2, b/2, 255 );
	gEngfuncs.pfnFillRGBA( x+w+4, y-4, 1, h+10 , r/2, g/2, b/2, 255 );

	gEngfuncs.pfnFillRGBA( x-4, y-4, w+8, 2 , 0, 0, 5, 255 );
	gEngfuncs.pfnFillRGBA( x-4, y-4, 2, h+8 , 0, 0, 5, 255 );
	gEngfuncs.pfnFillRGBA( x-4, y+h+3, w+8, 2 , 0, 0, 5, 255 );
	gEngfuncs.pfnFillRGBA( x+w+2, y-4, 2, h+9 , 0, 0, 5, 255 );*/

	oglSubtractive = false;
	gEngfuncs.pfnFillRGBA( x-5, y-5, w+10, 1 , 0, 0, 0, 192 );
	gEngfuncs.pfnFillRGBA( x-5, y-5, 1, h+10 , 0, 0, 0, 192 );
	gEngfuncs.pfnFillRGBA( x-2, y+h+2, w+4, 1 , 0, 0, 0, 192 );
	gEngfuncs.pfnFillRGBA( x+w+1, y-1, 1, h+3 , 0, 0, 0, 192 );
}

static OgcCommandMenu::Menu* curMenu = ogcMenu.baseMenu;
bool  menu_active = false;
bool gMenuActive(){ return menu_active; }

//===================================================================================
void drawMenu(  )
{
	if(!curMenu) return;
	vector<OgcCommandMenu::MenuEntry>& items = curMenu->items;
	int x=cvar.menu_x, y=cvar.menu_y, w=cvar.menu_w;
	enum{ TEXT_H = 20 };
	int title_h = TEXT_H+5;
	int frame_y = cvar.menu_y - title_h;
	int frame_h = items.size()*TEXT_H + title_h;

	// title background
	tintArea(x,frame_y,w,title_h,colorList.get(21));
	consoleBorder(x,frame_y,w,frame_h);
	// upper background
	int h1 = curMenu->selection*TEXT_H;
	if(h1)
	{
		tintArea(x,y,w,h1,colorList.get(21));
		y+=h1;
	}
	// selection background
	tintArea(x,y,w,TEXT_H,colorList.get(22));
	y+=TEXT_H;
	// lower background
	int numItemsLeft = items.size() - curMenu->selection - 1;
	if(numItemsLeft>0)
	{
		tintArea(x,y,w,numItemsLeft*TEXT_H,colorList.get(21));
	}
	// draw frame
//	register int c = 200;
	gEngfuncs.pfnFillRGBA(x,frame_y,w,1        ,80,80,140,70);
	gEngfuncs.pfnFillRGBA(x,frame_y,1,frame_h  ,80,80,140,70);
	gEngfuncs.pfnFillRGBA(x+w,frame_y,1,frame_h,80,80,140,70);
	gEngfuncs.pfnFillRGBA(x,frame_y+frame_h,w,1,80,80,140,70);
	gEngfuncs.pfnFillRGBA(x,cvar.menu_y,w+1,1  ,80,80,140,70);
	DrawHudString(cvar.menu_x+10,frame_y+5,255,255,255,const_cast<char*>(curMenu->name.c_str()));
	y=cvar.menu_y+4;
	x+=4;
	for(unsigned int i=0;i<items.size();i++)
	{
		OgcCommandMenu::MenuEntry& item = items[i];
		ColorEntry* clr = colorList.get(23); // "menu_text"
		gEngfuncs.pfnDrawSetTextColor(clr->onebased_r,clr->onebased_g,clr->onebased_b);
		gEngfuncs.pfnDrawConsoleString(x,y,const_cast<char*>(item.name.c_str()));
		y+=TEXT_H;
	}
}

//===================================================================================
void menu_describe_current()
{
	if(!menu_active||!curMenu) return;
	OgcCommandMenu::MenuEntry& entry = curMenu->items[curMenu->selection];
	strcpy(gHudMessage2, entry.description.c_str());
	gHudTimer2.countdown(6);
}
//===================================================================================
void sendScriptSignal(const char* name)
{
	char signal[32];
	sprintf(signal,"sig%s",name);
	cmd.exec(signal);
}

//===================================================================================
inline void drawPlayerEsp(int ax)
{
	if( !vPlayers[ax].getPVS() ) { return; }
	cl_entity_s* ent = vPlayers[ax].getEnt();
	ColorEntry* color = PlayerColor(ax);
	int r = color->r * cvar.esp_trans;
	int g = color->g * cvar.esp_trans;
	int b = color->b * cvar.esp_trans;
	int a = color->a * cvar.esp_trans;
	oglSubtractive = true;
	switch(cvar.radar)
	{
		case 0: break;
		case 1: drawRadarPoint(ent->origin,color->r,color->g,color->b); break;
		case 2: if( isEnemy(ax)) drawRadarPoint(ent->origin,color->r,color->g,color->b); break;
		case 3: if(!isEnemy(ax)) drawRadarPoint(ent->origin,color->r,color->g,color->b); break;
	}
	oglSubtractive = false;
	float vecScreen[2];
	if (!CalcScreen(ent->origin,vecScreen)){ return; }
	float distance = vPlayers[ax].distance/22.0;
	int   boxradius = (300.0*90.0) / (distance*fCurrentFOV);
	BOUND_VALUE(boxradius,1,200);
	int x = vecScreen[0];
	int y = vecScreen[1];
	const char* format_string; 
	const char* format_int;
	if (vPlayers[ax].visible) { format_string="%s";  format_int="%i"; }
	else                      { format_string="-%s-";format_int="-%i-"; }
	enum{ CHAR_HEIGHT = 16 };
	int text_dist = (int)(cvar.esp_offs*(float)boxradius);
	int ystep = -CHAR_HEIGHT;
	if (cvar.weapon)
	{
		DrawHudStringCenter(x,y,r,g,b,format_string,gGetWeaponName(ent->curstate.weaponmodel));
		y += ystep;
	}
	if (cvar.name )
	{
		char displayname[32];
		int len = (cvar.name==1) ? 8 : cvar.name;
		if(len<0 || len>30) { len = 30; }
		strncpy(displayname,vPlayers[ax].entinfo.name,len);
		displayname[len] = 0;
		DrawHudStringCenter(x,y,r,g,b,format_string,displayname); 
		y += ystep;
	}
}
//===================================================================================
inline void drawEsp(int ab)
{
	float vecScreen[2];
	char weapon[64];
	vec3_t EntViewOrg;
	VectorCopy(vEntity[ab].ent->origin,EntViewOrg);		
	memset(&weapon,0,50);
	if(!(    vEntity[ab].ent 
		&& vEntity[ab].ent->model
		&& vEntity[ab].ent->model->name ))   { return; }
	char*        text = vEntity[ab].ent->model->name;
	ColorEntry* color = colorList.get(9);
	bool      special = false;
	int           len = strlen(text);
	if( len<8 ) { return; }			
	char* w_pos = strstr(text, "w_");
	if (len>10 && w_pos)
	{
		strcpy(weapon,text+9);
		if( (w_pos[2]=='c'&&w_pos[3]=='4')||
			(w_pos[2]=='b'&&w_pos[3]=='a'&&w_pos[4]=='c')  )
		{ 
			color = colorList.get(3); special=true; 
		}
	}
	else 
	{
		strcpy(weapon,text+7);
		vec3_t forward,right,up;
		gEngfuncs.pfnAngleVectors (vEntity[ab].ent->angles, forward, right, up);
		EntViewOrg = EntViewOrg + up * 75;
		color   = colorList.get(3); 
		special = true;
	}
	if(cvar.radar&&special){ drawRadarPoint(EntViewOrg,color->r,color->g,color->b,special); }
	if(cvar.esp && CalcScreen(EntViewOrg,vecScreen) && (!special||blinkPhase) )
	{
		weapon[strlen(weapon)-4]=(char)0;
		DrawHudStringCenter(vecScreen[0],vecScreen[1],color->r,color->g,color->b,weapon);
	}
}

//===================================================================================
void ogc_exec(const char* filename)
{
	string file = getOgcDirFile(filename);
	file += ".cfg";
	cmd.execFile( file.c_str() );
}

//===================================================================================
void playerCalcExtraData(int ax, cl_entity_s* ent)
{
	PlayerInfo& r = vPlayers[ax];
	if( ent->curstate.effects&EF_NODRAW ) { r.clearPVS(); return; }
	if( ent==me.ent                     ) { r.clearPVS(); return; }
	switch(ent->curstate.rendermode)
	{
	case kRenderTransColor:
	case kRenderTransTexture: 
		if( ent->curstate.renderamt<128 ) { r.clearPVS(); return; }
		break;
	case kRenderNormal:
		break;
	default:
		r.clearPVS();
		return;
	}
	if(!r.getPVS()) return;
	r.distance  = GetDistanceFrom (ent->origin); if(r.distance<1) r.distance=1;
	gAimbot.CalcFovangleAndVisibility(ax);
}

//========================================================================================
void drawSound()
{
	int old = cvar.confont;cvar.confont = 1;
	for(int pri=0;pri<3;pri++)
	for(int i=0;i<cvar.soundmax;i++)
	{
		SoundMarker& ref = vecSoundMarker[i];
		if( ref.priority==pri && ref.timer.running() )
		{  
			ColorEntry* clr = colorList.get(6);
			if(cvar.sounddisplay==1)
			{
				float timeleft = ref.timer.timeleft();
				int alpha = (int)( (255.0/cvar.soundtime)*(timeleft) );
				float distance = GetPseudoDistance(me.pmEyePos, ref.origin);
				int boxradius = (int)(3000.0/distance);
				BOUND_VALUE(boxradius,2,40);
				clr->compiled &= 0xFFFFFF00;
				clr->compiled += alpha;
				gDrawFilledBoxAtLocation( ref.origin, clr->compiled, boxradius );
			} else if(cvar.sounddisplay==2) {
				float vecScreen[2];
				if (CalcScreen(ref.origin,vecScreen))
				{
					DrawHudStringCenter(vecScreen[0],vecScreen[1],ref.color->r,ref.color->g,ref.color->b,ref.description);
				}
			}

			if(cvar.soundradar)
			{
				int screenx, screeny;
				calcRadarPoint(ref.origin, screenx,screeny);
				gEngfuncs.pfnFillRGBA(screenx,screeny,2,2,ref.color->r,ref.color->g,ref.color->b,255);
			}
		}
	}
	cvar.confont = old;
}
int ns;
//===================================================================================
void hudDrawInfoTexts()
{
	int line_y = displayCenterY*2-180;
	int line_x = 20;
	#define DECREMENT_LINE_Y { line_y-=16; }

	if (cvar.info)
	{
		ColorEntry* clr=colorList.get(13);

		if(gHudTimer.running())
		{
			DrawHudString(line_x,line_y,clr->r,clr->g,clr->b, gHudMessage );
			DECREMENT_LINE_Y
		}
        
		if( gHudTimer2.running() )
		{
			DrawHudString(line_x,line_y,clr->r,clr->g,clr->b,gHudMessage2 );
			DECREMENT_LINE_Y
		}
	}

}

//========================================================================================
void UpdateScreenInfo(void)
{
	screeninfo.iSize=sizeof(SCREENINFO);
	gEngfuncs.pfnGetScreenInfo(&screeninfo);
	displayCenterX = screeninfo.iWidth/2;
	displayCenterY = screeninfo.iHeight/2;
}
//===================================================================================
int HUD_Redraw (float x, int y)
{
	int nResult=0;
	if(cvar.chase_cam)
	{	
		cmd.exec("chase_active hide 1;r_drawviewmodel hide 0");
		cvar.chaseofs = -100;
	} else {
		cmd.exec("chase_active hide 0;r_drawviewmodel hide 1");
		cvar.chaseofs = 0.0;
	}
     if(cvar.chaseup) 
   { 
      gEngfuncs.pfnGetCvarPointer("chase_up")->value = cvar.chaseup; 
   }else{ 
      gEngfuncs.pfnGetCvarPointer("chase_up")->value = 0; 
   } 
   if(cvar.chaseback) 
   { 
      gEngfuncs.pfnGetCvarPointer("chase_back")->value = cvar.chaseback; 
   }else{ 
      gEngfuncs.pfnGetCvarPointer("chase_back")->value = 0; 
   } 
	ClientTime::reportMapTime(gEngfuncs.GetClientTime());
	me.ent = gEngfuncs.GetLocalPlayer();
	hudDrawInfoTexts();
	if (cvar.radar)	      { drawRadarFrame      (); }
	if (menu_active)	  { drawMenu            (); }
	if (cvar.soundfilter) { drawSound           (); }
	for( ScheduleList::iterator pos = scheduleList.begin();
		 pos != scheduleList.end();
		 ++pos)
	{
		if( (*pos).timer.expired() )
		{
			cmd.exec( const_cast<char*>((*pos).cmd.c_str()) );
			scheduleList.erase(pos);
			break;
		}	
	}
	static bool done=false;
	if(!done)
	{ 
		UpdateScreenInfo(); 
		done=true; 
	}
	static bool previous_alive_state=false;
	if     (  previous_alive_state && !me.alive )  { sendScriptSignal("dead" );  }
	else if( !previous_alive_state &&  me.alive )  { sendScriptSignal("alive" ); }
	previous_alive_state = me.alive;
	if(blinkTimer.expired())
	{
		blinkPhase = !blinkPhase;
		if(blinkPhase){ blinkTimer.countdown(0.2f); }
		else          { blinkTimer.countdown(0.1f); }
	}
	if (cvar.flash)
	{
		static screenfade_t sf = { 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0 }; 
		gEngfuncs.pfnSetScreenFade( &sf ); 
	}
	if(cvar.esp||cvar.radar) 
	for (int ab=0;ab<vEntity.size();ab++) drawEsp(ab);
	vEntity.clear();

	for (int ax=0;ax<vPlayers.size();ax++)
	{
		cl_entity_s* ent = vPlayers[ax].getEnt();
		int seqinfo = Cstrike_SequenceInfo[ent->curstate.sequence];
		float vecScreen[3];
		if(CalcScreen(vPlayers[ax].getEnt()->origin,vecScreen))
		{
			int x = vecScreen[0];
			int y = vecScreen[1];
			float val1 = vPlayers[ax].getEnt()->curstate.animtime;
			float val2 = vPlayers[ax].lastMsg;
			static lastSame[36] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
			static repeat[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			if(!lastSame[ax] && val1==val2)
			{
				repeat[ax]++;
				lastSame[ax] = true;
			}
			else if(lastSame[ax] && val1==val2)
				repeat[ax]++;
			else if(lastSame[ax] && val1!=val2)
			{
				repeat[ax] = 0;
				lastSame[ax] = false;
			}
			if(vPlayers[ax].getPVS() && vPlayers[ax].getEnt()->curstate.solid)
			{
				if(cvar.frzn_esp)
				{
					if(val1=val2 && repeat[ax]>34)
						DrawHudStringCenter(x,y+32,64,144,255,"frozEn");
				}
			}
		vPlayers[ax].lastMsg = vPlayers[ax].getEnt()->curstate.animtime;
		}
		if (vPlayers[ax].getPVS()) drawPlayerEsp(ax);
	}
	if(cvar.avdraw) gAimbot.Draw();
	gConsole.draw(cvar.con_x,cvar.con_y,cvar.con_w,cvar.con_h);

    if(cvar.spreaddraw)	DrawHudString(40,400,255,255,255,"%f", me.spread.spreadvar);

	return nResult;
}
//===================================================================================
void ActivatePlayer(struct cl_entity_s *ent)
{
	int px = ent->index;
	if(cvar.alive_method==1)
	{
		if(ent->curstate.solid) { vPlayers[px].setAlive(); }
		
		if(ent==me.ent) { me.alive = vPlayers[px].getAlive(); }
	}
	else if(cvar.alive_method==2)
	{
		if(ent->curstate.solid) { vPlayers[px].setAlive(); }
		else                    { vPlayers[px].setAlive(false);}
		
		if(ent==me.ent) { me.alive = vPlayers[px].getAlive(); }
	}
	vPlayers[px].setPVS();
	gEngfuncs.pfnGetPlayerInfo(ent->index, &vPlayers[px].entinfo);
	if(ent == me.ent) 
	{ 
		vPlayers[px].distance = 100000.0;
		vPlayers[px].visible  = 0;
		vPlayers[px].fovangle = 360;
		vPlayers[px].clearPVS(); 
		return;
	}
	playerCalcExtraData(px,ent);
}

//===================================================================================
void AddEntity(struct cl_entity_s *ent)
{
	hostage_info dummy;
	dummy.ent = ent;
	if(vEntity.size()<64) vEntity.push_back(dummy);
}

//===================================================================================
inline void drawLine(float*from, float*to, ColorEntry*color)
{
	gEngfuncs.pEfxAPI->R_BeamPoints(from,to,beamindex,0.001f,0.9f,0.5,32,2,0,10,color->fr,color->fg,color->fb);
}

//===================================================================================
void HUD_TempEntUpdate (
		double frametime,   			
		double client_time, 			
		double cl_gravity,  			
		TEMPENTITY **ppTempEntFree,   	
		TEMPENTITY **ppTempEntActive,
		int		( *Callback_AddVisibleEntity )( cl_entity_t *pEntity ),
		void	( *Callback_TempEntPlaySound )( TEMPENTITY *pTemp, float damp ) )
{
	typedef void (*HUD_TempEntUpdateFunc)(double,double,double,TEMPENTITY**,TEMPENTITY**, int(*)(cl_entity_t*),void(*)(TEMPENTITY*,float));	
	(*(HUD_TempEntUpdateFunc)(pHUD_TempEntUpdate))(
		frametime,
		client_time,
		cl_gravity,
		ppTempEntFree,
		ppTempEntActive,
		Callback_AddVisibleEntity,
		Callback_TempEntPlaySound 
		);
	me.ent = gEngfuncs.GetLocalPlayer();
	ClientTime::reportMapTime( client_time );
	if(!beamindex) { beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr"); }
}

//===================================================================================
void PreV_CalcRefdef (struct ref_params_s *pparams)
{
	VectorCopy(pparams->punchangle,me.punchangle);
	if (cvar.recoil) for (int i=0; i<3; i++) pparams->punchangle[i] = 0; 	
	if( pparams->nextView == 0 )
	{
		if (cvar.spec_fix)
		{	
			gAimbot.FindTarget();
		} else {
			gAimbot.target = -1;
		}

		if ( !me.alive) { cvar.speed = 0.0; }
		if ( cvar.aim ) 
		if ( (me.iClip && me.pmMoveType!=5) || !me.alive )
		if(cvar.FBIpred)
		{
			if(currentWeaponID>=WEAPON_DEAGLE || currentWeaponID>=WEAPON_GLOCK || currentWeaponID>=WEAPON_USP)
			{
				cvar.pred = 0;
			}
			if(currentWeaponID>=WEAPON_COLT || currentWeaponID>=WEAPON_AK || currentWeaponID>=WEAPON_SIG)
			{
				cvar.pred = 2;
			}
		}
		if ( gAimbot.target!=-1 )
		if ( pparams->nextView == 0 )
		{
			Aimbot_active = true;
			gAimbot.CalcViewAngles();
			VectorCopy(gAimbot.aim_viewangles, pparams->viewangles);
			VectorCopy(gAimbot.aim_viewangles, pparams->cl_viewangles);
		}
		for (int ax=0;ax<vPlayers.size();ax++) 
		{
			vPlayers[ax].decrPVS();
			vPlayers[ax].numTargetSpots = 0; 
		}
	}
}

//===================================================================================
void PostV_CalcRefdef (struct ref_params_s *pparams)
{
	if( pparams->nextView == 0 )
	{
		VectorCopy(pparams->viewangles,mainViewAngles);
		VectorCopy(pparams->vieworg,mainViewOrigin);
		if(cvar.chaseofs)
		{
			vec3_t vecEnd, up, right, forward, EntViewOrg;
			gEngfuncs.pfnAngleVectors (mainViewAngles, forward, right, up);
			mainViewOrigin[0] += cvar.chaseofs*forward[0];
			mainViewOrigin[1] += cvar.chaseofs*forward[1];
			mainViewOrigin[2] -= cvar.chaseofs*forward[2];
		}
	}
}

//===================================================================================
void V_CalcRefdef (struct ref_params_s *pparams)
{
	me.ent = gEngfuncs.GetLocalPlayer();
	PreV_CalcRefdef(pparams);
	pV_CalcRefdef(pparams);
	PostV_CalcRefdef(pparams);
}
bool IsFrozen(void)
{
	if (cvar.mefrozen)
		return gEngfuncs.GetClientTime() - me.ent->ph[me.ent->current_position].animtime > cvar.mefrozen;
	return false;
}
	  
void opengl_ClearBackground();
//===================================================================================
void CL_CreateMove (float frametime, struct usercmd_s *usercmd, int active)
{	
	static StopTimer nextSwitchWait;
	static float recoiltime = 0;
	float offset[3];
    me.ent = gEngfuncs.GetLocalPlayer();
	bNewFrame = true;
	if (cvar.wall >= 1)  { opengl_ClearBackground(); }
	if (IsFrozen())
	{
		cvar.speed = cvar.fspeed;

		return;
	}
	if (gAimbot.target != -1 && me.iClip && me.pmMoveType != 5 && cvar.aim) 
	{
			VectorCopy              ( gAimbot.aim_viewangles, usercmd->viewangles );
			gEngfuncs.SetViewAngles ( gAimbot.aim_viewangles);
	}
	if ((gAimbot.target!=-1) && cvar.aim)
	{
		if (me.iClip)
		{
			if (cvar.shoot)
			{
				usercmd->buttons |= IN_ATTACK;
			}
		} else {
			if (playerItems.canSwitch() && nextSwitchWait.expired())
			{
				nextSwitchWait.countdown(.001);
				cmd.exec("#slot2");
			}
			if (playerItems.canswitch() && nextSwitchWait.expired())
			{
				nextSwitchWait.countdown(.001);
				cmd.exec("#slot1");
			}
		}
	}
	if (usercmd->buttons&IN_ATTACK && me.alive)
	{ 
		if (!me.iClip && playerItems.canSwitch() && nextSwitchWait.expired())
		{ 
			nextSwitchWait.countdown(.001);
			cmd.exec("#slot2");
		}
		if (!me.iClip && playerItems.canswitch() && nextSwitchWait.expired())
		{
			nextSwitchWait.countdown(.001);
			cmd.exec("#slot1");
		}
		if (currentWeaponID>=WEAPON_USP && currentWeaponID<=WEAPON_FIVE7)
		{
			static bool flipFlop = true;
			if(flipFlop){ usercmd->buttons |= IN_ATTACK;    }
			else        { usercmd->buttons &= (~IN_ATTACK); }
			flipFlop = !flipFlop;
		}
		if (cvar.aspeed>1 && currentWeaponID!=WEAPON_C4 && currentWeaponID!=WEAPON_HE && currentWeaponID!=WEAPON_FLASH && currentWeaponID!=WEAPON_SMOKE)
		{
			cvar.speed = cvar.aspeed;
		}
		if (cvar.aspeed>1 && currentWeaponID!=WEAPON_C4)
		{
			cvar.speed = cvar.aspeed;
		}
	} else {
		if (cvar.aspeed>1)
		{ 
		cvar.speed = 1.0; 
		}
	}

	if (currentWeaponID!=WEAPON_HE && currentWeaponID!=WEAPON_FLASH && currentWeaponID!=WEAPON_SMOKE && currentWeaponID!=WEAPON_C4 && me.alive)
	{ cvar.aim = 1;} else { cvar.aim = 0;}
	if (me.alive)
	{
		if (cvar.nospread == 1)
		{
			GetRecoilOffset(me.spread.random_seed, 1, usercmd->viewangles, me.pmVelocity, offset);
			usercmd->viewangles[0] += offset[0];
			usercmd->viewangles[1] += offset[1];
			usercmd->viewangles[2] += offset[2];
		}

		if (cvar.recoil)
		{
			usercmd->viewangles[0] -= (me.punchangle[0] * cvar.recoil);
			usercmd->viewangles[1] -= (me.punchangle[1] * cvar.recoil);
		}
		if (usercmd->buttons&IN_FORWARD && me.alive && cvar.autojump)
{
  if (me.pmVelocity[0]==0 && me.pmVelocity[1]==0  && me.pmVelocity[2]==0 )
  {
   usercmd->buttons |= IN_JUMP; 
  }

}
		if (cvar.jump  && (usercmd->buttons & IN_JUMP) && !(me.pmFlags & FL_ONGROUND)) usercmd->buttons &= ~IN_JUMP;
		if (cvar.duck && !(me.pmFlags & FL_ONGROUND)  && me.pmVelocity[2]>0)  usercmd->buttons |= IN_DUCK; 
	}
}


//===================================================================================
void HUD_PlayerMove (struct playermove_s *a, int b)
{
	me.ent = gEngfuncs.GetLocalPlayer();
	me.pmFlags = a->flags;
	me.pmMoveType = a->movetype;
    VectorCopy(a->velocity,me.pmVelocity);
	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(me.pmEyePos);
	if(me.alive) me.pmEyePos[2] += cvar.height_correction;
    me.pmEyePos[0]+= a->origin[0];
	me.pmEyePos[1]+= a->origin[1];
	me.pmEyePos[2]+= a->origin[2];
	float yaw = a->angles[1] * (3.141592/180 );
	me.sin_yaw       =  sin(yaw);
	me.minus_cos_yaw = -cos(yaw);
	me.viewAngles[0] = a->angles[0];
	me.viewAngles[1] = a->angles[1];
	me.viewAngles[2] = a->angles[2];
}


static void playerRenderOptions(cl_entity_s* ent)
{
	ColorEntry* color = PlayerColor(ent->index);
}

int AddEntResult = 1;

//===================================================================================
void HUD_AddEntity (int type, struct cl_entity_s *ent, const char *modelname)
{
	int retval = 1;
	static int beamindex = 0;
	if (ent->player){
		ActivatePlayer(ent);
		if(vPlayers[ent->index].getPVS()) {
			ent->curstate.rendermode = 0;
			playerRenderOptions(ent);
		}
	}else if(strstr(modelname, "w_") || strstr(modelname, "hostage") || strstr(modelname, "scientist")) {
		AddEntity(ent);
	}
	else if (type==ET_NORMAL && cvar.trans && ent->curstate.solid==SOLID_BSP)
	{
			ent->curstate.rendermode=2;
			ent->curstate.renderamt=cvar.trans;
	}
	if (cvar.smoke && (strstr(modelname, "black_smoke")||strstr(modelname, "fast_wallpuff"))) 
		retval= 0;
	if (cvar.smoke > 1 && strstr(modelname, "steam1"))
		retval= 0;
	AddEntResult = retval;
}


//===================================================================================
HSPRITE	SPR_Load(const char *szPicName)
{
	HSPRITE spr = gEngfuncs.pfnSPR_Load (szPicName);
	if(strstr(szPicName,"sniper_scope"))   sniper_scope = spr;
	else if(strstr(szPicName,"ch_sniper")) ch_sniper = spr;
    add_sprite (spr, szPicName);
	return spr;
}

//===================================================================================
bool passesSoundFilter(const float* const origin,const int bx)
{
	if(cvar.soundfilter==4)
		return true;
	if( GetPseudoDistance(origin,me.pmEyePos)<cvar.soundtol){ return false; }
	int ax;
	switch(cvar.soundfilter)
	{
	case 1:
		for (ax=0;ax<vPlayers.size();ax++)
		{
			if( vPlayers[ax].getPVS()  ) 
			{
				if( GetPseudoDistance(vPlayers[ax].getEnt()->origin, origin)<cvar.soundtol )
				{
					return false;
				}
			}
		}
		return true;
	case 2:
		return (vPlayers[bx].team!=me.team);
	default:
		return true;
	}
}

//===================================================================================
inline void strcpy_x(char* dest, const char* pos)
{
	do{ 
		while( *pos>='0' && *pos<='9' )  ++pos;
		if(*pos=='.')              {*dest=0;break; }
		*dest=*pos;
		++dest;
		++pos; 
	}
	while(*pos);
}

//===================================================================================
inline void strcpy_x2(char* dest, const char* pos)
{
	do{ 
		if(*pos=='.'||*pos=='-'||*pos=='_') {*dest=0;break; }
		*dest=*pos;
		++dest;
		++pos; 
	}
	while(*pos);
}

//===================================================================================
void PreS_DynamicSound(const DWORD a,const DWORD b,const DWORD c,const char*const sample,
						 const float*const origin,DWORD f,const DWORD g,const DWORD h)
{
	if(cvar.soundfilter == -1)
		return;
	BOUND_VALUE(soundMarkerCurrentIndex,0,64);
	SoundMarker& ref = vecSoundMarker[soundMarkerCurrentIndex];
	string theMessage;
	int length;
	if(b>0 && b<=36)
	{
		theMessage = vPlayers[b].entinfo.name;
		length = theMessage.length();
		ref.team = vPlayers[b].team;
	}
	if(sample[0]=='p' && sample[3]=='y' && sample[6]=='/')
	{
		if(!passesSoundFilter(origin,b)) return;
		const char* pos = sample+7;
		if(pos[0]=='p' && pos[2]=='_')
		{
			if(pos[3]=='s' && pos[4]=='h' && pos[5]=='e' && pos[6]=='l' )  return;   
			pos+=3;
			ref.color = colorList.get(37+ref.team);
			ref.priority = 0;
		} else if( pos[0]=='b'&& pos[1]=='h' && pos[2]=='i' ) { 
			pos+=5;
			ref.color    = colorList.get(32);
			ref.priority = 2;
		}
		strcpy_x(ref.description,pos);
	}
	else
	if( sample[0]=='w' && sample[1]=='e' && sample[7]=='/' ) 
	{
		char* pos = (char*)sample+8;
		if(   (pos[0]=='r' && pos[1]=='i')    
			||(pos[0]=='b' && pos[1]=='u') )  
		{
			return;
		}
		if(!passesSoundFilter(origin,b)) return;
		ref.priority = 1;
		ref.color = colorList.get(37+ref.team);
		strcpy_x2(ref.description,pos);
		for(pos=(char*)ref.description;*pos;++pos){ *pos = toupper(*pos); }
	}
	else
	{
		return;
	}
	if(!cvar.soundfilter) { return; }
	VectorCopy( origin, ref.origin );
	ref.timer.countdown( cvar.soundtime );
	++soundMarkerCurrentIndex;
	if(soundMarkerCurrentIndex>=cvar.soundmax){ soundMarkerCurrentIndex=0;}
}

//===================================================================================
inline static void lowercase(char* str)
{
	while(*str){ *str = tolower(*str); ++str; }
}

//===================================================================================
void semicolonToZero(string& s)
{
	for(string::iterator pos = s.begin();pos!=s.end();pos++)
	{
		if(*pos==';') { *pos=0; }
	}
}

//===================================================================================
void func_alias()
{
	const char*   name     = cmd.argC(1);
	string&       content  = cmd.argS(2);
	cmd.AddAlias(name,content);
}

//===================================================================================
void func_bind()
{
	keyBindManager.addBind(cmd.argC(1),cmd.argC(2));
}

//===================================================================================
void func_unbind()
{
	keyBindManager.removeBind(cmd.argC(1));
}
//===================================================================================
void func_remap()
{
	keyBindManager.remapKey(cmd.argC(1),cmd.argC(2));
}

//===================================================================================
void hookCommandParser( char* cmdString, int size )
{
	char* pos       = cmdString;
	int   charsLeft = size;
	bool passThroughBlock = false;
	if( *(DWORD*)pos == cmd.excludePrefixDword )
	{
		passThroughBlock = true;
		pos[0]=pos[1]=pos[2]=pos[3]=' ';
		charsLeft-=4;
		pos+=4;
	}
	while(charsLeft>0 && (*pos==' '||*pos=='\t')){pos++;charsLeft--;}
	if(charsLeft<=0){ return; }
	char  command[32];
	char* commandPos = command;
	int   commandCharsLeft = 30;
	while(charsLeft>0 && *pos>' ' && commandCharsLeft>0)
	{
		*commandPos = tolower(*pos);
		commandPos++;
		pos++;
		charsLeft--;
		commandCharsLeft--;
	}
	*commandPos = 0;
	if(!*command) return;
	if(!strcmp(command,"messagemode")||!strcmp(command,"messagemode2"))
	{
		keyBindManager.messageMode = true;
	}
}

//=================================================================================== 
void CB_KeyManager_Exec(const string& commandlist)
{
	cmd.exec( const_cast<char*>( commandlist.c_str() ) );
}

//===================================================================================
void CB_KeyManager_ConType(char ch)
{
	gConsole.key(ch);
}

//===================================================================================
void gSetHudMessage2(const char* message)
{
	strcpy(gHudMessage2,message);
	gHudTimer2.countdown(4);
}

//===================================================================================
void gSetHudMessage(const char* message)
{
	strcpy(gHudMessage,message);
	gHudTimer.countdown(4);
}
//===================================================================================
void AtRoundStart(void)
{
	int Id;
	gAimbot.target = -1;
	static bool wlcm_msg = true;
	if (wlcm_msg)
	{
		wlcm_msg = false;
		strcpy(gHudMessage2,"got karma?");
		gHudTimer2.countdown(5);
	}
	cvar.spec_fix = true;
	playerItems.clearWeaponSlots();
	cvar.inpvs = 0;
	cvar.inpvs = 2;
	if(me.team>0 && cvar.alive_method==1 )
	{
		me.alive = true; 
	}
	sendScriptSignal("round");

	Id = playerItems.CurrentID();
	if (Id == WEAPONLIST_SG550 || Id == WEAPONLIST_G3SG1)
    me.spread.brokentime = 0; // This catches all cases except when you buy one and than shoot some bullets and that buy another




}
//===================================================================================
void AtMapChange()
{
	iRounds=0;
	bBombIsPlanted=false;
    fBombPlantedTime=0;
    iC4Timer=0;
	gAimbot.target = -1;
	playerItems.clearWeaponSlots();
	gHudTimer.setExpired();
	gHudTimer2.setExpired();

	me.spread.prcflags = 0;
	me.spread.random_seed = 0;
	me.spread.recoil = 0;
	me.spread.gtime = 0;
	me.spread.prevtime = 0;
	me.spread.brokentime = 0;
	me.spread.spreadvar = 0;
	me.spread.recoiltime = 0;
	me.spread.firing = false;
}
//===================================================================================
void HlEngineCommand(const char* command)
{
	if(!gEngfuncs.pfnClientCmd) { return; }
	gEngfuncs.pfnClientCmd( const_cast<char*>(command) );
}

//===================================================================================
void HandleCvarInt(char* name, int* value)
{
	char* arg1 = cmd.argC(1); 
	if (!strcmp(arg1,"change")) 
	{
		if (*value) *value=0; else *value=1; 
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s changed to %d", name, *value );gHudTimer.countdown(3);}
		return;
	}
	if (!strcmp(arg1,"up"))     
	{
		*value += cmd.argI(2);
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s increased to %d", name, *value );gHudTimer.countdown(3);}
		return;
	}
    if (!strcmp(arg1,"down"))   
	{ 
		*value -= cmd.argI(2);
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s decreased to %d", name, *value );gHudTimer.countdown(3);}
		return;
	}
	if (!strcmp(arg1,"hide")) 
    { 
        *value = cmd.argI(2);
        return;
    }
	if (!*arg1) 
	{ 
		Con_Echo( "CvaR &w%s&a = %i\n",name,*value); 
		return;
	}
	*value = cmd.argI(1);
	if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s: %d", name, *value );gHudTimer.countdown(3);}
}
//===================================================================================
void HandleCvarFloat(char* name, float* value)
{
	char* arg1 = cmd.argC(1);  
	if (!strcmp(arg1,"change")) 
	{
		if (*value) *value=0; else *value=1; 
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s changed to %f", name, *value );gHudTimer.countdown(3); }
		return;
	}
	if (!strcmp(arg1,"up"))     
	{ 
		*value += cmd.argF(2); 
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s increased to %f", name, *value );gHudTimer.countdown(3);}
		return;
	}
    if (!strcmp(arg1,"down"))   
	{ 
		*value -= cmd.argF(2); 
		if(cvar.info==3||menu_active){ sprintf(gHudMessage,"%s decreased to %f", name, *value );gHudTimer.countdown(3);}
		return;
	}
	if (!strcmp(arg1,"hide")) 
    { 
        *value = cmd.argI(2);
        return;
    }
	if (!*arg1) 
	{ 
		Con_Echo( "CVAR &w%s&r = %f\n",name,*value); 
		return;
	}
    *value = cmd.argF(1);
	if(cvar.info==3||menu_active){sprintf(gHudMessage,"%s: %f", name, *value );gHudTimer.countdown(3);}
}

//===================================================================================
bool isHlCvar(char* name)
{
	if(!gEngfuncs.pfnGetCvarPointer) { return false; }
	cvar_s* test = gEngfuncs.pfnGetCvarPointer(name);
	return (test!=NULL);
}

//===================================================================================
void HandleHlCvar(char* name)
{
	if(!gEngfuncs.pfnGetCvarPointer) { return; }
	cvar_s* ptr = gEngfuncs.pfnGetCvarPointer(name);
	if(!ptr) return;
	HandleCvarFloat(name,&ptr->value);
}
//===================================================================================
void func_menu_back()
{
	if(!menu_active || !curMenu) return;
	curMenu = curMenu->parent;
	if(!curMenu) 
	{ 
		curMenu = ogcMenu.baseMenu; 
		menu_active=false; 
		gHudTimer.countdown(0.1);
		sendScriptSignal("menuoff");
	}	
	curMenu->boundSelection();
	menu_describe_current();
}

//===================================================================================
void func_menu_up()
{
	if(!menu_active || !curMenu) return;
	--curMenu->selection;
	curMenu->boundSelection();
	menu_describe_current();
}

//===================================================================================
void func_menu_off()
{
	menu_active=false;
	gHudTimer.countdown(0.1);
	sendScriptSignal("menuoff");
}

//===================================================================================
void func_menu_down()
{
	if(!menu_active || !curMenu) return;
	++curMenu->selection;
	curMenu->boundSelection();
	menu_describe_current();
}

//===================================================================================
void func_menu_select()
{
	if(!curMenu) 
	{
		curMenu = ogcMenu.baseMenu;
		menu_active = true;
		menu_describe_current();
		return;
	}
	if(menu_active)
	{
		OgcCommandMenu::MenuEntry& entry = curMenu->items[curMenu->selection];
		if(entry.menu)
		{
			curMenu = entry.menu;
			curMenu->boundSelection();
			menu_describe_current();
		}
		else
		{
			cmd.exec(const_cast<char*>(entry.content.c_str()));
		}
	}
	menu_active=true;
}

//===================================================================================
void func_echo()
{
	string str;
	cmd.collectArguments(str);
	Con_Echo ("%s\n",str.c_str());
}

//===================================================================================
void func_text()
{
	string s;
	cmd.collectArguments(s);
	gSetHudMessage2( s.c_str() );
}

//===================================================================================
void func_avadd(void) 
{ 
   aimvec av; 
   int duck = cmd.argI(1); 
   int arg = 2; 
   while(*cmd.argC(arg)) 
   { 
      av.h = cmd.argF(arg++); 
      av.f = cmd.argF(arg++); 
      av.r = cmd.argF(arg++); 
      if (duck == 1)  
      {
          gAimbot.AimVecsD.push_back(av); 
      } 
      if (duck == 0) 
      { 
		  gAimbot.AimVecsS.push_back(av); 
      } 
      if (duck == 2)
	  {
          gAimbot.AimVecsJ.push_back(av); 
	  }
   } 
}

void func_avclear0(void) 
{ 
   gAimbot.AimVecsS.clear();
}
void func_avclear1(void) 
{ 
   gAimbot.AimVecsD.clear(); 
}
void func_avclear2(void) 
{ 
   gAimbot.AimVecsJ.clear();
}

//===================================================================================
void func_avlist(void)
{
   int dmmy = cmd.argI(1);
   if (dmmy == 1)
   {
       Con_Echo("Listing AimVecs(1) Ducking\n");
       for (vector<aimvec>::iterator si = gAimbot.AimVecsD.begin(); si != gAimbot.AimVecsD.end();++si)
       {
           Con_Echo("Height: &w%f&a Forward: &w%f&a Right: &w%f&a\n",si->h,si->f,si->r);
      }
   }
   if (dmmy == 0)
   {
       Con_Echo("Listing AimVecs(0) Standing / Walking\n");
       for (vector<aimvec>::iterator si = gAimbot.AimVecsS.begin(); si != gAimbot.AimVecsS.end();++si)
       {
         Con_Echo("Height: &w%f&a Forward: &w%f&a Right: &w%f&a\n",si->h,si->f,si->r);
       }
   }
   if (dmmy == 2)
   {
       Con_Echo("Listing AimVecs(2) Jumping\n");
       for (vector<aimvec>::iterator si = gAimbot.AimVecsJ.begin(); si != gAimbot.AimVecsJ.end();++si)
       {
           Con_Echo("Height: &w%f&a Forward: &w%f&a Right: &w%f&a\n",si->h,si->f,si->r);
       }
   }
}
//===================================================================================

void func_soundmax()
{
	HandleCvarInt  ("displayed sounds", &cvar.soundmax);
	BOUND_VALUE  (cvar.soundmax,0,64);
	for( int i=0;i<cvar.soundmax;i++)
	{
		vecSoundMarker[i].timer.setExpired();
	}
}

//===================================================================================
void func_later()
{
	if(cmd.argS(1).empty())
	{
		list<ScheduledCommand>::iterator pos;
		for(pos=scheduleList.begin();pos!=scheduleList.end();++pos)
		{
			Con_Echo("%6.2f sec: \"%s\"", (*pos).timer.timeleft(), (*pos).cmd.c_str() );
		}
		return;
	}
	if(cmd.argS(1)=="clear")
	{
		scheduleList.erase(scheduleList.begin(),scheduleList.end());
		return;
	}
	float time = cmd.argF(1);
	ScheduledCommand tmp;
	cmd.collectArguments( tmp.cmd, 2); 
	tmp.timer.countdown(time);
	if(scheduleList.size()<32) scheduleList.push_back(tmp);
}

//===================================================================================
void func_color()
{
	int arg=1;
	while(*cmd.argC(arg)){
		colorList.set( cmd.argC(arg  ), cmd.argC(arg+1), 
			 cmd.argC(arg+2),  cmd.argC(arg+3),  cmd.argC(arg+4));
		arg+=5;
	}
}

//===================================================================================
bool func_eval_condition(int& refArgIndex)
{
	char* test  = cmd.argC(refArgIndex++);
	char* szArg = cmd.argC(refArgIndex++);
	if(!*test||!*szArg){ return false; }
	if( !strcmp(test,"has"   )) { return  playerItems.hasWeapon(szArg);}
	if( !strcmp(test,"hasno" )) { return !playerItems.hasWeapon(szArg);}
	if( !strcmp(test,"is"    ))  
	{ 
		if(!strcmp(szArg,"t"    )) { return me.team==1; }
		if(!strcmp(szArg,"ct"   )) { return me.team!=1; }
	    if(!strcmp(szArg,"alive")) { return (me.alive); }
	}
	if(!strcmp(test,"money<" )){ return (playerItems.money<atoi(szArg)); }
	if(!strcmp(test,"money>="))
	{ 
		bool condition = (playerItems.money>=atoi(szArg)); 
		if(!condition){ strcpy(gHudMessage,"[OGC] Not enough money.");gHudTimer.countdown(1.5); }
		return condition;
	}
	if( !strcmp(test,"armor<" ) ){ return (playerItems.armor< atoi(szArg));}
	if( !strcmp(test,"armor>=") ){ return (playerItems.armor>=atoi(szArg));}
	return false;
}

//===================================================================================
void func_if()
{
	int  pos = 1;
	bool final_condition = func_eval_condition(pos);
	while(1){ 
		if( *cmd.argC(pos)=='&' ) {
			final_condition &= func_eval_condition(pos);
		} else if( *cmd.argC(pos)=='|' ) {
			final_condition |= func_eval_condition(pos);
		} else {
			break;
		} 
	}
	int truepos = pos;
	int elsepos = 0;
	while(1){
		char* arg = cmd.argC(pos);
		if(!*arg) { break; }
		if(!strcmp(arg,"else")) { elsepos = pos; break; }
		pos++;
	}
	string command;
	if( !elsepos )
	{
		if(final_condition)
		{
			cmd.collectArguments(command,truepos);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		}
	} else {
		if(final_condition)
		{
			cmd.collectArguments(command,truepos,elsepos-1);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		} else {
			cmd.collectArguments(command,elsepos+1);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		}
	}
}

void func_quit()     { ExitProcess(0); }
void func_exec()     { ogc_exec( cmd.argC(1) ); }
void func_colortag() { gConsole.setcolortag(cmd.argC(1)[0],cmd.argI(2),cmd.argI(3),cmd.argI(4)); }
void func_console_toggle() { gConsole.active = !gConsole.active; }   
//===================================================================================
void collectArgumentsX(string& dest,int from=1, int to=1000000)
{
	while(from<=to){
		char* arg = gEngfuncs.Cmd_Argv(from++);
		if(!*arg) { break;}
		dest += '\"';
		dest += arg ;
		dest += '\"';
		if(from!=to)dest += " ";
	};
}

//===================================================================================
void interface_func()
{
	string ogc_command = gEngfuncs.Cmd_Argv(1);
	ogc_command += " ";
	collectArgumentsX(ogc_command,2);
	cmd.exec( ogc_command.c_str() );
}

//===================================================================================
void func_interface() 
{
	if(cmd.argS(1).empty()) return;
	gEngfuncs.pfnAddCommand( strdup(cmd.argC(1)), interface_func );
}
void func_sigmenuoff() 
{
}
//===================================================================================
void Init_Command_Interpreter()
{
	cmd.init();
	cvar.init();
	cmd.AddCvarInt( "alive", (int*)&me.alive );
	cmd.AddCvarInt( "target", (int*)&gAimbot.target );

	#define REGISTER_COMMAND(c) cmd.AddCommand( #c, func_##c);    
	REGISTER_COMMAND(avadd      )        
	REGISTER_COMMAND(echo       )    
	REGISTER_COMMAND(later      )         
	REGISTER_COMMAND(alias      )                                         
	REGISTER_COMMAND(menu_select)  
    REGISTER_COMMAND(menu_back  )    
	REGISTER_COMMAND(colortag   )
    REGISTER_COMMAND(soundmax   )    
	REGISTER_COMMAND(menu_off   )  
	REGISTER_COMMAND(interface  )
	REGISTER_COMMAND(menu_up    )
	REGISTER_COMMAND(avclear0    )
	REGISTER_COMMAND(avclear1    )
	REGISTER_COMMAND(avclear2    )
	REGISTER_COMMAND(avlist     )
	REGISTER_COMMAND(text       ) 
	REGISTER_COMMAND(color      )
	REGISTER_COMMAND(if         )
	REGISTER_COMMAND(quit       ) 
	REGISTER_COMMAND(bind       )
	REGISTER_COMMAND(unbind     )
	REGISTER_COMMAND(exec       )
	REGISTER_COMMAND(menu_down  )
	REGISTER_COMMAND(sigmenuoff  )
	REGISTER_COMMAND(console_toggle)
	cmd.AddCommand("txt"     , func_text);
}


//=====================================================================================
int KeyEventResult = 1;
void HUD_Key_Event( int eventcode, int keynum, const char *pszCurrentBinding )
{
	const char PROC_NAME[50] = "HUD_Key_Event_Hook\0";
	int retval = 1;
	if (keynum >= 239 && keynum <= 244)
	{
		switch (keynum) {
			case 239:
				keyBindManager.notifyMouseEvent("mwheeldown", (eventcode == 1));
				break;
			case 240:
				keyBindManager.notifyMouseEvent("mwheelup", (eventcode ==1));
				break;
			case 241:
				keyBindManager.notifyMouseEvent("mouse1", (eventcode == 1));
				break;
			case 242:
				keyBindManager.notifyMouseEvent("mouse2", (eventcode == 1));
				break;
			case 243:
				keyBindManager.notifyMouseEvent("mouse3", (eventcode == 1));
				break;
			case 244:
				keyBindManager.notifyMouseEvent("mouse4", (eventcode == 1));
				break;
			case 245:
				keyBindManager.notifyMouseEvent("mouse5", (eventcode == 1));
				break;
		}
	}
	else
		if (eventcode)
			keyBindManager.notifyKeyEvent(keynum, true, false);
		else
			keyBindManager.notifyKeyEvent(keynum, false, false);

	retval = (keyBindManager.keyBlocked(keynum, (eventcode) ? true : false)) ? 0 : 1;

	if( gMenuActive() && (keynum >= 239 && keynum <= 244))
		retval = 0;

	KeyEventResult = retval;
}
void DefaultSpreadVar(int weaponid)
{
	if (weaponid == WEAPONLIST_DEAGLE)
		me.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_MP5)
		me.spread.spreadvar = 0;
	else if (weaponid == WEAPONLIST_AK47)
		me.spread.spreadvar = 0.2342f;
	else if (weaponid == WEAPONLIST_SG552)
		me.spread.spreadvar = 0.2342f;
	else if (weaponid == WEAPONLIST_AUG)
		me.spread.spreadvar = 0.3f;
	else if (weaponid == WEAPONLIST_M4A1)
		me.spread.spreadvar = 0.2342f;
	else if (weaponid == WEAPONLIST_M249)
		me.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_MAC10)
		me.spread.spreadvar = 0.15f;
	else if (weaponid == WEAPONLIST_UMP45)
		me.spread.spreadvar = 0;
	else if (weaponid == WEAPONLIST_TMP)
		me.spread.spreadvar = 0.2f;
	else if (weaponid == WEAPONLIST_P90)
		me.spread.spreadvar = 0.15f;
	else if (weaponid == WEAPONLIST_P228)
		me.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_FIVESEVEN)
		me.spread.spreadvar = 0.92f;
	else if (weaponid == WEAPONLIST_ELITE)
		me.spread.spreadvar = 0.88f;
	else if (weaponid == WEAPONLIST_GLOCK18)
		me.spread.spreadvar = 0.9f;
	else if (weaponid == WEAPONLIST_USP)
		me.spread.spreadvar = 0.92f;
	else if (weaponid == WEAPONLIST_G3SG1)
		me.spread.spreadvar = 0.2342f;
	else if (weaponid == WEAPONLIST_SG550)
		me.spread.spreadvar = 0.2342f;
	else
		me.spread.spreadvar = 0;
}

//===================================================================================
void HUD_PostRunCmd( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed )
{
	int Id;

if (runfuncs)
	{
		me.spread.random_seed = random_seed;
		me.spread.gtime = time;
		me.spread.prcflags = to->client.flags;
		Id = to->client.m_iId;
		if (Id >= 0 && Id < MAX_WEAPONS)
			me.spread.WeaponState = to->weapondata[Id].m_iWeaponState;
		if (!(usercmd->buttons & (IN_ATTACK | IN_ATTACK2)))
		{
			if (me.spread.firing)
			{
				me.spread.firing = false;

				if (me.spread.recoil > 15)
					me.spread.recoil = 15;

				me.spread.recoiltime = time + 0.4f;
			}

			if (currentWeaponID >= WEAPON_USP && currentWeaponID <= WEAPON_FIVE7)
			{
				me.spread.recoil = 0;
			}
			else if (me.spread.recoil > 0)
			{
				if (me.spread.recoiltime <= time)
				{
					me.spread.recoiltime = me.spread.recoiltime + 0.0225f;
					me.spread.recoil--;
				}
			}
		}
		if (Id >= 0 && Id < MAX_WEAPONS && to->weapondata[Id].m_fInReload)
		{
			me.spread.recoil = 0;
			me.spread.prevtime = 0;
			DefaultSpreadVar(Id);
			me.spread.recoiltime = time;
			me.spread.firing = false;
		}
		if (lastWeaponID != currentWeaponID)
		{
			lastWeaponID = currentWeaponID;

			me.spread.recoil = 0;
			me.spread.prevtime = 0;
			DefaultSpreadVar(Id);
			me.spread.recoiltime = time;
			me.spread.firing = true;

						if (cvar.vswitch && me.alive)
			{
				if( strstr("usp",gGetWeaponName(me.ent->curstate.weaponmodel)) ||
					strstr("glock18",gGetWeaponName(me.ent->curstate.weaponmodel)) ||
					strstr("deagle",gGetWeaponName(me.ent->curstate.weaponmodel)) ||
					strstr("p228",gGetWeaponName(me.ent->curstate.weaponmodel)) ||
					strstr("elite",gGetWeaponName(me.ent->curstate.weaponmodel)) ||
					strstr("fiveseven",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("pistolvec");
				}
				else if ( strstr("ump45",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("umpvec");
				}
				else if( strstr("ak47",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("akvec");
				}
				else if( strstr("sg552",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("sigvec");
				}
				else if( strstr("aug",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("augvec");
				}
				else if( strstr("m4a1",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("coltvec");
				}
				else if( strstr("scout",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("scoutvec");
				}
				else if( strstr("knife",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("knifevec");
				}
				else if( strstr("m249",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("paravec");
				}
				else if( strstr("g3sg1",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("g3sg1vec");
				}
				else if( strstr("sg550",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("sg550vec");
				}
				else if( strstr("mp5",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("mp5vec");
				}
				else if( strstr("xm1014",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("xm1014vec");
				}
				else if( strstr("m3",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("m3vec");
				}
				else if( strstr("awp",gGetWeaponName(me.ent->curstate.weaponmodel)) )
				{
					cmd.exec("awpvec");
				}
			}
		}
	}
}
//===================================================================================
void OGC_ConsoleScrollUp(const char* message)
{
	gConsole.echo(message);
}
void OGCmessage(const char *fmt, ... )
{
	const char PROC_NAME[50] = "OGCmessage\0";

	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	// echo + scroll up:
	OGC_ConsoleScrollUp(buf);
}
DWORD retaddress;
void OGCmessageD(const char *fmt, ... )
{
	const char PROC_NAME[50] = "OGCmessage\0";

	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	// echo + scroll up:
}

void HUD_VoiceStatus(int entindex, qboolean bTalking)
{
	OGCmessageD("%d %d", entindex, bTalking);
}
int HUD_Init (void)
{
	return 1;
}

__declspec( naked )void Gateway2_HUD_VoiceStatus( void )
{
	__asm
	{
		call HUD_VoiceStatus;
		jmp retaddress;
	}
}

DWORD VoiceStatus = (DWORD)&Gateway2_HUD_VoiceStatus;
__declspec( naked )void Gateway1_HUD_VoiceStatus( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		mov retaddress,esi;
		push VoiceStatus;
		pop esi;
		mov dword ptr ss:[esp+0x10],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_V_CalcRefdef( void )
{
	__asm
	{
		call PostV_CalcRefdef;
		jmp retaddress;
	}
}

DWORD CalcRef = (DWORD)&Gateway2_V_CalcRefdef;
__declspec( naked )void Gateway1_V_CalcRefdef( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		push esi;
		call PreV_CalcRefdef;
		add esp,4;
		mov esi,dword ptr ss:[esp+0x0c];
		mov retaddress,esi;
		push CalcRef;
		pop esi;
		mov dword ptr ss:[esp+0x0c],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_CL_CreateMove( void )
{
	__asm
	{
		call CL_CreateMove;
		jmp retaddress;
	}
}

DWORD CreateMove = (DWORD)&Gateway2_CL_CreateMove;
__declspec( naked )void Gateway1_CL_CreateMove( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x28];
		mov retaddress,esi;
		push CreateMove;
		pop esi;
		mov dword ptr ss:[esp+0x28],esi;
		pop esi;
		ret;
	}
}
__declspec( naked )void Gateway2_HUD_TempEntUpdate( void )
{
	__asm
	{
		call HUD_TempEntUpdate;
		jmp retaddress;
	}
}

DWORD Temp= (DWORD)&Gateway2_HUD_TempEntUpdate;
__declspec( naked )void Gateway1_HUD_TempEntUpdate( void )
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x35];
		mov retaddress,esi;
		push Temp
		pop esi;
		mov dword ptr ss:[esp+0x35],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_HUD_Redraw( void )
{
	__asm
	{
		call HUD_Redraw;
		jmp retaddress;
	}
}

DWORD Redraw= (DWORD)&Gateway2_HUD_Redraw;
__declspec( naked )void Gateway1_HUD_Redraw( void )
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		mov retaddress,esi;
		push Redraw
		pop esi;
		mov dword ptr ss:[esp+0x10],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_HUD_PostRunCmd( void )
{
	__asm
	{
		call HUD_PostRunCmd;
		jmp retaddress;
	}
}

DWORD PostRunCmd= (DWORD)&Gateway2_HUD_PostRunCmd;
__declspec( naked )void Gateway1_HUD_PostRunCmd( void )
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x38];
		mov retaddress,esi;
		push PostRunCmd
		pop esi;
		mov dword ptr ss:[esp+0x38],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_HUD_PlayerMove( void )
{
	__asm
	{
		call HUD_PlayerMove;
		jmp retaddress;
	}
}

DWORD PlayerMove = (DWORD)&Gateway2_HUD_PlayerMove;
__declspec( naked )void Gateway1_HUD_PlayerMove( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		mov retaddress,esi;
		push PlayerMove
		pop esi;
		mov dword ptr ss:[esp+0x10],esi;
		pop esi;
		ret;
	}
}

__declspec( naked )void Gateway2_HUD_Init( void )
{
	__asm
	{
		call HUD_Init;
		jmp retaddress;
	}
}

DWORD Init = (DWORD)&Gateway2_HUD_Init;
__declspec( naked )void Gateway1_HUD_Init( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x08];
		mov retaddress,esi;
		push Init
		pop esi;
		mov dword ptr ss:[esp+0x08],esi;
		pop esi;
		ret;
	}
}
__declspec( naked )void Gateway2_HUD_AddEntity( void )
{
	__asm
	{
		mov AddEntResult, eax;
		call HUD_AddEntity;
		mov eax, AddEntResult;
		jmp retaddress;
	}
}

DWORD AddEnt = (DWORD)&Gateway2_HUD_AddEntity;
__declspec( naked )void Gateway1_HUD_AddEntity( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x14];
		mov retaddress,esi;
		push AddEnt
		pop esi;
		mov dword ptr ss:[esp+0x14],esi;
		pop esi;
		ret;
	}
}
 
__declspec( naked )void Gateway2_HUD_Key_Event( void )
{
	__asm
	{
		mov KeyEventResult, eax;
		call HUD_Key_Event;
		mov eax, KeyEventResult;
		jmp retaddress;
	}
}

DWORD KeyEvent = (DWORD)&Gateway2_HUD_Key_Event;
__declspec( naked )void Gateway1_HUD_Key_Event( void )
{
	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x14];
		mov retaddress,esi;
		push KeyEvent
		pop esi;
		mov dword ptr ss:[esp+0x14],esi;
		pop esi;
		ret;
	}
}

__declspec(naked) void nullStub()
{
	_asm ret;
}


#pragma warning (disable:4800)
DWORD* slots;
static bool bFirstTime = true;
static bool bStudioFirst = true;
static bool bTableFirst = true;
static bool loaded = false;
static bool done = false;

int	DrawConsoleString( int x, int y, char *string )
{
	if (!strstr(string, ".wav"))
	{
		return gEngfuncs.pfnDrawConsoleString( x, y, string);
	}
	
	return 0;
}
static bool hackActive = false;
//===================================================================================
DWORD FindPattern(char *pattern, int len, DWORD dwStart, DWORD dwLen)
{
    char *m = (char *)dwStart;
    for(; (DWORD)m < (dwStart + dwLen); m++)
    if(!memcmp(m, pattern, len))
    return (DWORD)m;
    return NULL;
}
void initialize(LARGE_INTEGER *lp, bool force = true)
{
  char engine_sig[] = "\x89\x74\x24\x5C\x89\x74\x24\x60\x89\x74\x24\x64\x89\x74\x24\x68\x89\x74\x24\x6C\x89\x74\x24\x70\x89\x74\x24\x74\x89\x74\x24\x78\x89\x74\x24\x7C\0";
    DWORD dwEngine = FindPattern(engine_sig, strlen(engine_sig), 0x01D60000, 0x01000000);
    DWORD Slots_sig = (*(DWORD*)(dwEngine - 0x74));
    DWORD* slots = (DWORD*)Slots_sig;
  
    static bool NotPatched = true;

    if (NotPatched)
    {
        int a;
        for(a=0x01D00000;a<0x01D0FFFF;a++)
        {
            if(*(PBYTE)(a) == 0x68 && *(PBYTE)(a + 0x05) == 0xE8 &&
            *(PBYTE)(a + 0x0A) == 0x6A && *(PBYTE)(a + 0x0B) == 0x07 &&
            *(PBYTE)(a + 0x0C) == 0x68 && *(PBYTE)(a + 0x11) == 0xFF &&
            *(PBYTE)(a + 0x12) == 0x15 && *(PBYTE)(a + 0x17) == 0x68)
            {
                pEngfuncs = (cl_enginefunc_t*)*(DWORD*)(a+0x0D);
                break;
            }
        }
        int b;
        for(b=0x01D00000;b<0x01D0FFFF;b++)
        {
            if(*(PBYTE)(b) == 0xE8 && *(PBYTE)(b + 0x05) == 0x6A &&
            *(PBYTE)(b + 0x06) == 0x07 && *(PBYTE)(b + 0x07) == 0x68 &&
            *(PBYTE)(b + 0x0C) == 0xFF && *(PBYTE)(b + 0x0D) == 0x15 &&
            *(PBYTE)(b + 0x12) == 0x68 && *(PBYTE)(b + 0x1C) == 0x81)
            {
                ppmove = (playermove_s*)*(DWORD*)(b+0x13);
                break;
            }
        }
        int c;
        for(c=0x01D00000;c<0x01D0FFFF;c++)
        {
            if(*(PBYTE)(c) == 0x68 && *(PBYTE)(c + 0x05) == 0x68 &&
            *(PBYTE)(c + 0x0A) == 0x6A && *(PBYTE)(c + 0x0B) == 0x01 &&
            *(PBYTE)(c + 0x0C) == 0xFF && *(PBYTE)(c + 0x0D) == 0xD0 &&
            *(PBYTE)(c + 0x0E) == 0x83 && *(PBYTE)(c + 0x27) == 0xC3)
            {
                pstudio = (engine_studio_api_s*)*(DWORD*)(c+0x01);
                break;
            }
        }
        NotPatched = false;
    }


	if( pEngfuncs->pfnHookUserMsg && pEngfuncs->pfnHookEvent && bFirstTime )
	{
		memcpy( &gEngfuncs, pEngfuncs, sizeof( cl_enginefunc_t ) );

		HookUserMsgOrg = (DWORD)*pEngfuncs->pfnHookUserMsg;

		// Start hooking game functions
		pEngfuncs->pfnSPR_Set = &SPR_Set;
		pEngfuncs->pfnSPR_DrawHoles = &SPR_DrawHoles;
		pEngfuncs->pfnSPR_DrawAdditive = &SPR_DrawAdditive;
		pEngfuncs->pfnSPR_Draw = &SPR_Draw;
		pEngfuncs->pfnSPR_Load = &SPR_Load;
		pEngfuncs->pfnHookUserMsg = &HookUserMsg;
		pEngfuncs->pfnDrawCharacter = &DrawCharacter;
		pEngfuncs->pfnFillRGBA = &FillRGBA;
		pEngfuncs->pfnHookEvent = &HookEvent;
		pEngfuncs->pfnDrawConsoleString = &DrawConsoleString;


		keyBindManager.init();
		keyBindManager.CallBack_Execute = CB_KeyManager_Exec;
		keyBindManager.CallBack_ConType = CB_KeyManager_ConType;
		keyBindManager.con_visible = gEngfuncs.Con_IsVisible;

    	gConsole.setcolortag( 'b', 115, 175,80);
		gConsole.setcolortag( 'r',160, 155,230);
		gConsole.setcolortag( 'g', 80, 155,155);
		gConsole.setcolortag( 'w',240,255,255);
		gConsole.setcolortag( 'y', 140, 155,200);
		
		gConsole.echo("&w*** 1Vs16 HooK  Version 1.0  ***");
                gConsole.echo("%w联系QQ�:29871555");
                gConsole.echo("&rEmail:CsCheats@163.Com"); 
		gConsole.echo("&w下载地址:HTTP://OGC.COM.CN");
                
                gConsole.echo("");

		ogcMenu.init(getOgcDirFile("menu.txt").c_str());
		Init_Command_Interpreter();
		ogc_exec("main");
		cvar.alive_method = 1;
		detour_S_DynamicSound();
		soundPatch.apply();
		cvar.spec_fix = false;

		me.spread.prcflags = 0;
		me.spread.random_seed = 0;
		me.spread.recoil = 0;
		me.spread.gtime = 0;
		me.spread.prevtime = 0;
		me.spread.brokentime = 0;
		me.spread.spreadvar = 0;
		me.spread.recoiltime = 0;
		me.spread.firing = false;

		bool AllowedToHookGL();
		if (!AllowedToHookGL())
			OGCmessage("&wNOTICE: &r OpenGL Is NOT Hooked");

		void initWeaponID();
		initWeaponID();

		bFirstTime = false;
	}
    if( pstudio->GetModelByIndex && bStudioFirst )
    {
        memcpy( &IEngineStudio, pstudio, sizeof( engine_studio_api_t ) );
        bStudioFirst = false;
    }
	
	static bool patched = false;

if( (me.alive && !done) || force || GetAsyncKeyState(VK_F12)&0x8000) 
	{
		unsigned char* c;
		if(!patched)
		{
			patched=true;
			for(int s=0;s<40;s++)
			{
				c = (unsigned char*)slots[s];
				if( c[0] != 0xc3 )
				{
					patched=false;
					MessageBox(NULL, "Things look wrong... very wrong", NULL, MB_OK);
				}
			}
		}
		bool detour_S_DynamicSound();
		detour_S_DynamicSound();
		c = (unsigned char*)slots[3];
		if( c[0] == 0xc3 ){
			slots[3]  = (DWORD)&Gateway1_HUD_Redraw;   //HUD_Redraw
		}
		c = (unsigned char*)slots[6];
		if( c[0] == 0xc3 ){
			slots[6]  = (DWORD)&Gateway1_HUD_PlayerMove;} //HUD_PlayerMove
		c = (unsigned char*)slots[14];
		if( c[0] == 0xc3 ){
			slots[14] = (DWORD)&Gateway1_CL_CreateMove;}//CL_CreateMove
		c = (unsigned char*)slots[19];
		if( c[0] == 0xc3 ){
			slots[19] = (DWORD)&Gateway1_V_CalcRefdef; }//V_CalcRefdef
		c = (unsigned char*)slots[20];
		if( c[0] == 0xc3 ){
			slots[20] = (DWORD)&Gateway1_HUD_AddEntity;}//HUD_AddEntity
		c = (unsigned char*)slots[25];
		if (c[0] == 0xc3){
			slots[25] = (DWORD)&Gateway1_HUD_PostRunCmd;      }        //HUD_PostRunCmd
		c = (unsigned char*)slots[34];
		if( c[0] == 0xc3 ){
			slots[34] = (DWORD)&Gateway1_HUD_Key_Event;  }//HUD_Key_Event
		c = (unsigned char*)slots[35];
		if (c[0] == 0xc3){
			slots[55] = (DWORD)&Gateway1_HUD_TempEntUpdate;}    //HUD_TempEntUpdate
		c = (unsigned char*)slots[37];
		if (c[0] == 0xc3){
			slots[37] = (DWORD)&Gateway1_HUD_VoiceStatus;
		}
		hackActive = true;
		AtRoundStart();
		done = true;
	}	
	if (force)
		return;
}
//===================================================================================
int HUD_DirectorEvent( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags )
{
	int retval = NULL;
	retval = pHUD_DirectorEvent( command, firstObject, secondObject, flags );
	return( retval );
}

//===================================================================================
void HUD_Frame( double time )
{
	pHUD_Frame( time );
}
//===================================================================================
void HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
	pHUD_PlayerMoveInit( ppmove );
}

//===================================================================================
char HUD_PlayerMoveTexture( char *name )
{
	char retval = NULL;
	retval = pHUD_PlayerMoveTexture( name );
	return( retval );
}
void HUD_Shutdown( void )
{
	if (gAimbot.AimVecsS.size() > 0 && gAimbot.AimVecsD.size() > 0)
		cmd.exec("saveaimvecs autovecs testvec");

	pHUD_Shutdown( );
}	   
//===================================================================================
void IN_MouseEvent( int mstate )
{
	pIN_MouseEvent( mstate );
}	 

//===================================================================================
struct kbutton_s *KB_Find( const char *name )
{
	struct kbutton_s *retval = NULL;
	retval = pKB_Find( name );
	return( retval );
}
	 
//===================================================================================
void HUD_DirectorMessage(unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags)
{
	pHUD_DirectorEvent(command, firstObject, secondObject, flags);
}
	  
//===================================================================================
r_studio_interface_t *pOrigEngineStudioInterface;
int GetPlayerByIndex(int index)
{
	for (unsigned int ax=0;ax<vPlayers.size();ax++)
		if (vPlayers[ax].index == index) return ax;
		return -1;
}

int R_StudioDrawPlayer(int flags, entity_state_t *pplayer)
{
	int retval = 0;
	const char PROC_NAME[50] = "R_StudioDrawPlayer\0";
	retval = (*pOrigEngineStudioInterface->StudioDrawPlayer)(flags, pplayer);

	if (!hackActive) return retval;

		cl_entity_t *curEntity;

		curEntity = IEngineStudio.GetCurrentEntity();

		int ax;

		ax = GetPlayerByIndex(curEntity->index);

		studiohdr_t *a;
		model_t *b;

		b = IEngineStudio.SetupPlayerModel(pplayer->number-1);
		a = (studiohdr_t *)IEngineStudio.Mod_Extradata(b);

		model_t *mdl;
		mdl = IEngineStudio.GetModelByIndex(pplayer->modelindex);

		mstudiobone_t *testbone;
		testbone = (mstudiobone_t *)((byte *)a + a->boneindex);

		int bonenumber = -1;
		static bool doneThisOnce = false;

		doneThisOnce = true;

	return retval;
}
int R_StudioDrawModel(int flags)
{
	return (*pOrigEngineStudioInterface->StudioDrawModel)(flags);
}

r_studio_interface_t HookGameStudioInterface =
{
	STUDIO_INTERFACE_VERSION,
	R_StudioDrawModel,
	R_StudioDrawPlayer,
};
int HUD_GetStudioModelInterface_Hook (int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio)
{
	const char PROC_NAME[50] = "HUD_GetStudioModelInterface_Hook\0";
	int retval;
	retval = pHUD_GetStudioModelInterface (version,&pOrigEngineStudioInterface,pstudio);
	*ppinterface = &HookGameStudioInterface;
	return retval;
}

void PatchClientFunc( DWORD *ClientFunc_ADDR )
{
	*( ( DWORD * )&pHUD_GetStudioModelInterface ) = *( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x9C ) );
	*( ( DWORD * )( *( ( DWORD * )&ClientFunc_ADDR ) + 0x9C ) ) = ( DWORD )&HUD_GetStudioModelInterface_Hook;		
}
