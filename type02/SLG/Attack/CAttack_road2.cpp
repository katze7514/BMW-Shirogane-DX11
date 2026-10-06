#include "stdafx.h"

#include "../../Ability/Ability/CAbility_Magician.h"
#include "../../Spirit/Spirit/CSpirit_Snipe.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/COffsetWeapon.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CAttack_range2.h"
#include "CAttack_road2.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_road2::OnReset(Task::CTaskContext* pContext)
{
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
}

void CAttack_road2::OnInit(Task::CTaskContext* pContext)
{
	// 移動範囲のクリア
	p->clearMove();

	// 不変値の設定
	actionAbility(p->getCtrlCharaData());
}

void CAttack_road2::OnAction(Task::CTaskContext* pContext)
{
	// 計算
	calcAttack(p->getMapChip(p->getCtrlCharaData()->getIndex()),0,0,0,Way::NO);

	// 計算が終わったらリターン
	//getTaskListCtrl()->returnTaskList();
}

void CAttack_road2::actionAbility(CDataCharaSLG* pChara)
{
	// 保持してる武器の射程とかいろいろゲット
	nHeight_ = p->getMapChip(pChara->getIndex())->getMapInfo().getHeight();
	nPhase_=pChara->getPhase();
	nIndex_=pChara->getIndex();

	COffsetRange range;
	nMax_=0;
	nMin_=INT_MAX;
	nReach_=0;
	Weapon::CDataWeaponBattle* pWeapon;
	Chara::CDataCharaBattle& battle = pChara->getBattle();
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = p->getWeaponData(*battle.nextWeapon());

		// F武器は除く
		if(pWeapon->IsF()) continue;

		if(pChara->getState().getAct()!=Act::BEFORE)
		{// 移動後だったらP武器じゃないとあかん
			if(!(pWeapon->IsP() 
			|| (pChara->getBattle().IsSpirit(Chara::CValidSpirit::CHARGE))
				))
				continue;
		}

		// 射程補正値計算
		CAttack_range2::calcRange(pChara,pWeapon,p,pWeapon->getRange());

		if(nMax_ < pWeapon->getMax()+range.getMax())
			nMax_=pWeapon->getMax()+range.getMax();

		if(nMin_ > pWeapon->getMin())
			nMin_=pWeapon->getMin();

		if(nReach_ < pWeapon->getHeight()+range.getReach())
			nReach_=pWeapon->getHeight()+range.getReach();
	}
}

void CAttack_road2::calcAttack(Map::CMapChip* pMap, int nAttack, int nOff, int nDist, int nToward)
{
	// マップが存在しなくても終了
	if(pMap==NULL) return;

	// 次の一歩
	int nNextAttack = nAttack+1+nOff;

	// 最大射程を超えたら終了
	if(nNextAttack>nMax_) return;

	Map::CMapChip* pOn;
	// 上へいけるか
	if(nToward!=Way::TOP
	&& getIndex()!=pMap->getMapInfo().getOnMap(Way::TOP))
	{
		pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::TOP));
		switch(IsAttack(pOn))
		{
		case ENABLE:
			setIndex(pOn, nNextAttack, nDist+1);
		case THROUGH:
			calcAttack(pOn, nNextAttack, 0, nDist+1, Way::BOTTOM);
		break;

		case ENABLE_CHARA:
			setIndex(pOn, nNextAttack, nDist+1);
			calcAttack(pOn, nNextAttack, 1, nDist+1, Way::BOTTOM);
		break;

		default: break;
		}
	}

	// 左へいけるか
	if(nToward!=Way::LEFT
	&& getIndex()!=pMap->getMapInfo().getOnMap(Way::LEFT))
	{
		pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::LEFT));
		switch(IsAttack(pOn))
		{
		case ENABLE:
			setIndex(pOn, nNextAttack, nDist+1);
		case THROUGH:
			calcAttack(pOn, nNextAttack, 0, nDist+1, Way::RIGHT);
		break;

		case ENABLE_CHARA:
			setIndex(pOn, nNextAttack, nDist+1);
			calcAttack(pOn, nNextAttack, 1, nDist+1, Way::RIGHT);
		break;

		default: break;
		}
	}

	// 下へいけるか
	if(nToward!=Way::BOTTOM
	&& getIndex()!=pMap->getMapInfo().getOnMap(Way::BOTTOM))
	{
		pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::BOTTOM));
		switch(IsAttack(pOn))
		{
		case ENABLE:
			setIndex(pOn, nNextAttack, nDist+1);
		case THROUGH:
			calcAttack(pOn, nNextAttack, 0, nDist+1, Way::TOP);
		break;

		case ENABLE_CHARA:
			setIndex(pOn, nNextAttack, nDist+1);
			calcAttack(pOn, nNextAttack, 1, nDist+1, Way::TOP);
		break;

		default: break;
		}
	}

	// 右へいけるか
	if(nToward!=Way::RIGHT
	&& getIndex()!=pMap->getMapInfo().getOnMap(Way::RIGHT))
	{
		pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::RIGHT));
		switch(IsAttack(pOn))
		{
		case ENABLE:
			setIndex(pOn, nNextAttack, nDist+1);
		case THROUGH:
			calcAttack(pOn, nNextAttack, 0, nDist+1, Way::LEFT);
		break;

		case ENABLE_CHARA:
			setIndex(pOn, nNextAttack, nDist+1);
			calcAttack(pOn, nNextAttack, 1, nDist+1, Way::LEFT);
		break;
			
		default: break;
		}
	}
}

int CAttack_road2::IsAttack(Map::CMapChip* pMap)
{
	if(pMap==NULL) return NOTENABLE;
	// いるマップの攻撃検索マップの高さ差が、到達度以上かどうか
	int nHeight = pMap->getMapInfo().getHeight() - nHeight_;
	if(abs(nHeight) > nReach_)
	{// 山なりの時は、それ以上攻撃できない
	 // ただ、谷の時は、そこは攻撃できなだけ
		return nHeight>0 ? NOTENABLE : THROUGH;
	}
	// そのマップにだれかいるか
	// そのマップにだれかいるか
	ITaskBase* pBase = pMap->getTask(Map::CMapChip::CHARA);
	if(pBase==NULL)
	{// いないなら次へ
		return ENABLE;
	}
	else
	{// いるなら、仲間かどうかチェック
		// 仲間だったらスルー、そうじゃなかったらオフセットが付く
		return getPhase() == (p->getCharaData(static_cast<Map::CMapChipChara2*>(pBase)->getID()))->getPhase()
			   ? ENABLE : ENABLE_CHARA;
	}
}

void CAttack_road2::setIndex(Map::CMapChip* pMap, int nAttack, int nDist)
{
	Map::CMapChipState* pState = pMap->getMapChipState();
	int nMapMove = pState->getMove();
	if(nMapMove<0 || nMapMove > nAttack)
	{
		p->getIndexSet().insert(pMap->getIndex());
		pState->setMove(nAttack);
	}
}

} // namespace Attack end
} // namespace SLG end
} // mamespace BMW end