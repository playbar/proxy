#pragma  once 

//extern LPDIRECT3DDEVICE8	g_pd3dDevice;

void	IGATextureDestory();
HRESULT IGATextureInit( LPDIRECT3DDEVICE8 pd3dDevice );
LPDIRECT3DTEXTURE8 IGAGetTexture( int id, bool* bIsPlayEnd, float* w = NULL, float* h = NULL );

int     DrawPlayer();
void	PlayAVI( LPDIRECT3DDEVICE8 pd3dDevice, char* name);
void	PlayFLASH(char* name);
void	PausePlayer();
void	StopPlayer( DWORD dwID);