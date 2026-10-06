#include "stdafx.h"

#include "../IDSLG.h"

#include "../Context/CSLGDef.h"
#include "../Context/CSLGContext.h"
#include "CVictory_check2.h"

namespace BMW{
namespace SLG{
namespace Victory{

void CVictory_check2::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	CSLGDef& def = p->getSLGDef();
	// ”s–kðŒ
	if(def.IsLose(p))
	{// ”s–k
		p->push(Victory::LOSE);
	} // n—û“xðŒ
	else if(def.IsExpert(p))
	{// n—û“xŠl“¾
		p->push(Victory::EXPERT);
	}
	else if(def.IsVictory(p))
	{// Ÿ—˜ðŒ’B¬II
		p->push(Victory::VICTORY);
	}
	else
	{// “Á‚É‰½‚à‚È‚¢
		p->push(Victory::NO);
	}

	getTaskListCtrl()->returnTaskList();
}

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end