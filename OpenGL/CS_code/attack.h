
#if !defined(AFX_ATTACK_H__41C3EB49_F58F_4166_9868_379D8C2A0421__INCLUDED_)
#define AFX_ATTACK_H__41C3EB49_F58F_4166_9868_379D8C2A0421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

enum 
{
	 WEAPON_NONE = 0
	,WEAPON_USP,WEAPON_GLOCK,WEAPON_DEAGLE,WEAPON_P228,WEAPON_ELITE,WEAPON_FIVE7
	,WEAPON_XM,WEAPON_M3
	,WEAPON_MP5,WEAPON_TMP,WEAPON_P90,WEAPON_MAC,WEAPON_UMP
	,WEAPON_AK,WEAPON_SIG,WEAPON_COLT,WEAPON_AUG
	,WEAPON_SCOUT,WEAPON_AWP,WEAPON_G3SG1,WEAPON_SG550
	,WEAPON_PARA,WEAPON_KNIFE
	,WEAPON_FLASH,WEAPON_HE,WEAPON_SMOKE,WEAPON_C4
};
class BurstInfo
{
public:
	BurstInfo(){ reset(); }
	struct RangeInfo
	{
		int   range;
		float ftime;
		float dtime;
		float sdelay;
		int   flags; 
	};
	enum { BURSTFLAG_DUCK  = 1, 
		   BURSTFLAG_NOAIM = 2, 
		   BURSTFLAG_STOP  = 4, 
	       BURSTFLAG_ZOOM  = 8, 
		   BURSTFLAG_GROUND=16, 
		   BURSTFLAG_USE   =32};
public:
	RangeInfo* get(int weaponID, int idx);
	void       set(int weaponID, BurstInfo::RangeInfo* newEntry, int idx);
	char*      description(int weaponID, int idx);
	static char* description(const BurstInfo::RangeInfo*);
	static int getWeaponID(const char* weapon);
	bool copy(const char* weaponfrom, const char* weaponto);
	bool savecfg (const char* filename, const char* prefix);
	void reset();
	const RangeInfo* getTimeInfo(int weapon,int distance);
protected:
	enum { NUM_USABLE_WEAPONS = 30 };
	RangeInfo range[NUM_USABLE_WEAPONS][5];
};

#endif
