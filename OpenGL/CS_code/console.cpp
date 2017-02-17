



#define WIN32_LEAN_AND_MEAN
#pragma warning (disable:4786)
#include <windows.h>
#include <string>
#include <vector>
#include "console.h"
#include "client.h"
#include "stringfinder.h"
#include "interpreter.h"
#include "color.h"
#include "cvar.h"

OgcConsole gConsole;
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
void tintArea(int x,int y,int w,int h, struct ColorEntry* clr);
extern bool oglSubtractive;

//===================================================================================
void OgcConsole::draw(int con_x, int con_y, int con_w, int con_h)
{
	if(!active && !curHeight ) return;
	int nscroll = scrollEventCounter.get();
	for(int i=0;i<nscroll;i++)
	{
		if( active ){
			if(curHeight!=con_h) 
			{ 
				curHeight += cvar.con_slidespeed; 
				if(curHeight>con_h) curHeight=con_h; 
			}
		} else {
			curHeight -= cvar.con_slidespeed; 
			if(curHeight<=0) { curHeight=0; return; }
		}
	}
	ColorEntry* color = colorList.get(17);	
	
	int x = con_x+3;
	int y = con_y+curHeight-14;
	if(blinkTimer.expired())
	{
		blink = !blink;
		if(blink) { blinkTimer.countdown(0.2); }
		else      { blinkTimer.countdown(0.2); }
	}
	if(blink)
	{
		
		int length, height, ch_length;
		char  save;
		save = cursorpos[0]; cursorpos[0]=0;
		gEngfuncs.pfnDrawConsoleStringLen( editbuf, &length, &height );
		cursorpos[0]=save;
		save = cursorpos[1]; cursorpos[1]=0;
		gEngfuncs.pfnDrawConsoleStringLen( cursorpos, &ch_length, &height );
		cursorpos[1]=save;
		if(!*cursorpos) ch_length=5;
        tintArea(x+length,y,ch_length,height-2, colorList.get(19));
	}
	color = colorList.get(20);
	gEngfuncs.pfnDrawSetTextColor(color->onebased_r,color->onebased_g,color->onebased_b);
	consoleBorder(con_x,con_y,con_w,con_h);
	gEngfuncs.pfnDrawConsoleString(x,y,editbuf);
	lines.reset();
	for(;;)
	{
		y-=14;
		if(y<con_y) break;
		string& curLine = lines.read(); 
		lines.prev();
		drawConsoleLine(curLine,x,y);
	}
}

//===================================================================================
void OgcConsole::echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[384];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	lines.add( buf );
}

//===================================================================================
void OgcConsole::say(const char* text, const char* name, int team )
{
	if(team==1) { echo("&r%s :", name); echo(" %s",text); }
	else        { echo("&b%s :", name); echo(" %s",text); }
}

//===================================================================================
void OgcConsole::setcolortag(unsigned char ch, int r, int g, int b)
{
	ch -= 'a';
	if(ch>=26) { echo("color tags must be labeled \'a\' to \'z\'"); return; }
	colorTags[ch].r = r;
	colorTags[ch].g = g;
	colorTags[ch].b = b;
	colorTags[ch].fill_missing();
}

//===================================================================================
void OgcConsole::drawConsoleLine( const string& str, int x, int y )
{
	const char* line = str.c_str();
	char  buf[256];
	char* bufpos;

	for(;;)
	{
		// extract string part
		bufpos=buf;
		for(;;) { *bufpos=*line; if(!*line||*line=='&')break;  ++line; ++bufpos; };
		bufpos[0]=0;bufpos[1]=0;

		// draw
		int length, height;
		gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
		gEngfuncs.pfnDrawSetTextColor(colorTags[curColorTag].onebased_r, 
									  colorTags[curColorTag].onebased_g, 
									  colorTags[curColorTag].onebased_b);
		gEngfuncs.pfnDrawConsoleString(x,y,buf);

		// advance
		x+=length;
		if(*line=='&')
		{
			unsigned char ch = *++line - 'a';
			if(ch<26) curColorTag=ch;
			else         break;
			if(!*++line) break;
		}
		else 
		{
			break;
		}
	}
	curColorTag=0;
}

//===================================================================================
void OgcConsole::key(int ch)
{
	char* pos;
	switch(ch)
	{
	case -1:
		if(cursorpos==editline) return;
		pos = --cursorpos;
		while(pos[0]) { pos[0]=pos[1]; ++pos; }
		return;
	case -2:
		if(hist_direction!=DIR_BACK) { history.prev(); history.prev(); hist_direction=DIR_BACK;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.prev();
		return;
	case -3:
		if(hist_direction==DIR_BACK) { history.next(); history.next(); hist_direction=DIR_FORWARD;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.next();
		return;
	case -4: 
		if(cursorpos!=editline) --cursorpos;
		return;
	case -5:
		if(cursorpos!=(editline+strlen(editline))) ++cursorpos;
		return;
	case '\n':
		if( !strcmp(editline,"===") )
		{
			if(mode==MODE_EXECUTE) { mode=MODE_CHAT;    echo("&b*** &aCONSOLE: &wCHAT MODE &b***"); }
			else                   { mode=MODE_EXECUTE; echo("&b*** &aCONSOLE: &wEXEC MODE &b***"); }
		}
		else if(mode==MODE_EXECUTE)
		{
			echo    ( "&x%s",editbuf );
			if(editline[0])
			{
				cmd.exec( editline );
				history.add(editline);
				history.reset();
			}
		} 
		else if(mode==MODE_CHAT)
		{
			char* text = editline; while(*text==' ')++text;
			char buf[256];sprintf(buf,"say \"%s\"",text);
			gEngfuncs.pfnClientCmd(buf);
		}	
		editline[0]=0;
		cursorpos = editline;
		return;
	default:
		if(strlen(editbuf)>(EDIT_MAX-4)) return;
		if(!cursorpos[0]) { cursorpos[0]=ch; ++cursorpos; cursorpos[0]=0; return; }
		pos = editbuf+strlen(editbuf)+1; 
		while(pos>cursorpos) { pos[0]=pos[-1]; --pos; }
		*cursorpos = ch;
		++cursorpos;
		return;
	}
}
