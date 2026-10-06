#include "stdafx.h"

#ifdef BMW_DEBUG
#include "../mode.h"
#include "../Input/CTaskInput.h"
//#define FACE_VIEWER  // FaceViewer優先モード
#endif

#include "../Scene/GUI/CGraphicFace.h"
#include "../Scene/GUI/CGraphicName.h"

#include "../Scene/IScene.h"

#include "CDictCharaScene.h"
#include "CDictCharaSelect.h"

namespace BMW{
namespace Dict{

CDictCharaSelect::~CDictCharaSelect()
{
	DELETE_SAFE(pPanel_);
}

void CDictCharaSelect::Task(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);

	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

void CDictCharaSelect::OnInit(Task::CTaskContext* pContext)
{
	// インターフェイス生成
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_CHARA_JITEN");
	// 処理簡易化のために取得
	pChange_ = pPanel_->getWidgetCast<GUI::CPanel>("PAGECHANGE");
	pPage_    = pChange_->getWidgetCast<GUI::CNum>("PAGE");
	pFace_   = pPanel_->getWidgetCast<GUI::CGraphicFace>("FACE");
	pName_   = pPanel_->getWidgetCast<GUI::CGraphicName>("NAME");

	// とりあえず、非表示
	pFace_->visible(false);
	pName_->visible(false);

	// ハンドラ
	GUI::CButton::ButtonEvent funButton(this,&CDictCharaSelect::eventButton);
	GUI::CButton::setButtonEvent(pChange_->getWidgetCast<GUI::CButton>("BUTTON"),funButton, CHANGE);
}

void CDictCharaSelect::OnReset(Task::CTaskContext* pContext)
{
	// 辞書ファイルに合わせて、PANEL_CHARAを生成
	CDictCharaContext* p = static_cast<CDictCharaContext*>(pContext);
	CDictCharaContext::chara_item_map& mapCharaItem = p->getCharaItemMap();

	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CDictCharaSelect::eventButton);

	GUI::CGuiDefDB& gui = p->getScene()->getGuiDefDB();

	// ページ数
	nDiv_ = (int)ceil((double)mapCharaItem.size() / 36.0);

	pSelect_ = new GUI::CPanelCtrl();
	pPanel_->swapWidget(pSelect_,"PANEL_CHARA");

	int nPage=0, nRow=1, nCol=1;
	GUI::CPanel *pPagePanel, *pRow, *pChip;
	CDictCharaContext::chara_item_map::iterator it;

	// 1ページ生成
	pPagePanel = gui.createInterfaceCast<GUI::CPanel>("PANEL_CHARA");
	pSelect_->addWidget(pPagePanel,Misc::linkStrAndNum("PAGE",nPage));

	// 1行ゲット
	pRow = pPagePanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nRow));

	for(it=mapCharaItem.begin(); it!=mapCharaItem.end(); ++it)
	{
		CDictCharaItem* pItem = it->second;

		// 1チップゲット
		pChip = pRow->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHIP",nCol));
		// キャラデータ生成
		Chara::CDataCharaTrain train;
		train.setLv(50);
		p->getApp()->getChara().setBattle(pItem->getCharaData().getBattlePtr(), Chara::Const::charaID_.getValue(pItem->getCharaID()), train);
		// 出現状態
		pItem->getCharaData().getStatePtr()->apper(true);
		// 必要なSymbolデータをセット
		pItem->getChipDB().setSymbol(pItem->getCharaData().getBattle().getMapSymbolID());
		pItem->getSymbolDB().setSymbol(pItem->getCharaData().getBattle().getSymbolID());
		// キャラボタンセット
		GUI::CButton* pCharaChip = pItem->getChipDB().createSymbolStrCast<GUI::CButton>("BUTTON_INTER");
		pChip->swapWidget(pCharaChip,"CHIP");
		// イベントハンドラ設定
		GUI::CButton::setButtonEvent(pCharaChip,fun,pItem->getNo());

	#ifdef BMW_DEBUG
		// デバグのため、POPUPにデータをちょこっと入れておく
		pCharaChip->setPopUp(pItem->getCharaID() + " " + CStringScanner::NumToString(pItem->getCharaData().getPhase()));
	#endif

		// ID設定
		int nID = nCol+((nRow-1)*6)+(nPage*36);
		pChip->getWidgetCast<GUI::CPanel>("SORT")->getWidgetCast<GUI::CPanel>("ID")->getWidgetCast<GUI::INum>("NUM")->setNum(nID);
		pItem->setID(nID);

		// 位置をずらす
		if(++nCol>6)
		{// 1列終わったら、次の行へ
			nCol=1;
			++nRow;
			if(nRow>6)
			{// 全行終わったら次のページへ
				nRow=1;
				++nPage;
				pPagePanel = gui.createInterfaceCast<GUI::CPanel>("PANEL_CHARA");
				pSelect_->addWidget(pPagePanel,Misc::linkStrAndNum("PAGE",nPage));
			}
			// 行取得
			pRow = pPagePanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("ROW",nRow));
		}
	}

	// あまりは削除
	for(int i=nCol; i<=6; ++i)
		pRow->delWidget(Misc::linkStrAndNum("CHIP",i));

	for(int i=nRow+1; i<=6; ++i)
		pPagePanel->delWidget(Misc::linkStrAndNum("ROW",i));

	// 1ページ目から
	pPage_->setNum(1);
	pSelect_->validWidget(0);
}

void CDictCharaSelect::OnAction(Task::CTaskContext* pContext)
{
	// キャンセルしたら、タイトルへ
	if(Input::releaseCancel(pContext))
	{
		pContext->getScene()->setState(CDictCharaScene::END);
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
	}
#ifdef BMW_DEBUG
	else if(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPush(DIK_LEFT))
	{// 前の顔リストへ
		int nNum = pPage_->getNum();
		if(nNum<=1) nNum=nDiv_+1;
		--nNum;
		pPage_->setNum(nNum);
		pSelect_->validWidget(nNum-1);
	}
	else if(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPush(DIK_RIGHT))
	{// 次の顔リストへ
		int nNum = pPage_->getNum();
		++nNum;
		if(nNum>nDiv_) nNum=1;
		pPage_->setNum(nNum);
		pSelect_->validWidget(nNum-1);
	}
#endif
}

void CDictCharaSelect::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	// VIEWからもどってきたコレ
	pContext->getInput()->cursolVisible(true);
	pContext->getInput()->guard(false);
}

void CDictCharaSelect::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// 押された
		if(pButton->getValue()==CHANGE)
		{// ページ変更
			int nNum = pPage_->getNum();

		#ifdef BMW_DEBUG
			if(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPress(DIK_B))
			{// B押しながらだとバック
				if(nNum<=1) nNum=nDiv_+1;
				--nNum;
			}
			else
			{// 何も押さなければ進む
				++nNum;
				if(nNum>nDiv_) nNum=1;
			}
			pPage_->setNum(nNum);
		#else //  BMW_DEBUG
			if(++nNum>nDiv_) nNum=1;
			pPage_->setNum(nNum);
		#endif

			pSelect_->validWidget(nNum-1);
		}
		else
		{// お、キャラ押されたので、VIEWへGO！
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			static_cast<CDictCharaContext*>(pContext)->setCharaIterator(pButton->getValue());
			pContext->getApp()->getFoward()->clearPopUp();
		#ifdef BMW_DEBUG
			#ifdef FACE_VIEWER
				// F押しながらだとキャラ説明モード
				if(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPress(DIK_F))
					getTaskListCtrl()->callTaskList(CDictCharaScene::VIEW,true);
				ef(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPress(DIK_S))
					getTaskListCtrl()->callTaskList(CDictCharaScene::SYMBOL,true);
				else
					getTaskListCtrl()->callTaskList(CDictCharaScene::FACE,true);
			#else // FACE_VIEWER
				// F押しながらだと顔Viewerモード
				if(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPress(DIK_F))
					getTaskListCtrl()->callTaskList(CDictCharaScene::FACE,true);
				ef(static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard().IsKeyPress(DIK_S))
					getTaskListCtrl()->callTaskList(CDictCharaScene::SYMBOL,true);
				else
					getTaskListCtrl()->callTaskList(CDictCharaScene::VIEW,true);
			#endif
		#else // BMW_DEBUG
			getTaskListCtrl()->callTaskList(CDictCharaScene::VIEW,true);
		#endif
		}
	}
	ef(pButton->getState()==GUI::CButton::OVER_IN)
	{// OVER_INしたら顔と名前を表示
		if(pButton->getValue()==CHANGE) return;

		CDictCharaItem* pItem = static_cast<CDictCharaContext*>(pContext)->getDictCharaItem(pButton->getValue());
		// 顔
		pFace_->visible(true);
		pFace_->setFace(pItem->getCharaData().getBattle().getFaceID(),"DEFAULT",pContext);
		// 名前
		pName_->visible(true);
		pName_->setCharaName(pItem->getCharaData().getBattle().getFaceID(),pContext);
	}
	ef(pButton->getState()==GUI::CButton::OVER_OUT)
	{// OVER_OUTしたら非表示
		pFace_->visible(false);
		pName_->visible(false);
	}
}

} // namespace Dict end
} // namespace BMW end
