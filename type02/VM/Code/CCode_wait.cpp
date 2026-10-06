/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_wait.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_wait::OnAction(Task::CTaskContext* pContext)
{// 指定フレーム数waitする
	if(!(getState()<=nFrame_)){
		++nFrame_;
		pContext->getTaskList()->setState(getTaskPriority());
	}
	else // こいつを抜ける時にリセットしておく
		nFrame_=1;
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
