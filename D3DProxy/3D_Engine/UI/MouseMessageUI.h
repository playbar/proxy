#pragma  once
#include "UIDialog.h"
#include "ItemUI.h"

enum
{
	UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_BACK,
	UI_GAME_MOUSEMESSAGE_CONTROL_STATIC_MESSAGE,
};

class CMouseMessageUI : public CUIDialog
{
public:
	CMouseMessageUI()
	{

	}
	~CMouseMessageUI()
	{

	}

	void SetMessage(ITEM_INFO * pItemInfo,float x,float y);

	void Init( DWORD dwId, float x, float y, float width, float height, bool bActivate, CBaseUI * pBaseUI );
	void Release();
	void Render();
	bool Process(float x,float y);
	bool WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	bool Event(ST_UI_MSG * pMsg);
protected:
private:
};