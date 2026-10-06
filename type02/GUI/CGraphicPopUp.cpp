#include "stdafx.h"

#include "CGraphicPopUp.h"

namespace BMW{
namespace GUI{

CGraphicPopUp::CGraphicPopUp()
{
	pButton_ = new GUI::IButton();
	pButton_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CGraphicPopUp::~CGraphicPopUp()
{
	DELETE_SAFE(pButton_);
}

void CGraphicPopUp::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid()) // ポップアップ動作
		{	pButton_->OnAction(pContext);  }
	}
	else
	{
		if(IsVisible())
		{	OnDraw(pContext); }
	}
}

void CGraphicPopUp::setRange(const RECT& rect)
{
	pButton_->setRange(rect);
}

const string& CGraphicPopUp::getPopUp()const
{
	return pButton_->getPopUp();
}

string& CGraphicPopUp::getPopUp()
{
	return pButton_->getPopUp();
}

void CGraphicPopUp::setPopUp(const string& sPopUp)
{
	pButton_->setPopUp(sPopUp);
}

} // namespace GUI end
} // namespace BMW end