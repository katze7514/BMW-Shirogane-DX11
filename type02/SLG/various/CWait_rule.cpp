#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CWait_rule.h"

namespace BMW{
namespace SLG{
namespace Wait{

void CWait_rule::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	
	if(pChara->IsExist())
	{// キャラ存在してれば
		if(pChara->getBattle().IsSpirit(Chara::CValidSpirit::AWAKE))
		{// 覚醒が入ってたら、BEFOREに
			pChara->getState().setAct(Act::BEFORE);
			pChara->getBattle().spirit(false,Chara::CValidSpirit::AWAKE);
		}
		else
		{// 操作対象キャラのActをAFTERにする
			pChara->getState().setAct(Act::AFTER);
		}
	}

	// Wait後は2をスタックに積む
	p->push(2);
	getTaskListCtrl()->returnTaskList();
}

} // namespace Wait end
} // namespace SLG end
} // namespace BMW end