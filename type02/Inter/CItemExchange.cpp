#include "stdafx.h"

#include "../Scene/IScene.h"
#include "../Scene/Unit/IDHelp.h"

#include "CInterContext.h"
#include "CInterChara.h"
#include "CDataItemInter.h"

#include "item_fun.h"
#include "CChara.h"
#include "CItemExchange.h"

namespace BMW{
namespace Inter{
namespace Chara{
///////////////////////////////////////////
// タスク
///////////////////////////////////////////
void CItemExchange::Task(Task::CTaskContext* pContext)
{ 
	pPanel_->Task(pContext);
	if(pContext->IsAction() && IsValid()) OnAction(pContext);
}

void CItemExchange::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_ITEM2");
	pPanel_->setParent(getParent());
	// インターフェイス展開
	pNowStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CURRENT_STATUS");
	pUpStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CHANGE_STATUS");
}

void CItemExchange::OnReset(Task::CTaskContext* pContext)
{
	if(!pContext->getValue(Flag::INITIALIZE)) return;
	if(pContext->getValue(Flag::TARGET_CHARA)>=0
	&& pContext->getValue(Flag::TARGET_ITEM)>=0)
	{// 装備対象と装備アイテムが設定されてるなら行う
		pContext->getApp()->getFoward()->clearPopUp();

		pContext->setValue(1,Flag::EXCHANGE);

		CInterContext* p = static_cast<CInterContext*>(pContext);
		p->setExchangeChara(-1);
		BMW::Chara::CDataCharaInter* pChara = p->getTargetCharaData()->getData();
		CDataItemInter* pItem = p->getExchangeItemData();

		// 交換アイテム設定
		Item::CItemDB& db = pContext->getApp()->getItem();
		int nID = pChara->hasItemAttr(p->getExchangeItemAttr());
		// 交換場所アイテム
		if(nID<0) // こっちだったらホルダ
			p->getScene()->getGuiDefDB().getSymbolDB().setGraphicGui(pPanel_->getWidgetCast<GUI::CGraphicPopUp>("SETITEM"),"PERGE_G");
		else // 普通にアイテム
			db.setGraphicHolder(pPanel_->getWidgetCast<GUI::CGraphicPopUp>("SETITEM"),nID);

		// 交換後アイテム
		db.setGraphicHolder(pPanel_->getWidgetCast<GUI::CGraphicPopUp>("CHANGEITEM"),pItem->getID());

		// 持ってるキャラ一覧生成
		pView_ = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("SET_CHARAS");
		pPanel_->swapWidget(pView_,"SET_CHARAS");
		CInterChara* pInter;
		GUI::CPanel* pChip;
		GUI::CButton::ButtonEvent fun;
		fun.set(this,&CItemExchange::eventButton);

		int nTarget=p->getTargetCharaData()->getData()->getTrainData()->getID();
		int nPos=-1;
		int nBackID=-1;
		GUI::CButton* pButton;
		pItem->beginChara();
		while(!pItem->endChara())
		{
			nID = *pItem->nextChara();
			// 前と同じだったり、自身だったらスキップ
			if(nID==nBackID || nID==nTarget) continue;
			nPos++;
			nBackID=nID;
			// キャラID設定
			anCharaID_[nPos]=nID;
			pInter = p->getCharaData(nID);
			pChip = pInter->getChipSort();
			pChip->getWidget("SORT")->visible(false);
			// 場所設定
			pButton = pChip->getWidgetCast<GUI::CButton>("CHIP");
			pButton->setPopUp("所持数："+CStringScanner::NumToString(pItem->countChara(nID)));
			GUI::CButton::setButtonEvent(pButton,fun,nPos);
			pView_->swapWidget(pChip,Misc::linkStrAndNum("CHIP",nPos+1));
		}

		for(int i=nPos+1; i<9; ++i)
		{// 残りは削除
			pView_->delWidget(Misc::linkStrAndNum("CHIP",i+1));
		}

		if(nPos<0)
		{// nPosの設定が無いときは交換する必要がない
			actionBack(pContext);
			return;
		}

		// ステータスは非表示
		pNowStatus_->visible(false);
		pUpStatus_->visible(false);

		setState(NORMAL);

		// ヘルプモード
		Unit::Help::callHelp(Unit::Help::INTER_ITEM_CHANGE, "INTER_ITEM_CHANGE", pContext);
	}
	else
	{// 設定されてなかったら戻る
		fun_(CChara::S_ITEM_END,pContext);
	}
}

void CItemExchange::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==END)
	{// 終了
		actionBack(pContext);
	}
	ef(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
	{// キャンセルされたら戻る
		// 後始末
		setState(END);
	}
}

///////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
void CItemExchange::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		// けってー
		actionOK(pContext);
		setState(END);
	}
	ef(GUI::IsOverIn(pButton))
	{// オーバー
		pContext->setValue(pButton->getValue(),Flag::EXCHANGE_CHARA);
		// ステータス変更
		actionStatus(pContext);
		// ステータス表示
		pNowStatus_->visible(true);
		pUpStatus_->visible(true);
	}
	ef(GUI::IsOverOut(pButton))
	{// はずれー
		if(pContext->getValue(Flag::EXCHANGE_CHARA)!=pButton->getValue()) return;

		pContext->setValue(-1,Flag::EXCHANGE_CHARA);
		// ステータス非表示
		pNowStatus_->visible(false);
		pUpStatus_->visible(false);
	}
}

//////////////////////////////////////////
// アクション
//////////////////////////////////////////
void CItemExchange::actionOK(Task::CTaskContext* pContext)
{// けってー
	CInterContext* p = static_cast<CInterContext*>(pContext);
	CInterChara* pT = p->getTargetCharaData();
	BMW::Chara::CDataCharaInter* pTarget = pT->getData();
	CInterChara* pE = p->getCharaData(anCharaID_[p->getExchangeChara()]);
	BMW::Chara::CDataCharaInter* pEx = pE->getData();
	Item::CItemDB& db = p->getApp()->getItem();
	CDataItemInter* pItem = p->getExchangeItemData();
	// まず、アイテムをはずす
	pEx->delItemID(pItem->getID(),db);
	pItem->delChara(pEx->getTrainData()->getID()/*pEx->getID()*/);

	// 交換ー
	// 交換場所にアイテムあり？
	int nID = pTarget->hasItemAttr(p->getExchangeItemAttr());
	if(nID>=0)
	{// あるならはずす
		pTarget->delItemID(nID,db);
		p->getItemData(nID)->delChara(pTarget->getID());
	}
	// 装備
	pTarget->addItem(pItem->getID(),p->getExchangeItemAttr(),db);
	pItem->addChara(pTarget->getTrainData()->getID()/*pTarget->getID()*/);

	// セーブデータへ反映
	//p->getApp()->getExec().getTrainData(pEx->getID(),false)->back(pEx);
	//p->getApp()->getExec().getTrainData(pTarget->getID(),false)->back(pTarget);

	// 各種ステータス反映
	fun_(CChara::S_EASY,pContext);
	fun_(CChara::S_BASE,p);
	fun_(CChara::S_BATTLE,pContext);
	fun_(CChara::S_WEAPON,pContext);
	pT->OnReset(p);
	pE->OnReset(p);
}

void CItemExchange::actionStatus(Task::CTaskContext* pContext)
{// ステータス
	CInterContext* p = static_cast<CInterContext*>(pContext);
	BMW::Chara::CDataCharaInter* pChara = p->getCharaData(anCharaID_[p->getExchangeChara()])->getData();
	setStatus(pNowStatus_,pChara);
	Item::CItemDB& db = p->getApp()->getItem();
	db.backStatus(*pChara,0,p->getExchangeItemID());
	setStatus(pUpStatus_,pChara);
	db.applyStatus(*pChara,0,p->getExchangeItemID());
	setStatusColor(pNowStatus_,pUpStatus_);
}

void CItemExchange::actionBack(Task::CTaskContext* pContext)
{// 戻る時は、SET_CHARASをどうにかしていく
	pView_->emptyWidget();
	fun_(CChara::S_ITEM_END,pContext);
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end