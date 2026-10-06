#include "stdafx.h"

#include "../CDemoContext.h"
#include "../GUI/CDemoDamage.h"

#include "CCode_damage.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_damage::OnAction(Task::CTaskContext* pContext)
{
	CDemoContext* p = static_cast<CDemoContext*>(pContext);
	p->getDamage()->action(getSide(),getValue());
}


} // namespace Code end
} // namespace Demo end
} // namespcae BMW end