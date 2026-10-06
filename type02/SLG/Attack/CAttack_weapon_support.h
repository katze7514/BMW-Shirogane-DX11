/*
	katze 06/02/17
	援護攻撃用武器選択
*/
#pragma once

#include "CAttack_weapon.h"

namespace BMW{
namespace SLG{
namespace Attack{

class CAttack_weapon_support : public CAttack_weapon
{/**
	援護攻撃用武器選択
 */
public:
	typedef delegate<void,int,Task::CTaskContext*> WeaponEvent;
	enum eEvent{
		WEAPON_EVENT,
		CANCEL_EVENT,
	};
	// デストラクタ
	virtual ~CAttack_weapon_support(){}
	// 設定
	void setEventHandler(const WeaponEvent& fun){ fun_=fun; }

	// イベントハンドラ
	void eventCircle(int nState,Task::CTaskContext* pContext);

private:
	// 選択された時に呼ばれる
	WeaponEvent fun_;
};

} // naemspace Attack end
} // namespace SLG end
} // namespace BMW end