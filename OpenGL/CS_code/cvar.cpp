
#include <stdio.h>
#include "cvar.h"
#include "interpreter.h"
CVARList cvar;

//===================================================================================
void CVARList::init()
{ 
	memset((char*)this, 0, sizeof(*this)); 
	#define REGISTER_CVAR_FLOAT(name,defaultvalue) cmd.AddCvarFloat(#name, &##name );name=defaultvalue##f;
	REGISTER_CVAR_FLOAT( speed ,1.0)
	REGISTER_CVAR_FLOAT( fov ,999999.0)
	REGISTER_CVAR_FLOAT( aspeed ,2.0)
	REGISTER_CVAR_FLOAT( recoil ,1.5)
	REGISTER_CVAR_FLOAT( chaseofs ,0.0)
	REGISTER_CVAR_FLOAT( esp_offs ,1.0)
    REGISTER_CVAR_FLOAT( soundtime ,2.00)
	REGISTER_CVAR_FLOAT( esp_trans ,1.0)
	REGISTER_CVAR_FLOAT( radar_range ,3500.0)
	REGISTER_CVAR_FLOAT( height_correction ,0.0)
	REGISTER_CVAR_FLOAT( mefrozen ,0.6)
	REGISTER_CVAR_FLOAT( fspeed ,0.25)
	REGISTER_CVAR_FLOAT( predahead , 0.05)
	//==================================================
	#define REGISTER_CVAR_INT(name,defaultvalue) cmd.AddCvarInt(#name, &##name );name=defaultvalue;
	REGISTER_CVAR_INT( wallsensitivity ,1)
	REGISTER_CVAR_INT( soundtol ,200)
	REGISTER_CVAR_INT( sounddisplay ,2)
	REGISTER_CVAR_INT( soundfilter ,2)
	REGISTER_CVAR_INT( soundradar ,1)
	REGISTER_CVAR_INT( confont ,0)
	REGISTER_CVAR_INT( name ,10) 
	REGISTER_CVAR_INT( aimthru ,0)
	REGISTER_CVAR_INT( radar ,1)
	REGISTER_CVAR_INT( spreaddraw ,0) 
	REGISTER_CVAR_INT( trans ,100)
	REGISTER_CVAR_INT( weapon ,1)
	REGISTER_CVAR_INT( flash ,1)
	REGISTER_CVAR_INT( smoke ,1)   
	REGISTER_CVAR_INT( sniper ,1)   
	REGISTER_CVAR_INT( jump ,1)
	REGISTER_CVAR_INT( duck ,0)  
	REGISTER_CVAR_INT( aim ,1)
	REGISTER_CVAR_INT( esp ,0)  
	REGISTER_CVAR_INT( mode ,0) 
	REGISTER_CVAR_INT( wall ,3)
	REGISTER_CVAR_INT( team ,0)  
	REGISTER_CVAR_INT( shoot ,1) 
	REGISTER_CVAR_INT( info ,3) 
	REGISTER_CVAR_INT( inpvs ,100)
	REGISTER_CVAR_INT( con_x ,15)  
	REGISTER_CVAR_INT( menu_x ,400)
	REGISTER_CVAR_INT( menu_y ,176)
	REGISTER_CVAR_INT( con_y ,100)
	REGISTER_CVAR_INT( menu_w ,180)
	REGISTER_CVAR_INT( con_slidespeed ,9999)
	REGISTER_CVAR_INT( avdraw ,1)  
	REGISTER_CVAR_INT( hud ,1)
	REGISTER_CVAR_INT( autojump ,1) 
	REGISTER_CVAR_INT( con_h ,300) 
	REGISTER_CVAR_INT( pistol ,1) 
	REGISTER_CVAR_INT( speedon ,9999) 
	REGISTER_CVAR_INT( speedoff ,0)  
	REGISTER_CVAR_INT( con_w ,700) 
	REGISTER_CVAR_INT( radar_x ,100) 
	REGISTER_CVAR_INT( radar_y ,200)
   	REGISTER_CVAR_INT( radar_size ,75)
	REGISTER_CVAR_INT( nosky ,1)
    REGISTER_CVAR_INT( reload ,0)
    REGISTER_CVAR_INT( alive_method ,0)
	REGISTER_CVAR_INT( spec_fix ,1)
	REGISTER_CVAR_INT( chase_cam ,0)
	REGISTER_CVAR_INT( frzn_esp ,1)
	REGISTER_CVAR_INT( vswitch ,1)
	REGISTER_CVAR_INT( nspeed ,0)
	REGISTER_CVAR_INT( autowall, 1)
	REGISTER_CVAR_INT( nospread, 1)
	REGISTER_CVAR_INT( antizoom ,0)
	REGISTER_CVAR_INT( bombtimer ,1)
	REGISTER_CVAR_INT( reloading,1)
	REGISTER_CVAR_INT( saystats,1)
	REGISTER_CVAR_INT( glextra ,0)
	REGISTER_CVAR_INT( FBIpred	,1)
	REGISTER_CVAR_INT( pred		,0)
	REGISTER_CVAR_INT( predback	,0)
	REGISTER_CVAR_INT( predtime	,0)
	REGISTER_CVAR_INT( predmax	,0)
    REGISTER_CVAR_FLOAT( chaseback   ,300.0) 
    REGISTER_CVAR_FLOAT( chaseup     ,100.0) 
    

}						
