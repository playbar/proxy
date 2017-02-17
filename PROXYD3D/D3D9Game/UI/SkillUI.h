#pragma once

#include "UIDialog.h"
#include "../D3D/Skill.h"
#include "MonsterUI.h"

struct SKILL 
{
	DWORD dwId;
};

enum 
{
	UI_GAME_SKILL_CONTROL_STATIC_BACK,
	UI_GAME_SKILL_CONTROL_BUTTON_ITEMBUTTON,
	UI_GAME_SKILL_CONTROL_STATIC_SKILL,
};


class CSkillUI : public CUIDialog
{
public:
	CSkillUI()
	{
		for (int i = 0; i < 12; i ++)
		{
			m_Skill[i].dwId = NULL;
		}
	}
	~CSkillUI();

	SKILL     m_Skill[12];

	void SetSkill(int iNum);

	CUIButton * m_pUIItemButton;
	CUIButton * m_pUIFixButton;
	CUIButton * m_pUISkillBookButton;

	void Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
protected:
private:
};

enum
{
UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BACK,
UI_GAME_SKILLBOOKUI_CONTROL_STATIC_BUTTON,
UI_GAME_SKILLBOOKUI_CONTROL_STATIC_SKILL,
};
class CSkillBookUI : public CUIDialog
{
public:
	CSkillBookUI()
	{
		for (int i = 0; i < 12; i ++)
		{
			m_Skill[i].dwId = NULL;
		}
		m_bSetPos = false;
	}
	~CSkillBookUI(){}

	SKILL m_Skill[12];

	float     m_fXTemp;
	float     m_fYTemp;
	bool      m_bSetPos;
	void SetPos(float x,float y);

	void Init(DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);

	SKILL m_MouseSkill;
protected:
private:
};