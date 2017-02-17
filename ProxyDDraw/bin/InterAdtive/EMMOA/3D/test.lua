--神泣 
--test.lua
 --全局状态列表
State = {}; --系统状态表
State[7] = 1;
State[8] = 0;
------------------------------
--C函数
--CGetState(): 获取状态函数



--CSetState(): 设置状态函数
--参数表说明
-- 1、操作类型，整数值，根据不同的值进行操作处理，如添加模型信息
-- 2-...、操作类型的参数，根据具体而定


---------------------------------------------
--状态标志
CLEAR_SELECT_ID = 0;
SHOW_MESH   = 1;
SHOW_TEXT   = 2;
OPEN_URL	= 3;
SHOW_DLG	= 4;


-----------------------------------------------
--主函数
function Run()
  
   --------------------------------
   --先绘制模型 
   	--		   显示类型   mesh_id     x		  	 y	  		  z    		s_xyz  	r_x    	r_y  r_z  tex_id
--	金牌(173,127)
	CSetState( SHOW_MESH,    6, 	1644.16, 	47.5, 		1775.4, 	0.0600,  	0,  	1.570,   0,	-1);
    
--	奖牌 
	CSetState( SHOW_MESH,    7, 	1617.27, 	49.44, 	    1697.25, 	0.060,  	0,  	1.570,   0,	-1);
-- 	
-- 	可乐罐 
	CSetState( SHOW_MESH,    8, 	1630.47, 	47.2, 	    1721.71, 	0.020,  	0,  	1.570,   0,	-1);
--	
--	汽车 
	CSetState( SHOW_MESH,    9, 	1685.02, 	38.1, 		1784.9, 	0.010,  	0,  	0,   	 0,	-1);
--                                                        
--  福娃晶晶                                              
    CSetState( SHOW_MESH,    10, 	1696.37, 	37.0, 		1792.7,		0.0005,  	0,  	0,   	 0,	-1);
--                                                        
--	企鹅                                                   
	CSetState( SHOW_MESH,    11, 	1597.81, 	56.5, 		1714.66,	0.0005,  	0,  	-1.570,   0,	-1);

	CSetState( SHOW_MESH,    13, 	1590.0, 	48.25, 		1673.40,	0.05,  	   1.570,    0,       0,	-1);

--	广告牌 
    bill_board.Draw();

--
-- 打印机
    Printer.Draw();
    
   --------------------------------------
   --获取系统状态
   State[1] = nil;
   State[1], State[2], State[3], State[4], State[5], State[6] = CGetState();
   
   if State[1] == 0 then 
   		if State[2] == 12 then
   			CSetState( SHOW_DLG );
        	CSetState( SHOW_TEXT,  "参加炫飞打印行动？" ,40, 450 );
--        	CSetState( OPEN_URL,  "http://www.canon.com.cn/products/printer/selphy/index.html" );
--		    CSetState( CLEAR_SELECT_ID );
  		elseif State[2] == 9 then
  			CSetState( SHOW_DLG );
		    CSetState( SHOW_TEXT,  "IGA3D 拾取到了：汽车" , 40, 450 );
		elseif State[2] == 7 then
   			CSetState( SHOW_DLG );
		    CSetState( SHOW_TEXT,  "IGA3D 拾取到了：奖牌" , 40, 450 );  
		elseif State[2] == 8 then
   			CSetState( SHOW_DLG );
		    CSetState( SHOW_TEXT,  "百事可乐：奥运大惊喜！" ,40, 450 );
--		    CSetState( 3,  "http://www.pepsi.cn/" );
--		    CSetState( CLEAR_SELECT_ID );
		elseif State[2] == 13 then
   			CSetState( SHOW_DLG );
		    CSetState( SHOW_TEXT,  "参加炫飞打印行动？" ,40, 450 );
		else 
		    CSetState( CLEAR_SELECT_ID );
		end 
   end
   
   
--   if MyTimer() then
--   	    CSetState( CLEAR_SELECT_ID );
--   	    CSetState( SHOW_MESH,    12, 	1581.0, 	59.5, 	1716.03, 	0.02,  	0,  0,   0,	-1);
--   end
	
   
   

end

------------------------------
--定时器
timer = 0;

function MyTimer()
   timer = timer + 1;

   if timer > 50 then
	  timer = 0
	  return true;
   else
	  return false;
   end
end


bill_board  =
{
	mesh_id  	= 4,   --使用的网格id
	tex_id      = 0,
	max_tex_id  = 9,

	x    	= 1609.94,
	y       = 57.8,
	z       = 1769.88,

	s_xyz   = 1.100,

	r_x     = 0,
	r_y     = 0,
	r_z     = 0,

	-----------------------------
	--方法
	Draw    = function()
			   --CSetState( SHOW_MESH,    4, 	112000, 163800, 86200, 650,  -1570,  0,   0,	-1);
			   bill_board.AI();
	           CSetState( SHOW_MESH, bill_board.mesh_id, bill_board.x, bill_board.y, bill_board.z, bill_board.s_xyz, bill_board.r_x, bill_board.r_y, bill_board.r_z, bill_board.tex_id );
			   end,

	---------------------------
	--AI
	AI      = function()
	            b_change = MyTimer();
	            if b_change == true then
                   bill_board.tex_id = bill_board.tex_id + 1;
				end

				if bill_board.tex_id > bill_board.max_tex_id then
				   bill_board.tex_id = -1;
				end

			  end
}


--闪动的打印机
PTimer = 0;
function PTimerFun()
    PTimer = PTimer + 1;
    if PTimer > 10 then
		PTimer = 0;
		return true;
	else
		return false;
	end
end


Printer =
{
    mesh_id  	= 12,   --使用的网格id
	tex_id      = 10,
    min_tex_id  = 10,
	max_tex_id  = 19,

	x    	= 1582.57,
	y       = 59.07,
	z       = 1716.03,

	s_xyz   = 0.02,

	r_x     = 0,
	r_y     = 0,
	r_z     = 0,

	-----------------------------
	--方法
	Draw    = function()
			   Printer.AI();
	           CSetState( SHOW_MESH, Printer.mesh_id, Printer.x, Printer.y, Printer.z, Printer.s_xyz, Printer.r_x, Printer.r_y, Printer.r_z, Printer.tex_id );
			   end,

    ---------------------------
	--AI
	AI      = function()
				 local b = PTimerFun();
				 if b == true then
                    Printer.tex_id = Printer.tex_id + 1;
				 end
				 
				 if Printer.tex_id > Printer.max_tex_id then
                    Printer.tex_id = Printer. min_tex_id;
				 end
 			  end
}


--------------------------------------------------
--系统配置信息
DP =
{
	Per_SetTransform	= 0;
	Post_SetTransform	= 1;
	Per_Present			= 2;
	Post_Present		= 3;
}

DrawPos	= DP.Per_Present;