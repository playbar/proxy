
#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>  // directX includes


#include "t3dlib1.h"
#include "t3dlib4.h"
#include "t3dlib5.h"
#include "t3dlib6.h"
#include "t3dlib7.h"
#include "t3dlib8.h"

#include <d3dx9.h>

#include "Surface.h"
#include "DirectXVideoTexture.h"
#include "FlashSprite.h"
#include "TestPopup.h"

#include "drawing3D.h"

extern DDSURFACEDESC	     g_ddsd;
extern DDSURFACEDESC2        g_ddsd2; 
extern LPDIRECTDRAWSURFACE4 g_pddbacksurface;

//记录鼠标当前位置
#define VK_S	0x53
#define VK_W	0x57
#define VK_B	0x42
#define VK_L	0x4C
#define VK_A	0x41
#define VK_I	0x49
#define	VK_O	0x4F
#define VK_P	0x50
#define VK_H	0x48
#define VK_X	0x58
#define	VK_Y	0x59
#define VK_Z	0x5A


//LPDIRECT3D9			g_pD3D       = NULL;
//LPDIRECT3DDEVICE9	g_pd3dDevice = NULL; 
//LPDIRECT3DVERTEXBUFFER9 g_pVB = 0;

IDirect3DSurface9* g_pSurface;

const int Width  = 640;
const int Height = 480;
IDirect3DIndexBuffer9*  g_pIB = 0;
LPDIRECT3DTEXTURE9      g_pTex		 = NULL;
D3DXMATRIX WorldMatrix;

const DWORD Vertex::FVF = D3DFVF_XYZ;

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)  

extern int g_nWidthPos;
extern int g_nHeightPos;

extern BITMAP_FILE      bitmap;  
extern BITMAP_FILE      bitmap_close;
extern BITMAP_FILE		bitmap_close_over;
extern BITMAP_FILE		bitmap_playwindow;
extern BITMAP_FILE		bitmap_playwindow_over;

extern FlashSprite				* g_pFlashSprite;
extern UCHAR					* g_pFlashData;

extern CDirectXVideoTexture		*g_pIDirectVideo; 
extern UCHAR					*g_pVideoData ;

//////////////////////////////////////////////////////////////////////////

CAM4DV1    cam;     // the single camera
OBJECT4DV1 obj;     // used to hold our cube mesh  

OBJECT4DV2     obj_flat_cube;

RENDERLIST4DV2 rend_list; // the render list

RGBAV1 white, gray, black, red, green, blue;


//#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

POINT4D  cam_pos = {0,0,0,1};
VECTOR4D cam_dir = {0,0,0,1};
POINT4D  cam_target = {0,0,0,1};

// all your initialization code goes here...
VECTOR4D vscale={1.0, 1.0, 1.0, 1}, 
		 vpos = {0,   0,   150, 1}, 
		 vrot = {0,   0,   0,   1};

UCHAR * g_zoomBmpbuffer;

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define SPOT_LIGHT1_INDEX     4 // point light index
#define SPOT_LIGHT2_INDEX     3 // spot light index
#define NUM_OBJECTS           6 // number of objects system loads


OBJECT4DV2_PTR  obj_work;               // pointer to active working object
OBJECT4DV2      obj_array[NUM_OBJECTS]; // array of objects 


// filenames of objects to load
char *object_filenames[NUM_OBJECTS] = { "cube_flat_01.cob",
										"cube_gouraud_01.cob",
										"cube_flat_textured_01.cob",
										"sphere02.cob",
										"sphere03.cob",
										"hammer03.cob",
										};

int curr_object = 0;                  // currently active object index


#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) // for padding
#define HDIB				HANDLE
///////////////////////////////////////////////////////////////////
// Function name    : ZoomOutBmp
// Description      : creates a new bitmap which is a grayscale
//                    zoomed out version of the original
// Return type      : HDIB - handle to a new bitmap
// Argument         : double zoom - number of times to zoom out
// Argument         : HDIB hSrcDIB - handle to a source bitmap
///////////////////////////////////////////////////////////////////
HDIB ZoomOutBmp(double zoom, HDIB hSrcDIB)
{
	if (hSrcDIB == NULL) // nothing to do
		return NULL;

	if (zoom < 1) // no zoomin in this function
		return NULL;

	LPSTR pSrcDIB = (LPSTR) ::GlobalLock((HGLOBAL) hSrcDIB);

	BITMAPINFOHEADER& bmihSrc = *(BITMAPINFOHEADER*)pSrcDIB;
	ASSERT(bmihSrc.biBitCount == 1); // only monochrome bitmaps supported
	LPSTR pSrcBits = (LPSTR) (pSrcDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2);

	BITMAPINFOHEADER bmihDst = bmihSrc;
	bmihDst.biWidth = (LONG)(bmihDst.biWidth / zoom + 0.5);
	bmihDst.biHeight = (LONG)(bmihDst.biHeight / zoom + 0.5);
	bmihDst.biBitCount = 8; // grayscale in any case
	bmihDst.biClrUsed = 0;

	// prepare destination bitmap
	DWORD dwDIBSize = sizeof(bmihDst) + sizeof(RGBQUAD)*256 +
		WIDTHBYTES(bmihDst.biWidth * bmihDst.biBitCount) * bmihDst.biHeight;

	bmihDst.biSizeImage = dwDIBSize;

	// allocate space for the new bitmap
	HDIB hDstDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIBSize);

	if (hDstDIB == 0) {
		::GlobalUnlock((HGLOBAL) hSrcDIB);
		return NULL;
	}

	LPSTR pDstDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDstDIB);

	// copy header
	memcpy(pDstDIB, &bmihDst, sizeof(bmihDst));

	// prepare grayscale palette
	int i = 0;
	for ( i=0; i < (1 << bmihDst.biBitCount); i++) 
	{
		RGBQUAD& palEntry = *(RGBQUAD*)(pDstDIB + sizeof(bmihDst) + i * sizeof(RGBQUAD));
		palEntry.rgbRed = palEntry.rgbGreen = palEntry.rgbBlue = i;
	}

	LPSTR pDstBits = (LPSTR) (pDstDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);

	// now fill the bits
	LPSTR curSrcLineBits, curDstLineBits;
	int j, k;
	int scale = (int)(zoom + 0.5); // integer zoom out factor, i.e. 1:5
	int hBase, vBase;
	unsigned char value;

	// for every _scale_ lines in a source bitmap we will get one line
	// in the destination bitmap. Similarly for _scale_ columns in the
	// source we'll obtain one destination column.

	for (int strip=0; strip < bmihDst.biHeight; strip++) 
	{ // for every dst line

		curDstLineBits = pDstBits + strip * WIDTHBYTES(bmihDst.biWidth * bmihDst.biBitCount);
		vBase = int(strip * zoom + 0.5);

		for (i=0; i < scale; i++) {  // accumulate _scale_ rows

			curSrcLineBits = pSrcBits + (vBase + i) * WIDTHBYTES(bmihSrc.biWidth * bmihSrc.biBitCount);

			// prepare horizontally condensed lines for this strip
			for (j=0; j < bmihDst.biWidth; j++) { // for all bits in line

				hBase = int(j * zoom + 0.5); // mapped index on source
				for (k=0; k < scale; k++) { // accumulate _scale_ columns

					value = (curSrcLineBits[(hBase+k)/8] & (1 << (7 - (hBase+k)%8))) ? 0xff : 0;
					curDstLineBits[j] += value / scale / scale; // main accumulator
				}
			}
		}

	}

	// unlock memory
	::GlobalUnlock((HGLOBAL) hSrcDIB);
	::GlobalUnlock((HGLOBAL) hDstDIB);

	return hDstDIB;
}


bool ZoomBmpBuffer(UCHAR * pInData, int nSrcWidth, int nSrcHeigth,int bpp, UCHAR * pOutData, int nDstWidth, int nDstHeight)
{
	int hscale = (int)(nSrcHeigth / nDstHeight + 0.5); 
	int wscale = (int)(nSrcWidth / nDstWidth + 0.5);

	UCHAR * curSrcLineBits;
	UCHAR * curDstLineBits;
	int   hBase, vBase;
	unsigned char value;

	int i = 0;
	for (int strip=0; strip < nDstHeight; strip++) 
	{ // for every dst line

		curDstLineBits = pOutData + strip * WIDTHBYTES(nDstWidth * bpp);
		
		vBase = int(strip * nSrcHeigth/ nDstHeight + 0.5);

		for (i=0; i < hscale; i++) {  // accumulate _scale_ rows

			curSrcLineBits = pInData + (vBase + i) * WIDTHBYTES(nSrcWidth * bpp);

			// prepare horizontally condensed lines for this strip
			for (int j=0; j < nDstWidth; j++) { // for all bits in line

				hBase = int(j * (nSrcWidth / nDstWidth) + 0.5); // mapped index on source
				for (int k=0; k < wscale; k++) { // accumulate _scale_ columns

					value = (curSrcLineBits[(hBase+k)/8] & (1 << (7 - (hBase+k)%8))) ? 0xff : 0;
					curDstLineBits[j] += value / wscale / hscale; // main accumulator
				}
			}
		}

	}

	return true;
}

//bool Setup()
//{
//	//
//	// Create vertex and index buffers.
//	//
//
//	g_pd3dDevice->CreateVertexBuffer(
//		8 * sizeof(Vertex), 
//		D3DUSAGE_WRITEONLY,
//		Vertex::FVF,
//		D3DPOOL_MANAGED,
//		&g_pVB,
//		0);
//
//	g_pd3dDevice->CreateIndexBuffer(
//		36 * sizeof(WORD),
//		D3DUSAGE_WRITEONLY,
//		D3DFMT_INDEX16,
//		D3DPOOL_MANAGED,
//		&g_pIB,
//		0);
//
//	//
//	// Fill the buffers with the cube data.
//	//
//
//	// define unique vertices:
//	Vertex* vertices;
//	g_pVB->Lock(0, 0, (void**)&vertices, 0);
//
//	// vertices of a unit cube
//	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
//	vertices[1] = Vertex(-1.0f,  1.0f, -1.0f);
//	vertices[2] = Vertex( 1.0f,  1.0f, -1.0f);
//	vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);
//	vertices[4] = Vertex(-1.0f, -1.0f,  1.0f);
//	vertices[5] = Vertex(-1.0f,  1.0f,  1.0f);
//	vertices[6] = Vertex( 1.0f,  1.0f,  1.0f);
//	vertices[7] = Vertex( 1.0f, -1.0f,  1.0f);
//
//	g_pVB->Unlock();
//
//	// define the triangles of the cube:
//	WORD* indices = 0;
//	g_pIB->Lock(0, 0, (void**)&indices, 0);
//
//	// front side
//	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
//	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;
//
//	// back side
//	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
//	indices[9]  = 4; indices[10] = 7; indices[11] = 6;
//
//	// left side
//	indices[12] = 4; indices[13] = 5; indices[14] = 1;
//	indices[15] = 4; indices[16] = 1; indices[17] = 0;
//
//	// right side
//	indices[18] = 3; indices[19] = 2; indices[20] = 6;
//	indices[21] = 3; indices[22] = 6; indices[23] = 7;
//
//	// top
//	indices[24] = 1; indices[25] = 5; indices[26] = 6;
//	indices[27] = 1; indices[28] = 6; indices[29] = 2;
//
//	// bottom
//	indices[30] = 4; indices[31] = 0; indices[32] = 3;
//	indices[33] = 4; indices[34] = 3; indices[35] = 7;
//
//	g_pIB->Unlock();
//
//	//
//	// Position and aim the camera.
//	//
//
//	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
//	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
//	D3DXMATRIX V;
//	D3DXMatrixLookAtLH(&V, &position, &target, &up);
//
//	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
//
//	//
//	// Set the projection matrix.
//	//
//
//	D3DXMATRIX proj;
//	D3DXMatrixPerspectiveFovLH(
//		&proj,
//		D3DX_PI * 0.5f, // 90 - degree
//		(float)Width / (float)Height,
//		1.0f,
//		1000.0f);
//	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
//
//	g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
//
//	return true;
//}
//////////////////////////////////////////////////////////////////////////

//HRESULT InitD3D( HWND hWnd )
//{
//	//创建Direct3D对象, 该对象用于创建Direct3D设备对象
//	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
//		return E_FAIL;
//
//	//设置D3DPRESENT_PARAMETERS结构, 准备创建Direct3D设备对象
//	D3DPRESENT_PARAMETERS d3dpp;
//	ZeroMemory( &d3dpp, sizeof(d3dpp) );
//	d3dpp.Windowed = TRUE;
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
//
//	//创建Direct3D设备对象
//	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
//		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//		&d3dpp, &g_pd3dDevice ) ) )
//	{
//		return E_FAIL;
//	}
//
//	return S_OK;
//}
//
//HRESULT InitVB()
//{
//	//顶点数据
//	CUSTOMVERTEX vertices[] =
//	{
//		{ 100.0f, 400.0f, 0.5f, 1.0f, 0xffff0000, },
//		{ 300.0f,  50.0f, 0.5f, 1.0f, 0xff00ff00, }, 
//		{ 500.0f, 400.0f, 0.5f, 1.0f, 0xff0000ff, },
//	};
//
//	//创建顶点缓冲区
//	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX),
//		0, D3DFVF_CUSTOMVERTEX,
//		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
//	{
//		return E_FAIL;
//	}
//
//	//填充顶点缓冲区
//	VOID* pVertices;
//	if( FAILED( g_pVB->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
//		return E_FAIL;
//	memcpy( pVertices, vertices, sizeof(vertices) );
//	g_pVB->Unlock();
//
//	return S_OK;
//}
//
//VOID Cleanup()
//{
//	//释放顶点缓冲区对象
//	if( g_pVB != NULL )        
//		g_pVB->Release();
//
//	//释放Direct3D设备对象
//	if( g_pd3dDevice != NULL ) 
//		g_pd3dDevice->Release();
//
//	//释放Direct3D对象
//	if( g_pD3D != NULL )       
//		g_pD3D->Release();
//}
//
//VOID Render()
//{
//	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(45, 50, 170), 1.0f, 0 );
//	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
//	{
//		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(CUSTOMVERTEX) );
//		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
//		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
//		g_pd3dDevice->EndScene();
//	}
//
//	//将在后台缓冲区绘制的图形提交到前台缓冲区显示
//	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
//}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_PAINT:
		ValidateRect( hWnd, NULL );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

bool InitDrawing()
{
	//////////////////////////////////////////////////////////////////////////
	//HWND hwnd = GetForegroundWindow();
	//InitD3D(hwnd);
	//InitVB();

	//////////////////////////////////////////////////////////////////////////
	if ( NULL == g_pFlashSprite)
	{
		g_pFlashSprite = new FlashSprite();
	}
	if (NULL == g_pIDirectVideo )
	{
		g_pIDirectVideo = new CDirectXVideoTexture();
	}
	g_pFlashSprite->StartFlash("test.swf",128,128);
	g_pIDirectVideo->CreateFromAVIFile("test.avi");
	//Load_Bitmap_File_My(&bitmap,"bitmap24.bmp");
	Load_Bitmap_File_My(&bitmap_close, "CLOSE.bmp");
	Load_Bitmap_File_My(&bitmap_close_over, "CLOSE_OVER.bmp");
	Load_Bitmap_File_My(&bitmap_playwindow, "PLAY_WINDOW.bmp");
	//Load_Bitmap_File_My(&bitmap_playwindow_over, "PLAY_WINDOW_OVER.bmp");
	
	//InstallHotKey();

	RGB16Bit = RGB16Bit565;

	//g_zoomBmpbuffer = new UCHAR[128 * 128 * 3];


	//Open_Error_File("ERROR.TXT");

	//// initialize math engine
	//Build_Sin_Cos_Tables();

	//// initialize the camera with 90 FOV, normalized coordinates
	//Init_CAM4DV1(&cam,      // the camera object
	//	CAM_MODEL_EULER, // the euler model
	//	&cam_pos,  // initial camera position
	//	&cam_dir,  // initial camera angles
	//	&cam_target,      // no target
	//	100.0,        // near and far clipping planes
	//	1000.0,
	//	120.0,      // field of view in degrees
	//	1024,   // size of final screen viewport
	//	768);

	//// load flat shaded cube
	//VECTOR4D_INITXYZ(&vscale,20.00,20.00,20.00); 

	//// load all the objects in
	//for (int index_obj=0; index_obj < NUM_OBJECTS; index_obj++)
	//{
	//	Load_OBJECT4DV2_COB(&obj_array[index_obj], object_filenames[index_obj],  
	//		&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ  |
	//		VERTEX_FLAGS_TRANSFORM_LOCAL 
	//		/* VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD*/ );

	//} // end for index_obj

	//// set current object
	//curr_object = 0;
	//obj_work = &obj_array[curr_object];

	//// set up lights
	//Reset_Lights_LIGHTV2(lights2, MAX_LIGHTS);

	//// create some working colors
	//white.rgba = _RGBA32BIT(255,255,255,0);
	//gray.rgba  = _RGBA32BIT(100,100,100,0);
	//black.rgba = _RGBA32BIT(0,0,0,0);
	//red.rgba   = _RGBA32BIT(255,0,0,0);
	//green.rgba = _RGBA32BIT(0,255,0,0);
	//blue.rgba  = _RGBA32BIT(0,0,255,0);

	//// ambient light
	//Init_Light_LIGHTV2(lights2,
	//	AMBIENT_LIGHT_INDEX,   
	//	LIGHTV2_STATE_ON,      // turn the light on
	//	LIGHTV2_ATTR_AMBIENT,  // ambient light type
	//	gray, black, black,    // color for ambient term only
	//	NULL, NULL,            // no need for pos or dir
	//	0,0,0,                 // no need for attenuation
	//	0,0,0);                // spotlight info NA

	//VECTOR4D dlight_dir = {-1,0,-1,1};

	//// directional light
	//Init_Light_LIGHTV2(lights2,
	//	INFINITE_LIGHT_INDEX,  
	//	LIGHTV2_STATE_ON,      // turn the light on
	//	LIGHTV2_ATTR_INFINITE, // infinite light type
	//	black, gray, black,    // color for diffuse term only
	//	NULL, &dlight_dir,     // need direction only
	//	0,0,0,                 // no need for attenuation
	//	0,0,0);                // spotlight info NA


	//VECTOR4D plight_pos = {0,200,0,1};

	//// point light
	//Init_Light_LIGHTV2(lights2,
	//	POINT_LIGHT_INDEX,
	//	LIGHTV2_STATE_ON,      // turn the light on
	//	LIGHTV2_ATTR_POINT,    // pointlight type
	//	black, green, black,   // color for diffuse term only
	//	&plight_pos, NULL,     // need pos only
	//	0,.001,0,              // linear attenuation only
	//	0,0,1);                // spotlight info NA


	//VECTOR4D slight2_pos = {0,200,0,1};
	//VECTOR4D slight2_dir = {-1,0,-1,1};

	//// spot light2
	//Init_Light_LIGHTV2(lights2,
	//	SPOT_LIGHT2_INDEX,
	//	LIGHTV2_STATE_ON,         // turn the light on
	//	LIGHTV2_ATTR_SPOTLIGHT2,  // spot light type 2
	//	black, red, black,      // color for diffuse term only
	//	&slight2_pos, &slight2_dir, // need pos only
	//	0,.001,0,                 // linear attenuation only
	//	0,0,1);    


	//// create lookup for lighting engine
	//RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
	//	palette,             // source palette
	//	rgblookup);          // lookup table

	//////////////////////////////////////////////////////////////////////////
	//绘制正方体
	//Open_Error_File("ERROR.TXT");
	//Build_Sin_Cos_Tables();

	//Init_CAM4DV1(&cam,      // the camera object
	//	CAM_MODEL_EULER, // the euler model
	//	&cam_pos,  // initial camera position
	//	&cam_dir,  // initial camera angles
	//	NULL,      // no target
	//	50.0,      // near and far clipping planes
	//	500.0,
	//	90.0,      // field of view in degrees
	//	800,   // size of final screen viewport
	//	600);

	////// load the cube
	//Load_OBJECT4DV1_PLG(&obj, "cube2.plg",&vscale, &vpos, &vrot);

	////// set the position of the cube in the world
	//obj.world_pos.x = 0;
	//obj.world_pos.y = 0;
	//obj.world_pos.z = 100;
	//InstallHotKey();


	////////////////////////////////////////////////////////////////////////////
	////绘制带纹理的正方体
	//
	//srand(Start_Clock()); 

	Open_Error_File("ERROR.TXT");
	// initialize math engine
	Build_Sin_Cos_Tables();
	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // the euler model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		&cam_target,      // no target
		200.0,      // near and far clipping planes
		12000.0,
		120.0,      // field of view in degrees
		SCREEN_WIDTH,   // size of final screen viewport
		SCREEN_HEIGHT);


	// load flat shaded cube
	VECTOR4D_INITXYZ(&vscale,5.00,5.00,5.00);
	Load_OBJECT4DV2_COB(&obj_flat_cube,"cube_flat_textured_01.cob",  
		&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ | 
		VERTEX_FLAGS_TRANSFORM_LOCAL | 
		VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD );



	// set up lights
	Reset_Lights_LIGHTV1();

	// create some working colors
	white.rgba = _RGBA32BIT(255,255,255,0);
	gray.rgba  = _RGBA32BIT(100,100,100,0);
	black.rgba = _RGBA32BIT(0,0,0,0);
	red.rgba   = _RGBA32BIT(255,0,0,0);
	green.rgba = _RGBA32BIT(0,255,0,0);
	blue.rgba  = _RGBA32BIT(0,0,255,0);

	// ambient light
	Init_Light_LIGHTV1(AMBIENT_LIGHT_INDEX,   
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_AMBIENT,  // ambient light type
		gray, white, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0,0,0,                 // no need for attenuation
		0,0,0);                // spotlight info NA

	VECTOR4D dlight_dir = {-1,0,-1,0};

	// directional light
	Init_Light_LIGHTV1(INFINITE_LIGHT_INDEX,  
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_INFINITE, // infinite light type
		white, gray, white,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0,0,0,                 // no need for attenuation
		0,0,0);                // spotlight info NA


	VECTOR4D plight_pos = {0,200,0,0};

	// point light
	Init_Light_LIGHTV1(POINT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_POINT,    // pointlight type
		white, green, white,   // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0,.001,0,              // linear attenuation only
		0,0,1);                // spotlight info NA


	VECTOR4D slight2_pos = {0,200,0,0};
	VECTOR4D slight2_dir = {-1,0,-1,0};

	// spot light2
	Init_Light_LIGHTV1(SPOT_LIGHT2_INDEX,
		LIGHTV1_STATE_ON,         // turn the light on
		LIGHTV1_ATTR_SPOTLIGHT2,  // spot light type 2
		white, red, white,      // color for diffuse term only
		&slight2_pos, &slight2_dir, // need pos only
		0,.001,0,                 // linear attenuation only
		0,0,1);    


	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table

	
	return true;
}


bool UnInitDrawing()
{
	if ( NULL != g_pFlashSprite )
	{
		delete g_pFlashSprite;
		g_pFlashSprite = NULL;
	}
	if ( NULL != g_pIDirectVideo )
	{
		delete g_pIDirectVideo;
		g_pIDirectVideo = NULL;
	}

	//Unload_Bitmap_File(&bitmap);
	Unload_Bitmap_File(&bitmap_close);
	Unload_Bitmap_File(&bitmap_close_over);
	Unload_Bitmap_File(&bitmap_playwindow);
	g_pIDirectVideo->Stop();

	Close_Error_File();

	//if( g_pd3dDevice != NULL) 
	//	g_pd3dDevice->Release();

	////释放Direct3D对象
	//if( g_pD3D != NULL)
	//	g_pD3D->Release();

	//Cleanup();
	//delete [] g_zoomBmpbuffer;
	//UninstHotKey();
	return true;
}

bool Draw_Flash()
{

	static MATRIX4X4 mrot;   // general rotation matrix
	static float view_angle = 0; 
	static float camera_distance = 6000;
	static VECTOR4D pos = {0,0,0,0};
	static float tank_speed;
	static float turning = 0;
	// state variables for different rendering modes and help
	static int wireframe_mode = 1;
	static int backface_mode  = 1;
	static int lighting_mode  = 1;
	static int help_mode      = 1;
	static int zsort_mode     = 1;
	char work_string[256]; // temp string
	int index = 0; // looping var

	static float plight_ang = 0, slight_ang = 0; // angles for light motion

	//////////////////////////////////////////////////////////////////////////

	USHORT * back_buffer = (USHORT *)g_ddsd2.lpSurface;
	int back_lpitch = (int)(g_ddsd2.lPitch  >> 1 );

	//////////////////////////////////////////////////////////////////////////

	int nFlashHeight = g_pFlashSprite->m_nHeight;
	int nFlashWidth  = g_pFlashSprite->m_nWidth;
	g_pFlashSprite->m_pFlashPlayer->Update();
	g_pFlashSprite->m_pFlashPlayer->Render();
	g_pFlashData = (UCHAR *)g_pFlashSprite->m_pFlashPlayer->GetFlashFrameBuffer();

	//播放窗口高宽
	int nWindowHeight = 22 + nFlashHeight + 28;
	int nWindowWidth  = 8 + nFlashWidth + 8;

	int nbitPlayHeight = bitmap_playwindow.bitmapinfoheader.biHeight;
	int nbitPlayWidth  = bitmap_playwindow.bitmapinfoheader.biWidth;

	int nDWidth  = nWindowWidth - nbitPlayWidth;
	int nDHeight = nWindowHeight - nbitPlayHeight;

	//关闭位图的高宽
	int bitCloseHeight = bitmap_close.bitmapinfoheader.biHeight;
	int bitCloseWidth  = bitmap_close.bitmapinfoheader.biWidth;

	UCHAR *lpBuffer = g_pFlashData;

	static bool bOver = false;
	static bool bDown = false;
	static bool bShow = true;
	short isDown = KEY_DOWN(VK_LBUTTON);

	if (KEY_DOWN( VK_S ))
	{
		bShow =true;
	}

	HWND hWnd = GetActiveWindow();
	POINT kCursor;
	GetCursorPos(&kCursor);
	ScreenToClient(hWnd, &kCursor);

	if (isDown)
	{
		if (bOver)
		{
			bShow = false;
		}

		if ( !bDown && (kCursor.x >g_nWidthPos) && (kCursor.x < g_nWidthPos + nWindowWidth) &&
			(kCursor.y > g_nHeightPos) && (kCursor.y < g_nHeightPos + nWindowWidth) )
		{
			g_nWidthPos = kCursor.x;
			g_nHeightPos = kCursor.y;
			bDown = true;
		}
		if ( bDown )
		{
			g_nWidthPos = kCursor.x;
			g_nHeightPos = kCursor.y;
		}

		if (g_nWidthPos >= g_ddsd2.dwWidth - nWindowWidth )
		{
			g_nWidthPos = g_ddsd2.dwWidth - nWindowWidth;
		}
		if (g_nHeightPos >= g_ddsd2.dwHeight - nWindowHeight )
		{
			g_nHeightPos = g_ddsd2.dwHeight - nWindowHeight;
		}
	}
	else
	{
		bDown = false;
	}

	if ((kCursor.x > g_nWidthPos + nWindowWidth - bitCloseWidth) && (kCursor.x < g_nWidthPos + nWindowWidth ) &&
		(kCursor.y > g_nHeightPos + nWindowHeight - bitCloseHeight) && (kCursor.y < g_nHeightPos + nWindowHeight))
	{
		bOver = true;
	}
	else
	{
		bOver = false;
	}

	UCHAR blue  = 0;
	UCHAR green = 0;
	UCHAR red   = 0;
	USHORT pixTB = 0;
	USHORT pixLR = 0;
	USHORT pixel = 0;

	if (bShow)
	{
		if (nDWidth > 0 && nDHeight > 0)
		{
			for (int yh = 0; yh < nWindowHeight; yh ++)
			{
				for (int xw = 0; xw < nWindowWidth; xw++)
				{
					//绘制播放窗口头部分
					if ( yh < 22 )
					{
						if ( xw < 110 )
						{
							blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 1]) >>2;
							red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos) *back_lpitch ] = pixel;
							if (yh == 0 && xw == 109)
							{
								pixTB = pixel;
							}
						}
						else if ( xw < 110 + nDWidth )  
						{
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos)* back_lpitch ] = pixTB;
						}
						else 
						{
							blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
							red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos ) *back_lpitch ] = pixel;

						}
					}
					//绘制视频部分
					else if ( yh < 22 + nFlashHeight )
					{
						//////////////////////////////////////////////////////////////////////////
						if ( xw < 8 )
						{
							if (yh < 105 )
							{
								blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;
								if ( xw == 0 && yh == 104)
								{
									pixLR = pixel;
								}

							}
							else if ( yh  < 105 + nDHeight )
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixLR;
							}
							else
							{
								blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + xw * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
						}

						//////////////////////////////////////////////////////////////////////////
						//播放视频部分
						else if ( xw <= nFlashWidth + 8 )  
						{
							blue  = lpBuffer[ (yh - 22) * nFlashWidth * 4 + (xw - 8) * 4 + 0 ] >>3;
							green = lpBuffer[ (yh - 22) * nFlashWidth * 4 + (xw - 8) * 4 + 1 ] >>2;
							red   = lpBuffer[ (yh - 22) * nFlashWidth * 4 + (xw - 8) * 4 + 2 ] >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos  + ( g_nHeightPos + yh )*back_lpitch ] = pixel;

							//back_buffer[xw +g_nWidthPos  + (nWindowHeight + g_nHeightPos - yh - 1)*back_lpitch ] = pixel;
						}
						//////////////////////////////////////////////////////////////////////////
						if ( xw > nWindowWidth - 8 )
						{
							if (yh < 105 )
							{
								blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
							else if ( yh  < 105 + nDHeight )
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixLR;
							}
							else
							{
								blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
						}
					}
					//绘制播放窗口尾部分
					else if ( yh >= nWindowHeight - 28 )
					{
						if ( xw < 235 )
						{
							blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 1]) >>2;
							red   = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;
						}
						else if ( xw < 235 + nDWidth )  
						{
							if (yh - 22 - nFlashHeight < 5)
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = 0x0000;
							}
							else
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixTB;
							}

						}
						else 
						{
							blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
							red   = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (bOver)
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nWindowWidth - bitCloseWidth   + (index_y + g_nHeightPos + nWindowHeight - bitCloseHeight) *back_lpitch ] = pixel;
					}
				}
			}
		}

		//如果播放视频小于位图
		else
		{
			for (int y = 0 ; y < nFlashHeight; y ++) 
			{
				for (int x = 0 ; x < nFlashWidth; x ++)
				{
					UCHAR blue  = lpBuffer[ y*nFlashWidth*4 + x*4 + 0 ] >>3;
					UCHAR green = lpBuffer[ y*nFlashWidth*4 + x*4 + 1 ] >>2;
					UCHAR red   = lpBuffer[ y*nFlashWidth*4 + x*4 + 2 ] >>3;
					USHORT pixel = _RGB16BIT565(red, green, blue );
					back_buffer[x +g_nWidthPos  + ( g_nHeightPos + y )*back_lpitch ] = pixel;
				}
			}
			/*
			if (bOver)
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nFlashWidth - bitCloseWidth   + (index_y + g_nHeightPos + nFlashHeight - bitCloseHeight) *back_lpitch ] = pixel;
					}
				}
			}
			else
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nFlashWidth - bitCloseWidth   + (index_y + g_nHeightPos + nFlashHeight - bitCloseHeight) *back_lpitch ] = pixel;

					}
				}
			}
			//*/
		}

	}

	//////////////////////////////////////////////////////////////////////////
	// move point light source in ellipse around game world

	Reset_RENDERLIST4DV2(&rend_list);

	lights[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	lights[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	lights[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	lights[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_ON;


	lights[POINT_LIGHT_INDEX].pos.x = 1000*Fast_Cos(plight_ang);
	lights[POINT_LIGHT_INDEX].pos.y = 100;
	lights[POINT_LIGHT_INDEX].pos.z = 1000*Fast_Sin(plight_ang);

	if ((plight_ang+=3) > 360)
		plight_ang = 0;

	// move spot light source in ellipse around game world
	lights[SPOT_LIGHT2_INDEX].pos.x = 1000*Fast_Cos(slight_ang);
	lights[SPOT_LIGHT2_INDEX].pos.y = 200;
	lights[SPOT_LIGHT2_INDEX].pos.z = 1000*Fast_Sin(slight_ang);

	if ((slight_ang-=5) < 0)
		slight_ang = 360;

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	// use these to rotate objects
	static float x_ang = 0, y_ang = 0, z_ang = 0;


	//////////////////////////////////////////////////////////////////////////
	// flat shaded textured cube

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(&obj_flat_cube);


	// set position of constant shaded cube
	//obj_flat_cube.world_pos.x = 0;
	//obj_flat_cube.world_pos.y = 0;
	obj_flat_cube.world_pos.z = 150;

	if ( KEY_DOWN(VK_LEFT))
	{
		obj_flat_cube.world_pos.x --;
		if (obj_flat_cube.world_pos.x < -80)
		{
			obj_flat_cube.world_pos.x = -80;
		}
	}

	if ( KEY_DOWN(VK_RIGHT) )
	{
		obj_flat_cube.world_pos.x ++;
		if (obj_flat_cube.world_pos.x  > 80)
		{
			obj_flat_cube.world_pos.x =  80;
		}
	}

	if ( KEY_DOWN(VK_UP))
	{
		obj_flat_cube.world_pos.y ++;
		if (obj_flat_cube.world_pos.y  > 60)
		{
			obj_flat_cube.world_pos.y =  60;
		}
	}

	if ( KEY_DOWN(VK_DOWN))
	{
		obj_flat_cube.world_pos.y --;
		if (obj_flat_cube.world_pos.y  < -60)
		{
			obj_flat_cube.world_pos.y =  -60;
		}
	}

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(&obj_flat_cube, &mrot, TRANSFORM_LOCAL_TO_TRANS,1);

	// perform world transform
	Model_To_World_OBJECT4DV2(&obj_flat_cube, TRANSFORM_TRANS_ONLY);

	//Insert_POLYF4DV2_RENDERLIST4DV2( &rend_list, &face1);
	// insert the object into render list

	obj_flat_cube.texture->buffer = lpBuffer;
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_flat_cube,0);

	// update rotation angles
	//*
	if ((x_ang+=1) > 360) 
		x_ang = 0;
	if ((y_ang+=2) > 360) 
		y_ang = 0;
	if ((z_ang+=3) > 360) 
		z_ang = 0;
	//*/
	// remove backfaces
	
	Remove_Backfaces_RENDERLIST4DV2(&rend_list, &cam);

	// light scene all at once 
	
	Light_RENDERLIST4DV2_World16(&rend_list, &cam, lights, 4);

	// apply world to camera transform
	World_To_Camera_RENDERLIST4DV2(&rend_list, &cam);

	// sort the polygon list (hurry up!)
	
	Sort_RENDERLIST4DV2(&rend_list,  SORT_POLYLIST_AVGZ);

	// apply camera to perspective transformation
	Camera_To_Perspective_RENDERLIST4DV2(&rend_list, &cam);

	// apply screen transform
	Perspective_To_Screen_RENDERLIST4DV2(&rend_list, &cam);

	Draw_RENDERLIST4DV2_Solid16(&rend_list, (UCHAR *)back_buffer, back_lpitch <<1);

	//Draw_RENDERLIST4DV2_Wire16(&rend_list, (UCHAR *)back_buffer, back_lpitch << 1);

	return true;

}


bool Draw_Video()
{
	//USHORT * back_buffer = (USHORT *)g_ddsd2.lpSurface;
	//int back_lpitch = (int)(g_ddsd2.lPitch  >> 1 );


	USHORT * back_buffer = (USHORT *)g_ddsd.lpSurface;
	int back_lpitch = (int)(g_ddsd.lPitch  >> 1 );

	if ( ! ( back_lpitch == 800 )  )
	{
		return true;
	}

	/*
	*视频播放部分
	*/
	
	g_pIDirectVideo->CheckForLoop();
	g_pVideoData = (UCHAR *)g_pIDirectVideo->m_pVideoBuffer;
	int nVideoHeight = g_pIDirectVideo->m_VideoHeight;
	int nVideoWidth  = g_pIDirectVideo->m_VideoWidth;

	//播放窗口高宽
	int nWindowHeight = 22 + nVideoHeight + 28;
	int nWindowWidth  = 8 + nVideoWidth + 8;

	int nbitPlayHeight = bitmap_playwindow.bitmapinfoheader.biHeight;
	int nbitPlayWidth  = bitmap_playwindow.bitmapinfoheader.biWidth;

	int nDWidth  = nWindowWidth - nbitPlayWidth;
	int nDHeight = nWindowHeight - nbitPlayHeight;

	//关闭位图的高宽
	int bitCloseHeight = bitmap_close.bitmapinfoheader.biHeight;
	int bitCloseWidth  = bitmap_close.bitmapinfoheader.biWidth;

	UCHAR *lpBuffer = g_pVideoData;

	static bool bOver = false;
	static bool bDown = false;
	static bool bShow = true;
	short isDown = KEY_DOWN(VK_LBUTTON);

	if (KEY_DOWN( VK_S ))
	{
		bShow =true;
	}

	HWND hWnd = GetActiveWindow();
	POINT kCursor;
	GetCursorPos(&kCursor);
	ScreenToClient(hWnd, &kCursor);

	if (isDown)
	{
		if (bOver)
		{
			bShow = false;
		}

		if ( !bDown && (kCursor.x >g_nWidthPos) && (kCursor.x < g_nWidthPos + nWindowWidth) &&
			(kCursor.y > g_nHeightPos) && (kCursor.y < g_nHeightPos + nWindowWidth) )
		{
			g_nWidthPos = kCursor.x;
			g_nHeightPos = kCursor.y;
			bDown = true;
		}
		if ( bDown )
		{
			g_nWidthPos = kCursor.x;
			g_nHeightPos = kCursor.y;
		}

		if (g_nWidthPos >= g_ddsd2.dwWidth - nWindowWidth )
		{
			g_nWidthPos = g_ddsd2.dwWidth - nWindowWidth;
		}
		if (g_nHeightPos >= g_ddsd2.dwHeight - nWindowHeight )
		{
			g_nHeightPos = g_ddsd2.dwHeight - nWindowHeight;
		}
	}
	else
	{
		bDown = false;
	}

	if ((kCursor.x > g_nWidthPos + nWindowWidth - bitCloseWidth) && (kCursor.x < g_nWidthPos + nWindowWidth ) &&
		(kCursor.y > g_nHeightPos + nWindowHeight - bitCloseHeight) && (kCursor.y < g_nHeightPos + nWindowHeight))
	{
		bOver = true;
	}
	else
	{
		bOver = false;
	}


	/*UCHAR blue  = 0x0A >> 3;
	UCHAR green = 0x0A >> 2;
	UCHAR red   = 0x79 >> 3;
	USHORT pixTB = _RGB16BIT565( red, green, blue);
	red   = 0xAA >> 3;
	green = 0x35 >> 2;
	blue  = 0x35 >> 3;
	USHORT pixLR = _RGB16BIT565(red, green, blue);
	USHORT pixel = pixTB;*/

	UCHAR blue  = 0;
	UCHAR green = 0;
	UCHAR red   = 0;
	USHORT pixTB = 0;
	USHORT pixLR = 0;
	USHORT pixel = 0;

	if (bShow)
	{
		if (nDWidth > 0 && nDHeight > 0)
		{
			for (int yh = 0; yh < nWindowHeight; yh ++)
			{
				for (int xw = 0; xw < nWindowWidth; xw++)
				{
					//绘制播放窗口头部分
					if ( yh < 22 )
					{
						if ( xw < 110 )
						{
							blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 1]) >>2;
							red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos) *back_lpitch ] = pixel;
							if (yh == 0 && xw == 109)
							{
								pixTB = pixel;
							}
						}
						else if ( xw < 110 + nDWidth )  
						{
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos)* back_lpitch ] = pixTB;
						}
						else 
						{
							blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
							red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos ) *back_lpitch ] = pixel;

						}
					}
					//绘制视频部分
					else if ( yh < 22 + nVideoHeight )
					{
						//////////////////////////////////////////////////////////////////////////
						if ( xw < 8 )
						{
							if (yh < 105 )
							{
								blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;
								if ( xw == 0 && yh == 104)
								{
									pixLR = pixel;
								}

							}
							else if ( yh  < 105 + nDHeight )
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixLR;
							}
							else
							{
								blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + xw * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
						}

						//////////////////////////////////////////////////////////////////////////
						else if ( xw <= nVideoWidth + 8 )  
						{
							blue  = lpBuffer[ (yh - 22) * nVideoWidth * 3 + (xw - 8) * 3 + 0 ] >>3;
							green = lpBuffer[ (yh - 22) * nVideoWidth * 3 + (xw - 8) * 3 + 1 ] >>2;
							red   = lpBuffer[ (yh - 22) * nVideoWidth * 3 + (xw - 8) * 3 + 2 ] >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos  + (nWindowHeight + g_nHeightPos - yh - 7)*back_lpitch ] = pixel;

							//back_buffer[xw +g_nWidthPos  + (nWindowHeight + g_nHeightPos - yh - 1)*back_lpitch ] = pixel;
						}
						//////////////////////////////////////////////////////////////////////////
						if ( xw > nWindowWidth - 8 )
						{
							if (yh < 105 )
							{
								blue  = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[yh * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
							else if ( yh  < 105 + nDHeight )
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixLR;
							}
							else
							{
								blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
								green = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
								red   = (bitmap_playwindow.buffer[(yh - nDHeight)  * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
								pixel = _RGB16BIT565(red, green, blue );
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

							}
						}
					}
					//绘制播放窗口尾部分
					else if ( yh >= nWindowHeight - 28 )
					{
						if ( xw < 235 )
						{
							blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 1]) >>2;
							red   = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + xw * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;
						}
						else if ( xw < 235 + nDWidth )  
						{
							if (yh - 22 - nVideoHeight < 5)
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = 0x0000;
							}
							else
							{
								back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixTB;
							}
							
						}
						else 
						{
							blue  = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 0]) >>3;
							green = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 1])>>2;
							red   = (bitmap_playwindow.buffer[(yh - nDHeight) * nbitPlayWidth * 3 + (xw - nDWidth) * 3 + 2]) >>3;
							pixel = _RGB16BIT565(red, green, blue );
							back_buffer[xw +g_nWidthPos + (yh + g_nHeightPos  ) *back_lpitch ] = pixel;

						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (bOver)
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nWindowWidth - bitCloseWidth   + (index_y + g_nHeightPos + nWindowHeight - bitCloseHeight) *back_lpitch ] = pixel;
					}
				}
			}
		}
		
		//如果播放视频小于位图
		else
		{
			for (int y = 0 ; y < nVideoHeight; y ++) 
			{
				for (int x = 0 ; x < nVideoWidth; x ++)
				{
					UCHAR blue  = lpBuffer[ y*nVideoWidth*3 + x*3 + 0 ] >>3;
					UCHAR green = lpBuffer[ y*nVideoWidth*3 + x*3 + 1 ] >>2;
					UCHAR red   = lpBuffer[ y*nVideoWidth*3 + x*3 + 2 ] >>3;
					USHORT pixel = _RGB16BIT565(red, green, blue );
					back_buffer[x +g_nWidthPos  + (nVideoHeight + g_nHeightPos - y - 1)*back_lpitch ] = pixel;
				}
			}

			if (bOver)
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close_over.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nVideoWidth - bitCloseWidth   + (index_y + g_nHeightPos + nVideoHeight - bitCloseHeight) *back_lpitch ] = pixel;
					}
				}
			}
			else
			{
				for (int index_y = 0; index_y < bitCloseHeight; index_y ++)
				{
					for ( int index_x = 0; index_x < bitCloseWidth; index_x ++)
					{
						UCHAR blue  = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 0]) >>3;
						UCHAR green = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 1])>>2;
						UCHAR red   = (bitmap_close.buffer[index_y * bitCloseWidth * 3 + index_x * 3 + 2]) >>3;
						USHORT pixel = _RGB16BIT565(red, green, blue );
						back_buffer[index_x +g_nWidthPos + nVideoWidth - bitCloseWidth   + (index_y + g_nHeightPos + nVideoHeight - bitCloseHeight) *back_lpitch ] = pixel;
		
					}
				}
			}
		}
		
	}

	return true;
}

bool ShowDialog()
{
	if (KEY_DOWN(VK_RIGHT))
	{
		//HWND hwnd = GetForegroundWindow();
		//MessageBox(hwnd, "test","test",MB_OK);
		
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
			GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
			"ClassName", NULL };
		RegisterClassEx( &wc );

		//创建窗口
		HWND hWnd = CreateWindow( "ClassName", "最简单的Direct3D程序", 
			WS_OVERLAPPEDWINDOW, 200, 100, 600, 500,
			NULL, NULL, wc.hInstance, NULL );

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		SetFocus(hWnd);
		SetForegroundWindow(hWnd);
		SetWindowLong(hWnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW );
		ShowCursor(true);
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

		UpdateWindow( hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		while( GetMessage( &msg, NULL, 0, 0))
		{
			TranslateMessage( &msg);
			DispatchMessage(&msg);
		}
		UnregisterClass( "ClassName", wc.hInstance );
	}
	return true;
}

unsigned   char*   Color256(FILE *fpsource, int   sourcelen,int   sourcewid)   
{   
	int   Colorcount[4096];   
	short     Colorindex[4096];   
	unsigned   char  *bmphead, *buff, *Contensta;   
	buff=new   unsigned   char[sourcelen*sourcewid*3];   
	Contensta=new   unsigned   char[sourcelen*sourcewid];   
	memset(Colorindex,0,4096*sizeof(short));   
	memset(Colorcount,0,4096*sizeof(int));   
	fseek(fpsource,54,0);   
	fread(buff,1,sourcelen*sourcewid*3,fpsource);   
	int   i,j;   
	int   blue,green,red,clrindex,colornum=0;   
	struct   panel
	{   
		int   blue;   
		int   green;   
		int   red;   
	}pan[256];

	for(i=0;i<sourcelen*sourcewid*3;)   
	{   
		blue=buff[i++]&0xf0;   
		green=buff[i++]&0xf0;   
		red=buff[i++]&0xf0;   
		clrindex=(blue<<4)+green+(red>>4);   
		Colorcount[clrindex]++;   
	}   
	for(i=0;i<4096;i++)   
	{   
		if(Colorcount[i]!=0)   
		{   
			Colorcount[colornum]=Colorcount[i];   
			Colorindex[colornum]=short(i);   
			colornum++;   
		}   
	}   
	//quicksort(Colorcount,Colorindex,0,colornum-1);   

	for(i=0,j=0;i<256;i++)   
	{   
		bmphead[54+j++]=(Colorindex[colornum-i-1]&0xf00)>>4;   
		bmphead[54+j++]=(Colorindex[colornum-i-1]&0x0f0);   
		bmphead[54+j++]=(Colorindex[colornum-i-1]&0x00f)<<4;   
		bmphead[54+j++]=0;   
		Colorcount[colornum-i-1]=i;   
		pan[i].blue=(Colorindex[colornum-i-1]&0xf00)>>4;   
		pan[i].green=(Colorindex[colornum-i-1]&0x0f0);   
		pan[i].red=(Colorindex[colornum-i-1]&0x00f)<<4;   
	}   
	if(colornum>256)   
	{   
		int   Colorerror,Colorerr;   
		for(i=0;i<colornum-256;i++)   
		{   
			Colorerr=1000000;   
			blue=(Colorindex[i]&0xf00)>>4;   
			green=(Colorindex[i]&0x0f0);   
			red=(Colorindex[i]&0x00f)<<4;   
			clrindex=0;   
			for(j=0;j<256;j++)   
			{   
				Colorerror=(blue-pan[j].blue)*(blue-pan[j].blue)+   
					(green-pan[j].green)*(green-pan[j].green)+   
					(red-pan[j].red)*(red-pan[j].red);   
				if(Colorerror<Colorerr)   
				{   
					Colorerr=Colorerror;   
					clrindex=j;   
				}   
			}   
			Colorcount[i]=clrindex;   
		}   
	}   
	for(i=0;i<sourcelen*sourcewid*3;)   
	{   
		blue=buff[i++]&0xf0;   
		green=buff[i++]&0xf0;   
		red=buff[i++]&0xf0;   
		clrindex=(blue<<4)+green+(red>>4);   
		for(j=colornum-1;j>=0;j--) 
		{
			if(clrindex==Colorindex[j])   
			{   
				Contensta[ sourcewid * (sourcelen-(i- 3)/3/sourcewid-1) + (i-3)/3%sourcewid] = (unsigned char)(Colorcount[j]);
				break;   
			}   
		}
	}   
	delete   buff;   
	return   Contensta;   
}
