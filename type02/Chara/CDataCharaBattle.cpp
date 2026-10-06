#include "stdafx.h"

#include "CDataCharaBattle.h"

namespace BMW{
namespace Chara{

void CDataCharaBattle::Serialize(ISerialize& s)
{// 差分をシリアライズ
	// 基本的なデータ
	s << nID_ << nLv_ << nExp_  << nKill_ << nMental_ << fundOffset_ << battleOffset_ 
		<< spiritValid_ << condValid_ << nCure_ << nRefill_ << nAttack_ << nDefence_;

	int nSize;
	if(s.IsStoring())
	{// Save
		// 保持アイテム
		// サイズをsave
		nSize = (int)listItem_.size();
		s << nSize;
		item_list::iterator it_i;
		for(it_i=listItem_.begin(); it_i!=listItem_.end(); ++it_i)
			s << *it_i;

		// 武器
		nSize = (int)listWeapon_.size();
		s << nSize;
		weapon_list::iterator it_w;
		for(it_w=listWeapon_.begin(); it_w!=listWeapon_.end(); ++it_w)
			s << *it_w;
	}
	else
	{// Load
		// 保持アイテム
		// リストをクリア
		CStatusAbility ability;
		listItem_.clear();
		// サイズを取得
		s << nSize;
		for(int i=0; i<nSize; i++)
		{
			s << ability;
			addItem(ability);
		}

		// 武器
		listWeapon_.clear();
		s << nSize;
		int nID;
		for(int i=0; i<nSize; i++)
		{
			s << nID;
			listWeapon_.push_back(nID);
		}
	}
}

int CDataCharaBattle::hasSkill(int nID)const
{// あったらAttrを返す。無い時は-1を返す
	int	nAttr=-1;
	skill_list::iterator it;
	beginSkill();
	while(!endSkill())
	{
		it=nextSkill();
		if(it->getID()==nID)
		{	// 底力などの養成した技能分も足し合わせるためにこうなる
			nAttr<0 ? nAttr=it->getAttr() : nAttr+=it->getAttr();
		}
	}
	return nAttr;
}

bool CDataCharaBattle::IsHasSpirit(int nSpirit)const
{
	for(int i=0; i<6; ++i)
		if(getSpirit(i).getID()==nSpirit) return true;

	return false;
}

int CDataCharaBattle::hasSpirit(int nSpirit)const
{
	for(int i=0; i<6; ++i)
		if(getSpirit(i).getID()==nSpirit) return getSpirit(i).getAttr();

	return -1;
}

bool CDataCharaBattle::IsHasItem(int nItem)const
{
	beginItem();
	while(!endItem())
		if(nextItem()->getID()==nItem) return true;

	return false;
}

int CDataCharaBattle::countHasItem(int nItem)const
{
	int n=0;
	beginItem();
	while(!endItem())
		if(nextItem()->getID()==nItem) ++n;

	return n;
}


} // namespace Chara end
} // namespace BMW end