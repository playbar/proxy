
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "stdio.h"
#include "attack.h"
#include "stringfinder.h"
#include <fstream>
#pragma optimize("gt",on)
static StringFinder findWeaponID;

using namespace std;

//===================================================================================
void initWeaponID()
{
	findWeaponID.clear();
    findWeaponID.add("usp"         , WEAPON_USP     );
    findWeaponID.add("glock18"     , WEAPON_GLOCK   );
    findWeaponID.add("deagle"      , WEAPON_DEAGLE  );
    findWeaponID.add("p228"        , WEAPON_P228    );
    findWeaponID.add("elite"       , WEAPON_ELITE   );
    findWeaponID.add("fiveseven"   , WEAPON_FIVE7   );
	findWeaponID.add("xm1014"      , WEAPON_XM      );
    findWeaponID.add("m3"          , WEAPON_M3      );
    findWeaponID.add("mp5navy"     , WEAPON_MP5     );
    findWeaponID.add("tmp"         , WEAPON_TMP     );
    findWeaponID.add("p90"         , WEAPON_P90     );
    findWeaponID.add("mac10"       , WEAPON_MAC     );
    findWeaponID.add("ump45"       , WEAPON_UMP     );
    findWeaponID.add("ak47"        , WEAPON_AK      );
    findWeaponID.add("sg552"       , WEAPON_SIG     );
    findWeaponID.add("m4a1"        , WEAPON_COLT    );
    findWeaponID.add("aug"         , WEAPON_AUG     );
    findWeaponID.add("scout"       , WEAPON_SCOUT   );
    findWeaponID.add("awp"         , WEAPON_AWP     );
    findWeaponID.add("g3sg1"       , WEAPON_G3SG1   );
    findWeaponID.add("sg550"       , WEAPON_SG550   );
    findWeaponID.add("m249"        , WEAPON_PARA    ); 
    findWeaponID.add("knife"       , WEAPON_KNIFE   ); 
	findWeaponID.add("flashbang"   , WEAPON_FLASH   ); 
    findWeaponID.add("hegrenade"   , WEAPON_HE      ); 
    findWeaponID.add("smokegrenade", WEAPON_SMOKE   ); 
    findWeaponID.add("c4"          , WEAPON_C4      ); 
}
//===================================================================================
BurstInfo::RangeInfo* BurstInfo::get(int weaponID, int dist)
{	
	static RangeInfo standard = { 100, 0.2f, 0.4f, 0 };
    if(weaponID<0 || weaponID>27){ return &standard; }
	RangeInfo* p = range[weaponID];
	RangeInfo* last = p+4;
	if( !p->range ){ return &standard; }
	for(;;)
	{
		if( p->range >= dist ){ return p; }
		if( p == last        ){ return p; }
		if( !p[1].range      ){ return p; }
	    ++p;
	}
}

//===================================================================================
void BurstInfo::set(int weaponID, BurstInfo::RangeInfo* newEntry, int idx)
{
	if( weaponID<=0||weaponID>27) { return; }
	range[weaponID][idx] = *newEntry;
}

//===================================================================================
char* BurstInfo::description(const BurstInfo::RangeInfo* r)
{
	static char buf[256];
	char flags[40]="|";
	if(r->flags&BURSTFLAG_DUCK  )strcat(flags,"DUCK|");
	if(r->flags&BURSTFLAG_NOAIM )strcat(flags,"NOAIM|");
	if(r->flags&BURSTFLAG_STOP  )strcat(flags,"STOP|");
	if(r->flags&BURSTFLAG_ZOOM  )strcat(flags,"ZOOM|");
	if(r->flags&BURSTFLAG_GROUND)strcat(flags,"GROUND|");
	if(r->flags&BURSTFLAG_USE   )strcat(flags,"USE|");
	enum { BURSTFLAG_DUCK  = 1,
		   BURSTFLAG_NOAIM = 2, 
		   BURSTFLAG_STOP  = 4, 
	       BURSTFLAG_ZOOM  = 8, 
		   BURSTFLAG_GROUND=16, 
		   BURSTFLAG_USE   =32};
	sprintf(buf,"Range<=%d Rate: %f:%f Delay:%f Flags:%s=%d"
		         ,r->range,r->ftime,r->dtime,r->sdelay,flags,r->flags);
	return buf;
}

//===================================================================================
char* BurstInfo::description(int weaponID, int idx)
{
	char* invalidWeapon = "Invalid Weapon";
	if( weaponID<0||weaponID>27 ) { return invalidWeapon;  }
	else { 	return description(&range[weaponID][idx]); }
}

//===================================================================================
int BurstInfo::getWeaponID(const char* weapon)
{
	if( findWeaponID.find(weapon) ){ return findWeaponID.num; }
	else                           { return 0; }
}

//===================================================================================
bool BurstInfo::copy(const char* weaponfrom, const char* weaponto)
{
	int from = getWeaponID(weaponfrom); if(!from) { return false; }
	int to   = getWeaponID(weaponto);   if(!to)   { return false; }
	for(int i=0;i<5;i++)
	{
		range[to][i] = range[from][i];
	}
	return true;
}

//===================================================================================
bool BurstInfo::savecfg(const char* filename, const char* prefix)
{
    ofstream ofs(filename, ios::binary);
	if(!ofs){ return false; }
	for( findWeaponID.it_start(); findWeaponID.it_running(); findWeaponID.it_next() )
	{
		const char* weaponname = findWeaponID.str;
		int id = findWeaponID.num;
		for(int i=0;i<5;i++)
		{
			RangeInfo& r = range[id][i]; 
			if(r.range)
			{
				ofs <<prefix<<"setburst "<<weaponname<<' '<<i+1<<' '<<r.range<<' '
					<<r.ftime<<' '<<r.dtime<<' '<<r.sdelay<<' '<<r.flags
					<<(char)0x0D<<(char)0x0A;
			} else{
				break;
			}
		}
	}
	ofs.close();
	return true;
}

//===================================================================================
void BurstInfo::reset()
{
	for(int w=0;w<BurstInfo::NUM_USABLE_WEAPONS;++w)
	{
		for(int i=0;i<5;i++)
		{
			range[w][i].range = 0;
			range[w][i].ftime = 0;
			range[w][i].dtime = 0;
			range[w][i].flags = 0;
		}	
	}
	initWeaponID();
}


