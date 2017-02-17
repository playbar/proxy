
//========================================================================
class OgcCommandMenu
{
public:
	struct Menu;
	struct MenuEntry{
		MenuEntry() :menu(0){}

		string name;
		string content;
		string description;
		Menu*  menu;
	};

	struct Menu {
		Menu():parent(0),selection(0){}
		void boundSelection()
		{ 
			// wrap around
			if(selection<0) { selection = items.size()-1; }
			else  if(selection >= (int)items.size()) { selection = 0; }
		}

		Menu*  parent;
		string name; 
		int    selection;
		vector<MenuEntry> items;
	};

	OgcCommandMenu():baseMenu(0){}
	void init(const char* filename);

	struct Menu* baseMenu;
};

extern OgcCommandMenu ogcMenu;