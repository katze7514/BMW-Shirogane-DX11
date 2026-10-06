#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "CDemoDamage.h"

namespace BMW{
namespace Demo{

CDemoDamage::~CDemoDamage()
{
	DELETE_SAFE(pNum_);
	DELETE_SAFE(pCritical_);
}

void CDemoDamage::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction() && IsValid()) OnAction(pContext);
	pNum_->Task(pContext);
	pCritical_->Task(pContext);
}

void CDemoDamage::OnInit(Task::CTaskContext* pContext)
{
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	pNum_ = db.getSymbolDB().createSymbolStrCast<GUI::CNum>("LR_N");
	pNum_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pNum_->setY(320);

	pCritical_ = db.getSymbolDB().createSymbolStr("CRITICAL_G");
	pCritical_->setParent(smart_ptr<Task::ITaskBase>(pNum_,false));
	pCritical_->visible(false);

	setState(NORMAL);
}

void CDemoDamage::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case VIEW:
		pNum_->setNum(++counter_);
		if(counter_.IsEnd()){ setState(WAIT); counter_.Set(0,15,15); }
	break;

	case WAIT:
		++counter_;
		if(counter_.IsEnd()){ setState(NORMAL); pNum_->visible(false); pCritical_->visible(false); }
	break;
	default: break;
	}
}

void CDemoDamage::action(int nSide, int nValue)
{
	const LONG BASE_X[2]={260,380};

	counter_.Set(0,nValue,15);
	pNum_->setNum(nValue);
	LONG lWidth,lHeight;
	pNum_->getDrawSize(lWidth, lHeight);
	pNum_->setX(BASE_X[nSide]+lWidth/2);
	pNum_->setNum(counter_);
	pNum_->visible(true);

	setState(VIEW);
}

void CDemoDamage::ct()
{
	pCritical_->OnReset(NULL);
	pCritical_->visible(true);
}

} // nmaespace Demo end
} // namespace BMW end