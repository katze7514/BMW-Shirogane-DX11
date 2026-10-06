#include "stdafx.h"

#include "../../Scene/GUI/CGage.h"

#include "CDemoEasyStatus.h"

namespace BMW{
namespace Demo{

void CDemoEasyStatus::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction() && IsValid())
	{
		OnAction(pContext);
	}
	ef(!pContext->IsAction() && IsVisible())
	{
		pPanel_->Task(pContext);
	}
}

void CDemoEasyStatus::OnInit(Task::CTaskContext* pContext)
{
	SLG::CStatusCharaVeryEasy::OnInit(pContext);
	// ƒQ[ƒW‚ÌŽæ“¾
	pHP_ = pPanel_->getWidgetCast<GUI::CGage>("HP");
	pEN_ = pPanel_->getWidgetCast<GUI::CGage>("EN");

	nY_ = pPanel_->getDrawInfo().getY();
}

void CDemoEasyStatus::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case HP_DOWN:
		counter_++;
		if(counter_.IsEnd()) setState(NORMAL);
		pHP_->setCurrentNum(counter_);
		pHP_->actionChangeNum();
	break;

	case EN_DOWN:
		counter_++;
		if(counter_.IsEnd()) setState(NORMAL);
		pEN_->setCurrentNum(counter_);
		pEN_->actionChangeNum();
	break;

	case HPEN_DOWN:
		counter_++;
		counter2_++;

		if(counter_.IsEnd() && counter2_.IsEnd())
			setState(NORMAL);

		pHP_->setCurrentNum(counter_);
		pHP_->actionChangeNum();

		pEN_->setCurrentNum(counter2_);
		pEN_->actionChangeNum();
	break;

	case INTRO:
	case EXIT:
		pPanel_->setY(++y_);
		if(y_.IsEnd()) setState(NORMAL);
	break;

	default: break;
	}
}

void CDemoEasyStatus::action(int nDel, int nState, int nDel2)
{
	setState(nState);
	if(nState==HP_DOWN)
	{
		nDel=pHP_->getCurrentNum()-nDel;
		if(nDel<0) nDel=0;
		counter_.Set(pHP_->getCurrentNum(), nDel,15);
	}
	ef(nState==EN_DOWN)
	{
		nDel=pEN_->getCurrentNum()-nDel;
		if(nDel<0) nDel=0;
		counter_.Set(pEN_->getCurrentNum(), nDel,15);
	}
	ef(nState==HPEN_DOWN)
	{
		nDel=pHP_->getCurrentNum()-nDel;
		if(nDel<0) nDel=0;
		counter_.Set(pHP_->getCurrentNum(), nDel,15);

		nDel2=pEN_->getCurrentNum()-nDel2;
		if(nDel2<0) nDel2=0;
		counter2_.Set(pEN_->getCurrentNum(), nDel2, 15);
	}
	ef(nState==INTRO)
	{
		y_.Set(pPanel_->getDrawInfo().getY(), nY_, 5);
	}
	ef(nState==EXIT)
	{
		y_.Set(pPanel_->getDrawInfo().getY(), -40-nY_, 5);
	}
}

} // namespace Demo end
} // namespace BMW end