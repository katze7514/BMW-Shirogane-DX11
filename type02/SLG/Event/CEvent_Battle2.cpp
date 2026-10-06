#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGDef.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Context/DB/CBattleEvent.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"

#include "CEvent_Battle2.h"

namespace BMW{
namespace SLG{
namespace Event{

namespace{

void setBattleDataAbility(CBattleEventAbility& abi, CDataBattleAbility& abi_b)
{// 戦闘設定データから実戦闘データへ変換
	// Abilityセット
	set<int>& abiset = abi.getAbilitySet();
	set<int>& abiset_b = abi_b.getAbilitySet();
	abiset_b = abiset;

	abi_b.setEnAbility(abi.getEnAbility());
	abi_b.setMsgList(abi.getMsgList());
	abi_b.setMsgListBackup(abi.getMsgListBackup());
}

void setBattleDataAtk(CBattleEventAttack& atk, CDataBattleAttack& atk_b, int nDefHP)
{// 戦闘設定データから実戦闘データへ変換
	setBattleDataAbility(atk,atk_b);
	
	// EN
	atk_b.setEN(atk.getEN());
	// CT
	atk_b.ct(atk.IsCT());
	// Death
	atk_b.death(atk.IsDeath());

	// ダメージ
	if(atk.getType()==CBattleEventAttack::RATIO)
	{// 比率
		atk_b.setDamage(nDefHP*atk.getDamage()/100);
	}
	else
	{// 絶対値ならそのまま
		atk_b.setDamage(atk.getDamage());
	}
}

void setBattleDataDef(CBattleEventDefence& def, CDataBattleDefence& def_b)
{// 戦闘設定データから実戦闘データへ変換
	setBattleDataAbility(def,def_b);

	// 行動ID
	def_b.setAction(def.getAction());
}

__inline int getCharaHP(int nIndex, smart_ptr<CDataBattle>& pBattle)
{
	return  pBattle->getBattleData(nIndex).getChara()->getBattle().getHP();
}

void setBattleData(CBattleEventData* pData, smart_ptr<CDataBattle>& pBattleData, CSLGContext* p)
{
	// キャラデータ設定
	CDataCharaSLG* pChara;
	for(int i=0; i<4; ++i)
	{
		CBattleEventBase& base = pData->getEventBase(i);
		if(base.getChara()<0) continue;
		pChara = p->getCharaData(base.getChara());
		pBattleData->getBattleData(i).setChara(smart_ptr<CDataCharaSLG>(pChara,false));
	}

	// 防御側HP取得
	int nDefHP[3];
	// 攻撃→ 反撃？援護防御？
	if(pData->getEventBase(CBattleEventData::COUNTER_BACK).getChara()>=0)
	{// 援護防御あり
		nDefHP[0] = getCharaHP(CDataBattle::COUNTER_BACK,pBattleData);
	}
	else
	{// なし
		nDefHP[0] = getCharaHP(CDataBattle::COUNTER,pBattleData);
	}
	// 反撃→攻撃
	nDefHP[1] = getCharaHP(CDataBattle::ATTACK,pBattleData);
	// 援護攻撃→反撃
	nDefHP[2] = getCharaHP(CDataBattle::COUNTER,pBattleData);

	// データ設定
	Weapon::CDataWeaponBattle* pWeapon;
	for(int i=0; i<4; ++i)
	{
		// イベント戦闘データ取得
		CBattleEventBase& base = pData->getEventBase(i);
		if(base.getChara()<0) continue;
		// 設定対象取得
		CDataBattleBase& battle = pBattleData->getBattleData(i);
		// 武器データの設定

		pWeapon = p->getWeaponData(battle.getChara()->getWeaponSlgID(base.getAtk().getWeaponID(),*p));

#ifdef BMW_DEBUG
		CDbg().Out("EVENT WEAPON %d %d %d",base.getAtk().getWeaponID(),battle.getChara()->getWeaponSlgID(base.getAtk().getWeaponID(),*p), pWeapon);
#endif

		battle.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(pWeapon,false));
		// 攻撃データ設定
		if(i<3) setBattleDataAtk(base.getAtk(), battle.getAttack(), nDefHP[i]);
		// 防御データ設定
		setBattleDataDef(base.getDef(), battle.getDefence());
	}
}

} // namespace end

void CEvent_Battle2::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	smart_ptr<CDataBattle>& pBattleData = p->getBattleData(); 
	pBattleData->clearBattleData();
	pBattleData->setBack(p->getMap()->getDemoBack());
	// イベントモード
#ifdef BMW_DEBUG
	pBattleData->event(false);
#else
	pBattleData->event(true);
#endif

	// スタックに
	// 攻撃サイド
	pBattleData->setSide(p->top());
	p->pop();
	// デモフラグ
	int nDemo = p->top();
	p->pop();
	// 現在のデモフラグを保存
	nDemo_ = p->getValue(Flag::DEMO);
	// デモフラグが設定されてれば
	// 前の状態を引き継ぐ時は、負の値
	if(nDemo>=0) p->setValue(nDemo,Flag::DEMO);
	// イベント戦闘データ
	CBattleEventData* pData = p->getSLGDef().getBattleEvent(p->top());
	p->pop();
	if(pData==NULL)
	{// データなかったらすぐにリターン
		setState(END);
	}
	else
	{// データにあわせて、戦闘データ生成
		setBattleData(pData,pBattleData,p);
		setState(DEMO);
	}
}

void CEvent_Battle2::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DEMO:
		// デモを呼び出す
		getTaskListCtrl()->callTaskList(Rule::ATTACK_DEMO,true);
		setState(NORMAL);
	break;

	case APPLY:
		//applyData(pContext);
		setState(END);
	break;

	case END:
		// 終了したので、リターン
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CEvent_Battle2::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::ATTACK_DEMO)
	{// デモから戻って来た
		// デモフラグを元に戻す
		pContext->setValue(nDemo_,Flag::DEMO);
		// んで、適用
		setState(APPLY);
	}
}

} // namespace Event end
} // namespace SLG end
} // namepsace BMW end