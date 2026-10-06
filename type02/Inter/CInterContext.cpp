#include "stdafx.h"

#include "../Scene/Unit/CSortUnit.h"

#include "CInterChara.h"
#include "CDataItemInter.h"
#include "CInterContext.h"

#include "InterSort.h"

namespace BMW{
namespace Inter{

CInterContext::CInterContext()
{
	clearFlag();
}

CInterContext::~CInterContext()
{
	// キャラ
	clearCharaData();
	// アイテム
	clearItemData();
}

void CInterContext::clearFlag()
{
	setValue(-1,Flag::TARGET_CHARA);
	setValue(-1,Flag::EXCHANGE_CHARA);
	setValue(-1,Flag::TARGET_ITEM);
	setValue(-1,Flag::CTRL_ITEM);
	setValue(-1,Flag::TARGET_ABILITY);
	setValue(-1,Flag::CTRL_ABILITY);
	setValue(0,Flag::INITIALIZE);
	setValue(0,Flag::EXCHANGE);
	setValue(0,Flag::LOAD);
}

void CInterContext::sortChara(int nKey, int nOrder)
{// フラグに合わせてソートする
	if(nOrder==Unit::CSortUnit::UP)
	{// 昇順
		switch(nKey)
		{
		case Unit::CSortUnit::LV:	listChara_.sort(sort_LvUp(mapChara_));		break;
		case Unit::CSortUnit::HP:	listChara_.sort(sort_HpUp(mapChara_));		break;
		case Unit::CSortUnit::EN:	listChara_.sort(sort_EnUp(mapChara_));		break;
		case Unit::CSortUnit::SP:	listChara_.sort(sort_SpUp(mapChara_));		break;
		case Unit::CSortUnit::NEXT: listChara_.sort(sort_NextUp(mapChara_));	break;
		case Unit::CSortUnit::KI:	listChara_.sort(sort_MentalUp(mapChara_));	break;
		default:					listChara_.sort(sort_IdUp(mapChara_));		break;
		}
	}
	else
	{//	降順
		switch(nKey)
		{
		case Unit::CSortUnit::LV:	listChara_.sort(sort_LvDown(mapChara_));	break;
		case Unit::CSortUnit::HP:	listChara_.sort(sort_HpDown(mapChara_));	break;
		case Unit::CSortUnit::EN:	listChara_.sort(sort_EnDown(mapChara_));	break;
		case Unit::CSortUnit::SP:	listChara_.sort(sort_SpDown(mapChara_));	break;
		case Unit::CSortUnit::NEXT: listChara_.sort(sort_NextDown(mapChara_));	break;
		case Unit::CSortUnit::KI:	listChara_.sort(sort_MentalDown(mapChara_));break;
		default:					listChara_.sort(sort_IdDown(mapChara_));	break;
		}
	}
}

void CInterContext::setChara()
{
	int nTragetID=getTargetChara();
	for(it=listChara_.begin(); it!=listChara_.end(); it++)
		if(nTragetID==*it) break;
}

void CInterContext::setCharaData(int nID, CInterChara* pData)
{
	mapChara_.insert(pair<int, CInterChara*>(nID,pData));
}

void CInterContext::clearCharaData()
{
	chara_map::iterator it;
	for(it=mapChara_.begin(); it!=mapChara_.end(); it++)
		DELETE_SAFE(it->second);

	mapChara_.clear();
}

CDataItemInter* CInterContext::getItemData(int nID)
{
	item_map::iterator it = mapItem_.find(nID);
	if(it==mapItem_.end()) return NULL;
	return it->second;
}

void CInterContext::setItemData(int nID, CDataItemInter* pData)
{
	mapItem_.insert(pair<int, CDataItemInter*>(nID,pData));
}

void CInterContext::clearItemData()
{
	item_map::iterator it_i;
	for(it_i=mapItem_.begin(); it_i!=mapItem_.end(); it_i++)
		DELETE_SAFE(it_i->second);

	mapItem_.clear();
}

} // namespace Inter end
} // namespace BMW end