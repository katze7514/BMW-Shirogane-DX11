#include "stdafx.h"

#include "../../Scene/IScene.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"

#include "CDemo_map_battle.h"
#include "CDemo_map_spirit.h"
#include "CDemo_map.h"

namespace BMW{
namespace SLG{
namespace Demo{

CDemo_map::CDemo_map()
{}

CDemo_map::~CDemo_map()
{
	DELETE_SAFE(pPanel_);
}

void CDemo_map::OnReset(Task::CTaskContext* pContext)
{
	// インターフェイスゲット
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_DEMOOFF");
	// 敵パネル
	pLeft_ = pPanel_->getWidgetCast<GUI::CPanel>("ENEMY");
	pLeft_->getWidgetCast<GUI::INum>("DAMAGE")->plus(true);
	pLeftHead_ = pLeft_->getWidgetCast<GUI::CPanelCtrl>("HEADER");

	pChara_[0] = pLeft_->getWidget("CHIP");
	pEffect_[0] = pLeft_->getWidget("SERIF");
	

	// 味方パネル
	pRight_ = pPanel_->getWidgetCast<GUI::CPanel>("PLAYER");
	pRight_->getWidgetCast<GUI::INum>("DAMAGE")->plus(true);
	pRightHead_ = pRight_->getWidgetCast<GUI::CPanelCtrl>("HEADER");

	pChara_[1] = pRight_->getWidget("CHIP");
	pEffect_[1] = pRight_->getWidget("SERIF");
	
	// 各マップ上デモを担うクラスを生成
	taskCtrl_.setParent(smart_ptr<Task::ITaskBase>(this,false));
	// Battle用
	CDemo_map_battle* pBattle = new CDemo_map_battle();
	taskCtrl_.addTask(pBattle,BATTLE);
	pBattle->OnInit(pContext);
	// Cure用
	//CDemo_map_cure* pCure = new CDemo_map_cure();
	//taskCtrl_.addTask(pCure,CURE);
	//pCure->OnInit(pContext);
	// Refill用
	//CDemo_map_refill* pRefill = new CDemo_map_refill();
	//taskCtrl_.addTask(pRefill,REFILL);
	//pRefill->OnInit(pContext);
	// Spirit用（全部こいつに統合されている）
	CDemo_map_spirit* pSpirit = new CDemo_map_spirit();
	taskCtrl_.addTask(pSpirit,SPIRIT);
	pSpirit->OnInit(pContext);
	// Item用
	//CDemo_map_item* pItem = new CDemo_map_item();
	//taskCtrl_.addTask(pItem,ITEM);
	//pItem->OnInit(pContext);
}

void CDemo_map::OnInit(Task::CTaskContext* pContext)
{// デモシーン同様、ムービー生成をする
	setState(NORMAL);
	// まずは、どんな状況で呼び出されたかによって、処理を分ける
	// それはスタックトップに状況IDが積まれている
	taskCtrl_.setState(getDemoID(pContext->top()));
	// 戦闘以外だったらdup
	if(pContext->top()!=BATTLE)	pContext->push(pContext->top());
	pContext->pop();
	// 設定されたやつをリセット
	taskCtrl_.getCurrentTask()->OnReset(pContext);

	// デモ中のスキップはフラグ次第
	pContext->getApp()->animeSkip();
	pContext->getInput()->cursolVisible(false);

	// クイックロードできない
	static_cast<CSLGContext*>(pContext)->quickLoad(false);
}

void CDemo_map::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==END)
	{// ムービーが終わったら、return
		setState(NORMAL);
		getTaskListCtrl()->returnTaskList();
		// 終了したらskipを有効に
		pContext->getApp()->skip(true);
		pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
		// クイックロードできるように
		static_cast<CSLGContext*>(pContext)->quickLoad(true);
	}
}

void CDemo_map::callTaskAction(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
	taskCtrl_.Task(pContext);
}

void CDemo_map::callTaskDraw(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}

int CDemo_map::getDemoID(int nID)
{
	switch(nID)
	{
	case CURE:
	case REFILL:
	case ITEM: return SPIRIT;
	default: return nID;
	}
}

void CDemo_map::resetLeft()
{
	pLeft_->removeWidget("CHIP");
	pLeft_->addWidget(pChara_[0],"CHIP");
	pLeft_->removeWidget("SERIF");
	pLeft_->addWidget(pEffect_[0],"SERIF");
}

void CDemo_map::resetRight()
{
	pRight_->removeWidget("CHIP");
	pRight_->addWidget(pChara_[1],"CHIP");
	pRight_->removeWidget("SERIF");
	pRight_->addWidget(pEffect_[1],"SERIF");
}

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end