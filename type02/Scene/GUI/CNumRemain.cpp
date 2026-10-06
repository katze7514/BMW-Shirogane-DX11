#include "stdafx.h"

#include "CNumCtrl.h"
#include "CNumRemain.h"

namespace BMW{
namespace GUI{

CNumRemain::CNumRemain():nTurn_(0)
{
	pCurrent_ = new CNumCtrl();
	pCurrent_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	GUI::CNum* pNum = new GUI::CNum();
	pCurrent_->addNumGui(pNum,WHITE);
	pNum = new GUI::CNum();
	pCurrent_->addNumGui(pNum,RED);

	pMax_ = new CNum();
	pMax_->setParent(smart_ptr<Task::ITaskBase>(this,false));

	pSlash_ = new CGraphic();
	pSlash_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CNumRemain::~CNumRemain()
{
	DELETE_SAFE(pCurrent_);
	DELETE_SAFE(pMax_);
	DELETE_SAFE(pSlash_);
}

void CNumRemain::OnDraw(Task::CTaskContext* pContext)
{
	pSlash_->Task(pContext);
	pMax_->Task(pContext);
	pCurrent_->Task(pContext);
}

LONG CNumRemain::getCurrentNum() const
{ 
	return pCurrent_->getNum();
}

void CNumRemain::setCurrentNum(LONG lNum)
{ 
	pCurrent_->setNum(lNum);

	// Œ»İ’l‚ª•~‹’l%ˆÈ‰º‚É‚È‚Á‚½‚çF‚ğ•Ï‚¦‚é
	if(getRate() <= (double)getTurn()/100.0)
		pCurrent_->validNumGui(RED);
	else 
		pCurrent_->validNumGui(WHITE);
}

double CNumRemain::getRate()
{
	return getMaxNum()==0 ? 0.0 : ((double)getCurrentNum()/(double)getMaxNum());
}

} // namespace GUI end
} // namespace BMW end