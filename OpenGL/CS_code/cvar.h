
#ifndef  CVAR_H
#define  CVAR_H

class CVARList 
{
public:
	void init();
public:
	float recoil;
	float fov;
	float soundtime;
	float radar_range;
	float speed;    
	float aspeed;
	float height_correction; 
	float chaseofs; 
	float esp_offs;
	float esp_trans;
	float mefrozen;
	float fspeed;
	float predahead;
    float chaseup; 
    float chaseback; 
	//====================================================

	int wallsensitivity;
	int trans;
	int smoke;
	int flash;
	int sniper;
	int esp;
	int jump;
	int duck;
	int autojump;
	int team;
	int aim;
	int spreaddraw;
	int mode;
	int shoot;
	int wall;
	int aimthru;
	int weapon;
	int name;
	int info;        
	int confont;
	int soundtol;
	int soundmax;  
	int soundfilter;
	int sounddisplay;
	int soundradar;
    int avdraw;
	int radar;
	int radar_x;
	int radar_y;
	int radar_size;
	int pistol;
	int speedon;
	int speedoff;
	int hud;
	int con_x;
	int con_y;
	int con_w;
	int con_h;
	int menu_x;
	int menu_y;
	int menu_w;
	int con_slidespeed;     
	int inpvs;  
	int reload;
	int alive_method;
	int spec_fix;
	int frzn_esp;
	int chase_cam;
	int nosky;
	int vswitch;
	int nspeed;
	int autowall;
	int nospread;
	int antizoom;
	int bombtimer;
	int reloading;
	int saystats;
	int eswitch;
	int glextra;
	int FBIpred;
	int pred;
	int predtime;
	int predmax;
	int predback;

};
extern CVARList cvar;

#endif
