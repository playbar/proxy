// IGACommon.cpp : source file
//

#include "stdafx.h"
#include "IGACommon.h"

// 写只读的内存
void WriteMemory(LPCVOID lpAddress, LPCVOID lpData, uint32 nBytes)
{
	MEMORY_BASIC_INFORMATION mbi;
	::VirtualQuery(lpAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));  
	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	::CopyMemory(const_cast<void*>(lpAddress), lpData, nBytes);
	
	DWORD dwOldProtect;
	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
}

// 获得对象的虚函数个数
int32 GetVFNumber(PIGAVOBJECT pVObject)
{
    int32 i;
    IGAVFT pVft;

	pVft = pVObject->vft;
	if (pVft == NULL)
		return 0;

	i = 0;
	while (1) {
        if (NULL == pVft[i]) {
			return i;
        }
		i++;
    }
	return i;
}

// 替换虚函数表
// 将 pObject 的虚函数表，拷贝到 pVFT 中。
// iVftSize：pVFT 中可以存放的函数指针的个数
int32 CopyVFT(IGAVFT pVFT, int32 iVFTSize, PIGAVOBJECT pVObject)
{
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	int32 iVFNum = GetVFNumber(pVObject);
	iVFNum = min(iVFNum, iVFTSize);
	for (int32 i = 0; i < iVFNum; i++) {
		pVFT[i] = pVObject->vft[i];
	}
	return iVFNum;
}

// 名称： VOID ReplaceVFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
int32 ReplaceVFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject)
{
	if (pTargetVObject == NULL || pTargetVObject->vft == NULL)
		return 0;
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	INT nVfNumber;
	INT nTvfNumber = GetVFNumber(pTargetVObject);
	INT nRvfNumber = GetVFNumber(pVObject);

	nVfNumber = min(nTvfNumber, nRvfNumber);
	for (int i = 0; i < nVfNumber; i++) {
		WriteMemory(&pTargetVObject->vft[i], &pVObject->vft[i], sizeof(uint32));
	}
	return nVfNumber;
}

// 名称： VOID ReplaceN_VFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject, int32 iVFTSize);
// 功能： 替换虚函数表
// 描述： 将 pTargetObject 的虚函数表，替换为 pObject 的虚函数表。
//	       iVFTSize 替换的虚函数个数
int32 ReplaceN_VFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject, int32 iVFTSize)
{
	if (pTargetVObject == NULL || pTargetVObject->vft == NULL)
		return 0;
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	INT nVfNumber;
	INT nTvfNumber = GetVFNumber(pTargetVObject);
	INT nRvfNumber = iVFTSize;

	nVfNumber = min(nTvfNumber, nRvfNumber);
	for (int i = 0; i < nVfNumber; i++) {
		WriteMemory(&pTargetVObject->vft[i], &pVObject->vft[i], sizeof(uint32));
	}
	return nVfNumber;
}