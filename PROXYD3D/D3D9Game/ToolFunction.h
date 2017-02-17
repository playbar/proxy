
#pragma once

#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
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
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <direct.h>

#include <d3dx9.h>


namespace APP_Tools
{
	void appIntToChar(int num,char * text);
	void appIntToChar(int num,TCHAR * text);

	bool appInRect(int x,int y,RECT rect);
	bool appInRect(int x,int y,int rectX,int rectY,int rectWidth,int rectHeight);

	bool appRectangleBumpCheck( RECT rect1, RECT rect2 );
	bool appRectangleBumpCheck( int x1, int y1, int width1, int height1,
		int x2, int y2, int width2, int height2 );


	////////////////////////////////////////////////////////////////////////////

	void appVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans );




	///////////////////////////////////////////////////////////////////////////////

	double appGetTime();


	////////////////////////////////////////////////////////////////////////////////

	bool appRayPlaneCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 & vOut );

	/////////////////////////////////////////////////////////////////////////////////

	void ComputeTriNormal( D3DXVECTOR3 * v1, D3DXVECTOR3 * v2, D3DXVECTOR3 * v3,D3DVECTOR & normalVec );

	///////////////////////////////////////////////////////////////////////////////////


};