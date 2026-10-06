#include "stdafx.h"

#include "CRuleCancel.h"

namespace BMW{
namespace Rule{

void CRuleCancel::OnAction(Task::CTaskContext* pContext)
{
	using BMW::Input::IInput;

	// キャンセルされたら、親状態を設定されてる値にする
	if(pContext->getInput()->getInputState(IInput::CANCEL)==IInput::RELEASE)
		getParent()->setState(getValue());
}

} // namespace Rule end
} // namespace BMW end