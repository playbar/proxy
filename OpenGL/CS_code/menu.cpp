

#include <vector>
#include <string>
using namespace std;
#include "menu.h"
#include <fstream>

//========================================================================================
OgcCommandMenu ogcMenu;
typedef OgcCommandMenu::Menu Menu;
typedef OgcCommandMenu::MenuEntry MenuEntry;


//========================================================================================
void getToken(istream& in, string& token)
{
	char ch;
	for(;;){
		in.get(ch);
		if(ch=='\"')
		{
			token.erase();
			for(;;)
			{
				in.get(ch);
				if(!in||ch=='\"') return;
				token+=ch;
			}
		} 
		else if(ch=='{') {token = "{";return;}
		else if(ch=='}') {token = "}";return;}
		//else if(ch=='-') {token = "-";return;}
		if(!in)          {token = "}";return;}
	}
}

//========================================================================================
void extractCommentString(istream& in, string& dest)
{
	dest.erase();
	char ch;in.get(ch);
	while(ch==' ' && ch!='\t')in.get(ch);
	if(ch=='/')
	{
		in.get(ch);
		if(ch=='/')
		{
			while(ch!=0x0D && ch!=0x0A)
			{
				in.get(ch);
				dest+=ch;
			}
		}
	}
}

#include <windows.h>

//========================================================================================
void readMenu(istream& in, Menu* pMenu)
{
	string dummy;
	for(;;)
	{
		getToken(in, dummy);    // " " or }
		if(dummy[0]=='}') { return; } // done with menu

		MenuEntry newEntry;

		getToken(in, newEntry.name    ); // "name"
		getToken(in, newEntry.content ); // "asdf;qqq" or {

		// extract comments
		extractCommentString(in, newEntry.description);

		if( newEntry.content[0]=='{' )
		{
			newEntry.menu = new Menu;
			newEntry.menu->name = newEntry.name;
			newEntry.menu->parent = pMenu;

			pMenu->items.push_back(newEntry);
			readMenu(in,newEntry.menu);
		} else {
			newEntry.menu = 0;
			pMenu->items.push_back(newEntry);
		}
		if(!in) break;
	}
}

//========================================================================================
void OgcCommandMenu::init(const char* filename)
{
	if(baseMenu) return;

	ifstream ifs;
	ifs.open(filename);
	baseMenu = new Menu;
	baseMenu->name = " 1Vs16 HooK";
	readMenu(ifs,baseMenu);
}



