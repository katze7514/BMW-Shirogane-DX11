/*
	katze 06/06/25
	アーチャー狙撃
*/
#pragma once

namespace BMW{
namespace SLG{
namespace T_20{

class CArcher_snipe : public Task::ITaskList
{/**
	アーチャー狙撃
 */
public:
	// デストラクタ
	~CArcher_snipe();

	// タスク
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// 狙撃エフェクト
	Movie::CMovieClip* pEffect_;
	// 対象マップ
	Map::CMapChip* pMap_;
	// スプライト
	Task::ITaskBase* pSprite_;
};

} // namespace T_20 end
} // namespace SLG end
} // namespace BMW end