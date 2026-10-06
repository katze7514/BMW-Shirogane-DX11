#include "stdafx.h"

#include "CScript.h"

namespace BMW{
namespace VM{

void CScript::addCode(Task::ITaskBase* pBase)
{
	// コードの実行位置を設定する
	pBase->setTaskPriority(getState());
	setState(getState()+1);
	pBase->setParent(smart_ptr<Task::ITaskBase>(this,false));
	vecCode_.push_back(pBase);
}

void CScript::clearCode()
{
	code_vec::iterator it;
	for(it=vecCode_.begin(); it!=vecCode_.end(); ++it)
		DELETE_SAFE(*it);

	vecCode_.clear();
}

} // namespace VM end
} // namespace BMW end