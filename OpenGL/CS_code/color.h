#ifndef COLOR_H
#define COLOR_H
#include "stringfinder.h"
#include <vector>
#include <assert.h>
//====================================================================
struct ColorEntry 
{ 
	DWORD r,g,b,a; 
	DWORD compiled;
	float fr,fg,fb;
	float onebased_r,onebased_g,onebased_b,onebased_a;
	
	ColorEntry():r(255),g(255),b(255),a(255)
		,fr(1.0),fg(1.0),fb(1.0)
		,onebased_r(1.0),onebased_g(1.0),onebased_b(1.0),onebased_a(1.0)
		,compiled(0xFFFFFFFF){}

	void fill_missing();
};

//====================================================================
class ColorManager
{
public:
	ColorManager(){ init(); }
	void set(char* name, char* red, char* green, char* blue, char* alpha);
	
	// see init() for valid indexes
	ColorEntry* get(unsigned int index)
	{
		if(index<list.size())
			return &(list[index]);
		else
			return &(list[0]);
	} 
private:
	StringFinder indexes;
	std::vector<ColorEntry> list;
	int insertpos;

	void add(const char* name,int r, int g, int b, int a=255);
	void init(); 
};

// Color Manager
extern ColorManager colorList;

#endif