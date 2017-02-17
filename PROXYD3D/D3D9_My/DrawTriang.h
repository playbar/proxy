
#pragma  once 

//绘制矩形
struct SVertex
{
	float x,y,z,rhw;
	DWORD color;
	float u,v;
	const static DWORD _fvf = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1;
};

struct CUSTOMVERTEX
{
	FLOAT x, y, z;    //顶点位置  
	FLOAT u,v ;		  //顶点纹理坐标
};
#define D3DFVF_CUSTOMVERTEX   (D3DFVF_XYZ|D3DFVF_TEX1)


void DrawRectMy( LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DTEXTURE9 pTex );
