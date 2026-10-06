#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../CSLGScene.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Demo/CDemo_map.h"

#include "CAttack_demo.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_demo::OnReset(Task::CTaskContext* pContext)
{
}

void CAttack_demo::OnInit(Task::CTaskContext* pContext)
{// デモのON/OFFによって、戦闘デモシーンに以降するか、
 // MAP上でやるかをディスパッチ
	smart_ptr<CDataBattle>& pData = pContext->getBattleData();

	// DEMOON/OFFによる振り分け
	//setState(DEMO_OFF);
	setState(pContext->getValue(Flag::DEMO) ? DEMO : DEMO_MAP);
	// 音楽再生設定
	if(pData->IsEvent())
	{// イベント時は攻撃側優先
		playBgmEvent(pData->getBattleData(CDataBattle::ATTACK),
					 pData->getBattleData(CDataBattle::COUNTER),
					 pContext);
	}
	else
	{
		int nSide = pData->getSide();
		// 攻撃側の武器がステータスアップとか味方対象の時は
		// LEFt/RIGHTを逆にする
		int nKind = pData->getBattleData(CDataBattle::ATTACK).getAttack().getWeaponData()->getKind();
		if(nKind==Weapon::Kind::STATUS) nSide = (nSide==CDataBattle::LEFT?CDataBattle::RIGHT:CDataBattle::LEFT);

		if(nSide==CDataBattle::LEFT)
		{// 攻撃側が左ということは、アタックが敵
			playBgm(pData->getBattleData(CDataBattle::COUNTER),
					pData->getBattleData(CDataBattle::ATTACK),
					CDataBattle::LEFT,
					pContext);
		}
		else
		{// 攻撃側が右ということは、アタックが味方
			playBgm(pData->getBattleData(CDataBattle::ATTACK),
					pData->getBattleData(CDataBattle::COUNTER),
					CDataBattle::RIGHT,
					pContext);
		}
	}

	// デモ中はカーソル消す
	pContext->getInput()->cursolVisible(false);
}

void CAttack_demo::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DEMO:
	{// デモシーンへ
		static_cast<CSLGContext*>(pContext)->getScene()->setState(CSLGScene::DEMO);
		setState(DEMO_END);
	}
	break;

	case DEMO_END:
	{
		CSLGContext *p = static_cast<CSLGContext*>(pContext);
		if(p->getScene()->getState()==CSLGScene::DEMO_END)
		{// デモシーンから戻ってきてたら
			setState(END);
			p->getScene()->setState(CSLGScene::NORMAL);
		}
	}
	break;

	case DEMO_MAP:
		pContext->push(Demo::CDemo_map::BATTLE);
		getTaskListCtrl()->callTaskList(Rule::DEMO_MAP,true);
	break;

	case END:
		// とりあえずは、終了
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CAttack_demo::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{
	case Rule::DEMO_MAP:
		setState(END);
	break;
	
	default: break;
	}
}

void CAttack_demo::playBgm(CDataBattleBase& player, CDataBattleBase& enemy, int nSide, Task::CTaskContext* pContext)
{// 優先順位
 // 敵武器 > 敵キャラ > 味方武器 > 味方キャラ
	int nBgm=-1;
	if(nSide==CDataBattle::LEFT)
	{// 敵から攻撃
		nBgm = enemy.getAttack().getWeaponData()->getBgmID();
		if(nBgm>=0) goto PLAY;
		nBgm = enemy.getChara()->getBattle().getBgmID();
		if(nBgm>=0) goto PLAY;
		nBgm = player.getChara()->getBattle().getBgmID();
	}
	ef(nSide==CDataBattle::RIGHT)
	{// 味方から攻撃
		nBgm = enemy.getChara()->getBattle().getBgmID();
		if(nBgm>=0) goto PLAY;
		nBgm = player.getAttack().getWeaponData()->getBgmID();
		if(nBgm>=0) goto PLAY;
		nBgm = player.getChara()->getBattle().getBgmID();
	}
	
	
PLAY:
	if(nBgm<0) return;
	if(pContext->getBgmSound()->getBgmID()!=nBgm)
	{
		if(getState()==DEMO_MAP
		&& !pContext->getApp()->getGlobal().IsDemoOff())
		// マップデモでデモOFF時BGM切り替えフラグが倒れてたらスルー
			return;

		pContext->getBgmSound()->change(nBgm);
		pContext->getBgmSound()->FadeIn(15);
	}
}

void CAttack_demo::playBgmEvent(CDataBattleBase& attack, CDataBattleBase& counter, Task::CTaskContext* pContext)
{
	int nBgm=-1;
	nBgm = attack.getAttack().getWeaponData()->getBgmID();
	if(nBgm>=0) goto PLAY;
	nBgm = attack.getChara()->getBattle().getBgmID();
	if(nBgm>=0) goto PLAY;
	nBgm = counter.getChara()->getBattle().getBgmID();
	if(nBgm>=0) goto PLAY;	
	
PLAY:
	if(nBgm<0) return;
	if(pContext->getBgmSound()->getBgmID()!=nBgm)
	{
		if(getState()==DEMO_MAP
		&& !pContext->getApp()->getGlobal().IsDemoOff())
		// マップデモでデモOFF時BGM切り替えフラグが倒れてたらスルー
			return;

		pContext->getBgmSound()->change(nBgm);
		pContext->getBgmSound()->FadeIn(15);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end