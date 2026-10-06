#include "stdafx.h"

#include "../../Scene/Unit/CYesNoUnit.h"
#include "../GUI/CStatusCharaVeryEasy.h"
#include "../Phase/CPhaseBall.h"
#include "../Context/CSLGContext.h"
#include "../Effect/CEffectInfo.h"

#include "CEvent.h"

namespace BMW{
namespace SLG{
namespace Event{

CEvent::~CEvent()
{ 
	listTask_.clear();
	DELETE_SAFE(pStatusChara_[0]);
	DELETE_SAFE(pStatusChara_[1]);
	DELETE_SAFE(pTurnBall_);
	DELETE_SAFE(pUnit_);

	effect_map::iterator it;
	for(it=mapEffect_.begin(); it!=mapEffect_.end(); ++it)
		DELETE_SAFE(it->second);

	mapEffect_.clear();
}

void CEvent::OnInit(CSLGContext* pContext)
{
	// メッセージ設定
	msgCtrl_.setParent(smart_ptr<Task::ITaskBase>(this,false));

	ADV::CMsgBoard* pMsg = new ADV::CMsgBoard();
	pMsg->setSide(ADV::CMsgBoard::LEFT);
	pMsg->OnInit(pContext);
	pMsg->getPanel()->setY(365); // SLG用の位置
	pMsg->visible(true);
	msgCtrl_.addTask(pMsg,ADV::CMsgBoard::LEFT);

	pMsg = new ADV::CMsgBoard();
	pMsg->setSide(ADV::CMsgBoard::RIGHT);
	pMsg->OnInit(pContext);
	pMsg->visible(true);
	msgCtrl_.addTask(pMsg,ADV::CMsgBoard::RIGHT);

	// 超簡易ステータス
	pStatusChara_[CStatusCharaVeryEasy::LEFT] = new CStatusCharaVeryEasy();
	pStatusChara_[CStatusCharaVeryEasy::LEFT]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pStatusChara_[CStatusCharaVeryEasy::LEFT]->setSide(CStatusCharaVeryEasy::LEFT);
	pStatusChara_[CStatusCharaVeryEasy::LEFT]->OnInit(pContext);

	pStatusChara_[CStatusCharaVeryEasy::RIGHT] = new CStatusCharaVeryEasy();
	pStatusChara_[CStatusCharaVeryEasy::RIGHT]->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pStatusChara_[CStatusCharaVeryEasy::RIGHT]->setSide(CStatusCharaVeryEasy::RIGHT);
	pStatusChara_[CStatusCharaVeryEasy::RIGHT]->OnInit(pContext);

	// ターンボール
	pTurnBall_ = new Phase::CPhaseBall();
	addTask(pTurnBall_,TURN);
	pTurnBall_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pTurnBall_->OnInit(pContext);
	pTurnBall_->OnReset(pContext);

	// YesNoダイアログ
	pUnit_ = new Unit::CYesNoUnit();
	pUnit_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pUnit_->OnInit(pContext);
}

///////////////////////////////////////////////
// 有効化
///////////////////////////////////////////////
// 有効化とか
void CEvent::validBoard(bool bValid)
{
	if(bValid){ // 表示
		if(getTask(MSG)==NULL) // 重複する可能性があるので
			addTask(&msgCtrl_,MSG);
	}
	else
	{// 非表示
		removeTask(MSG);
	}
}

void CEvent::validStatus(bool bValid, int nSide)
{
	if(bValid)
	{// 表示
		if(pStatusChara_[nSide]->getTaskPriority()!=-1)
			removeTask(VERY_EASY_STATUS_L+nSide);

		addTask(pStatusChara_[nSide], VERY_EASY_STATUS_L+nSide);
	}
	else
	{// 非表示
		Task::ITaskBase* pBase = removeTask(VERY_EASY_STATUS_L+nSide);
		if(pBase) pBase->setTaskPriority(-1);
	}
}

void CEvent::validYesNo(bool bValid)
{
	if(bValid) // 表示
		addTask(pUnit_, YES_NO);
	else		// 非表示
		removeTask(YES_NO);
}

////////////////////////////////////////////////
// エフェクト管理
////////////////////////////////////////////////
Effect::CEffectInfo* CEvent::getEffect(int nID)
{
	effect_map::iterator it = mapEffect_.find(nID);
	return it==mapEffect_.end() ? NULL : it->second;
}

void CEvent::addEffect(int nNo, Task::ITaskBase* pEffect, Task::ITaskList* pList)
{
	Effect::CEffectInfo* pInfo = new Effect::CEffectInfo();
	pInfo->pEffect_=pEffect;
	pInfo->pList_=pList;
	mapEffect_.insert(pair<int, Effect::CEffectInfo*>(nNo, pInfo));
}

void CEvent::delEffect(int nNo)
{
	Effect::CEffectInfo* pInfo = getEffect(nNo);
	DELETE_SAFE(pInfo);
	mapEffect_.erase(nNo);
}

} // namespace Event end
} // namespace SLG end
} // namespace BMW end