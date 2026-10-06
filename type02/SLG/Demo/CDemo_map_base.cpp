#include "stdafx.h"

#include "CDemo_map.h"
#include "CDemo_map_base.h"

namespace BMW{
namespace SLG{
namespace Demo{

void CDemo_map_base::OnInit(Task::CTaskContext* pContext)
{
	pDemo_ = smart_ptr_static_cast<CDemo_map>(getParent());
}

void CDemo_map_base::actionEnd(Task::CTaskContext* pContext)
{
	pDemo_->resetLeft();
	pDemo_->resetRight();
	pDemo_->setState(CDemo_map::END);
}

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end