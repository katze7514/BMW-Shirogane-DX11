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

__inline void setItemIcon(GUI::CButtonSymbol* pButton, int nID, Item::CItemDB& db, GUI::CGuiDefDB& gui)
{
	if(nID<0)	gui.getSymbolDB().setButtonGui(pButton,"PERGE_BUTTON");
	else		db.setButtonHolder(pButton,nID);

	pButton->setState(GUI::CButton::NORMAL);
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
	pGet_ = pPanel_->getWidgetCast<GUI::CPanel>("ITEM_VIEW");
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW1"),7,fun,0);
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW2"),7,fun,7);
	setItemEventHandler(pGet_->getWidgetCast<GUI::CPanel>("ROW3"),7,fun,14);
	GUI::CPanel* pPanel = pGet_->getWidgetCast<GUI::CPanel>("ROW4");
	setItemEventHandler(pPanel,6,fun,21);
	// アイテムはずしボタン
	GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>("CHANGEITEM"),fun,REMOVE);

	pItemNum_ = pPanel_->getWidgetCast<GUI::CNumRemain>("ITEM_NUMBER");

	pHave_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("SET_ITEMS");
	fun.set(this,&CItemEqup::eventHave);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET1"),1,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET2"),2,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET3"),3,fun,0);
	setItemEventHandler(pHave_->getWidgetCast<GUI::CPanel>("SET4"),4,fun,0);

	pNowStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CURRENT_STATUS");
	pUpStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("CHANGE_STATUS");
}

void CItemEqup::OnReset(Task::CTaskContext* pContext)
{
	CInterContext* p = static_cast<CInterContext*>(pContext);
	p->setTargetItem(-1);
	p->setCtrlItem(-1);
	// 現在のアイテムリストに合わせてまずは、アイテム一覧生成
	Item::CItemDB& db = p->getApp()->getItem();
	typedef CInterContext::item_map item_map;
	item_map& mapItem = p->getItemMap();
	pGet_->validAll(false);
	pGet_->visibleAll(false);
	// アイテムアイコン設定
	int nPos=0;
	GUI::CButtonSymbol* pButton;
	item_map::iterator it;
	for(it=mapItem.begin(); it!=mapItem.end(); ++it)
	{
		anGetID_[nPos]=it->second->getID();
		pButton = getItemButton(nPos+1);
		db.setButtonHolder(pButton, it->second->getID());
		pButton->setState(GUI::CButton::NORMAL);
		pButton->valid(true);
		pButton->visible(true);
	}

	// 所持アイテム
	GUI::CGuiDefDB& gui = pContext->getScene()->getGuiDefDB();
	BMW::Chara::CDataCharaInter* pChara = p->getTargetCharaData()->getData();
	// 持てる最大数によって変更
	switch(pChara->getItemMax())
	{
	case 2:
		pHave_->validWidget("SET2");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		setItemIconList(2,db,gui);
	break;

	case 3:
		pHave_->validWidget("SET3");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		anHaveID_[2]=pChara->hasItemAttr(2);
		setItemIconList(3,db,gui);
	break;

	case 4:
		pHave_->validWidget("SET4");
		anHaveID_[0]=pChara->hasItemAttr(0);
		anHaveID_[1]=pChara->hasItemAttr(1);
		anHaveID_[2]=pChara->hasItemAttr(2);
		anHaveID_[3]=pChara->hasItemAttr(3);
		setItemIconList(4,db,gui);
	break;

	default:
		pHave_->validWidget("SET1");
		anHaveID_[0]=pChara->hasItemAttr(0);
		setItemIconList(1,db,gui);
	break;
	}
	// アイテム個数
	pItemNum_->getCurrentNumGui()->visible(false);
	pItemNum_->getMaxNumGui()->visible(false);

	// ステータス
	setStatus(pNowStatus_,pChara);
	setStatus(pUpStatus_,pChara);
}

///////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
void CItemEqup::eventHave(const smart_ptr<GUI::CEventButton>& pButton ,Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(pContext->getValue(Flag::TARGET_ITEM)>=0)
		{// 装備対象が決まってたら、アイテム装備
			pContext->setValue(pButton->getValue(),Flag::CTRL_ITEM);
			actionOK(pContext);
		}
		else
		{// 決まってないなら保持
			// 排他処理
			if(pContext->getValue(Flag::CTRL_ITEM)>=0)
				pHave_->getValidWidgetCast<GUI::CPanel>()
						->getWidget(pContext->getValue(Flag::CTRL_ITEM))->setState(GUI::CButton::NORMAL);

			pContext->setValue(pButton->getValue(),Flag::CTRL_ITEM);
		}
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		pContext->setValue(-1,Flag::CTRL_ITEM);
		actionStatus(pContext);
	}
	ef(GUI::IsOverIn(pButton))
	{// マウスオーバー
		if(pContext->getValue(Flag::TARGET_ITEM)>=0)
		{// 対象が選択されたら、現在の状態にあわせてステータスとか変更
			actionStatus(pContext);
		}
	}
	ef(GUI::IsOverOut(pButton))
	{// マウスアウト
		// 別のが設定されてたら、リターン
		if(pContext->getValue(Flag::CTRL_ITEM)!=pButton->getValue()) return;
		// そうじゃなかったら、キャンセルと同じ
		pContext->setValue(-1,Flag::CTRL_ITEM);
		actionStatus(pContext);
	}
}

void CItemEqup::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		if(pContext->getValue(Flag::CTRL_ITEM)>=0)
		{// 装備対象が決まってたら、アイテム装備決定
			pContext->setValue(pButton->getValue(),Flag::TARGET_ITEM);
			actionOK(pContext);
		}
		else
		{// 決まってないなら保持
			// 排他処理
			int nEx = pContext->getValue(Flag::TARGET_ITEM);
			if(nEx==REMOVE)
				pGet_->getWidgetRecCast<GUI::CButton>("ROW4/CHANGEITEM")->setState(GUI::CButton::NORMAL);
			ef(nEx>=0)
				getItemButton(pButton->getValue())->setState(GUI::CButton::NORMAL);

			// 保持数変更・ステータス変更
			actionStatus(pContext);
		}
	}
	ef(GUI::IsCancel(pButton))
	{// キャンセルされた
		pContext->setValue(-1,Flag::TARGET_ITEM);
		// 保持数変更・ステータス変更
		actionStatus(pContext);
	}
	ef(GUI::IsOverIn(pButton))
	{// マウスオーバー
		pContext->setValue(pButton->getValue(),Flag::TARGET_ITEM);
		if(pContext->getValue(Flag::CTRL_ITEM)>=0)
		{// 対象が選択されたら、現在の状態にあわせてステータスとか変更
			actionStatus(pContext);
		}
	}
	ef(GUI::IsOverOut(pButton))
	{// マウスアウト
		// 別のが設定されてたら、リターン
		if(pContext->getValue(Flag::TARGET_ITEM)!=pButton->getValue()) return;
		// そうでなかったら、キャンセルと同じ
		pContext->setValue(-1,Flag::TARGET_ITEM);
		actionStatus(pContext);
	}
}

///////////////////////////////////////////
// アクション
///////////////////////////////////////////
void CItemEqup::actionOK(Task::CTaskContext* pContext)
{// 決定ー
	// 状況に応じて、交換にいったりいかなかったり
	CInterContext* p = static_cast<CInterContext*>(pContext);
	int nGet = anGetID_[p->getTargetItem()];
	if(pItemNum_->getCurrentNum()>0)
	{// 残数があれば装備
		BMW::Chara::CDataCharaInter* pChara = p->getTargetCharaData()->getData();
		int nHave = anHaveID_[p->getCtrlItem()]; 
		if(nHave>=0)
		{// 装備ヶ所にアイテムがあれば交換
			// 装備アイテムをはずす
			p->getApp()->getItem().backStatus(*pChara,0,nHave);
			pChara->delItem(p->getCtrlItem());
			p->getItemData(nHave)->delChara(pChara->getID());
		}
		// アイテム装備
		pChara->addItem(nGet,p->getCtrlItem());
		p->getItemData(nGet)->addChara(pChara->getID());
		p->getApp()->getItem().applyStatus(*pChara,0,nGet);
		// セーブデータへ反映
		p->getApp()->getExec().getTrainData(pChara->getID(),false)->back(pChara);
		// ステータス反映
		fun_(CChara::S_BASE,p);
		fun_(CChara::S_BATTLE,p);
		fun_(CChara::S_WEAPON,p);
		// リセット
		OnReset(p);
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
		CDataItemInter* pItem = p->getTargetItemData();
		pItemNum_->setCurrentNum(pItem->sizeChara());
		pItemNum_->setMaxNum(pItem->getNum());

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
///////////////////////////////////////////
// インターフェイス
///////////////////////////////////////////
GUI::CButtonSymbol* CItemEqup::getItemButton(int nPos)
{
	if(nPos<7)		return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW1/ITEM",nPos));
	ef(nPos<14)		return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW2/ITEM",nPos-7));
	ef(nPos<21)		return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW3/ITEM",nPos-14));
	else			return pGet_->getWidgetRecCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ROW4/ITEM",nPos-21));
}

void CItemEqup::setItemIconList(int nNum, Item::CItemDB& db, GUI::CGuiDefDB& gui)
{
	GUI::CPanel* pPanel = pHave_->getValidWidgetCast<GUI::CPanel>();

	for(int i=0; i<nNum; ++i)
		setItemIcon(pPanel->getWidgetCast<GUI::CButtonSymbol>(Misc::linkStrAndNum("ITEM",i+1)),
					anHaveID_[i],db,gui);
}


void CItemEqup::changeStatus(int nFrom, int nTo, CInterContext& p)
{// アイテムをnFromかnToへ変更するという仮定
	Item::CItemDB& db = p.getApp()->getItem();
	BMW::Chara::CDataCharaInter* pChara = p.getTargetCharaData()->getData();
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