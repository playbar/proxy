#ifndef _IGA_COMMON_H_
#define _IGA_COMMON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IGACommon.h : header file
//

typedef char  int8;
typedef short int16;
typedef int   int32;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

// 写只读的内存
void WriteMemory(LPCVOID lpAddress, LPCVOID lpData, uint32 nBytes);

// 虚函数表操作

// 转换含有虚函数表的对象，到PIGAVOBJECT类型
#define CPVO2PIVO(pAnyObj) ((PIGAVOBJECT)(LPVOID)(pAnyObj))

// 指向虚函数表的指针
typedef uint32* IGAVFT;
// 指向任意对象的指针
typedef void* IGAANYOBJECT;
// 含有虚函数表的结构体
typedef struct tagIGAVOBJECT {
	IGAVFT vft;
} IGAVOBJECT, *PIGAVOBJECT;

// 获得对象的虚函数个数
int32 GetVFNumber(PIGAVOBJECT pVObject);

// 替换虚函数表
// 将 pObject 的虚函数表，拷贝到 pVFT 中。
// iVftSize：pVFT 中可以存放的函数指针的个数
int32 CopyVFT(IGAVFT pVFT, int32 iVFTSize, PIGAVOBJECT pVObject);

// 名称： VOID ReplaceVFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
int32 ReplaceVFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject);

// 名称： VOID ReplaceN_VFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject, int32 iVFTSize);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
//	       iVFTSize 替换的虚函数个数
int32 ReplaceN_VFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject, int32 iVFTSize);

#endif // #ifndef _IGA_COMMON_H_