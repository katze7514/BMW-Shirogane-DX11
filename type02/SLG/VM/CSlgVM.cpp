#include "stdafx.h"

#include "../CSLGScene.h"

#include "CSlgVM.h"

namespace BMW{
namespace SLG{

void CSlgVM::noExist(Task::CTaskContext* pContext)
{
	getParent()->setState(CSLGScene::END);	
}

} // namespace SLG end
} // namespace BMW end