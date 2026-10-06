#include "stdafx.h"

#include "../Status/status_fun.h"
#include "../Scene/IScene.h"
#include "../Scene/Unit/CSortUnit.h"
#include "../Scene/Unit/CExitUnit.h"

#include "IDInter.h"
#include "CInterScene.h"
#include "CInterContext.h"
#include "CInterChara.h"

#include "CSelect.h"

namespace BMW{
namespace Inter{
namespace Select{

CSelect::~CSelect()
{
	clearChipPanel();
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(pExitUnit_);
}

void CSelect::Task(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
	pExitUnit_->Task(pContext);

	if(pContext->IsAction()
	&& IsValid())
		OnAction(pContext);
}

void CSelect::OnInit(Task::CTaskContext* pContext)
{
	pExitUnit_ = new Unit::CExitUnit();
	pExitUnit_->OnInit(pContext);
	pExitUnit_->valid(false);
	pExitUnit_->visible(false);
	// イベントハンドラ
	Unit::CExitUnit::ExitEvent funExit;
	funExit.set(this,&CSelect::eventExit);
	pExitUnit_->setEventHandler(funExit);
	pExitUnit_->setX(320);
	pExitUnit_->setY(240);

	// インターフェイス
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("INTERMISSION");
	// ヘッダ
	pHeader_ = pPanel_->getWidgetCast<GUI::CPanel>("HEADER");
	// ソートユニット
	pSort_ = new Unit::CSortUnit();
	pSort_->OnInit(pContext);
	pPanel_->swapWidget(pSort_,"SORTUNIT");
	// イベントハンドラ
	Unit::CSortUnit::SortEvent funSort;
	funSort.set(this,&CSelect::eventSort);
	pSort_->setEventHandler(funSort);

	// チェンジボタン
	pChange_ = pPanel_->getWidgetCast<GUI::CPanel>("PAGECHANGE");

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CSelect::eventButton);
	// ページ変更
	pPage_ = pChange_->getWidgetCast<GUI::CNum>("PAGE");
	GUI::CButton::setButtonEvent(pChange_->getWidgetCast<GUI::CButton>("BUTTON"),
								 funButton, CHANGE);
	// 検索
	//GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("KENSAKU"), funButton, SEARCH);
	// データ
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("SAVE"), funButton, DATA);
	// EXIT
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("TOTITLE"), funButton, EXIT);
	// NEXT STATGE
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("NEXTSTAGE"), funButton, NEXT);

	// ステータス
	pStatus_ = pPanel_->getWidgetCast<GUI::CPanel>("EASY_STATUS");

	// 初期設定
	OnReset(pContext);

	// 初期化フラグを立てておく
	pContext->setValue(1,Flag::INITIALIZE);
}

void CSelect::OnReset(Task::CTaskContext* pContext)
{// セーブデータが読み直された時などに呼ばれる
	// ヘッダ
	Status::setClearHeaderInter(pHeader_,*pContext);
	pSort_->setKey(Unit::CSortUnit::ID);
	pSort_->setOrder(Unit::CSortUnit::UP);
	// キャラチップ＆チェンジボタン＆ソートユニット
	createChipPanel(Unit::CSortUnit::ID, Unit::CSortUnit::UP, pContext);
	// EASY_STATUSはとりあえず非表示
	pStatus_->visible(false);

	setState(NORMAL);
}

void CSelect::OnAction(Task::CTaskContext* pContext)
{
	if(getState()==SORT)
	{
		static_cast<CInterContext*>(pContext)->sortChara(pSort_->getKey(),pSort_->getOrder());
		createChipPanel(pSort_->getKey(),pSort_->getOrder(),pContext);
		setState(NORMAL);
	}
}

void CSelect::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	if(nID==Scene::CHARA)
	{// キャラ別画面から戻ったらヘッダ情報が変わってる可能性があるので
	 // 設定し直し
		pContext->getApp()->getFoward()->clearPopUp();
		pContext->getInput()->guard(false);
		Status::setClearHeaderInter(pHeader_,*pContext);

		if(pContext->getValue(Flag::EXCHANGE))
		{// あとアイテム交換を経ていると座標とかも変わっているので設定し直し
			createChipPanel(pSort_->getKey(),pSort_->getOrder(),pContext);
			// EASY_STATUSはとりあえず非表示
			pStatus_->visible(false);
			pContext->setValue(0,Flag::EXCHANGE);
		}
	}
}
////////////////////////////////////////////////////
// 設定という生成？
////////////////////////////////////////////////////
void CSelect::createChipPanel(int nKey, int nOrder, Task::CTaskContext* pContext)
{
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSelect::eventChara);
	CInterContext* p = static_cast<CInterContext*>(pContext);

	GUI::CGuiDefDB& gui = p->getScene()->getGuiDefDB();
	// EventCharaSet取得
	set<int>& eventCharaSet = p->getApp()->getScenario().getScenario(p->getApp()->getExec().getNextScenario())->getEventCharaSet();

	if(p->IsInit())	clearChipPanel();
	
	// ソート
	pSort_->actionSort();

	// 必要なキャラ数に合わせて選択パネルを生成
	int nSize = (int)p->getCharaList().size();
	nDiv_ = (int)ceil((double)nSize / 9.0);

	CInterChara* pChara;
	GUI::CPanel* pPanel;
	GUI::CPanel* pChip;
	GUI::CPanelCtrl* pSort;
	GUI::CButton* pButton;
	int nPos=1;
	if(nDiv_==1)
	{// 1枚ならCtrlじゃなくて、Panel一枚で生成
		// CHANGEボタン使用しない
		pChange_->valid(false);
		pChange_->visible(false);
		// とりあえず、ひな形生成
		pPanel = gui.createInterfaceCast<GUI::CPanel>("CHIPS_SORT");
		pCharaPanel_ = pPanel;

		p->beginChara();
		while(!p->endChara())
		{// パネル設定ループ
			pChara = p->getCharaData(*p->nextChara());
			pChip = pChara->getChipSort();
			pSort = pChip->getWidgetCast<GUI::CPanelCtrl>("SORT");
			pSort->visible(true);
			pSort->validWidget(nKey);
			pButton = pChip->getWidgetCast<GUI::CButton>("CHIP");
			pButton->getPopUp().clear();
			GUI::CButton::setButtonEvent(pButton, fun, pChara->getData()->getID());

			// EVキャラ？
			if(eventCharaSet.find(pChara->getData()->getID())!=eventCharaSet.end())
			{// EVキャラだったらEVマーク投下
				Task::ITaskBase* pEv = gui.createInterface("EV_G");
				pEv->setX(16);
				pEv->setY(-40);
				pChip->setID("EV");
				pChip->addWidget(pEv, "EV");
			}

			pPanel->swapWidget(pChip,Misc::linkStrAndNum("CHIP",nPos++));
		}
	}
	else
	{// 2枚以上なら、コントローラで生成
		// CHANGEボタン使用する
		pChange_->valid(true);
		pChange_->visible(true);

		GUI::CPanelCtrl* pCtrl = new GUI::CPanelCtrl();
		pCharaPanel_ = pCtrl;

		int nDiv=0;
		pPanel = gui.createInterfaceCast<GUI::CPanel>("CHIPS_SORT");
		pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));

		p->beginChara();
		while(!p->endChara())
		{// パネル設定ループ
			if(nPos>9)
			{// 9個越えたら次
				nPos=1;
				pPanel = gui.createInterfaceCast<GUI::CPanel>("CHIPS_SORT");	
				pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));
			}

			pChara = p->getCharaData(*p->nextChara());
			pChip = pChara->getChipSort();
			pSort = pChip->getWidgetCast<GUI::CPanelCtrl>("SORT");
			pSort->visible(true);
			pSort->validWidget(nKey);
			pButton = pChip->getWidgetCast<GUI::CButton>("CHIP");
			pButton->getPopUp().clear();
			GUI::CButton::setButtonEvent(pButton, fun, pChara->getData()->getID());

			// EVキャラ？
			if(eventCharaSet.find(pChara->getData()->getID())!=eventCharaSet.end())
			{// EVキャラだったらEVマーク投下
				Task::ITaskBase* pEv = gui.createInterface("EV_G");
				pEv->setX(16);
				pEv->setY(-40);
				pChip->setID("EV");
				pChip->addWidget(pEv, "EV");
			}

			pPanel->swapWidget(pChip,Misc::linkStrAndNum("CHIP",nPos++));
		}
		// とりあえず、1を表示
		pPage_->setNum(1);
		pCtrl->validWidget(0);
	}
	// 最後のパネルの余った部分のタスクは余計なので、削除してしまう
	for(int i=nPos; i<=9; ++i)
		pPanel->delWidget(Misc::linkStrAndNum("CHIP",i));

	// パネルを設定
	pPanel_->swapWidget(pCharaPanel_,"CHIPS_SORT");
}

void CSelect::clearChipPanel()
{// キャラパネルの中を一掃
	if(nDiv_==1)
	{// 一枚の時
		static_cast<GUI::CPanel*>(pCharaPanel_)->emptyWidget();
	}
	else
	{// 複数の時
		GUI::CPanelCtrl* pCtrl = static_cast<GUI::CPanelCtrl*>(pCharaPanel_);
		for(int i=0; i<nDiv_; ++i)
			pCtrl->getWidgetCast<GUI::CPanel>(i)->emptyWidget();
	}
}
/////////////////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////////////////
void CSelect::eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 何か押された
		// キャラIDを設定して、キャラ個別しょりGO！
		CInterContext* p = static_cast<CInterContext*>(pContext);
		p->setTargetChara(pButton->getValue());
		p->setChara();
		getTaskListCtrl()->callTaskList(Scene::CHARA,true);
	}
	ef(GUI::IsOverIn(pButton))
	{// キャラオーバーならそのキャラを簡易ステ更新
		Status::setEasyStatus(pStatus_, *static_cast<CInterContext*>(pContext)->getCharaData(pButton->getValue()), *pContext);
		pStatus_->visible(true);
	}
	ef(GUI::IsOverOut(pButton))
	{// キャラオーバーアウトならそのキャラを簡易ステ非表示
		pStatus_->visible(false);
	}
}

void CSelect::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		switch(pButton->getValue())
		{
		case CHANGE:
		{// ページを進める
			int nPage = pPage_->getNum();
			if(++nPage>nDiv_) nPage=1;
			pPage_->setNum(nPage);
			static_cast<GUI::CPanelCtrl*>(pCharaPanel_)->validWidget(nPage-1);
		}
		break;

		case DATA: // データ
			getParent()->setState(CInterScene::DATA);
		break;

		case SEARCH: // 検索
		break;

		case EXIT: // EXIT選択メニューを開く
			pExitUnit_->OnReset(pContext);
			pExitUnit_->valid(true);
			pExitUnit_->visible(true);
			pPanel_->valid(false);
		break;

		case NEXT: // 次の話へ
			pContext->push(1);
			getParent()->setState(CInterScene::END);
		break;

		default: break;
		}
	}
}

void CSelect::eventSort(int nKey, int nOrder, Task::CTaskContext* pContext)
{
	if(nKey==Unit::CSortUnit::START)
	{// ソートユニット動作中なので、こっちのパネル動作停止
		pPanel_->validAll(false);
		pSort_->valid(true);
	}
	ef(nKey==Unit::CSortUnit::END)
	{// ソートユニット動作終了なので、こっちのパネル動作開始
		pPanel_->validAll(true);
		pChange_->valid(nDiv_!=1);
	}
	else
	{// ソートして、キャラボタン位置を再設定
		setState(SORT);
	}
}

void CSelect::eventExit(int nState, Task::CTaskContext* pContext)
{
	switch(nState)
	{
	case Unit::CExitUnit::TITLE: // タイトルへ
		pContext->push(0);
		getParent()->setState(CInterScene::END);
	break;

	case Unit::CExitUnit::END: // ゲーム終了
		pContext->getApp()->end();
	break;

	case Unit::CExitUnit::CANCEL: // キャンセル
		pExitUnit_->valid(false);
		pExitUnit_->visible(false);
		pPanel_->valid(true);
	break;

	default: break;
	}
}

} // namespace Select end
} // namespace Inter end
} // namespace BMW end