#include "stdafx.h"

#include "CTextPopUp.h"

namespace BMW{
namespace GUI{

CTextPopUp::CTextPopUp()
{
	pButton_ = new GUI::IButton();
	pButton_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CTextPopUp::~CTextPopUp()
{
	DELETE_SAFE(pButton_);
}

void CTextPopUp::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid()) // ポップアップ動作
		{	pButton_->OnAction(pContext); }
	}
	else
	{
		if(IsVisible())
		{	OnDraw(pContext); }
	}
}

void CTextPopUp::UpdateText()
{
	CText::UpdateText();
	// 反応範囲の設定
	int nX,nY;
	text_.GetFont()->GetSize(nX,nY);
	pButton_->setRange(-nOffX_, 0, -nOffX_+nX, nY);
}

void CTextPopUp::UpdateTextA()
{
	CText::UpdateTextA();
	// 反応範囲の設定
	int nX,nY;
	text_.GetFont()->GetSize(nX,nY);
	pButton_->setRange(-nOffX_, 0, -nOffX_+nX, nY);
}

void CTextPopUp::UpdateTextAA()
{
	CText::UpdateTextAA();
	// 反応範囲の設定
	int nX,nY;
	text_.GetFont()->GetSize(nX,nY);
	pButton_->setRange(-nOffX_, 0, -nOffX_+nX, nY);
}

void CTextPopUp::setRange(const RECT& rect)
{
	pButton_->setRange(rect);
}

const string& CTextPopUp::getPopUp()const
{
	return pButton_->getPopUp();
}

string& CTextPopUp::getPopUp()
{
	return pButton_->getPopUp();
}

void CTextPopUp::setPopUp(const string& sPopUp)
{
	pButton_->setPopUp(sPopUp);
}

} // namespace GUI end
} // namespace BMW end