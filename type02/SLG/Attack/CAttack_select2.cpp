#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../slg_fun.h"

#include "../Event/CEvent.h"
#include "../GUI/CStatusCharaVeryEasy.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
//#include "../Map/CMapChipChara.h"

#include "CAttack_calc.h"
#include "CAttack_road.h"
#include "CAttack_select2.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_select2::OnReset(Task::CTaskContext* pContext)
{
	// OKタスク
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	pOK->setValue(OK);
	addTask(pOK, OK_T);

	// Cancelタスク
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CANCEL);
	addTask(pCancel, CANCEL_T);

	p = static_cast<CSLGContext*>(pContext);
}

void CAttack_select2::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);

	moveCursol(pContext);

	pContext->getInput()->guard(false);
	pContext->getInput()->guardDrag(false);
	Map::CMapChipState::mapValid(true);
	Map::CMapChipState::attack(true);
	Map::CMapChipState::action(true);

	// ヘルプモード
	Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();
	switch(pWeapon->getKind())
	{
	// 合体攻撃
	case Weapon::Kind::FIGHT_COLLAB:
	case Weapon::Kind::MAGIC_COLLAB:
		callHelp(Unit::Help::SLG_ATK_COLLAB, "SLG_ATK_COLLAB", pContext);
	break;

	// 状態変化
	case Weapon::Kind::FIGHT_COND:
	case Weapon::Kind::MAGIC_COND:
		callHelp(Unit::Help::SLG_ATK_COND, "SLG_ATK_COND", pContext);
	break;

	// ステータスアップ
	case Weapon::Kind::STATUS:
		callHelp(Unit::Help::SLG_ATK_STATUS, "SLG_ATK_STATUS", pContext);
	break;

	// 治癒
	case Weapon::Kind::CURE:
		callHelp(Unit::Help::SLG_CURE, "SLG_CURE", pContext);
	break;

	// 補給
	case Weapon::Kind::REFILL:
		callHelp(Unit::Help::SLG_PIT, "SLG_PIT", pContext);
	break;

	// 対象選択
	default:
		callHelp(Unit::Help::SLG_ATK_SELECT, "SLG_ATK_SELECT", pContext);
	break;
	}
}

namespace{
__inline bool IsRange(CSLGContext& p)
{
	Map::CMapChip* pChip = p.getTargetMapChip();
	return pChip!=NULL 
		&& pChip->getMapChipState()->IsAtkRange() && pChip->getMapChipState()->IsHeight()
		&& p.getTargetChara()>=0
		&& p.getCtrlWeaponData()->enableTarget(*p.getCtrlCharaData(),*p.getTargetCharaData(),p);
}
} // namespace end

void CAttack_select2::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
	{
		// それが攻撃可能範囲内かをチェック
		if(IsRange(*p))
		{// 攻撃可能範囲内かつ、そこにキャラが入れば
			p->push(p->getTargetChara());
			actionEnd(pContext);
		}
		else
		{// 攻撃範囲外なら、選択状態を維持
			setState(NORMAL);
		}
	}
	break;

	case CANCEL:
		// キャンセルされたら-1を積んで終了
		pContext->push(-1);
		actionEnd(pContext);
	break;

	default: // 何もなければ、
	{
		// それが攻撃可能範囲内かをチェック
		if(IsRange(*p))
		{// 攻撃可能範囲内かつ、そこにキャラが入れば
		 // 命中値を持った超簡易ステータス表示
			if(nChara_!=p->getTargetChara())
			{// 現在表示してるのとキャラが違うなら設定しなおし
				CDataCharaSLG* pTarget = p->getTargetCharaData();
				nChara_=p->getTargetChara();
				Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();
				if(pWeapon->getKind()!=Weapon::Kind::STATUS
				&& pWeapon->getKind()!=Weapon::Kind::CURE
				&& pWeapon->getKind()!=Weapon::Kind::REFILL)
				{
					CDataCharaSLG* pCtrl = p->getCtrlCharaData();
					// 相手までの距離と高さが入ってるマップ状態を取得
					Map::CMapChipState* pTargetMapState = p->getTargetMapChip()->getMapChipState();

					int nHeight = pTargetMapState->getAtkHeight();
					// 敵の方が位置高かったらマイナスになる
					if(p->getMapChip(pCtrl->getIndex())->getMapInfo().getHeight() < p->getTargetMapChip()->getMapInfo().getHeight())
						nHeight = -nHeight;

					// 命中計算
					int nHit=CAttack_calc::calcHit(*pCtrl, pWeapon, *pTarget,
												  pTargetMapState->getAttack(),
												  nHeight,
												  *p);
					if(nHit==CAttack_calc::HIT)
					{
						nHit=200;  // 自分が必中
					}
					ef(nHit==CAttack_calc::AVOID)
					{
						nHit=0;	// 相手閃き
					}
					else
					{
						nHit+=CAttack_calc::calcOffHit(*pCtrl,*pTarget,*p);
						if(nHit<0) nHit=0;
						else if(nHit>200) nHit=200;
					}
					p->getEvent()->getStatus(CStatusCharaVeryEasy::LEFT).actionReset(*pTarget, CStatusCharaVeryEasy::HIT, nHit);
				
					nSide_= CStatusCharaVeryEasy::LEFT;
				}
				else
				{// 仲間を対象とする時は気力で
					p->getEvent()->getStatus(CStatusCharaVeryEasy::RIGHT).actionReset(*pTarget, CStatusCharaVeryEasy::MENTAL, pTarget->getBattle().getMental());
					nSide_ = CStatusCharaVeryEasy::RIGHT;
				}
				p->getEvent()->validStatus(true,nSide_);
			}
		}
		else
		{// 何もないなら表示消し
			if(nChara_>=0) p->getEvent()->validStatus(false,nSide_);
			nChara_=-1;
		}
	}
	break;
	}
}

void CAttack_select2::actionEnd(Task::CTaskContext* pContext)
{
	if(nChara_>=0) p->getEvent()->validStatus(false,nSide_);
	setState(NORMAL);
	pContext->getInput()->guard(true);
	pContext->getInput()->guardDrag(true);
	Map::CMapChipState::attack(false);
	Map::CMapChipState::action(false);
	getTaskListCtrl()->returnTaskList();
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end