/*
	katze 05/05/04
	update 06/02/16
	武器選択
*/
#pragma once

#include "../../Scene/Event/IListenerCircleMenu.h"

namespace BMW{

namespace GUI{
class CCircleMenu;
} // namespace GUI end

namespace Weapon{
CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
namespace Attack{

struct weapon_state{
	int nWeaponID_;
	bool bEnable_;

	weapon_state(int nWeaponID=-1, bool bEnable=true):nWeaponID_(nWeaponID),bEnable_(bEnable){}
};

class CAttack_weapon : public BMW::Rule::CRuleList, public BMW::Event::IListenerCircleMenu
{/**
	武器選択

	スタックに相手との距離を積んでおくと、
	それにもとづいてenableが判定される
 */
public:
	typedef map<int, weapon_state> w_state_map;
	enum eState{
		NORMAL,
		OK,
		CANCEL,
	};

	enum ePriority{
		CANCEL_T,
		MENU,
		WEAPON,
	};
	// デストラクタ
	virtual ~CAttack_weapon(){}
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントハンドラ
	virtual void eventCircle(int nState,Task::CTaskContext* pContext);
	virtual void eventButton(const smart_ptr<GUI::CEventButton>& pButton,Task::CTaskContext* pContext);

	// アクション
	void actionMenu(int nState, Task::CTaskContext* pContext);

	void createWeaponPopUp(const string& sName, 
						   int nMin,		int nMax,
						   int nCoreMin,	int nCoreMax,
						   int nReach,
						   string& sPopUp);
	// 武器選択時の中央にでるやつの設定
	void setWeaponDataGui(GUI::CPanel* pWeapon, Weapon::CDataWeaponBattle& weapon, SLG::CDataCharaSLG& chara);
	void setWeaponDataGuiPos(int nPos);


private:
	GUI::CPanel*		pWeapon_;
	w_state_map			mapWeaponID_;	// ボタン位置と武器IDのマップ
	bool				bBack_;			// 援護攻撃選択？

	// 武器情報設定
	void actionWeaponData(Weapon::CDataWeaponBattle& weapon, CDataCharaSLG& chara, bool bEnable);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end