/*
	katze 07/03/18

	現在位置から指定された位置に
	マップをトゥイーンさせる
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Map{
class CMap;

class CMap_scroll : public Task::ITaskList
{/**
	現在位置から指定された位置に
	マップをトゥイーンさせる

	移動先のx,y
	ステップ数
	イージング

	と積んでおく
 */
public:
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	CMap*			pMap_;
	Movie::CMotion	motion_;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end