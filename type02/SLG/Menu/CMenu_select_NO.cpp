#include "stdafx.h"

#include "../IDSLG.h"
#include "../GUI/CStatusCharaVeryEasy.h"
#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CMenu_select_player.h"
#include "CMenu_select_NO.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_select_NO::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

void CMenu_select_NO::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	if(getParent()->getState()==CMenu_select_player::NORMAL)
	{// 何も起きてない
		int nTarget	   = p->getTargetChara();

		if(nTarget>=0)
		{// カーソルがキャラチップの上にあったりする
		// じゃ、超簡易ステータス表示
			if(nID_!=nTarget){ changeStatus(p); }
		}
		else 
		{// キャラがいないなら、非表示に
			actionInValid(p);
		}
		nID_=nTarget;
	}
	else
	{// 何かがおきたら、超簡易ステ消す
		actionInValid(p);
	}
}

void CMenu_select_NO::changeStatus(CSLGContext* p)
{
	CDataCharaSLG* pTarget = p->getTargetCharaData();
	if(pTarget==NULL){ actionInValid(p); return; }
	// 現在表示中のを消す
	if(nSide_!=-1) p->getEvent()->validStatus(false, nSide_);
	nSide_ = pTarget->getPhase()==Phase::PLAYER ? 1 : 0;

	p->getEvent()->getStatus(nSide_).actionReset(*pTarget, CStatusCharaVeryEasy::MENTAL, pTarget->getBattle().getMental());
	p->getEvent()->validStatus(true, nSide_);
}

void CMenu_select_NO::actionInValid(CSLGContext* p)
{
	if(nSide_>=0)
	{
		p->getEvent()->validStatus(false, nSide_);
		nSide_=-1;
	}
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end