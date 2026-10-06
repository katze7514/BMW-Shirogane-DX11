/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../IDRule.h"
#include "../slg_fun.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CSLGContext.h"
#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CMenu_chara2.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_chara2::OnReset(Task::CTaskContext* pContext)
{// 初期化
	// ようはキャッシュの生成
	// キャンセル
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel(CANCEL);
	addTask(pCancel,CANCEL_T);

	// メニュー生成
	pMenu_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CCircleMenu>("CHARA_CIRCLE");
	addTask(pMenu_,MENU);

	// イベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CMenu_chara2::eventCircle);
	pMenu_->setEventHandler(funCircle);

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this, &CMenu_chara2::eventButton);
	pMenu_->setButtonEventHandler("MOVE",fun,MOVE);
	pMenu_->setButtonEventHandler("SPEC",fun,STATUS);
	pMenu_->setButtonEventHandler("SP",fun,SPIRIT);
	pMenu_->setButtonEventHandler("ITEM",fun,ITEM);
	pMenu_->setButtonEventHandler("CURE",fun,CURE);
	pMenu_->setButtonEventHandler("PIT",fun,REFILL);
	pMenu_->setButtonEventHandler("END",fun,WAIT);
	pMenu_->setButtonEventHandler("LOVE",fun,LOVE);

	// 攻撃だけちょっと特別
	GUI::CPanel* pAtk = static_cast<GUI::CPanel*>(pMenu_->getButton("ATK")->getTask());
	GUI::CButton::setButtonEvent(pAtk->getWidgetCast<GUI::CButton>("ATK_B"),fun,ATTACK);
	pAtkBatsu_ = pAtk->getWidget("BATSU");
}

void CMenu_chara2::OnInit(Task::CTaskContext* pContext)
{// キャラの状態に合わせて、タスクを出し入れする
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	if(pChara==NULL)
	{// 適切な操作キャラが設定されてなかったら、すぐに終了
		getTaskListCtrl()->returnTaskList();
		return;
	}
	// とりあえず、対象キャラにもしておく
	p->setTargetChara(p->getCtrlChara());
	// 画面を選択キャラが中央に来るように移動する
	p->getMap()->scrollIndex(pChara->getIndex());
	// その位置を元にサークルメニュー表示位置計算
	p->setCirclePos(Pos::CHARA);
	// メニュー位置
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());
	pMenu_->valid(false);
	pMenu_->visible(false);

	// とりあえず、全武器モードで、範囲計算
	if(pChara->getState().getAct()!=Act::HIT_AWAY_MOVE)
	{	getTaskListCtrl()->callTaskList(Rule::ATTACK_RANGE,true);	}
	else
	{// 一撃離脱移動後は攻撃できないので、普通にメニューを出す
		p->clearRange();
		updateMenu(p);
	}

	setState(NORMAL);	
}

void CMenu_chara2::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
		pContext->push(-1);
		actionCall(-1,pContext);
	break;

	default: break;
	}
}

void CMenu_chara2::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::ATTACK_RANGE)
	{// 攻撃範囲計算から戻ってきた
		updateMenu(static_cast<CSLGContext*>(pContext));
	}
	else
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);

		if(pContext->top()==2)
		{// 2が積まれてたら、Wait後なのでそのままスルー
			getTaskListCtrl()->returnTaskList();
			if(p->getCtrlCharaData()->IsExist()) // キャラ生きてたらカーソルをその場所に
				moveCursol(p);
		#ifdef BMW_DEBUG
			//static_cast<CSLGContext*>(pContext)->getCtrlCharaData()->getState().setAct(Act::BEFORE);
		#endif
		}
		ef(pContext->top()>=0)
		{// ルール実行が終了したので、終了
			pContext->pop();
			pContext->push(0);
			getTaskListCtrl()->returnTaskList();

			if(nID!=Rule::SPIRIT_RULE
			   // 精神ルールの時はself掛けだったら自分とこに戻す
			|| p->getCtrlCharaData()->getID()==p->getTargetCharaData()->getID())
				moveCursol(p);
		}
		else
		{// ルール実行がキャンセルされたので、もう一度表示
			pContext->pop();
			OnInit(pContext);
		}
	}
}

void CMenu_chara2::updateMenu(CSLGContext* p)
{
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	switch(pChara->getState().getAct())
	{
	case Act::MOVE: // 移動後
		// キャンセル
		getTask(CANCEL_T)->valid(true);
		// 攻撃
		if(pChara->getState().IsValid(CCharaState::ATTACK))
		{
			pMenu_->validButton(true, "ATK", p);
			pAtkBatsu_->visible(!IsAtk(pChara,p,true));
		}
		
		// 治癒
		if(pChara->getBattle().IsCure()>=0
		&& p->getWeaponData(pChara->getBattle().IsCure())->enable(*pChara,true,*p))
		{// 使用できるのであれば
			if(pChara->getState().IsValid(CCharaState::CURE))
				pMenu_->validButton(true, "CURE", p);
		}
		// 説得（対象がいれば
		if(pChara->getState().IsValid(CCharaState::PERS)
		&& !pChara->getBattle().IsCond(Chara::CValidCond::ACTION))
		{
			if(actionPers(pChara,p))
				pMenu_->validButton(true, "LOVE", p);
		}
	break;

	case Act::HIT_AWAY: // ヒット＆アウェイ or 二回攻撃
		// キャンセル不可
		getTask(CANCEL_T)->valid(false);

		if(pChara->getBattle().hasSkill(Ability::HITAWAY)>=0)
		{// 一撃離脱持ってたら、移動ができるかもしれない
			if(pChara->getState().IsValid(CCharaState::MOVE))
				pMenu_->validButton(true, "MOVE", p);
		}
		if(
			(   pChara->getBattle().IsTalent(Ability::TWICE_ATTACK)
			&&  p->getApp()->getAbility().enable(*pChara,0,*p,Ability::TWICE_ATTACK)
			)
		|| pChara->getBattle().IsTalent(Ability::TWICE_ATTACK_ENEMY)
		)
		{// 二回攻撃持ってたら、攻撃もできるかもしれない
			// 二度目移動後攻撃扱い
			if(pChara->getState().IsValid(CCharaState::ATTACK))
			{
				pMenu_->validButton(true, "ATK", p);
				pAtkBatsu_->visible(!IsAtk(pChara,p,true));
			}
		}
	break;

	case Act::HIT_AWAY_MOVE:
		// キャンセル
		getTask(CANCEL_T)->valid(true);
	break;

	case Act::BEFORE: // 行動前
	{
		getTask(CANCEL_T)->valid(true);
		
		if(!pChara->getBattle().IsCond(Chara::CValidCond::ACTION))
		{
			// 移動
			if(pChara->getState().IsValid(CCharaState::MOVE))
				pMenu_->validButton(true, "MOVE", p);

			// 攻撃
			if(pChara->getState().IsValid(CCharaState::ATTACK))
			{
				pMenu_->validButton(true, "ATK", p);
				pAtkBatsu_->visible(!IsAtk(pChara,p,false));
			}
			// 治癒
			if(pChara->getBattle().IsCure()>=0
			&& p->getWeaponData(pChara->getBattle().IsCure())->enable(*pChara,false,*p))
			{// 使用できるのであれば
				if(pChara->getState().IsValid(CCharaState::CURE))
					pMenu_->validButton(true, "CURE", p);
			}
			// 補給
			if(pChara->getBattle().IsRefill()>=0
			&& p->getWeaponData(pChara->getBattle().IsRefill())->enable(*pChara,false,*p))
			{// 使用できるのであれば
				if(pChara->getState().IsValid(CCharaState::REFILL))
					pMenu_->validButton(true, "PIT", p);
			}
			// 精神
			if(pChara->getState().IsValid(CCharaState::SPIRIT))
				pMenu_->validButton(true, "SP", p);

			// アイテム
			if(pChara->getState().IsValid(CCharaState::ITEM))
			{
				if(pChara->IsItem(p))// 使用できるアイテムがあれば
					pMenu_->validButton(true, "ITEM", p);
			}
		}
		// 能力
		pMenu_->validButton(true, "SPEC", p);

		// 説得（対象がいれば）
		if(pChara->getState().IsValid(CCharaState::PERS)
		&& !pChara->getBattle().IsCond(Chara::CValidCond::ACTION))
		{
			if(actionPers(pChara,p))
				pMenu_->validButton(true, "LOVE", p);
		}
	}
	break; // 行動前 end

	default: break;
	}

	// BEFORE以外で表示
	// 待機
	if(pChara->getState().getAct()!=Act::BEFORE)
		pMenu_->validButton(true, "END", p);

	// 登場
	actionMenu(GUI::CCircleMenu::INTRO,p);
	pMenu_->valid(true);
	pMenu_->visible(true);
	setState(NORMAL);
	nCall_=-1;
}

////////////////////////////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////////////////////////////
void CMenu_chara2::eventCircle(int nState,Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::INTRO)
	{// 登場終了
		pContext->getInput()->guard(false);
		// ヘルプモード
		Unit::Help::callHelp(Unit::Help::SLG_CHARA_MENU,"SLG_CHARA_MENU",pContext);
	}
	else
	{// 退場終了
		if(getState()==CALL)
		{// 現在の状態に合わせて処理分岐
			if(nCall_>=0)
			{	getTaskListCtrl()->callTaskList(nCall_,true);	}
			else
			{	moveCursol(pContext); getTaskListCtrl()->returnTaskList();	}
		}
	}
}

void CMenu_chara2::eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// なんか押されたで
		switch(pButton->getValue())
		{
		case MOVE:		actionCall(Rule::MOVE_RULE,pContext);	break;
		case ATTACK:	actionCall(Rule::ATTACK_RULE,pContext); break;
		case SPIRIT:	actionCall(Rule::SPIRIT_RULE,pContext);	break;
		case ITEM:		actionCall(Rule::ITEM_RULE,pContext);	break;
		case CURE:		actionCall(Rule::CURE_RULE,pContext);	break;
		case REFILL:	actionCall(Rule::REFILL_RULE,pContext);	break;
		case STATUS:	actionCall(Rule::STATUS_RULE,pContext);	break;
		case WAIT:		actionCall(Rule::WAIT_RULE,pContext);	break;
		case LOVE:		actionCall(Rule::LOVE_RULE,pContext);	break;
		default: break;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// 操作
////////////////////////////////////////////////////////////////////////
void CMenu_chara2::actionCall(int nState,Task::CTaskContext* pContext)
{
	// 呼び出すものがある時はコール
	nCall_=nState;
	actionMenu(GUI::CCircleMenu::EXIT,pContext);
	setState(CALL);
}

bool CMenu_chara2::actionPers(CDataCharaSLG* pChara, CSLGContext* p)
{
	ITaskBase* pBase;
	Map::CMapChip *pChip, *pChip2;

	// 移動可能な一マス以内にいる必要がある
	pChip = p->getMapChip(pChara->getIndex());
	for(int w=Way::TOP; w<=Way::RIGHT; w++)
	{
		// 隣のマスを見る
		pChip2 = p->getMapChip(pChip->getMapInfo().getOnMap(w));
		// なかったら、次へ
		if(pChip2==NULL) continue;
		// あったら、こっち
		if(abs(pChip->getMapInfo().getHeight() - pChip2->getMapInfo().getHeight()) <= pChara->getBattle().getJump())
		{// 高さが届く(つまり、移動可能)
			pBase = pChip2->getTask(Map::CMapChip::CHARA);
			if(pBase!=NULL)
			{// キャラは存在する
				Map::CMapChipChara2* pChipChara = static_cast<Map::CMapChipChara2*>(pBase);
				if(pChara->IsPers(pChipChara->getID()))
				{// そいつが説得対象なら、表示
					return true;
				}
			}
		}
	}
	return false;
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end
