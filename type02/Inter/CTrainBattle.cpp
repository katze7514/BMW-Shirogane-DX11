#include "stdafx.h"

#include "../Chara/ConstChara.h"
#include "../Scene/IScene.h"
#include "../Scene/GUI/CNumCtrl.h"

#include "train_fun.h"
#include "CInterContext.h"
#include "CInterChara.h"

#include "CChara.h"
#include "CTrainBattle.h"

namespace BMW{
namespace Inter{
namespace Chara{

namespace{
__inline void setBarEventHandler(GUI::CPanel* pPanel, const GUI::CButton::ButtonEvent& fun, int nDown, int nUp)
{
	GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("LEFT_DOWN"),fun,nDown);
	GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("RIGHT_UP"),fun,nUp);
}

__inline int sumArray(const int array[], int nSource, int nUp)
{
	int nSum=0;
	for(int i=nSource+1; i<=nSource+nUp; ++i)
		nSum += array[i];

	return nSum;
}

} // namespace end

int CTrainBattle::getPer(int nKind)
{
	switch(nKind)
	{
	case HP:	return BMW::Chara::Const::HP_TRAINING_VALUE;
	case EN:	return BMW::Chara::Const::EN_TRAINING_VALUE;
	case QUICK: return BMW::Chara::Const::QUICK_TRAINING_VALUE;
	case TOUGH: return BMW::Chara::Const::TOUGH_TRAINING_VALUE;
	default: return 0;
	}
}

int CTrainBattle::getTrainBP(int nKind, int nSource, int nUp)
{
	switch(nKind)
	{
	case HP:	return sumArray(BMW::Chara::Const::HP_TRAINING_BP,nSource,nUp);
	case EN:	return sumArray(BMW::Chara::Const::EN_TRAINING_BP,nSource,nUp);
	case QUICK: return sumArray(BMW::Chara::Const::QUICK_TRAINING_BP,nSource,nUp);
	case TOUGH: return sumArray(BMW::Chara::Const::TOUGH_TRAINING_BP,nSource,nUp);
	default: return 0;
	}
}
//////////////////////////////////////////////////////
// タスク
//////////////////////////////////////////////////////
void CTrainBattle::OnInit(Task::CTaskContext* pContext)
{
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CTrainBattle::eventButton);
	// インターフェイス取得
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_YOUSEI_BATTLE");
	pPanel_->setParent(getParent());
	// インターフェイス展開
	// バー
	pHP_ = pPanel_->getWidgetCast<GUI::CPanel>("HP");
	setBarEventHandler(pHP_,fun,HP_DOWN,HP_UP);
	pEN_ = pPanel_->getWidgetCast<GUI::CPanel>("EN");
	setBarEventHandler(pEN_,fun,EN_DOWN,EN_UP);
	pQuick_ = pPanel_->getWidgetCast<GUI::CPanel>("QUICK");
	setBarEventHandler(pQuick_,fun,QUICK_DOWN,QUICK_UP);
	pTough_ = pPanel_->getWidgetCast<GUI::CPanel>("TOUGH");
	setBarEventHandler(pTough_,fun,TOUGH_DOWN,TOUGH_UP);
	// BP
	pSum_ = pPanel_->getWidgetCast<GUI::CNum>("SUM");
	pRemain_ = pPanel_->getWidgetCast<GUI::CNumCtrl>("REMAIN");
	// ボタンイベントハンドラ
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("RESET"),fun,RESET);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("OK"),fun,OK);
}

void CTrainBattle::OnReset(Task::CTaskContext* pContext)
{
	pChara_ = static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData();
	// 養成段階取得
	const BMW::Chara::CStatusBattle& train = pChara_->getBattleTrain();
	// HP
	// 現在値
	pHP_->getWidgetCast<GUI::CNum>("NOW")->setNum(pChara_->getHP());
	nTrain_[HP][SOURCE]=train.getHP();
	nTrain_[HP][UP]=0;
	nTrain_[HP][BASE]=pChara_->getSourceHP();
	updateBar(pHP_,HP,pChara_->getHP(), pChara_->getItemHP());
	// EN
	pEN_->getWidgetCast<GUI::CNum>("NOW")->setNum(pChara_->getEN());
	nTrain_[EN][SOURCE]=train.getEN();
	nTrain_[EN][UP]=0;
	nTrain_[EN][BASE]=pChara_->getSourceEN();
	updateBar(pEN_,EN,pChara_->getEN(), pChara_->getItemEN());
	// Quick
	pQuick_->getWidgetCast<GUI::CNum>("NOW")->setNum(pChara_->getQuick());
	nTrain_[QUICK][SOURCE]=train.getQuick();
	nTrain_[QUICK][UP]=0;
	nTrain_[QUICK][BASE]=pChara_->getSourceQuick();
	updateBar(pQuick_,QUICK,pChara_->getQuick(), pChara_->getItemQuick());
	// TOUGH
	pTough_->getWidgetCast<GUI::CNum>("NOW")->setNum(pChara_->getTough());
	nTrain_[TOUGH][SOURCE]=train.getTough();
	nTrain_[TOUGH][UP]=0;
	nTrain_[TOUGH][BASE]=pChara_->getSourceTough();
	updateBar(pTough_,TOUGH,pChara_->getTough(), pChara_->getItemTough());
	// BP
	pSum_->setNum(0);
	pRemain_->setNum(pContext->getApp()->getExec().getBP());
	pRemain_->validNumGui(0/*白*/);
}

void CTrainBattle::actionUpdateBP(Task::CTaskContext* pContext)
{
	//CDbg().Out("BATTLE BP %d %d",pContext->getApp()->getExec().getBP(),pSum_->getNum());
	pRemain_->setNum(pContext->getApp()->getExec().getBP() - pSum_->getNum());
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

//////////////////////////////////////////////////////
// イベントハンドラ
//////////////////////////////////////////////////////
void CTrainBattle::decTrain(int nTrain, GUI::CPanel* pPanel, int nSource, int nItem)
{
	if(--nTrain_[nTrain][UP]<0) nTrain_[nTrain][UP]=0;
	else updateBar(pPanel,nTrain,nSource,nItem);
}

void CTrainBattle::incTrain(int nTrain, GUI::CPanel* pPanel, int nSource, int nItem)
{
	if(++nTrain_[nTrain][UP]>10-nTrain_[nTrain][SOURCE]) nTrain_[nTrain][UP]=10-nTrain_[nTrain][SOURCE];
	else updateBar(pPanel,nTrain,nSource,nItem);
}

void CTrainBattle::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押されたでー
		switch(pButton->getValue())
		{
		case HP_DOWN:		decTrain(HP,pHP_,pChara_->getHP(),pChara_->getItemHP()); break;
		case HP_UP:			incTrain(HP,pHP_,pChara_->getHP(),pChara_->getItemHP()); break;
		case EN_DOWN:		decTrain(EN,pEN_,pChara_->getEN(),pChara_->getItemEN()); break;
		case EN_UP:			incTrain(EN,pEN_,pChara_->getEN(),pChara_->getItemEN()); break;
		case QUICK_DOWN:	decTrain(QUICK,pQuick_,pChara_->getQuick(),pChara_->getItemQuick()); break;
		case QUICK_UP:		incTrain(QUICK,pQuick_,pChara_->getQuick(),pChara_->getItemQuick()); break;
		case TOUGH_DOWN:	decTrain(TOUGH,pTough_,pChara_->getTough(),pChara_->getItemTough()); break;
		case TOUGH_UP:		incTrain(TOUGH,pTough_,pChara_->getTough(),pChara_->getItemTough()); break;

		case RESET:
			// 全UPを0に
			nTrain_[HP][UP]=0;
			updateBar(pHP_,HP,pChara_->getHP(),pChara_->getItemHP());
			nTrain_[EN][UP]=0;
			updateBar(pEN_,EN,pChara_->getEN(),pChara_->getItemEN());
			nTrain_[QUICK][UP]=0;
			updateBar(pQuick_,QUICK,pChara_->getQuick(),pChara_->getItemQuick());
			nTrain_[TOUGH][UP]=0;
			updateBar(pTough_,TOUGH,pChara_->getTough(),pChara_->getItemTough());
		break;

		case OK:
			// 決定！
			if(pRemain_->getNum()>=0)
			{// 養成できるのであれば
				// 養成データ反映
				pChara_->calcHP(nTrain_[HP][UP]);
				pChara_->calcEN(nTrain_[EN][UP]);
				pChara_->calcQuick(nTrain_[QUICK][UP]);
				pChara_->calcTough(nTrain_[TOUGH][UP]);
				// セーブデータへ反映
				//pContext->getApp()->getExec().getTrainData(pChara_->getID(),false)->back(pChara_);
				// BP減らす
				pContext->getApp()->getExec().setBP(pRemain_->getNum());
				// ステータスへ反映
				fun_(CChara::S_HEAD,pContext);
				fun_(CChara::S_EASY,pContext);
				fun_(CChara::S_BATTLE,pContext);
				fun_(CChara::S_BP,pContext);
				CInterContext* p = static_cast<CInterContext*>(pContext);
				p->getTargetCharaData()->OnReset(p);
				// パネルへ反映
				OnReset(pContext);
			}
		break;

		default: break;
		}
	}
}
//////////////////////////////////////////////////////
// インターフェイス更新
//////////////////////////////////////////////////////
void CTrainBattle::updateBar(GUI::CPanel* pPanel,int nFirst, int nSource, int nItem)
{
	// バー
	updateTrainBar(pPanel->getWidgetCast<GUI::CPanel>("YOUSEI_LEVEL"),nTrain_[nFirst][SOURCE],nTrain_[nFirst][UP]);
	// UP
	GUI::CNumCtrl* pCtrl;
	pCtrl=pPanel->getWidgetCast<GUI::CNumCtrl>("UP");
	pCtrl->setNum(nTrain_[nFirst][BASE] + BMW::Chara::calcPer(nTrain_[nFirst][BASE], getPer(nFirst), nTrain_[nFirst][SOURCE]+nTrain_[nFirst][UP]) + nItem);
	pCtrl->validNumGui(pCtrl->getNum()>nSource?1/*緑*/:0/*白*/);
	// ボタン
	// 左
	Task::ITaskBase* pButton = pPanel->getWidget("LEFT_DOWN");
	pButton->valid(nTrain_[nFirst][UP]!=0);
	pButton->visible(nTrain_[nFirst][UP]!=0);
	// 右
	pButton = pPanel->getWidget("RIGHT_UP");
	pButton->valid(nTrain_[nFirst][SOURCE]+nTrain_[nFirst][UP]!=10);
	pButton->visible(nTrain_[nFirst][SOURCE]+nTrain_[nFirst][UP]!=10);

	// BP計算
	GUI::CNum* pNum = pPanel->getWidgetCast<GUI::CNum>("BP");
	// 一度、残BPを元に戻す
	pRemain_->setNum(pRemain_->getNum() + pSum_->getNum());
	// 合計値から、旧の値を取り除く
	pSum_->setNum(pSum_->getNum() - pNum->getNum());
	// 消費BP計算
	pNum->setNum(getTrainBP(nFirst,nTrain_[nFirst][SOURCE],nTrain_[nFirst][UP]));
	// 合計値計算
	pSum_->setNum(pSum_->getNum() + pNum->getNum());
	// 残BP再計算
	pRemain_->setNum(pRemain_->getNum() - pSum_->getNum());
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end