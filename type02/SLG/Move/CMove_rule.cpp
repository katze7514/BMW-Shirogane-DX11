/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDRule.h"
#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CMove_rule.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_rule::OnReset(Task::CTaskContext* pContext)
{
}

void CMove_rule::OnInit(Task::CTaskContext* pContext)
{
	// 移動範囲計算から
	setState(RANGE);
	p = static_cast<CSLGContext*>(pContext);
	p->clearMove();
	// 現在値を保存
	nTargetMap_=p->getCtrlCharaData()->getIndex();
}

void CMove_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case RANGE:		getTaskListCtrl()->callTaskList(Rule::MOVE_RANGE,true);		break;
	/*case CURSOL:	
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		// カーソル誘導セット
		int nIndex = p->getCtrlCharaData()->getIndex();
		int nX,nY;
		// 移動先
		p->getMap()->getMapChipPos(nIndex,nX,nY);
		p->push(nX);
		p->push(nY);
		// ステップ数
		p->push(3);
		// カーソル誘導コール
		getTaskListCtrl()->callTaskList(Rule::MAP_CURSOL,true);
	}
	break;*/
	case SELECT:	getTaskListCtrl()->callTaskList(Rule::MOVE_SELECT,true);	break;
	case ROAD:		getTaskListCtrl()->callTaskList(Rule::MOVE_ROAD,true);		break;
	case EXEC:		getTaskListCtrl()->callTaskList(Rule::MOVE_EXEC,true);		break;
	case MENU:		getTaskListCtrl()->callTaskList(Rule::MENU_CHARA,true);		break;
	case CANCEL:	moveCancel(pContext);										break;
	default: break;
	}
}

void CMove_rule::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	switch(nID)
	{// どこから戻って来たかによって処理が変わる
	case Rule::MOVE_RANGE:	setState(SELECT); break;

	//case Rule::MAP_CURSOL: setState(SELECT); break;

	case Rule::MOVE_SELECT:
		// キャンセルされたら、-1がスタックに積まれている
		if(pContext->top()<0)
		{// そしたら、リターンシーン
		 // あえて、popしないことで、MENU_CHARAでキャンセル処理をさせる
			static_cast<CSLGContext*>(pContext)->clearMove();
			setState(END);
			getTaskListCtrl()->returnTaskList();
		}
		else
		{// 決定されてたら、次へ
			
			// 移動後キャンセル処理のために、移動前状態を保存
			nTargetMap_=pContext->top();
			setPrevState(p->getCtrlCharaData()->getState());
			
			pContext->pop();
			setState(ROAD);
		}
	break;

	case Rule::MOVE_ROAD:
	{
		setState(EXEC);
		p->clearMove();
	}
	break;
	case Rule::MOVE_EXEC: setState(MENU); break;

	case Rule::MENU_CHARA:
		// キャンセルされたら、-1がスタックに積まれている
		// あえて、popしないことで、MENU_CHARAでキャンセル処理をさせる
		if(pContext->top()<0)
			setState(CANCEL);
		else
		{// 移動が終了するので、移動関係の精神はここでフラグを倒す
			flagSpirit(pContext);
			setState(END);
		}

		// ここまで、来たらとりあえずリターン
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CMove_rule::moveCancel(Task::CTaskContext* pContext)
{	
	// キャラの状態を元に戻す
	p->getCtrlCharaData()->setState(getPrevState());

	// キャラの表示位置を元に戻す
	p->getMapChip(getPrevState().getIndex())->addTask(
													p->getMapChip(nTargetMap_)->removeTask(Map::CMapChip::CHARA),
													Map::CMapChip::CHARA
													);
}

void CMove_rule::flagSpirit(Task::CTaskContext* pContext)
{// 移動関係の精神フラグが立っていたら倒す
	CDataCharaSLG* pChara = static_cast<CSLGContext*>(pContext)->getCtrlCharaData();

	// 加速
	if(pChara->getBattle().IsSpirit(Chara::CValidSpirit::ACC))
		pChara->getBattle().spirit(false,Chara::CValidSpirit::ACC);

	// 跳躍
	if(pChara->getBattle().IsSpirit(Chara::CValidSpirit::JUMP))
		pChara->getBattle().spirit(false,Chara::CValidSpirit::JUMP);
}

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
