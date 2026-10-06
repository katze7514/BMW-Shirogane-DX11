#include "stdafx.h"

#include "../../Spirit/IDSpirit.h"

#include "../GUI/CStatusCharaVeryEasy.h"
#include "../Event/CEvent.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CSpirit_select.h"

namespace BMW{
namespace SLG{
namespace Spirit{

void CSpirit_select::OnReset(Task::CTaskContext* pContext)
{
	Rule::CRuleCancel* pCancel = new Rule::CRuleCancel();
	addTask(pCancel,CANCEL_T);
	pCancel->setValue(CANCEL);

	Rule::CRuleOK* pOK = new Rule::CRuleOK();
	addTask(pOK,OK_T);
	pOK->setValue(OK);

	p = static_cast<CSLGContext*>(pContext);
}

void CSpirit_select::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 使う精神IDを取得
	int nID = p->getCtrlCharaData()->getBattle().getSpirit(p->getTargetAbility()).getID();
	
	setState(NORMAL);
	switch(p->getApp()->getSpirit().getRange(nID))
	{
	case BMW::Spirit::Range::FRIEND:
		// 味方を選択範囲に
		rangeFriend(nID,p);
		p->getInput()->guard(false);
		pContext->getInput()->guardDrag(false);
		Map::CMapChipState::move(true);
		Map::CMapChipState::action(true);
	break;

	case BMW::Spirit::Range::ENEMY:
		// 敵を選択範囲に
		rangeEnemy(nID,p);
		p->getInput()->guard(false);
		pContext->getInput()->guardDrag(false);
		Map::CMapChipState::move(true);
		Map::CMapChipState::action(true);
	break;

	default:
		// それの効果範囲がSELFだったら、自身を対象キャラにして
		// すぐにリターンする
		p->setTargetChara(p->getCtrlChara());
		setState(RETURN);
	break;
	}
}

void CSpirit_select::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
	{
		set<int>& Set = p->getIndexSet();
		if(Set.find(p->getTargetMap())!=Set.end())
		{// 範囲内だったら、決定する
			p->push(0);
			actionReturn(pContext);
		}
		else
		{// 範囲外ならもう一度ね
			setState(NORMAL);
		}
	}
	break;

	case CANCEL:
		// キャンセル
		pContext->push(-1);
		actionReturn(pContext);
	break;

	case RETURN:
		pContext->push(0);
		getTaskListCtrl()->returnTaskList();
	break;

	default:
	{// 何もなければ、超簡易ステータス表示
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		set<int>& Set = p->getIndexSet();
		if(Set.find(p->getTargetMap())!=Set.end())
		{
			int nTarget	   = p->getTargetChara();
			if(nTarget>=0)
			{// カーソルがキャラチップの上にあったりする
			//じゃ、超簡易ステータス表示
				if(nID_!=nTarget){ changeStatus(p); }
			}
			else 
			{// キャラがいないなら、非表示に
				actionInValid(p);
			}
			nID_=nTarget;
		}
		else
		{// 範囲外なら、非表示に
			actionInValid(p);
			nID_=-1;
		}
	}
	break;
	}
}

void CSpirit_select::actionReturn(Task::CTaskContext* pContext)
{
	pContext->getInput()->guard(true);
	pContext->getInput()->guardDrag(true);
	Map::CMapChipState::move(false);
	Map::CMapChipState::action(false);

	// 後始末～
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->clearMove();
	
	p->getEvent()->validStatus(false, nSide_);
	nSide_=-1;

	getTaskListCtrl()->returnTaskList();
}

void CSpirit_select::rangeFriend(int nID,CSLGContext* pContext)
{
	// とりあえず、効果範囲をクリア
	pContext->clearMove();
	// 味方を効果範囲に
	rangeSet(nID,pContext->getPlayerPhaseList(),pContext);
}

void CSpirit_select::rangeEnemy(int nID,CSLGContext* pContext)
{
	// とりあえず、効果範囲をクリア
	pContext->clearMove();
	// 敵効果範囲に
	rangeSet(nID,pContext->getEnemyPhaseList(),pContext);
	rangeSet(nID,pContext->getNeutralPhaseList(),pContext);
}

void CSpirit_select::rangeSet(int nID, list<int>& List,CSLGContext* pContext)
{
	set<int>& Set = pContext->getIndexSet();
	list<int>::iterator it;
	int nIndex;
	CDataCharaSLG *pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = pContext->getCharaData(*it);
		if(pChara==NULL
		|| !pChara->IsExist()) continue;
		if(pContext->getApp()->getSpirit().enableTarget(*pChara,0,*pContext,nID))
		{// その対象に対して使用可能なら、そこをセレクト対象とする
			nIndex = pChara->getIndex();
			Set.insert(nIndex);
			pContext->getMapChip(nIndex)->getMapChipState()->setMove(1);
		}
	}
}

void CSpirit_select::changeStatus(CSLGContext* p)
{
	CDataCharaSLG* pTarget = p->getTargetCharaData();
	if(pTarget==NULL){ actionInValid(p); return; }
	// 現在表示中のを消す
	if(nSide_!=-1) p->getEvent()->validStatus(false, nSide_);
	nSide_ = pTarget->getPhase()==Phase::PLAYER ? 1 : 0;

	p->getEvent()->getStatus(nSide_).actionReset(*pTarget, CStatusCharaVeryEasy::MENTAL, pTarget->getBattle().getMental());
	p->getEvent()->validStatus(true, nSide_);
}

void CSpirit_select::actionInValid(CSLGContext* p)
{
	if(nSide_>=0)
	{
		p->getEvent()->validStatus(false, nSide_);
		nSide_=-1;
	}
}

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end