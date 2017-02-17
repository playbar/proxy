#pragma once
#include <windows.h>
#include "../D3D/Skill.h"

#define MAX_SKILL 2



struct SKILL_INFO 
{
	TCHAR     szName[50];
	TCHAR     szlevel[50];
	TCHAR     szInfo[200];
	DWORD     dwPicID;
	DWORD     dwID;
};

SKILL_INFO * GetSkillInfo(DWORD dwID);
extern SKILL_INFO g_SkillInfo[MAX_SKILL];

