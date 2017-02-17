#include "stdafx.h"
#include "ToolFunction.h"


//整数转字符串,为什么不用 itoa()
void APP_Tools::appIntToChar(int num,char * text)
{
	int a = 0;
	if(num < 0)
	{
		text[0] = '-';
		a = 1;
		num = abs(num);
	}
	char tmp[10];
	for(int i=0;i<10;i++)
	{
		tmp[i] = num % 10 + 48;
		num = num / 10;
	}
	for(int i=9;i>=0;i--)
	{
		if(tmp[i] != '0')
		{
			text[a] = tmp[i];
			a ++;
		}
		else
		{
			if(a != 0 && text[0] !='-')
			{
				text[a] = tmp[i];
				a ++;
			}
		}
	}
	if(a == 0)
	{
		text[a] = '0';
		a ++;
	}
	text[a] = 0;
}
//
//void APP_Tools::appIntToChar(int num,TCHAR * text)
//{
//	int a = 0;
//	if(num < 0)
//	{
//		text[0] = '-';
//		a = 1;
//		num = abs(num);
//	}
//	TCHAR tmp[10];
//	for(int i=0;i<10;i++)
//	{
//		tmp[i] = num % 10 + 48;
//		num = num / 10;
//	}
//	for(int i=9;i>=0;i--)
//	{
//		if(tmp[i] != '0')
//		{
//			text[a] = tmp[i];
//			a ++;
//		}
//		else
//		{
//			if(a != 0 && text[0] !='-')
//			{
//				text[a] = tmp[i];
//				a ++;
//			}
//		}
//	}
//	if(a == 0)
//	{
//		text[a] = '0';
//		a ++;
//	}
//	text[a] = 0;
//}


/////////////////////////////////////////////////////////////////////////////////////////
//判断某点是否在矩形区域内
bool APP_Tools::appInRect(int x,int y,RECT rect)
{
	if(x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
		return true;
	else
		return false;
}

bool APP_Tools::appInRect(int x,int y,int rectX,int rectY,int rectWidth,int rectHeight)
{
	if(x >= rectX && x <= rectX + rectWidth && y >= rectY && y <= rectY + rectHeight)
		return true;
	else
		return false;
}


////////////////////////////////////////////////////////////////////////////////////////////
//2D矩形碰撞
bool APP_Tools::appRectangleBumpCheck( RECT rect1, RECT rect2 )
{
	int width2 = rect2.right - rect2.left;
	int height2 = rect2.bottom - rect2.top;

	if( ( rect1.left - rect2.left ) < width2 &&
		( rect2.right  - rect1.right ) < width2 &&
		( rect1.top - rect2.top ) < height2 &&
		( rect2.bottom - rect1.bottom ) < height2 )
		return true;
	else
		return false;
}


bool APP_Tools::appRectangleBumpCheck( int x1, int y1, int width1, int height1,
						    int x2, int y2, int width2, int height2 )
{
	if( ( x1 - x2 ) < width2 &&
		( x2 + width2 - x1 - width1 ) < width2 &&
		( y1 - y2 ) < height2 &&
		( y2 + height2 - y1 - height1 ) < height2 )
		return true;
	else
		return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////
//向量*变换矩阵
void APP_Tools::appVec3Transform( D3DXVECTOR3 & vPoint, D3DXMATRIX matTrans )
{
	D3DXVECTOR3 vTmp = vPoint;
	vPoint.x = vTmp.x * matTrans.m[0][0] + vTmp.y * matTrans.m[1][0] + vTmp.z * matTrans.m[2][0] + matTrans.m[3][0];
	vPoint.y = vTmp.x * matTrans.m[0][1] + vTmp.y * matTrans.m[1][1] + vTmp.z * matTrans.m[2][1] + matTrans.m[3][1];
	vPoint.z = vTmp.x * matTrans.m[0][2] + vTmp.y * matTrans.m[1][2] + vTmp.z * matTrans.m[2][2] + matTrans.m[3][2];
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////

double APP_Tools::appGetTime()
{
	return GetTickCount() / 1000.f;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3D射线检测
bool APP_Tools::appRayPlaneCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 & vOut )
{
	
	/*D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &v0, &v1, &v2 );

	D3DXVECTOR3 vTmp;
	if( !D3DXPlaneIntersectLine( &vTmp, &plane, &vPos1, &vPos2 ) )
		return false;

	float a = D3DXVec3Length( &( v0 - v1 ) );
	float b = D3DXVec3Length( &( v0 - v2 ) );
	float c = D3DXVec3Length( &( v1 - v2 ) );

	float p = ( a + b + c ) / 2;
	float s = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v0 - vTmp ) );
	b = D3DXVec3Length( &( v1 - vTmp ) );
	c = D3DXVec3Length( &( v0 - v1 ) );
	p = ( a + b + c ) / 2;
	float s1 = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v0 - vTmp ) );
	b = D3DXVec3Length( &( v2 - vTmp ) );
	c = D3DXVec3Length( &( v0 - v2 ) );
	p = ( a + b + c ) / 2;
	float s2 = p * ( p - a ) * ( p - b ) * ( p - c );

	a = D3DXVec3Length( &( v1 - vTmp ) );
	b = D3DXVec3Length( &( v2 - vTmp ) );
	c = D3DXVec3Length( &( v1 - v2 ) );
	p = ( a + b + c ) / 2;
	float s3 = p * ( p - a ) * ( p - b ) * ( p - c );

	s = sqrt( s ) - sqrt( s1 ) - sqrt( s2 ) - sqrt( s3 );
	if( s > -0.1f )
	{
		vOut = vTmp;
		return true;
	}
	else
	{
		return false;
	}*/
	
	D3DXVECTOR3 vDir = vPos2 - vPos1;

    // 算出两个边的向量
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &vDir, &edge2 );

    // 如果det为0，或接近于零则射线与三角面共面或平行，不相交
//此处det就相当于上面的，
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = vPos1 - v0;
    }
    else
    {
        tvec = v0 - vPos1;
        det = -det;
    }
    if( det < 0.0001f )
        return FALSE;
    // 计算uv并测试是否合法（在三角形内）
	float u,v,t;
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det )
        return FALSE;
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );
    v = D3DXVec3Dot( &vDir, &qvec );
    if( v < 0.0f || v + u > det )
        return FALSE;

    t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    t *= fInvDet;
    u *= fInvDet;
    v *= fInvDet;
	D3DXPLANE plane;
	D3DXPlaneFromPoints( &plane, &v0, &v1, &v2 );
	D3DXVec3Normalize( &vDir, &vDir );
	D3DXVECTOR3 vTmp;
	D3DXPlaneIntersectLine( &vTmp, &plane, &vPos1, &vPos2 );
	if( ( vTmp.y >= vPos1.y && vTmp.y <= vPos2.y ) || ( vTmp.y <= vPos1.y && vTmp.y >= vPos2.y ) )
		vOut = vTmp;
	else
		return false;
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//求法线
void APP_Tools::ComputeTriNormal( D3DXVECTOR3 * v1, D3DXVECTOR3 * v2, D3DXVECTOR3 * v3,D3DVECTOR & normalVec )
{
	D3DXVECTOR3 tmpV1 = *v1 - *v2;
	D3DXVECTOR3 tmpV2 = *v1 - *v3;
	D3DXVECTOR3 tmpNormalVec;
	D3DXVec3Cross( &tmpNormalVec, &tmpV1, &tmpV2 );
	D3DXVec3Normalize( &tmpNormalVec, &tmpNormalVec );
	normalVec = tmpNormalVec;
}

