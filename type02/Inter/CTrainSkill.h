/*
	katze 06/03/01
	技能養成
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CTrainSkill : public ITrainBase
{/**
	技能養成
 */
public:
	enum eHave{
		HAVE_0,
		HAVE_1,
		HAVE_2,
		HAVE_3,
		HAVE_4,
		HAVE_5,
	};
	enum eGet{
		// 一行目
		POWER,
		COUNTER,
		MOVE_UP,
		BACKUP_ATT,
		BACKUP_DEF,
		LINKAGE,
		HIT_ADN_WAY,
		// 二行目
		SPUP,
		SP_RECOVER,
		CONCENT,
		MAGICSAVE,
		BALLETSAVE,
		ATTACKER,
		REVENGE,
		// 三行目
		BATTLESPIRIT,
		FIGHTUP,
		RITHM,
		AGAINST,
		IKIYOYO,
		GUARD,
		BREAKLINE,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// イベントハンドラ
	void eventHave(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);
	void eventOK(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionUpdateFP(Task::CTaskContext*);

	// アビリティIDtoGetIDとのマップ
	static int ability2Get(int nID);
	static int get2Ability(int nID);

private:
	void	invalidGet(const BMW::Chara::CStatusAbility& skill, Ability::CAbilityDB& db, bool bTalent=false);
	void	updateLvSkill(const BMW::Chara::CDataCharaInter& chara, Ability::CAbilityDB& db);

	GUI::CPanel*	getBar(int nID);
	int				getGetFP(int nID, Ability::CAbilityDB& db);
	void			calcLv(int nID, int nValue);
	int				getAttr(int nID);

	// インターフェイス
	// GUI::CPanel* pPanel_;
	GUI::CPanel*	pHave_;
	GUI::CPanel*	pGet1_;
	GUI::CPanel*	pGet2_;
	GUI::CPanel*	pGet3_;
	GUI::CNum*		pNeed_;
	GUI::CNumCtrl*	pRemain_;
	GUI::CButton*	pOK_;
	
	int nHaveSkill_[6]; // 所持技能のID配列
	int nFP_; // 現在選択中の消費FP
	// Lv制技能の次に覚えるLv
	int nPower_;
	int nCounter_;
	int nBackAtk_;
	int nBackDef_;
	int nSpUp_;
	int nMoveUp_;
	void resetLv()
	{// Lvリセット
		nPower_=nCounter_=nBackAtk_=nBackDef_=nSpUp_=nMoveUp_=1;
	}
	void updateLvSkill(Ability::CAbilityDB& db);
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end