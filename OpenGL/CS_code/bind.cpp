
#pragma warning (disable:4786)
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include "cvar.h"
#include "bind.h"
#include "console.h"

using namespace std;

//===================================================================================
KeyBindManager keyBindManager;
void Con_Echo(const char *fmt, ... );
string getOgcDirFile(const char* basename);
bool gMenuActive();
void func_menu_select();
void func_menu_back  ();
void func_menu_up    ();
void func_menu_down  ();
static int getConsoleExtraCode( int scancode )
{
	switch(scancode)
	{
	case 127:  return -1;
	case 13:  return '\n';
	case 32:  return ' ';
	case 128: return -2; 
	case 129: return -3;   
	case 130: return -4;   
	case 131: return -5;   
	default:  return  0; 
	}
}

//===================================================================================
bool KeyBindManager::keyBlocked(int scancode,bool down)
{
	if( (con_visible && con_visible()) || messageMode ) return false;
	const char* name = keyNames[scancode].c_str();
	if( !*name ){ return false; }
	if( gConsole.active && down )
	{
		if( !name[1] && name[0]!='~' )      return true;
		if( getConsoleExtraCode(scancode) ) return true;
	}
	const char* keyname = keyNames[scancode].c_str();
	return myBind.find(keyname) ;
}

//===================================================================================
bool KeyBindManager::keyBlocked(const char* name)
{
	bool block = myBind.find(name);
	return block||gMenuActive();
}

//===================================================================================
void KeyBindManager::remapKey(char* from, char* to)
{
	int scancode_from = -1;
	int scancode_to   = -1;
	for(int i=0;i<256;i++)
	{
		if( !strcmp(from,keyNames[i].c_str()) ){ scancode_from = i; }
		if( !strcmp(to  ,keyNames[i].c_str()) ){ scancode_to   = i; }
	}
	keyRemapTable[scancode_from] = scancode_to;
}

//===================================================================================
void KeyBindManager::expandCommand(string& expand,bool down)
{
	if(expand[0]=='+')
	{
		if(down){
			CallBack_Execute(expand);
		}else {
			expand[0]='-';
			CallBack_Execute(expand);
			expand[0]='+';
		}
	}
	else if( (expand[0]=='.'||expand[0]=='#') && expand[1]=='+' )
	{
		if(down){
			CallBack_Execute(expand);
		}else {
			expand[1]='-';
			CallBack_Execute(expand);
			expand[1]='+';
		}
	}
	else
	{
		if(down) { CallBack_Execute(expand); }
	}
}

//===================================================================================
void KeyBindManager::notifyKeyEvent(int scancode,bool down,bool repeat)
{
	const char* keyname = keyNames[scancode].c_str();
	if(!*keyname ) 
		return;
	if( !keyname[1] && down && (messageMode || (con_visible && con_visible()))  )
		return;
	if( gConsole.active && down)
	{ 
		if( !keyname [1] && keyname [0]!='~' )
		{
			int code = keyname [0];
			if( GetAsyncKeyState(VK_SHIFT) & (1 << (sizeof(SHORT) * 8 - 1)))
			{
				code = shiftRemap[code];
			}
			if(CallBack_ConType) CallBack_ConType(code);
			return;
		} else {
			int extra = getConsoleExtraCode(scancode);
			if (extra && CallBack_ConType) CallBack_ConType(extra);
		}
	}
	if(myBind.find(keyname) && !repeat)
	{
		string& expand = myBindExpand[myBind.num];
		expandCommand(expand,down);
	}
}

//===================================================================================
void KeyBindManager::save()
{
	ofstream ofs(getOgcDirFile("fx-data//fx-bind.cfg").c_str());
	if(!ofs){ Con_Echo("&rcannot save x-bind.cfg"); return; }
	for(myBind.it_start();myBind.it_running();myBind.it_next())
	{
		string& value = myBindExpand[myBind.num];
		ofs << "bind " << myBind.str << " \"" << value << "\"\x0D\x0A";
	}
}

//===================================================================================
void KeyBindManager::notifyMouseEvent(char* name,bool down)
{
	if(gMenuActive())
	{
		if(!strcmp(name,"mouse1"     )) { if(down)func_menu_select(); return; } else
		if(!strcmp(name,"mouse2"     )) { if(down)func_menu_back  (); return; } else
		if(!strcmp(name,"mwheelup"   )) { if(down)func_menu_up    (); return; } else
		if(!strcmp(name,"mwheeldown" )) { if(down)func_menu_down  (); return; } 
	}
	if(myBind.find(name))
	{
		string& expand = myBindExpand[myBind.num];
		expandCommand(expand,down);
	}
}

//===================================================================================
inline static void lowercase(char* str)
{
	while(*str){ *str = tolower(*str); ++str; }
}

//===================================================================================
void KeyBindManager::addBind(char* key, char* value)
{
	if(!*key)
	{
		Con_Echo("valid keys: ");
		string line;
		for(int i=0;i<256;i++)
		{ 
			string& name = keyNames[i];
			if(!name.empty())
			{ 
				line += " [";
				line += name;
				line += "] ";
			}
			if(line.size()>50)
			{
				Con_Echo(line.c_str());
				line.erase();
			}
		}
		Con_Echo(" [mouse1] [mouse2] [mouse3] [mouse4]");
		Con_Echo(" [mouse5] [mwheelup] [mwheeldown]");
		return;
	}
	lowercase(key);
	if(myBind.find(key))
	{
		string& expand = myBindExpand[myBind.num];
		if(*value)
		{
			expand = value; 
		} else {
			Con_Echo( "%s is bound to \"%s\"",key,expand.c_str() );
		}
	} else {
		bool found = false;
		for(int i=0;i<256;i++)if(keyNames[i]==key) {found=true;break;}
		if( !strcmp(key,"mouse1") || !strcmp(key,"mouse2") || 
			!strcmp(key,"mouse3") || !strcmp(key,"mouse4") || 
			!strcmp(key,"mouse5") || !strcmp(key,"mwheelup") || 
			!strcmp(key,"mwheeldown") )
		{
			found = true;
		}
		if(found)
		{
			if(*value)
			{
				int index = myBindExpand.size();
				myBindExpand.push_back(value);
				myBind.add(key,index);
			} else {
				Con_Echo("%s is not bound",key);
			}
		} else {
			Con_Echo( "%s is not a valid key.\n",key);
		}
	}
}

//===================================================================================
void KeyBindManager::removeBind(char* key)
{
	myBind.erase(key);
}

//===================================================================================
void KeyBindManager::init()
{
	int i;
	for(i=0;i<256;i++)keyRemapTable[i]=i;
	keyNames[49] = "1";
	keyNames[50] = "2";
	keyNames[51] = "3";
	keyNames[52] = "4";
	keyNames[53] = "5";
	keyNames[54] = "6";
	keyNames[55] = "7";
	keyNames[56] = "8";
	keyNames[57] = "9";
	keyNames[48] = "0";
	keyNames[45] = "-";
	keyNames[61] = "=";
	keyNames[127] = "backspace";
	keyNames[9] = "tab";
	keyNames[113] = "q";
	keyNames[119] = "w";
	keyNames[101] = "e";
	keyNames[114] = "r";
	keyNames[116] = "t";
	keyNames[121] = "y";
	keyNames[117] = "u";
	keyNames[105] = "i";
	keyNames[111] = "o";
	keyNames[112] = "p";
	keyNames[91] = "[";
	keyNames[93] = "]";
	keyNames[13] = "enter";
	keyNames[133] = "ctrl";
	keyNames[97] = "a";
	keyNames[115] = "s";
	keyNames[100] = "d";
	keyNames[102] = "f";
	keyNames[103] = "g";
	keyNames[104] = "h";
	keyNames[106] = "j";
	keyNames[107] = "k";
	keyNames[108] = "l";
	keyNames[59] = ";";
	keyNames[39] = "'";
	keyNames[134] = "shift";
	keyNames[0] = "\\";
	keyNames[122] = "z";
	keyNames[120] = "x";
	keyNames[99] = "c";
	keyNames[118] = "v";
	keyNames[98] = "b";
	keyNames[110] = "n";
	keyNames[109] = "m";
	keyNames[44] = ",";
	keyNames[46] = ".";
	keyNames[47] = "kp_slash";
	keyNames[134] = "rshift";
	keyNames[42] = "*";
	keyNames[132] = "alt";
	keyNames[32] = "space";
	keyNames[175] = "capslock";
	keyNames[135] = "f1";
	keyNames[136] = "f2";
	keyNames[137] = "f3";
	keyNames[138] = "f4";
	keyNames[139] = "f5";
	keyNames[140] = "f6";
	keyNames[141] = "f7";
	keyNames[142] = "f8";
	keyNames[143] = "f9";
	keyNames[144] = "f10";
	keyNames[255] = "pause";
	keyNames[151] = "home";
	keyNames[128] = "uparrow";
	keyNames[150] = "pgup";
	keyNames[173] = "minus";
	keyNames[130] = "leftarrow";
	keyNames[164] = "kp_5";
	keyNames[131] = "rightarrow";
	keyNames[174] = "plus";
	keyNames[152] = "end";
	keyNames[129] = "downarrow";
	keyNames[149] = "pgdn";
	keyNames[147] = "ins";
	keyNames[148] = "del";
	keyNames[145] = "f11";
	keyNames[146] = "f12";
	keyNames[239] = "mwheeldown";
	keyNames[240] = "mwheelup";
	keyNames[241] = "mouse1";
	keyNames[242] = "mouse2";
	keyNames[243] = "mouse3";
	keyNames[244] = "mouse4";
	keyNames[245] = "mouse5";
	for(i=0;i<=255;i++)  shiftRemap[i] = toupper((char)i);
	shiftRemap['1'] = '!';
	shiftRemap['2'] = '@';
	shiftRemap['3'] = '#';
	shiftRemap['4'] = '$';
	shiftRemap['5'] = '%';
	shiftRemap['6'] = '^';
	shiftRemap['7'] = '&';
	shiftRemap['8'] = '*';
	shiftRemap['9'] = '(';
	shiftRemap['0'] = ')';
	shiftRemap['-'] = '_';
	shiftRemap['='] = '+';
	shiftRemap['\''] = '\"';
	shiftRemap[';'] = ':';
}

//===================================================================================
int getScanCode_FixMessage(LPMSG lpMsg)
{
	int scancode = (lpMsg->lParam>>16)&0xFF;
	scancode = keyBindManager.remapScanCode(scancode);
	lpMsg->lParam &=  0xFF00FFFF;
	lpMsg->lParam +=  (scancode<<16);
	return scancode;
}
