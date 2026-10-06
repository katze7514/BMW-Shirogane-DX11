#include "stdafx.h"

#include "CDataCharaInter.h"

namespace BMW{
namespace Chara{

void CDataCharaInter::addItem(int nID,int nAttr, Item::CItemDB& db)
{
	// アイテムの効果を適用する
	if(db.IsStatus(nID))
		db.applyStatus(*this,nAttr,nID);

	addItem(nID,nAttr);
}

bool CDataCharaInter::delItem(int nAttr, Item::CItemDB& db)
{
	item_list& listItem_ = pTrain_->getItemList();
	int nID=-1;
	item_list::iterator it;
	for(it=listItem_.begin(); it!=listItem_.end(); ++it)
	{
		if(it->getAttr()==nAttr)
		{
			nID = it->getID();
			listItem_.erase(it);
			break;
		}
	}
	
	if(nID>=0)
	{
		// アイテムの効果を戻す
		if(db.IsStatus(nID))
			db.backStatus(*this,nAttr,nID);

		return true;
	}

	return false;
}

bool CDataCharaInter::delItemID(int nID, Item::CItemDB& db)
{
	item_list& listItem_ = pTrain_->getItemList();
	int nAttr=-1;
	item_list::iterator it;
	for(it=listItem_.begin(); it!=listItem_.end(); ++it)
	{
		if(it->getID()==nID)
		{
			nAttr = it->getAttr();
			listItem_.erase(it);
			break;
		}
	}
	if(nAttr>=0)
	{
		// アイテムの効果を戻す
		if(db.IsStatus(nID))
			db.backStatus(*this,nAttr,nID);

		return true;
	}

	return false;
}

} // namespace Chara end
} // namespace BMW end