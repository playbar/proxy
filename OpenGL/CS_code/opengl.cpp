



#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "client.h"
#include <gl/gl.h>
#include <vector>
#include "stringfinder.h"
#include "color.h"
#include "cvar.h"
#include "aimbot.h"
#include "attack.h"
#include "opengl.h"

extern ColorManager colorList;
bool oglSubtractive = false;
int noSky = false;
typedef void (APIENTRY *Tgl1ui) (unsigned int);
typedef void (APIENTRY *Tgl3f)( float , float , float );
typedef void (APIENTRY *Tgl4f)( float , float , float , float );
typedef void (APIENTRY *Tgl2ui) (unsigned int, unsigned int);
typedef void (APIENTRY *TglTexEnvi) (unsigned int, unsigned int, int);
typedef void (APIENTRY *Tglv) (const float*);
typedef void (APIENTRY *Tglvui) (unsigned int,float*);
typedef void (APIENTRY *xglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef BOOL (APIENTRY *xwglSwapBuffers) (HDC);
typedef void (APIENTRY *xglBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *xglColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *xglLineWidth) (GLfloat width);
typedef void (APIENTRY *TglRectiFunc) (GLint x, GLint y, GLint x2, GLint y2);
Tgl1ui p_fnglBegin = NULL;
Tgl1ui p_fnglDisable= NULL;
Tgl1ui p_fnglEnable= NULL;
Tgl3f p_fnglColor3f = NULL;
Tgl3f p_fnglVertex3f = NULL;
Tglv p_fnglVertex3fv = NULL;
TglTexEnvi p_fnglTexEnvi=NULL;
Tgl2ui p_fnglPolygonMode= NULL;
Tgl4f p_fnglClearColor=NULL;
Tgl1ui p_fnglClear=NULL;
Tglvui p_fnglGetFloatv=NULL;
xglViewport oglViewport=NULL;
xwglSwapBuffers owglSwapBuffers=NULL;
xglColor4f oglColor4f=NULL;
xglBlendFunc oglBlendFunc=NULL;
xglLineWidth oglLineWidth=NULL;
TglRectiFunc oglRecti = NULL;


bool nodrawentities = false;

//===================================================================================
void opengl_ClearBackground()
{
	if(p_fnglClearColor && p_fnglClear)
	{
		ColorEntry* color = colorList.get(10);
		float r = float(color->r)/255.0;
		float g = float(color->g)/255.0;
		float b = float(color->b)/255.0;
		float a = float(color->a)/255.0;
		(*(Tgl4f )(p_fnglClearColor))(r,g,b,a);
		(*(Tgl1ui)(p_fnglClear))(GL_COLOR_BUFFER_BIT);
	}
}

//===================================================================================
void APIENTRY pglBlendFunc(GLenum sfactor, GLenum dfactor)
{
	if(oglSubtractive)
	{
		oglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		oglBlendFunc(sfactor, dfactor);
	}
}
bool AllowedToHookGL()
{
	char Buffer[MAX_PATH];
	GetPrivateProfileString("Settings", "OpenGL", "Yes", Buffer, MAX_PATH, "JoolzCheat.ini");
	if (!strcmp(Buffer, "Yes"))
		return true;
	else
		return false;
}
//===================================================================================
void APIENTRY pglViewport( GLint x,GLint y,GLsizei width,GLsizei height )
{	
	gAimbot.FindTarget();
	oglViewport(x,y,width,height);
}

//===================================================================================
void APIENTRY pglBegin(unsigned int mode)
{
	unsigned int mode2=mode;
	if( mode == GL_QUADS && cvar.nosky )
		noSky = true;
	else
		noSky = false;

	if (cvar.wall) 
	{
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			float curcolor[4];
			p_fnglGetFloatv(GL_CURRENT_COLOR, curcolor);
			p_fnglDisable(GL_DEPTH_TEST);
			p_fnglEnable (GL_BLEND);
			oglBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			oglColor4f(curcolor[0], curcolor[1], curcolor[2], .667f);
		}
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN)
		{
			p_fnglEnable(GL_DEPTH_TEST);
			p_fnglDisable(GL_BLEND);
		}
	}
	if (p_fnglBegin) (*p_fnglBegin)(mode);
}

//===================================================================================
void APIENTRY pglVertex3fv( const float* v )
{
	if(noSky)
		return;
	else
		(p_fnglVertex3fv)(v);
}

//===================================================================================
void APIENTRY pglVertex3f( float x, float y, float z) 
{
	if (p_fnglVertex3f) (*p_fnglVertex3f)(x, y, z);
}

//===================================================================================
void APIENTRY pglClear (GLbitfield mask)
{
	if((mask==GL_DEPTH_BUFFER_BIT)&&(cvar.wall == 1))
	{
		p_fnglClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	}
	else
	{
		p_fnglClear(mask);
	}
}

//===================================================================================
void CheckForOpenGlHook(FARPROC* pProc,LPCTSTR lpProcName)
{
	if (!strcmp(lpProcName,"glDisable")) 
	{
		p_fnglDisable = (Tgl1ui)*pProc;
	} else 
	if (!strcmp(lpProcName,"glEnable")) 
	{
		p_fnglEnable = (Tgl1ui)*pProc;
	} else 
    if (!strcmp(lpProcName,"glColor4f")) 
	{
		oglColor4f = (xglColor4f)*pProc;
	} else 
	if (!strcmp(lpProcName,"glBlendFunc")) 
	{
		oglBlendFunc = (xglBlendFunc)*pProc;
		*pProc = (FARPROC)&pglBlendFunc;
	} else 
	if (!strcmp(lpProcName,"glLineWidth")) 
	{
		oglLineWidth = (xglLineWidth)*pProc;
	} else 
	if (!strcmp(lpProcName,"glClear")) 
	{
		p_fnglClear = (Tgl1ui)*pProc;
		*pProc = (FARPROC)&pglClear;
	} else 
	if (!strcmp(lpProcName,"glTexEnvi")) 
	{
		p_fnglTexEnvi = (TglTexEnvi)*pProc;
	} else 
	if (!strcmp(lpProcName,"glColor3f")) 
	{
		p_fnglColor3f = (Tgl3f)*pProc;
	} else 
	if (!strcmp(lpProcName,"glPolygonMode")) 
	{
		p_fnglPolygonMode = (Tgl2ui)*pProc;
	} else 
	if (!strcmp(lpProcName,"glRecti")) 
	{
		oglRecti = (TglRectiFunc)*pProc;
	} else 
    if (!strcmp(lpProcName,"glGetFloatv")) 
	{
		p_fnglGetFloatv = (Tglvui)*pProc;
    } else 
    if (!strcmp(lpProcName,"glVertex3f")) 
	{
		p_fnglVertex3f = (Tgl3f)*pProc;
		*pProc = (FARPROC)&pglVertex3f;
	} else 
	if (!strcmp(lpProcName,"glBegin")) 
	{
		p_fnglBegin = (Tgl1ui)*pProc;
		*pProc = (FARPROC)&pglBegin;
	} else 
	if (!strcmp(lpProcName,"glClearColor")) 
	{ 
		p_fnglClearColor = (Tgl4f)*pProc;
	} else
	if (!strcmp(lpProcName,"glVertex3fv")) 
	{
		p_fnglVertex3fv = (Tglv)*pProc;
		*pProc = (FARPROC)&pglVertex3fv;
	} else
	if (!strcmp(lpProcName,"glViewport"))
	{
		oglViewport = (xglViewport)*pProc;
		*pProc = (FARPROC)&pglViewport;
	}
}
