#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattle.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGDef.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"

#include "CEvent_Battle.h"

namespace BMW{
namespace SLG{
namespace Event{

void CEvent_Battle::OnReset(Task::CTaskContext* pContext)
{// バトルデータのコピーを持っておく
	pBattleData_ = pContext->getBattleData();
}

void CEvent_Battle::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	pBattleData_->setBack(p->getMap()->getDemoBack());
	// 現在のデモフラグを保存
	nDemo_ = pContext->getValue(Flag::DEMO);
	setState(DEMO);
}

void CEvent_Battle::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DEMO:
		// デモを呼び出す
		getTaskListCtrl()->callTaskList(Rule::ATTACK_DEMO,true);
		setState(NORMAL);
	break;

	case APPLY:
		applyData(pContext);
		setState(END);
	break;

	case END:
		// 終了したので、リターン
		getTaskListCtrl()->returnTaskList();
	break;
	}
}

void CEvent_Battle::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Rule::ATTACK_DEMO)
	{// デモから戻って来た
		// デモフラグを元に戻す
		pContext->setValue(nDemo_,Flag::DEMO);
		// んで、適用
		setState(APPLY);
	}
}

////////////////////////////////////////////////////////////////
// 戦闘データへの一括設定系
////////////////////////////////////////////////////////////////
// 戦闘データへの操作系
void CEvent_Battle::clearData(int nSide)
{// データクリア
	pBattleData_->clearBattleData();
	pBattleData_->setSide(nSide);
	setState(DEMO);
}

void CEvent_Battle::setCharaData(CSLGContext& context, int nID, int nBattle)
{
	CDataBattleBase& data = pBattleData_->getBattleData(nBattle);
	// キャラデータ設定
	CDataCharaSLG* pChara = context.getCharaData(nID);
	data.setChara(smart_ptr<CDataCharaSLG>(pChara,false));
}

void CEvent_Battle::setCharaData(CSLGContext& context, const string& sID, int nBattle)
{
	setCharaData(context, context.getSLGDef().getSlgID(sID), nBattle);
}

// nAttack→nDef設定
void CEvent_Battle::setAttackData(CSLGContext& context, int nAttack, int nDef,
								  const string& sWeapon, int nDamage, bool bDeath, const string& sAttMsg,
								  int nAction, const string& sDefMsg, bool bBackUP)
{// 攻撃側データへの一括攻撃基本設定
	CDataBattleBase& attack = pBattleData_->getBattleData(nAttack);
	// 攻撃の設定
	int nWeapon = attack.getChara()->getWeaponSlgID(sWeapon,context); 

	setDataAttack(attack.getAttack(),
				  smart_ptr<Weapon::CDataWeaponBattle>(context.getWeaponData(nWeapon),false),
				  nDamage,bDeath,sAttMsg);

	CDataBattleBase& def = pBattleData_->getBattleData(nDef);
	// 防御の設定
	setDataDefence(bBackUP ? attack.getDefence() : def.getDefence(),
					nAction, sDefMsg);
}

void CEvent_Battle::setBackupDefData(CSLGContext& context, const string& sMsg)
{
	CDataBattleBase& def = pBattleData_->getBattleData(CDataBattle::COUNTER_BACK);
	setDataDefence(def.getDefence(), Battle::DEFENCE, sMsg);
}

void CEvent_Battle::setDataAttack(CDataBattleAttack& attack,
								  const smart_ptr<Weapon::CDataWeaponBattle>& pWeapon,
								  int nDamage, bool bDeath, const string& sAttMsg)
{
	// 武器の設定
	attack.setWeaponData(pWeapon);
	// ダメージ
	attack.setDamage(nDamage);
	// 死亡フラグ
	attack.death(bDeath);
	// メッセージリスト
	attack.setMsgList(sAttMsg);
}

void CEvent_Battle::setDataDefence(CDataBattleDefence& def, int nAction, const string& sDefMsg)
{
	// 行動設定
	def.setAction(nAction);
	// メッセージリスト
	def.setMsgList(sDefMsg);
}

} // namespace Event end
} // namespace SLG end
} // namepsace BMW end