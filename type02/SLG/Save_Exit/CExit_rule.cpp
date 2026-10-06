#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/Unit/CExitUnit.h"
#include "../CSLGScene.h"
#include "CExit_rule.h"

namespace BMW{
namespace SLG{
namespace Exit{

CExit_rule::~CExit_rule()
{
	DELETE_SAFE(pUnit_);
}

void CExit_rule::Task(Task::CTaskContext* pContext)
{
	pUnit_->Task(pContext);
}

void CExit_rule::OnReset(Task::CTaskContext* pContext)
{
	pUnit_ = new Unit::CExitUnit();
	pUnit_->OnInit(pContext);
	// イベントハンドラ設定
	Unit::CExitUnit::ExitEvent fun;
	fun.set(this,&CExit_rule::eventExit);
	pUnit_->setEventHandler(fun);
}

void CExit_rule::OnInit(Task::CTaskContext* pContext)
{
	// 座標
	int nX,nY;
	pContext->getInput()->getCursolPos(nX,nY);
	pUnit_->setX(nX);
	pUnit_->setY(nY);
	// 登場設定
	pUnit_->OnReset(pContext);
}

/////////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////////
void CExit_rule::eventExit(int nState, Task::CTaskContext* pContext)
{// Unitの反応あり！
	switch(nState)
	{
	case Unit::CExitUnit::TITLE:
		pContext->push(Victory::NO);
		pContext->getScene()->setState(CSLGScene::END);
		pContext->getInput()->guard(true);
		pContext->getInput()->cursolVisible(false);
	break;

	case Unit::CExitUnit::END:
		pContext->getApp()->end();
	break;

	case Unit::CExitUnit::CANCEL:
		pContext->getInput()->guard(true);
		getTaskListCtrl()->returnTaskList();
	break;

	default:
	break;
	}
}

/////////////////////////////////////////////////////////
// アクセッサ
/////////////////////////////////////////////////////////
bool CExit_rule::IsValid()const{ return pUnit_->IsValid(); }
void CExit_rule::valid(bool bV){ pUnit_->valid(bV); }
bool CExit_rule::IsVisible()const{ return pUnit_->IsVisible(); }
void CExit_rule::visible(bool bV){ pUnit_->visible(bV); }

} // namespace Exit end
} // namespace SLG end
} // namespace BMW end