#pragma once

typedef unsigned char UCHAR;

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};


//加载数据
bool InitDrawing();
//释放数据
bool UnInitDrawing();

//在lock 和unlock之间调用，用于绘制从缓冲区
bool Draw_Video();

bool Draw_Flash();

bool ZoomBmpBuffer(UCHAR * pInData, int nSrcWidth, int nSrcHeigth,int bpp, UCHAR * pOutData, int nDstWidth, int nDstHeight);

bool ShowDialog();


