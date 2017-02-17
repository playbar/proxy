
#ifndef BIND_H
#define BIND_H

#include "stringfinder.h"

class KeyBindManager
{
public:
	typedef void (*CallBack_ExecuteFunc)(const std::string& cmd);
	typedef void (*CallBack_ConTypeFunc)(char c);
	CallBack_ExecuteFunc CallBack_Execute;
	CallBack_ConTypeFunc CallBack_ConType;
	KeyBindManager() 
		: keyNames(256)
		, CallBack_Execute(NULL)
		, CallBack_ConType(NULL)
		, con_visible(NULL) 
		, messageMode(false)
	{}
	void notifyKeyEvent  ( int scancode,  bool down, bool repeat=false); 
	void notifyMouseEvent( char* keyname, bool down ); 
	bool keyBlocked    ( int scancode, bool down );
	bool keyBlocked    ( const char* name );
	int  remapScanCode ( int scancode ) { return keyRemapTable[scancode]; }
	void init( );
	void save( );
	void addBind   (char* key, char* value);
	void removeBind(char* key);
	void remapKey(char* from, char* to);
	bool messageMode;
	int  (*con_visible)();
protected:
	void expandCommand(std::string& cmd,bool down);
	StringFinder myBind;
	std::vector<std::string> myBindExpand;
	std::vector<std::string> keyNames;
	int   keyRemapTable[256];
	typedef unsigned char byte;
	byte  shiftRemap[256];
};
extern KeyBindManager keyBindManager;

#endif
