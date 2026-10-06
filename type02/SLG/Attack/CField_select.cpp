#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
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
#include "CField_select.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CField_select::OnReset(Task::CTaskContext* pContext)
{
	// OKタスク
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	pOK->setValue(OK);
	addTask(pOK, OK_T);

	// Cancelタスク
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CANCEL);
	addTask(pCancel, CANCEL_T);

	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
}

void CField_select::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);

	moveCursol(pContext);

	pContext->getInput()->guard(false);
	pContext->getInput()->guardDrag(false);
	Map::CMapChipState::attack(true);
	Map::CMapChipState::action(true);

	Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();

	setFieldType(pWeapon->getField());
	// 投げ込みタイプの時の初期設定
	if(getFieldType()==Weapon::Field::THROW)
	{
		// 移動範囲クリア
		p->clearMove();
		// 移動範囲表示
		Map::CMapChipState::move(true);
		// 現在いる場所の高さ
		nHeight_ = p->getMapChip(p->getCtrlCharaData()->getIndex())->getMapInfo().getHeight();
		// 武器の到達度
		nReach_ = pWeapon->getHeight();
		// 範囲の大きさ
		nSize_ =pWeapon->getFieldSize();
	}

	// ヘルプモード
	if(pWeapon->IsF())
	{// フィールド？
		// 中心型
		if(pWeapon->getField()==Weapon::Field::CENTER)
			callHelp(Unit::Help::SLG_ATK_FIELD_CENTER, "SLG_ATK_FIELD_CENTER", pContext);
		// ライン型
		ef(pWeapon->getField()==Weapon::Field::LINE)
			callHelp(Unit::Help::SLG_ATK_FIELD_LINE, "SLG_ATK_FIELD_LINE", pContext);
		// 投げ込み型
		ef(pWeapon->getField()==Weapon::Field::THROW)
			callHelp(Unit::Help::SLG_ATK_FIELD_THROW, "SLG_ATK_FIELD_THROW", pContext);
	}
}

namespace{
__inline bool IsRange(CSLGContext& p)
{
	return p.hasRangeIndex(p.getTargetMap(), true) >= 0;
}

} // namespace end

bool CField_select::IsTargetAttack()
{
	Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();

	int nPhase=pWeapon->IsFieldFriend()?p->getCtrlCharaData()->getPhase():-1;
	if(nPhase>=0
	&& pWeapon->getKind()==Weapon::Kind::STATUS)
	{
		if(nPhase==Phase::PLAYER) nPhase=Phase::ENEMY;
		ef(nPhase==Phase::ENEMY) nPhase=Phase::PLAYER;
	}

	if(getFieldType()==Weapon::Field::THROW)
		return p->IsRangeFieldThrow(pWeapon->IsFieldFriend()?p->getCtrlCharaData()->getPhase():-1);
	else
		return p->IsRangeField(Map::CMapChipState::getField(),
							   getFieldType()==Weapon::Field::LINE ? 1 : pWeapon->getMin(), // LINEの時のminは幅
							   pWeapon->getMax(),
							   nPhase);
}

void CField_select::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
	{	
		// それが攻撃可能範囲内かをチェック
		// 範囲内にキャラが存在しているかを改めてチェック
		if(IsTargetAttack())
		{// 攻撃可能範囲内なら次へ
			p->push(0);
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
		// 移動範囲クリア
		p->clearMove();
		pContext->push(-1);
		actionEnd(pContext);
	break;

	default: // 何もなければ、
	{
		bool bIndex = nIndex_!=p->getTargetMap();
		if(bIndex) nIndex_=p->getTargetMap();
		// フィールド武器のタイプによって、動作があったりなかったり
		Map::CMapChip* pChip;
		Map::CMapChipState* pState;
		if(getFieldType()==Weapon::Field::LINE)
		{// LINEタイプ、表示方向変更
			pChip = p->getTargetMapChip();
			if(pChip==NULL) return;
			pState = pChip->getMapChipState();
			if(pState==NULL) return;
			// 現在表示しているものと違う時は、そっちに変更
			if(pState->getFieldToward(0) != Map::CMapChipState::getField()
			&& 
			   pState->getFieldToward(1) != Map::CMapChipState::getField())
			{// どちらかと同じ時は変更しない
				if(pState->getFieldToward(0)>=0 && pState->getFieldToward(0) != Map::CMapChipState::getField())
					Map::CMapChipState::setField(pState->getFieldToward(0));
				ef(pState->getFieldToward(1)>=0 && pState->getFieldToward(1) != Map::CMapChipState::getField())
					Map::CMapChipState::setField(pState->getFieldToward(1));
			}
		}
		ef(getFieldType()==Weapon::Field::THROW)
		{// 投げ込みタイプは、現在のチップが範囲内なら、
		 // そこを中心に攻撃範囲が表示される
			if(bIndex)
			{
				// 移動範囲クリア
				p->clearMove();
				// 現在のマップが範囲内
				if(p->getTargetMapChip()->getMapChipState()->IsField()/*getFieldAttack()>=0*/)
					calcThrow(p->getTargetMapChip(),nSize_);
			}
		}
		
		// 命中率表示
		// それが攻撃可能範囲内かをチェック
		if(bIndex)
		{
			if(IsRange(*p))
			{
			// 攻撃可能範囲内かつ、そこにキャラが入れば
			// 命中値を持った超簡易ステータス表示
				CDataCharaSLG* pTargetChara = p->getTargetCharaData();
				if(pTargetChara!=NULL
				&& pTargetChara->IsLive()
				&& nChara_!=p->getTargetChara())
				{// 現在表示してるのとキャラが違うなら設定しなおし
					CDataCharaSLG* pTarget = p->getTargetCharaData();
					CDataCharaSLG* pCtrl = p->getCtrlCharaData();
					Weapon::CDataWeaponBattle* pWeapon = p->getCtrlWeaponData();
					// ただし攻撃対象になるやつの時だけ
					if(pWeapon->IsFieldFriend()
					&& pCtrl->getPhase()==pTarget->getPhase())
					{// 味方が対象外になるやつは、味方の時は表示しない
						p->getEvent()->validStatus(false,CStatusCharaVeryEasy::LEFT);
						nChara_=-1;
						return;
					}
					// 高さの設定
					CAttack_road road;
					// このとき、高さの計算は、Ctrl基準。
					// つまり、攻撃側基準。
					road.OnAction(p);
					// スタックに結果が積まれている

					// 命中計算			
					int nHit=CAttack_calc::calcHit(*pCtrl, pWeapon, *pTarget,
												p->getTargetMapChip()->getMapChipState()->getFieldAttack(),
												p->top(), *p);
					if(nHit==CAttack_calc::HIT)
					{// 自分が必中
						nHit=200;
					}
					ef(nHit==CAttack_calc::AVOID)
					{// 相手閃き
						nHit=0;
					}
					else						 
					{
						nHit+=CAttack_calc::calcOffHit(*pCtrl,*pTarget,*p);
						if(nHit<0) nHit=0;
						else if(nHit>200) nHit=200;
					}

					p->getEvent()->getStatus(CStatusCharaVeryEasy::LEFT).actionReset(*pTarget, CStatusCharaVeryEasy::HIT, nHit);

					// 高さ計算を捨てる
					p->pop();
					p->pop();
					
					p->getEvent()->validStatus(true,CStatusCharaVeryEasy::LEFT);
					nChara_=p->getTargetChara();
				}
			}
			else
			{// 何もないなら表示消し
				p->getEvent()->validStatus(false,CStatusCharaVeryEasy::LEFT);
				nChara_=-1;
			}
		}
	}
	break;
	}
}

void CField_select::actionEnd(Task::CTaskContext* pContext)
{
	static_cast<CSLGContext*>(pContext)->getEvent()->validStatus(false,CStatusCharaVeryEasy::LEFT);
	setState(NORMAL);
	pContext->getInput()->guard(true);
	pContext->getInput()->guardDrag(true);
	Map::CMapChipState::attack(false);
	Map::CMapChipState::move(false);
	Map::CMapChipState::action(false);
	getTaskListCtrl()->returnTaskList();
}

////////////////////////////////////
// 投げ込み範囲計算
////////////////////////////////////
void CField_select::calcThrow(Map::CMapChip* pMap, int nMove)
{
	// 移動力が無くなったら終了
	if(nMove<0) return;
	// 対象マップがNULLでも終了
	if(pMap==NULL) return;
	// 攻撃キャラがいるマップでも終了
	if(pMap->getIndex()==p->getCtrlCharaData()->getIndex()) return;
	
	// 検索されてないかったら、上書き
	if(pMap->getMapChipState()->getMove()<0
	|| pMap->getMapChipState()->getMove()>nMove)
	{
		if(abs(pMap->getMapInfo().getHeight()-nHeight_)<=nReach_)
		{
			p->getIndexSet().insert(pMap->getIndex());
			pMap->getMapChipState()->setMove(nSize_-nMove);
		}
	}
	else // そうで無かったら、検索済みなのでリターン
	{ return; }

	// 上へ
	calcThrow(p->getMapChip(pMap->getMapInfo().getOnMap(Way::TOP)),nMove-1);
	// 左へ
	calcThrow(p->getMapChip(pMap->getMapInfo().getOnMap(Way::LEFT)),nMove-1);
	// 下へ
	calcThrow(p->getMapChip(pMap->getMapInfo().getOnMap(Way::BOTTOM)),nMove-1);
	// 右へいけるか
	calcThrow(p->getMapChip(pMap->getMapInfo().getOnMap(Way::RIGHT)),nMove-1);
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end