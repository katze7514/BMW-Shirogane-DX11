#include "stdafx.h"

#include "IDAction.h"
#include "Action.h"
#include "CActionFactory.h"

namespace BMW{
namespace SLG{
namespace Action{

IAction* CActionFactory::createAction(int nID, list<int>& listParam)
{
	switch(nID)
	{
	case PLAYER:		return new CActionSimple();
	case NOACT:			return new CActionNo();
	case WALL:			return new CActionWall();
	case WALL_COUNTER:	return new CActionWallCounter();

	// 基本的にParam化
	case WAIT:
	{
		CActionNormalParam* pAction = new CActionNormalParam();
		pAction->setWait(listParam.front());
		pAction->setMove(Action::MOVE);
		pAction->setSnipeChara(-1);
		return pAction;
	}

	case NORMAL:
	{
		CActionNormalParam* pAction = new CActionNormalParam();
		pAction->setWait(0);
		pAction->setMove(Action::MOVE);
		pAction->setSnipeChara(-1);
		return pAction;
	}

	case BATTERY:
	{
		CActionNormalParam* pAction = new CActionNormalParam();
		pAction->setWait(0);
		pAction->setMove(Action::MOVE_NO);
		pAction->setSnipeChara(-1);
		return pAction;
	}

	case NORMAL_PARAM:
	{	
		list<int>::iterator it = listParam.begin();
		CActionNormalParam* pAction = new CActionNormalParam();
		pAction->setWait(*it++);
		pAction->setMove(*it++);
		pAction->setSnipeChara(*it++);
		return pAction;
	}

	case NORMAL_EVAL:
	{	
		list<int>::iterator it = listParam.begin();
		CActionNormalEval* pAction = new CActionNormalEval();
		pAction->setWait(*it++);
		pAction->setMove(*it++);
		pAction->setSnipeChara(*it++);
		pAction->setEvalMove(*(it++)==SHORT?30:-30);
		pAction->setEvalAtk(*(it++)==SHORT?-20:20);
		pAction->setEvalHP(*(it++)==LOW?1:-1);
		return pAction;
	}

	case FIELD_PARAM:
	{
		list<int>::iterator it = listParam.begin();
		CActionFieldParam* pAction = new CActionFieldParam();
		pAction->setWait(*it++);
		pAction->setMove(*it++);
		pAction->setSnipeChara(*it++);
		pAction->setCharaNum(*it);

		return pAction;
	}

	case FIELD_EVAL:
	{
		list<int>::iterator it = listParam.begin();
		CActionFieldEval* pAction = new CActionFieldEval();
		pAction->setWait(*it++);
		pAction->setMove(*it++);
		pAction->setSnipeChara(*it++);
		pAction->setEvalMove(*(it++)==SHORT?30:-30);
		pAction->setEvalAtk(*(it++)==SHORT?-20:20);
		pAction->setEvalHP(*(it++)==LOW?1:-1);
		pAction->setCharaNum(*it);
		return pAction;
	}

	case AKABINE:
	{	
		CActionAkabine* pAction = new CActionAkabine();
		list<int>::iterator it = listParam.begin();
		pAction->setWait(*it++);
		pAction->setMove(*it++);
		pAction->setSnipeChara(*it);
		return pAction;
	}

	default: return NULL;
	}
}


IAction* CActionFactory::createAction()
{// シリアライズ情報に合わせた生成
	return createAction(nID_, listParam_);
}

void CActionFactory::Serialize(ISerialize& s)
{// 復元だけ
	if(!s.IsStoring())
	{// とりあえず、IDとパラメタはゲット
		s << nID_;
		// パラメタ数
		int n;
		s << n;
		// パラメタ取得
		listParam_.clear();
		int nParam;
		for(int i=0; i<n; ++i)
		{
			s << nParam;
			listParam_.push_back(nParam);
		}
	}
}

} // namespace Action end
} // namespace SLG end
} // namespace BMW end