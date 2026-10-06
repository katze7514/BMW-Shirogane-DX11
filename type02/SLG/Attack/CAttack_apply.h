/*
	katze 05/05/07
	戦闘結果を適用する
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataBattleBase;
class CDataBattleMapAtk;
class CDataBattleMapDef;
class CSLGContext;

namespace Attack{

class CAttack_apply : public Task::ITaskList
{/**
	戦闘結果を適用する
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	void apply(CDataBattleBase& attack, CDataBattleBase& def, CSLGContext& p, bool bAttBack=false);
	void applyMap(CDataBattleMapAtk& attack, CDataBattleMapDef& def, CSLGContext& p);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end