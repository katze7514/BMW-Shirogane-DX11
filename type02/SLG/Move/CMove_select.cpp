#include "stdafx.h"

#include "../../Scene/Unit/IDHelp.h"

#include "../slg_fun.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CMove_select.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_select::OnReset(Task::CTaskContext* pContext)
{
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	pOK->setValue(OK);
	addTask(pOK, OK_T);

	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CANCEL);
	addTask(pCancel, CANCEL_T);
}

void CMove_select::OnInit(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pContext->getInput()->guard(false);
	pContext->getInput()->guardDrag(false);
	Map::CMapChipState::move(true);
	Map::CMapChipState::action(true);

	moveCursol(pContext);

	// ヘルプモード
	Unit::Help::callHelp(Unit::Help::SLG_MOVE_SELECT,"SLG_MOVE_SELECT",pContext);
}

void CMove_select::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
	{
		// コンテキスト変換
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// それが移動可能範囲内かをチェック
		set<int>& setIndex = p->getIndexSet();
		int nTarget = p->getTargetMap();
		if(setIndex.find(nTarget)!=setIndex.end() 
		&& nTarget!=p->getCtrlCharaData()->getIndex()
		&& p->getTargetMapChip()->getTask(Map::CMapChip::CHARA)==NULL)
		{// 移動可能範囲内かつ足下では無く、
		 // 誰もいなかったら、そこに移動
			p->push(p->getTargetMap());
			actionEnd(pContext);
		}
		else
		{// 移動範囲外なら、選択状態を維持
			setState(NORMAL);
		}
	}
	break;

	case CANCEL:
	{
		// キャンセルの時は、スタックに-1を積んで
		pContext->push(-1);
		actionEnd(pContext);
	}
	break;

	default: break;
	}
}

void CMove_select::actionEnd(Task::CTaskContext* pContext)
{
	setState(NORMAL);
	pContext->getInput()->guard(true);
	pContext->getInput()->guardDrag(true);
	Map::CMapChipState::move(false);
	Map::CMapChipState::action(false);
	getTaskListCtrl()->returnTaskList();
}

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
