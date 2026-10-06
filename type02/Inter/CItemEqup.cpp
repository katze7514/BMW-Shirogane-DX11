#include "stdafx.h"

#include "../Scene/IScene.h"

#include "CInterContext.h"
#include "CInterChara.h"
#include "CDataItemInter.h"

#include "item_fun.h"
#include "CChara.h"
#include "CItemEqup.h"

namespace BMW{
namespace Inter{
namespace Chara{

namespace{
__inline void setItemEventHandler(GUI::CPanel* pPanel, int nNum, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	for(int i=0; i<nNum; ++i)
		GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("ITEM",i+1)),
									 fun,
									 nValue+i);
}

__inline void setItemIcon(GUI::CButtonSymbol* pButton, int nID, Task::CTaskContext* p)
{
	if(nID<0)	p->getScene()->getGuiDefDB().setButtonHolder(pButton,"EMPTY_ITEM");
	else		p->getApp()->getItem().setButtonHolder(pButton,nID);

	pButton->OnReset(p);
}

} // namespace end
///////////////////////////////////////////
// タスク
///////////////////////////////////////////
void CItemEqup::OnInit(Task::CTaskContext* pContext)
{
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CItemEqup::eventButton);
	// インターフェイス
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_ITEM1");
	pPanel_->setParent(getParent());
	// アイテム一覧設定
	pGet_ = pPanel_->getWidgetCast<GUI::CPanel>("ITEM_VIEW");
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW1"),7,fun,0);
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW2"),7,fun,7);
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW3"),7,fun,14);
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW4"),7,fun,21);

	pItemNum_ = pPanel_->getWidgetCast<GUI::CNumRemain>("ITEM_NUMBER");

	// 装備しているアイテム
	pHave_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("SET_ITEMS");
	fun.set(this,&CItemEqup::eventHave);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET1"),1,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET2"),2,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET3"),3,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET4"),4,fun,0);

	// ステータス
	pNowStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CURRENT_STATUS");
	pUpStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CHANGE_STATUS");
}

void CItemEqup::OnReset(Task::CTaskContext* pContext)
{
	// 一端、ポップアップクリア
	pContext->getApp()->getFoward()->clearPopUp();
	// アイテム装備設定
	CInterContext* p = static_cast<CInterContext*>(pContext);
	p->setTargetItem(-1);
	p->setCtrlItem(-1);
	nHave_=-1;
	// 現在のアイテムリストに合わせてまずは、アイテム一覧生成
	if(!p->IsLoad()) initItem(p);

	// 所持アイテム
	BMW::Chara::CDataCharaInter* pChara = p->getTargetCharaData()->getData();
	pHave_->visible(true);
	pHave_->valid(true);
	// 持てる最大数によって変更
	switch(pChara->getItemMax())
	{
	case 1:
		pHave_->validWidget("SET1");
		anHaveID_[0]=pChara->hasItemAttr(0);
		setItemIconList(1,pContext);
	break;

	case 2:
		pHave_->validWidget("SET2");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		setItemIconList(2,pContext);
	break;

	case 3:
		pHave_->validWidget("SET3");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		anHaveID_[2]=pChara->hasItemAttr(2);
		setItemIconList(3,pContext);
	break;

	case 4:
		pHave_->validWidget("SET4");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		anHaveID_[2]=pChara->hasItemAttr(2);
		anHaveID_[3]=pChara->hasItemAttr(3);
		setItemIconList(4,pContext);
	break;

	default:
		pHave_->visible(false);
		pHave_->valid(false);
		anHaveID_[0]=-1;
		p->setCtrlItem(-1);
	break;
	}
	// アイテム個数
	pItemNum_->getCurrentNumGui()->visible(false);
	pItemNum_->getMaxNumGui()->visible(false);

	// はじめの空きスペースを装備箇所とする
	for(int i=0; i<pChara->getItemMax(); ++i)
		if(anHaveID_[i]<0){ p->setCtrlItem(i); break; }

	// ステータス
	updateStatus(pChara);
}

void CItemEqup::initItem(CInterContext* p)
{
	// 現在のアイテムリストに合わせてまずは、アイテム一覧生成
	Item::CItemDB& db = p->getApp()->getItem();
	typedef CInterContext::item_map item_map;
	item_map& mapItem = p->getItemMap();
	// とりあえず、全部止めておく
	GUI::CPanel* pPanel;
	pPanel = pGet_->getWidgetCast<GUI::CPanel>("ROW1");
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	pPanel = pGet_->getWidgetCast<GUI::CPanel>("ROW2");
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	pPanel = pGet_->getWidgetCast<GUI::CPanel>("ROW3");
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	pPanel = pGet_->getWidgetCast<GUI::CPanel>("ROW4");
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	
	// アイテムアイコン設定
	int nPos=0;
	GUI::CButtonSymbol* pButton;
	item_map::iterator it;
	for(it=mapItem.begin(); it!=mapItem.end(); ++it, ++nPos)
	{
		anGetID_[nPos]=it->second->getID();
		pButton = getItemButton(nPos+1);
		//CDbg().Out("ItemButton %d %d",nPos+1, it->second->getID());
		db.setButtonHolder(pButton, it->second->getID());
		pButton->setState(GUI::CButton::NORMAL);
		pButton->valid(true);
		pButton->visible(true);
		pButton->OnReset(p);
	}

	p->load(true);
}

///////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
void CItemEqup::eventHave(const smart_ptr<GUI::CEventButton>& pButton ,Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(anHaveID_[pButton->getValue()]>=0)
		{// そこにアイテムがあったらREMOVE
			pContext->setValue(pButton->getValue(),Flag::CTRL_ITEM);
			pContext->setValue(REMOVE,Flag::TARGET_ITEM);
			actionOK(pContext);
		}
	}
	ef(GUI::IsOverIn(pButton))
	{// マウスオーバー
		if(nHave_<0)
		{// 対象が選択されたら、現在の状態にあわせてステータスとか変更
			nHave_=pButton->getValue();
			int n = pContext->getValue(Flag::CTRL_ITEM);
			pContext->setValue(nHave_,Flag::CTRL_ITEM);
			pContext->setValue(REMOVE,Flag::TARGET_ITEM);
			actionStatus(pContext);
			pContext->setValue(n,Flag::CTRL_ITEM);
		}
	}
	ef(GUI::IsOverOut(pButton))
	{// マウスアウト
		// 別のが設定されてたら、リターン
		if(nHave_<0 || nHave_==pButton->getValue())
		{// そうじゃなかったら、キャンセルと同じ
			nHave_=-1;
			int n = pContext->getValue(Flag::CTRL_ITEM);
			pContext->setValue(-1,Flag::CTRL_ITEM);
			pContext->setValue(-1,Flag::TARGET_ITEM);
			actionStatus(pContext);
			pContext->setValue(n,Flag::CTRL_ITEM);
		}
	}
}

void CItemEqup::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(pContext->getValue(Flag::CTRL_ITEM)>=0)
		{// 装備対象が決まってたら、アイテム装備決定
			pContext->setValue(pButton->getValue(),Flag::TARGET_ITEM);

			if(pButton->getValue()!=REMOVE)
				getItemButton(pButton->getValue()+1)->OnReset(pContext);
			else
				pGet_->getWidgetRec("ROW4/CHANGEITEM")->OnReset(pContext);

			actionOK(pContext);
		}
	}
	ef(GUI::IsOverIn(pButton))
	{// マウスオーバー
		if(pContext->getValue(Flag::TARGET_ITEM)<0
		|| pContext->getValue(Flag::TARGET_ITEM)!=pButton->getValue())
		{// 現在の状態にあわせてステータスとか変更
			pContext->setValue(pButton->getValue(),Flag::TARGET_ITEM);
			actionStatus(pContext);
		}
	}
	ef(GUI::IsOverOut(pButton))
	{// マウスアウト
		if(pContext->getValue(Flag::TARGET_ITEM)<0
		|| pContext->getValue(Flag::TARGET_ITEM)==pButton->getValue())
		{
			pContext->setValue(-1,Flag::TARGET_ITEM);
			actionStatus(pContext);
		}
	}
}

///////////////////////////////////////////
// アクション
///////////////////////////////////////////
namespace{

__inline void removeItem(BMW::Chara::CDataCharaInter* pChara, int nID, int nAttr, CInterContext* p)
{
	// 装備アイテムをはずす
	pChara->delItem(nAttr,p->getApp()->getItem());
	p->getItemData(nID)->delChara(pChara->getTrainData()->getID()/*pChara->getID()*/);
}

} // namespace end
void CItemEqup::actionOK(Task::CTaskContext* pContext)
{// 決定ー
	// 状況に応じて、交換にいったりいかなかったり
	CInterContext* p = static_cast<CInterContext*>(pContext);
	BMW::Chara::CDataCharaInter* pChara = p->getTargetCharaData()->getData();
	int nHave;
	if(p->getTargetItem()==REMOVE)
	{// アイテムはずし
		nHave = anHaveID_[p->getCtrlItem()]; 
		if(nHave>=0) removeItem(pChara,nHave,p->getCtrlItem(),p);
		// リセット
		actionReset(p->getTargetCharaData(),p);
	}
	else
	{
		int nGet = anGetID_[p->getTargetItem()];
		if(pItemNum_->getCurrentNum()>0)
		{// 残数があれば装備
			nHave = anHaveID_[p->getCtrlItem()];
			// 装備ヶ所にアイテムがあれば交換
			if(nHave>=0) removeItem(pChara,nHave,p->getCtrlItem(),p);
			// アイテム装備
			pChara->addItem(nGet,p->getCtrlItem(),p->getApp()->getItem());
			p->getItemData(nGet)->addChara(pChara->getTrainData()->getID()/*pChara->getID()*/);
			// リセット
			actionReset(p->getTargetCharaData(),p);
		}
		else
		{// なければ交換へ
		// 交換位置をコンテキストに設定
			p->setExchangeItemID(nGet);
			p->setExchangeItemAttr(p->getCtrlItem());
			// アイテム交換準備をなげる
			fun_(CChara::S_ITEM_START,p);
		}
	}
}

void CItemEqup::actionReset(CInterChara* pChara, CInterContext* p)
{
	// セーブデータへ反映
	//p->getApp()->getExec().getTrainData(pChara->getData()->getID(),false)->back(pChara->getData());
	// ステータス反映
	fun_(CChara::S_EASY,p);
	fun_(CChara::S_BASE,p);
	fun_(CChara::S_BATTLE,p);
	fun_(CChara::S_WEAPON,p);
	pChara->OnReset(p);
	// リセット
	OnReset(p);
}

void CItemEqup::actionStatus(Task::CTaskContext* pContext)
{
	CInterContext* p = static_cast<CInterContext*>(pContext);
	if(p->getTargetItem()==REMOVE)
	{// はずす
		pItemNum_->getCurrentNumGui()->visible(false);
		pItemNum_->getMaxNumGui()->visible(false);
		changeStatus(p->getCtrlItem()>=0
						? anHaveID_[p->getCtrlItem()]
						: REMOVE,
					 REMOVE,
					 *p);
	}
	ef(p->getTargetItem()>=0)
	{// 対象が設定されている
		pItemNum_->getCurrentNumGui()->visible(true);
		pItemNum_->getMaxNumGui()->visible(true);
		CDataItemInter* pItem = p->getItemData(anGetID_[p->getTargetItem()]);
		pItemNum_->setMaxNum(pItem->getNum());
		pItemNum_->setCurrentNum(pItem->getNum()-pItem->sizeChara());

		// ステータス変更
		changeStatus(p->getCtrlItem()>=0
						? anHaveID_[p->getCtrlItem()]
						: REMOVE,
					 anGetID_[p->getTargetItem()],
					 *p);
	}
	else
	{// 設定されていない
		pItemNum_->getCurrentNumGui()->visible(false);
		pItemNum_->getMaxNumGui()->visible(false);
		changeStatus(REMOVE,REMOVE,*p);
	}
}


void CItemEqup::updateStatus(BMW::Chara::CDataCharaInter* pChara)
{
	setStatus(pNowStatus_,pChara);
	setStatus(pUpStatus_,pChara);
	// 色変更
	setStatusColor(pNowStatus_,pUpStatus_);
}

///////////////////////////////////////////
// インターフェイス
///////////////////////////////////////////
GUI::CButtonSymbol* CItemEqup::getItemButton(int nPos)
{
	if(nPos<=7)		return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW1/ITEM",nPos));
	ef(nPos<=14)	return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW2/ITEM",nPos-7));
	ef(nPos<=21)	return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW3/ITEM",nPos-14));
	else			return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW4/ITEM",nPos-21));
}

void CItemEqup::setItemIconList(int nNum, Task::CTaskContext* p)
{
	GUI::CPanel* pPanel = pHave_->getValidWidgetCast<GUI::CPanel>();

	for(int i=0; i<nNum; ++i)
		setItemIcon(pPanel->getWidgetCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ITEM",i+1)),
					anHaveID_[i],p);
}


void CItemEqup::changeStatus(int nFrom, int nTo, CInterContext& p)
{// アイテムをnFromかnToへ変更するという仮定
	Item::CItemDB& db = p.getApp()->getItem();
	BMW::Chara::CDataCharaInter* pChara = p.getTargetCharaData()->getData();
	if(nFrom<0) nFrom=REMOVE;
	if(nTo<0) nTo=REMOVE;
	if((nFrom==REMOVE) & (nTo==REMOVE))
	{// 両方はずすと通常どおり
		setStatus(pUpStatus_,pChara);
	}
	ef((nFrom==REMOVE) & (nTo!=REMOVE))
	{// 何もないところから装備をする
		db.applyStatus(*pChara,0,nTo);
		setStatus(pUpStatus_,pChara);
		db.backStatus(*pChara,0,nTo);
	}
	ef((nFrom!=REMOVE) & (nTo==REMOVE))
	{// 装備してるところからはずす
		db.backStatus(*pChara,0,nFrom);
		setStatus(pUpStatus_,pChara);
		db.applyStatus(*pChara,0,nFrom);
	}
	else
	{// 装備してるのを変更する
		db.backStatus(*pChara,0,nFrom);
		db.applyStatus(*pChara,0,nTo);
		setStatus(pUpStatus_,pChara);
		db.backStatus(*pChara,0,nTo);
		db.applyStatus(*pChara,0,nFrom);
	}
	// 色変更
	setStatusColor(pNowStatus_,pUpStatus_);
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end