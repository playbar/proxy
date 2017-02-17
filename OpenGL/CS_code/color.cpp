

#include "stringfinder.h"
#include <vector>


typedef unsigned long DWORD;
#include "color.h"

// Color Manager
ColorManager colorList;


//========================================================================================
void ColorManager::init()
{
	indexes.clear();
	list.clear();
	insertpos = 0;

	//   identifer     R   G   B   A      Access Index
	add("cross"      ,180,0,0,200	   ); // index: 0
	add("team1"      ,255, 40, 40      ); // index: 1
	add("team2"      , 40, 80,255      ); // index: 2
	add("special"    ,  0,  0,255      ); // index: 3
	add("target"     ,  0,255,  0      ); // index: 4
	add("hud"        ,120,120,255      ); // index: 5
	add("sound"      , 68, 68,255      ); // index: 6
	add("bombtimer"  ,255, 40, 40      ); // index: 7
	add("systime"    ,255,255,255      ); // index: 8
	add("esp"        ,255,255,  0      ); // index: 9
	add("back"       ,  0,  0,  0, 128 ); // index:10
	add("radar1"     , 80,150, 30, 110 ); // index:11
	add("radar2"     ,150,150,150,  60 ); // index:12
	add("text"       ,  255, 255,255      ); // index:13
	add("wire"       ,255,255,  0      ); // index:14
	add("lambert"    ,255,255,255      ); // index:15
	add("death"      ,255,  0,  0      ); // index:16
	add("con_back"   ,  160, 80,255, 128 ); // index:17
	add("con_text"   ,  255, 255,255, 128 ); // index:18
	add("con_text2"  ,160,160,160, 128 ); // index:19
	add("con_edit"   ,185,  200,  200      ); // index:20
    add("menu_back"  , 0 ,255 ,128, 25 ); // index:21
	add("menu_select",0 ,128 ,255 ,100 ); // index:22
	add("menu_text"  ,255,255,255     ); // index:23
	add("text_back"  ,  0,  0,  0, 100 ); // index:24
	add("snake_back" ,  0,  0,  0, 128 ); // index:25
	add("snake_body" ,  0,200, 66, 200 ); // index:26
	add("snake_item" ,  0, 55,222, 200 ); // index:27
	add("snake_wall" ,222, 66, 22, 200 ); // index:28
	add("tlight"     ,222,111,111, 255 ); // index:29
	add("snd_step"   ,255,255,255, 255 ); // index:30
	add("snd_weapon" ,144,155,255, 255 ); // index:31
	add("snd_hit"    ,222,100, 61, 254 ); // index:32
	add("snd_special",222,111,111, 254 ); // index:33
	add("snd_default",222,255,222, 255 ); // index:34
	add("team3"      ,200,200,  0      ); // index:35
	add("team4"      , 20,222,100      ); // index:36
	add("nade"       ,255,0,0          ); // index:37
	add("tsound"     ,255, 40, 40      ); // index:38
	add("ctsound"    , 40, 80,255      ); // index:39
	add("stats"      ,  0,255,0        ); // index:40

}

//========================================================================================
void ColorEntry::fill_missing()
{
	compiled = (r<<24) + (g<<16) + (b<<8) + a;
	fr= float(r)/255.0f/30.0f;
	fg= float(g)/255.0f/30.0f;
	fb= float(b)/255.0f/30.0f;

	onebased_r=float(r)/255.0f;
	onebased_g=float(g)/255.0f;
	onebased_b=float(b)/255.0f;
	onebased_a=float(a)/255.0f;
}

//========================================================================================
void ColorManager::add(const char* name,int r, int g, int b, int a)
{
	indexes.add(name,insertpos);

	ColorEntry tmp;
	tmp.r=r;
	tmp.g=g;
	tmp.b=b;
	tmp.a=a;
	tmp.fill_missing();

	list.push_back(tmp);

	insertpos++;
}
#define BOUND_INTEGER(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}


//========================================================================================
void ColorManager::set(char* name, char* red, char* green, char* blue, char* alpha)
{
	if( !indexes.find(name) ){ return; }

	int index = indexes.num;
	char format[]="%d";

	ColorEntry tmp;
	sscanf(red   ,format,&tmp.r);
	sscanf(green ,format,&tmp.g);
	sscanf(blue  ,format,&tmp.b);
	sscanf(alpha ,format,&tmp.a);
	
	tmp.fill_missing();

	BOUND_INTEGER(index,0,int(list.size()-1));
	list[index] = tmp;
}


