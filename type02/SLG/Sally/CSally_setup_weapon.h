/*
	katze 05/04/30
	武器のセットアップをする
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Weapon{
class CDataWeaponInit;
} // namespace Weapon end

namespace SLG{
class CSLGContext;

namespace Sally{

class CSally_setup_weapon : public Task::ITaskList
{/**
	武器のセットアップをする

	セットアップ対象（SLG ID）は、
	TargetCharaに設定しておく
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	// 武器データ生成
	template<class WeaponBattle>
	static WeaponBattle* createBattle(Weapon::CDataWeaponInit* pInit, int nID, CDataCharaSLG* pChara, CSLGContext* pContext);
	static bool	createBattleCollab(Weapon::CDataWeaponInit* pInit, int nID, CDataCharaSLG* pChara, CSLGContext* pContext);

	// 武器生成
	static void setupWeapon(Task::CTaskContext*);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end