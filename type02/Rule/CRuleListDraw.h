/*
	katze 06/02/02
	描画情報持ったタスクリストのルール版
*/
#pragma once

#include "../Task/CTaskListDraw.h"

namespace BMW{
namespace Rule{

class CRuleListDraw : public Task::CTaskListDraw
{/**
	描画情報持ったタスクリストのルール版
 */
public:
	virtual ~CRuleListDraw(){}
	virtual void Task(Task::CTaskContext*);
};

} // namespace Rule end
} // namespace BMW end