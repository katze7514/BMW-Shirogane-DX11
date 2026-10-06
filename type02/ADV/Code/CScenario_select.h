/*
	katze 05/07/24
	update 08/06/10
	シナリオセレクト
*/
#pragma once

namespace BMW{

namespace Scenario{
class CScenarioDB;
}// namespace Scenario end

namespace ADV{
namespace API{

class CScenario_select : public Task::ITaskList
{/**
	シナリオセレクト

	現在状況に合わせて、自動的に次に進むシナリオを選択する
	通常のnextだけではできないような判定時に使う

	Typeに合わせて、スクリプトでは面倒な処理を行う
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionNext(Save::CExecData& save, BMW::Scenario::CScenarioDB& db);
};

} // namespace API end
} // namespace ADV end
} // namespace BMW end