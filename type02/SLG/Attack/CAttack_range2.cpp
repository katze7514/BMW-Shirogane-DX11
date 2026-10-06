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

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_range2::OnReset(Task::CTaskContext* pContext)
{
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
}

void CAttack_range2::OnInit(Task::CTaskContext* pContext)
{
	pFieldWeapon_=NULL;
	// 攻撃範囲のクリア
	p->clearRange();

	// 不変値の設定
	actionAbility(p->getCtrlCharaData());
}

void CAttack_range2::OnAction(Task::CTaskContext* pContext)
{
	Map::CMapChip* pMapChip = p->getMapChip(p->getCtrlCharaData()->getIndex());
	// 計算
	calcAttack(pMapChip,0,0,0,Way::NO);

	// フィールドライン武器があったら、さらに範囲追加
	if(pFieldWeapon_!=NULL) calcField(pMapChip);

	// 計算が終わったらリターン
	getTaskListCtrl()->returnTaskList();
}

void CAttack_range2::actionAbility(CDataCharaSLG* pChara, bool bP)
{
	// 保持してる武器の射程とかいろいろゲット
	nIndex_=pChara->getIndex();
	nHeight_ = p->getMapChip(nIndex_)->getMapInfo().getHeight();
	nPhase_=pChara->getPhase();

	int nFieldMax=0;
	nMax_=0;
	nMin_=INT_MAX;
	nReach_=0;
	Weapon::CDataWeaponBattle* pWeapon;
	Chara::CDataCharaBattle& battle = pChara->getBattle();
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		int nWeapon=*battle.nextWeapon();
		pWeapon = p->getWeaponData(nWeapon);
	#ifdef BMW_DEBUG
		CDbg().Out("Weapon %d %d",nWeapon,pWeapon);
	#endif
	
		if(!pWeapon->IsF())
		{// フィール属性ではない通常の武器
		 // か、LINEタイプじゃないのフィールド武器
			if(bP && pChara->getState().getAct()!=Act::BEFORE)
			{// 移動後だったらP武器じゃないとあかん
				if(!(pWeapon->IsP() 
					|| pChara->getBattle().IsSpirit(Chara::CValidSpirit::CHARGE)
					)
				  ) continue;
			}

			// 射程補正値計算
			calcRange(pChara,pWeapon,p,pWeapon->getRange());

			if(nMax_ < pWeapon->getMax())
				nMax_=pWeapon->getMax();

			if(nMin_ > pWeapon->getMin())
				nMin_=pWeapon->getMin();

			if(nReach_ < pWeapon->getHeight())
				nReach_=pWeapon->getHeight();
		}
		else
		{// フィールド武器の場合は最大射程と到達度は確保
			//CDbg().Out("Field!!");
			nFieldMax=pWeapon->getMax() + (pWeapon->getField()==Weapon::Field::THROW ? pWeapon->getFieldSize() : 0 );
			// フィールドライン武器を設定
			pFieldWeapon_=pWeapon;
		}
	}
	// 範囲vector生成
	p->resizeRange(std::max(nMax_,nFieldMax));
}

void CAttack_range2::calcAttack(Map::CMapChip* pMap, int nAttack, int nOff, int nDist, int nToward)
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

int CAttack_range2::IsAttack(Map::CMapChip* pMap)
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
		CDataCharaSLG* pChara = p->getCharaData(static_cast<Map::CMapChipChara2*>(pBase)->getID());
		return (pChara->IsLive() && getPhase() == pChara->getPhase())
			   ? ENABLE : ENABLE_CHARA;
	}
}

void CAttack_range2::setIndex(Map::CMapChip* pMap, int nAttack, int nDist)
{
	Map::CMapChipState* pState = pMap->getMapChipState();
	// 距離
	int nMapDist = pState->getRealDist();
	if(nMapDist<0 || nMapDist>nDist)
		pState->setRealDist(nDist);

	int nMapAttack = pState->getAttack();
	//if(nAttack >= pState->getRealDist() // 実距離以上
	//&& nAttack >= nMin_)				// 最小射程以上
	//{// なら攻撃可
		if(nMapAttack<0 || nMapAttack > nAttack)
		{
			// 値更新前に前の値を削除
			if(nMapAttack>0) p->delRange(nMapAttack-1, pMap->getIndex());

			// attack更新
			pState->setAttack(nAttack);
			pState->setAtkHeight(abs(nHeight_-pMap->getMapInfo().getHeight()));
			Map::CMapChipChara2* pChip = static_cast<Map::CMapChipChara2*>(pMap->getTask(Map::CMapChip::CHARA));
			if(pChip!=NULL)
				p->addRange(nAttack-1, pChip->getID(), pMap->getIndex());
			else
				p->addRange(nAttack-1, -1, pMap->getIndex());
		}
	//}
	//else
	//{// 最小射程以下の証
	//	if(nMapAttack>0) p->delRange(nMapAttack-1, pMap->getIndex());
	//	pState->setAttack(-2);
	//	p->addRange(nAttack-1, -1, pMap->getIndex());
	//}
}

void CAttack_range2::calcRange(CDataCharaSLG* pChara, Weapon::CDataWeaponBattle* pWeapon, Task::CTaskContext* pContext, COffsetRange& range)
{
	// リセット
	range.reset();
	// 狙撃が入ってたら最大射程+2 かつ 中心最大射程+2 かつ 到達+4
	// 中心射程の最小と最大が同じ、MAP兵器は効果外
	if(pChara->getBattle().IsSpirit(Chara::CValidSpirit::SNIPE)
	&& pWeapon->getCoreMin()!=pWeapon->getCoreMax()
	&& !pWeapon->IsF()
	&& pWeapon->getKind()!=Weapon::Kind::STATUS)
		pContext->getApp()->getSpirit().getDataCast<Spirit::CSpirit_Snipe>(Spirit::SNIPE)->applyOffset(range);

	if(pWeapon->IsM())
	{// 魔術武器なら
		int nAttr = pChara->getBattle().hasSkill(Ability::MAGICIAN);
		// 魔術師持ち
		if(nAttr>0) pContext->getApp()->getAbility().getDataCast<Ability::CAbility_Magician>(Ability::MAGICIAN)->applyOffset(range,nAttr);
	}

	// 固有能力
	if((pChara->getBattle().IsTalent(Ability::FLY) // 飛行
		&& pContext->getApp()->getAbility().enable(*pChara,0,*static_cast<CSLGContext*>(pContext),Ability::FLY))
	|| (pChara->getBattle().IsTalent(Ability::FLOAT) // 浮揚
		&& pContext->getApp()->getAbility().enable(*pChara,0,*static_cast<CSLGContext*>(pContext),Ability::FLOAT))
	)
	{// 到達無限大
		range.fly(true);
	}
	else
	{ range.fly(false); }
}

void CAttack_range2::calcAllRange(CDataCharaSLG* pChara, CSLGContext* p)
{// このキャラが持ってる全武器の補正値を計算する
	Chara::CDataCharaBattle& battle = pChara->getBattle();
	Weapon::CDataWeaponBattle* pWeapon;
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		pWeapon = p->getWeaponData(*battle.nextWeapon());
		calcRange(pChara, pWeapon, p, pWeapon->getRange());
	}
}

} // namespace Attack end
} // namespace SLG end
} // mamespace BMW end