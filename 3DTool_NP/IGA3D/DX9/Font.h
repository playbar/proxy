#include "d3d9.h"
#include "d3dx9.h"

class IGA3DFont
{
public:
	IGA3DFont();
	~IGA3DFont();

public:
	HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );

	void	ShowText( WCHAR* text, int x, int y );

	void	Destory();	

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	LPD3DXFONT				m_pFont;

};

IGA3DFont* GetIGA3DFont();