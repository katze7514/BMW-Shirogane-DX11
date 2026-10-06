/*
	katze 05/05/20
	BGMのフェード終了待ちをする
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Sound{
namespace Code{

class CCode_bgm_wait : public Task::ITaskList
{/**
	bgmのフェード待ちをするサブルーチン
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
