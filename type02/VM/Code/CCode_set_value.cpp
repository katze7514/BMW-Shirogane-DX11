/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_set_value.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_set_value::OnAction(Task::CTaskContext* pContext)
{// 一時データに値を設定する
	int value = pContext->top();
	pContext->pop();
	pContext->setValue(value, getState());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
