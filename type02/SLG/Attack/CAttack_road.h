/*
	katze 06/01/16
	射線の計算
*/
#pragma once

namespace BMW{
namespace SLG{

namespace Map{
class CMapChip;
} // namespace Map end

class CSLGContext;
namespace Attack{

class CAttack_road : public Task::ITaskList
{/**
	射線の計算
	つまり、その攻撃ルート上の最小と最大の高さを計算する
	ついでにそのルート上に自分の敵と味方がどれだけいるかも計算
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);	
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end