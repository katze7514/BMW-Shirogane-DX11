#include "stdafx.h"

#include "CADVScene.h"

#include "CAdvVM.h"

namespace BMW{
namespace ADV{

void CAdvVM::noExist(Task::CTaskContext* pContext)
{
	getParent()->setState(CADVScene::END);
}

} // namespace ADV end
} // namespace BMW end