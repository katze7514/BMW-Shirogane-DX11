#include "stdafx.h"

#include "CRuleOK.h"

namespace BMW{
namespace Rule{

void CRuleOK::OnAction(Task::CTaskContext* pContext)
{
	using BMW::Input::IInput;

	// キャンセルされたら、親状態を設定されてる値にする
	if(pContext->getInput()->getInputState(IInput::OK)==IInput::RELEASE)
		getParent()->setState(getValue());
}

} // namespace Rule end
} // namespace BMW end