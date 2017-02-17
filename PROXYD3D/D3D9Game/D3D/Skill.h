#pragma once

#include "BillBoard.h"
#include "base/Texture.h"
#include "../Player.h"


class CSkill
{
public:
	CSkill();
	~CSkill();
	DWORD  m_dwKey;
	CBillBoard *      m_pBillBoard;
	bool SetSkill(CPlayer * pPlayer,CPlayer * pMonster,float fX,float fY,float fZ,float fDis);
protected:
private:

};

class CSkill1 : public CSkill
{
public:
	CSkill1(float fWidth,float fHeight,float fFrame,CPlayer * pPlayer,CPlayer * pMonster)
	{
		InitSkill(fWidth,fHeight,fFrame);
		SetSkill(pPlayer,pMonster,0.f,2.f,0.f,100.f);
	}
	~CSkill1()
	{
		CSkill::~CSkill();
	}
	void InitSkill(float fWidth,float fHeight,float fFrame);
	
protected:
private:
};
class CSkill2 : public CSkill
{
public:
	CSkill2(float fWidth,float fHeight,float fFrame,CPlayer * pPlayer,CPlayer * pMonster)
	{
		InitSkill(fWidth,fHeight,fFrame);
		SetSkill(pPlayer,pPlayer,0.f,2.f,0.f,0.f);
	}
	~CSkill2()
	{
		CSkill::~CSkill();
	}
	void InitSkill(float fWidth,float fHeight,float fFrame);
protected:
private:
};