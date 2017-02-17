#include "SkillInfoTable.h"
#include "../GameDefine.h"





SKILL_INFO * GetSkillInfo(DWORD dwID)
{
	for (int i = 0; i < MAX_SKILL; i ++)
	{
		if (g_SkillInfo[i].dwID == dwID)
		{
			return &g_SkillInfo[i];
		}
	}
	return NULL;
}

SKILL_INFO g_SkillInfo[MAX_SKILL] = 
{
	{L"治愈之光",L"（等级1）",L"雷电术\n等级1\n法力值 40\n伤害 70 - 85\n",UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_1,1},
	{L"炎墙",L"（等级1）",L"提高炎抗性",UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_2,2},
};
