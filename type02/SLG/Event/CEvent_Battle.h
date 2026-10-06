/*
	katze 05/07/19
	戦闘イベントのための補助クラス
*/
#pragma once

#include "../IDSLG.h"

namespace BMW{

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class CDataBattle;
class CSLGContext;

namespace Event{

class CEvent_Battle : public Task::ITaskList
{/**
	戦闘イベントのための補助クラス

	戦闘イベントを起こす時は、こいつを継承して
	おくと便利やぞ
 */
public:
	enum eState{
		NORMAL,
		DEMO,
		APPLY,
		END,
	};
	// デストラクタ
	virtual ~CEvent_Battle(){}

	// タスク
	virtual void OnReset(Task::CTaskContext*);
	virtual void OnInit(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);
	virtual void OnComeBack(int nID, Task::CTaskContext*);

	// 戦闘データへの操作系
	// まずは、こいつを呼び出す
	void clearData(int nSide=0);

	// ↓以外の部分は、バトルデータを直接触ること
	// 技能とかクリティカルとか

	// キャラデータ設定
	void setCharaData(CSLGContext& context, int nID, int nBattle);
	void setCharaData(CSLGContext& context, const string& sID, int nBattle);
	
	// 攻撃側データへの一括攻撃基本設定
	// nAttack→nDef設定
	void setAttackData(CSLGContext& context, int nAttack, int nDef,
					   const string& sWeapon, int nDamage=0, bool bDeath=false, const string& sAttMsg="",
					   int nAction=Battle::NO, const string& sDefMsg="", bool bBackUP=false);

	// 援護防御
	void setBackupDefData(CSLGContext& context, const string& sMsg="");

	// ↑らの下請け
	void setDataAttack(CDataBattleAttack& attack,
						const smart_ptr<Weapon::CDataWeaponBattle>& pWeapon,
						int nDamage, bool bDeath, const string& sAttMsg="");

	void setDataDefence(CDataBattleDefence& def,
						int nAction, const string& sDefMsg="");
	

	// イベント戦闘によるステータス変化
	// イベント戦闘時は、基本的にATTACK_APPLYは呼ばない
	virtual void applyData(Task::CTaskContext*){}

protected:
	int nDemo_;	// 一時デモフラグ
	smart_ptr<CDataBattle> pBattleData_;
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end