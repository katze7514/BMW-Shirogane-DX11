#include "stdafx.h"

#include "../CDemoContext.h"
#include "../GUI/CDemoDamage.h"

#include "CCode_damage_state.h"

namespace BMW{
namespace Demo{
namespace Code{

void CCode_damage_state::OnAction(Task::CTaskContext* pContext)
{
	CDemoContext* p = static_cast<CDemoContext*>(pContext);
	p->getDamage()->visible(IsVisible());
}


} // namespace Code end
} // namespace Demo end
} // namespcae BMW end