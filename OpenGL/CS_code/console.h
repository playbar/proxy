
#ifndef CONSOLE_H
#define CONSOLE_H

#pragma once
#include "timehandling.h"
#include "color.h"

template < typename T, int N >
class WrapHistory
{
protected:
	void increase(int& idx) { ++idx; if(idx==N) idx=0;     }
	void decrease(int& idx) { --idx; if(idx<0 ) idx=(N-1); }
public:
	WrapHistory() : readpos(0), writepos(0) {}
	void add(const T& entry) 
	{ 
		increase(writepos);
		entries[writepos]=entry; 
		reset();
	}
	void reset(){ readpos = writepos; }
	T&   read (){ return entries[readpos]; }
	void prev (){ decrease(readpos); if(readpos==writepos) increase(readpos); }
	void next (){ if(readpos!=writepos) increase(readpos);  }
protected:
	int readpos;
	int writepos;
	T entries[N];
};

using namespace std;

class OgcConsole
{
public:
	int active;
	int mode;   enum{ MODE_EXECUTE=0, MODE_CHAT=1 };
	void draw(int x, int y, int w, int h);
	void echo(const char *fmt, ... );
	void say (const char* text, const char* name, int team );
	void key (int ch);
	void setcolortag(unsigned char ch, int r, int g, int b);
protected:
	WrapHistory<string,80> lines;
	WrapHistory<string,80> history;
	enum { DIR_BACK, DIR_FORWARD } hist_direction;
	enum { EDIT_MAX=160 };
	char editbuf[EDIT_MAX];
	char* const editline;
	char* cursorpos;
	StopTimer blinkTimer;
	bool blink;
	ColorEntry colorTags[26];
	int curColorTag;
	void drawConsoleLine( const string& str, int x, int y );
	int curHeight;
	EventCounter<0.01> scrollEventCounter;
public:
	OgcConsole() : editline(editbuf+1) , hist_direction(DIR_BACK)
	{ 
		editbuf[0] = '>';
		editbuf[1] =  0 ;
		editbuf[EDIT_MAX-1] = -1;
		cursorpos = editline;
		curColorTag = 0;
		colorTags['a'-'a'] = *colorList.get(18);
		colorTags['x'-'a'] = *colorList.get(19);
		curHeight = 0;
		mode = MODE_EXECUTE;
	}
};
extern OgcConsole gConsole;

#endif
