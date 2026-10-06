#include "stdafx.h"

#include "../../Scene/IScene.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Effect/CEffectMovieClip.h"

#include "CPhaseBall.h"

namespace BMW{
namespace SLG{
namespace Phase{

CPhaseBall::~CPhaseBall()
{
	DELETE_SAFE(pButton_);
	DELETE_SAFE(pBall_);
	DELETE_SAFE(pEffect_);
}

void CPhaseBall::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			pButton_->OnAction(pContext);
			pBall_->Task(pContext);
			OnAction(pContext);
		}
	}
	else
	{
		if(IsVisible()) pBall_->Task(pContext);
	}
}

void CPhaseBall::OnInit(Task::CTaskContext* pContext)
{
	pButton_->setRange(-30,-30,30,30);
	// インターフェイス生成
	pBall_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("TURNBALL");
	pButton_->setParent(smart_ptr<Task::ITaskBase>(pBall_,false));
	nEndY_ = pBall_->getDrawInfo().getY();

	// 数字のアルファを下げる
	pTurn_ = pBall_->getWidgetCast<GUI::CPanelCtrl>("TURN");
	// 70%
	pTurn_->setAlpha(255*7/10);

	// エフェクト
	pEffect_ = static_cast<CSLGContext*>(pContext)->getEffectDB().createEffect("PHOTON");
	pEffect_->remove(true);
	pBall_->swapWidget(pEffect_,"PHOTON");
	pBall_->removeWidget("PHOTON");
}

void CPhaseBall::OnReset(Task::CTaskContext* pContext)
{// 現在の状態に合わせて状況設定
	// エフェクト設定
	pEffect_->OnReset(pContext);
	pBall_->addWidget(pEffect_,"PHOTON");
	setState(EFFECT);
}

void CPhaseBall::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EFFECT:
		if(pEffect_->getTask(0)->getState()==1)
		{// キーフレ二つ目で入れ替え
			setState(EFFECT_END);
			update(pContext);
		}
	break;
	
	case EFFECT_END:
		if(pEffect_->IsEnd())
			setState(NORMAL);
	break;

	case MOVE:
		if(y_.IsEnd())
			setState(NORMAL);
		else
			pBall_->setY(++y_);
	
	break;
	}
}

namespace{

__inline void setText(string& s, const string& sText, int n)
{
	s += Misc::linkStrAndNum(sText,n);
	s += "\n";
}

__inline void setPhase(string& s, int nPhase)
{
	s+="フェーズ　：";
	switch(nPhase)
	{
	case Phase::ENEMY:
		s+="敵";
	break;

	case Phase::NEUTRAL:
		s+="中立";
	break;

	default:
		s+="味方";
	break;
	}
	s+="\n";
}

} // namespace end

void CPhaseBall::update(Task::CTaskContext* p)
{
	// フェーズ
	pBall_->getWidgetCast<GUI::CPanelCtrl>("COLOR")->validWidget(p->getValue(Flag::PHASE));
	// ターン
	int nTurn = p->getValue(Flag::TURN);
	pTurn_->getWidgetCast<GUI::INum>(nTurn<10?"TURN1":"TURN2")->setNum(nTurn);
	pTurn_->validWidget(nTurn<10?"TURN1":"TURN2");

	string s;
	// ターン数
	setText(s,"ターン数　：",p->getValue(Flag::TURN));
	// フェーズ
	setPhase(s,p->getValue(Flag::PHASE));
	// BP
	setText(s,"BP　　　　：",p->getValue(Flag::BP));
	// FP
	setText(s,"FP　　　　：",p->getValue(Flag::FP));
	// 熟練度
	setText(s,"熟練度　　：",p->getValue(Flag::EXPERT));

	// 周回プレイ時は敵養成段階も
	if(p->getApp()->getExec().getFlag("HANDOVER",1) || p->getApp()->getExec().getFlag("HANDOVER",2))
	{
		// 熟練度
		int nTrain=0;
		p->getApp()->getExec().getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nTrain);
		setText(s,"敵養成段階：",nTrain);
	}

	// ポップアップ設定
	pButton_->setPopUp(s);
}

void CPhaseBall::intro(bool bIntro)
{
#ifdef BMW_DEBUG
	CDbg().Out("PhaseIntro %d",bIntro);
#endif
	Draw::CDrawInfo info = pBall_->getDrawInfo();
	// 登場
	if(bIntro) y_.Set(info.getY(),nEndY_,15);
	// 退場
	else y_.Set(info.getY(),-nEndY_,10);
	pBall_->setY(y_);

	setState(MOVE);
}

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end