--诛仙
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
SHOW_MESH  = 1;
SHOW_WINDOW = 2;

-----------------------------------------------
--主函数
function Run()
   --------------------------------------
   --获取系统状态
   State[1], State[2], State[3], State[4], State[5], State[6] = CGetState();
   
   --------------------------------
   --逻辑处理
   	--		   显示类型   mesh_id     x	   	  y	  	 z    s_xyz  r_x    r_y  r_z  tex_id
	CSetState( SHOW_MESH,    4, 	105, 	163.8, 	86.2, 0.65,  -1.57,  0,   0,	-1);
	CSetState( SHOW_MESH,    5, 	110, 	163.2, 	86.2, 0.004,   0,    0,   0,	-1);
	--CSetState( SHOW_MESH,  4, 	105000, 165000, 86200, 1000,  -1570,  0,   0,	-1);
  	--CSetState( SHOW_MESH,  4, 	110000, 165000, 86200, 1000,  -1570,  0,   0,	-1);
    bill_board.Draw();

end

------------------------------
--定时器
timer = 0;

function MyTimer()
   timer = timer + 1;
   
   if timer > 60 then
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
	
	x    	= 113,
	y       = 163.8,
	z       = 86.2,
	
	s_xyz   = 0.65,

	r_x     = -1.57,
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

