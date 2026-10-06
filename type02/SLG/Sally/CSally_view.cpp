#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/Unit/CSortUnit.h"
#include "../../Scene/Unit/IDHelp.h"
#include "../../Status/status_fun.h"

#include "../IDRule.h"
#include "../Map/CMap.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "../Context/SlgFunctor.h"

#include "CSally_view.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_view::OnReset(Task::CTaskContext* pContext)
{
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// インターフェイス
	pPanel_ = db.createInterfaceCast<GUI::CPanel>("KENSAKU");
	addTask(pPanel_,CHARA);

	// ソートユニット
	pSort_ = new Unit::CSortUnit();
	pSort_->OnInit(pContext);
	pPanel_->swapWidget(pSort_,"SORT");

	// イベントハンドラ
	Unit::CSortUnit::SortEvent funSort;
	funSort.set(this,&CSally_view::eventSort);
	pSort_->setEventHandler(funSort);

	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CSally_view::eventButton);
	
	// ページ変更
	GUI::CPanel* pPanel = pPanel_->getWidgetCast<GUI::CPanel>("PAGECHANGE");
	pPageButton_ = pPanel->getWidgetCast<GUI::CButton>("BUTTON");
	GUI::CButton::setButtonEvent(pPageButton_, funButton, CHANGE);
	pPage_ = pPanel->getWidgetCast<GUI::INum>("PAGE");

	// 精神検索ボタン
	pSpButton_ = pPanel_->getWidgetCast<GUI::CButton>("SP_KENSAKU");
	GUI::CButton::setButtonEvent(pSpButton_, funButton, SP);
}

void CSally_view::OnInit(Task::CTaskContext* pContext)
{
	// モード取得
	nMode_ = pContext->top();
	pContext->pop();

	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// リスト取得
	list<int>& listPlayer = p->getPlayerPhaseList();
	list<int> listTmp;
	listTmp = listPlayer;
	//copy(listPlayer.begin(), listPlayer.end(), listTmp.begin());
	// 通常モード時は、マップにいないキャラは削除
	if(nMode_==NORMAL) listTmp.remove_if(IsNoCtrl(*p));
	// 出撃選択用
	listChara_.clear();
	list<int>::iterator it;
	int n=1;
	for(it=listTmp.begin(); it!=listTmp.end(); ++it, ++n)
		listChara_.push_back(pair<int,int>(n,*it));
	
	// キャラパネル生成
	createChipPanel(Unit::CSortUnit::ID, Unit::CSortUnit::UP, p);

	// モードによっては精神検索しない
	pSpButton_->valid(nMode_==NORAML);
	pSpButton_->visible(nMode_==NORAML);

	pContext->getInput()->guard(false);
	setState(NORMAL);

	// ヘルプモード
	Unit::Help::callHelp(Unit::Help::SLG_ICHIRAN,"SLG_ICHIRAN",pContext);
}

namespace{
__inline bool releaseCancel(Task::CTaskContext* pContext)
{
	using Input::IInput;
	return pContext->getInput()->getInputState(IInput::CANCEL)==IInput::RELEASE;
}
} // namespace end

void CSally_view::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case NORMAL:
		if(releaseCancel(pContext))
		{// キャンセル
			pContext->push(-1);
			setState(END);
		}
	break;

	case END:
		pContext->getInput()->guard(true);
		if(pContext->top()>=0)
		{// キャラが選択されてれば、移動
			if(nMode_==NORMAL)
			{
				CSLGContext* p = static_cast<CSLGContext*>(pContext);
				// スクロール
				int nIndex = p->getCharaData(pContext->top())->getIndex();
				p->getMap()->scrollIndex(nIndex);
				// カーソル移動
				Input::IInput::InputEvent fun(this,&CSally_view::eventCursol);
				int nX,nY;
				p->getMap()->getMapChipPos(nIndex,nX,nY);
				p->getInput()->setCursolPos(nX+28,nY+14);
			}
		}
		getTaskListCtrl()->returnTaskList();
	break;

	default: break;
	}
}

void CSally_view::OnComeBack(int nID, Task::CTaskContext* pContext)
{
	// 精神検索から戻ってきた
	if(pContext->top()>=0)
	{// リターン
		setState(END);
	}
	else
	{// キャンセルっぽい
		pContext->pop();
		pContext->getInput()->guard(false);
		setState(NORMAL);
	}
}

//////////////////////////////////////////////////
// イベントハンドラ
//////////////////////////////////////////////////
void CSally_view::eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{// キャラ選択などが行われた時
	if(GUI::IsRelease(pButton))
	{// ボタン押された
		pContext->push(pButton->getValue());
		setState(END);
	}
}

void CSally_view::eventSort(int nKey, int nOrder, Task::CTaskContext* pContext)
{// ソートユニット動作
	if(nKey==Unit::CSortUnit::START)
	{// ソートユニット動作中なので、こっちのパネル動作停止
		pCharaPanel_->valid(false);
		pPageButton_->valid(false);
		pSpButton_->valid(false);
		setState(WAIT);
	}
	ef(nKey==Unit::CSortUnit::END)
	{// ソートユニット動作終了なので、こっちのパネル動作開始
		pCharaPanel_->valid(true);
		pPageButton_->valid(true);
		pSpButton_->valid(true);
		setState(NORMAL);
	}
	else
	{	// ソートして、キャラボタン位置を再設定
		createChipPanel(nKey,nOrder,static_cast<CSLGContext*>(pContext));
	}
}

void CSally_view::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{// パネル上のボタン
	if(GUI::IsRelease(pButton))
	{// ボタン押された
		if(pButton->getValue()==CHANGE)
		{// ページチェンジ
			int nPage = pPage_->getNum();
			if(++nPage>nDiv_) nPage=1;
			pPage_->setNum(nPage);
			static_cast<GUI::CPanelCtrl*>(pCharaPanel_)->validWidget(nPage-1);
		}
		ef(pButton->getValue()==SP)
		{// 精神検索呼ぶ
			pContext->getInput()->guard(true);
			getTaskListCtrl()->callTaskList(Rule::SALLY_SPIRIT,true);
		}
	}
}

void CSally_view::eventCursol()
{
	// カーソル移動が終わればリターン
	getTaskListCtrl()->returnTaskList();
}

//////////////////////////////////////////////////
// パネル生成
//////////////////////////////////////////////////
void CSally_view::createChipPanel(int nKey, int nOrder, CSLGContext* p)
{
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CSally_view::eventChara);

	// 一度、ID・昇順ソート
	sortChara(nKey,nOrder,*p);
	pSort_->setKey(nKey);
	pSort_->setOrder(nOrder);
	pSort_->actionSort();

	// 必要なキャラ数に合わせて選択パネルを生成
	int nSize = (int)listChara_.size();
	nDiv_ = (int)ceil((double)nSize / 9.0);

	CDataCharaSLG* pChara;
	GUI::CPanel* pPanel;
	GUI::CPanel* pChip;
	GUI::CButtonSymbol* pButton;
	GUI::CPanelCtrl* pSort;
	list<pair<int,int> >::iterator it;
	int nPos=1;
	if(nDiv_==1)
	{// 1枚ならCtrlじゃなくて、Panel一枚で生成
		// CHANGEボタン使用しない
		pPanel_->getWidget("PAGECHANGE")->valid(false);
		pPanel_->getWidget("PAGECHANGE")->visible(false);
		// とりあえず、ひな形生成
		pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARAS");
		pCharaPanel_ = pPanel;

		it=listChara_.begin();
		while(it!=listChara_.end())
		{// パネル設定ループ
			pChara = p->getCharaData(it->second);
			if(pChara!=NULL)
			{
				pChip = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARACHIP",nPos++));
				pButton = pChip->getWidgetCast<GUI::CButtonSymbol>("CHIP");
				pChara->getMapSymbol()->setButtonGui(pButton,pChara->getState().getAct()!=Act::BEFORE?"BUTTON_SLG":"BUTTON_INTER");
				pSort = pChip->getWidgetCast<GUI::CPanelCtrl>("SORT");
				Status::setCharaSort(pSort,it->first,*pChara);
				pSort->validWidget(nKey);
				GUI::CButton::setButtonEvent(pButton, fun, pChara->getID());
			}
			++it;
		}
	}
	else
	{// 2枚以上なら、コントローラで生成
		// CHANGEボタン使用する
		pPanel_->getWidget("PAGECHANGE")->valid(true);
		pPanel_->getWidget("PAGECHANGE")->visible(true);

		GUI::CPanelCtrl* pCtrl = new GUI::CPanelCtrl();
		pCharaPanel_ = pCtrl;

		int nDiv=0;
		pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARAS");
		pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));

		it=listChara_.begin();
		while(it!=listChara_.end())
		{// パネル設定ループ
			if(nPos>9)
			{// 9個越えたら次
				nPos=1;
				pPanel = p->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARAS");	
				pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));
			}

			pChara = p->getCharaData(it->second);
			pChip = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARACHIP",nPos++));
			pButton = pChip->getWidgetCast<GUI::CButtonSymbol>("CHIP");
			pChara->getMapSymbol()->setButtonGui(pButton,pChara->getState().getAct()!=Act::BEFORE?"BUTTON_SLG":"BUTTON_INTER");
			pSort = pChip->getWidgetCast<GUI::CPanelCtrl>("SORT");
			Status::setCharaSort(pSort,it->first,*pChara);
			pSort->validWidget(nKey);
			GUI::CButton::setButtonEvent(pButton, fun, pChara->getID());

			++it;
		}
		// とりあえず、1を表示
		pPage_->setNum(1);
		pCtrl->validWidget(0);
	}

	// 最後のパネルの余った部分のタスクは余計なので、削除してしまう
	for(int i=nPos; i<=9; i++)
		pPanel->delWidget(Misc::linkStrAndNum("CHARACHIP",i));

	// パネルを設定
	pPanel_->swapWidget(pCharaPanel_,"CHARAS");
}

/*void CSally_view::clearChipPanel()
{
	if(nDiv_==1)
	{// 一枚の時
		static_cast<GUI::CPanel*>(pCharaPanel_)->emptyWidget();
	}
	else
	{// 複数の時
		GUI::CPanelCtrl* pCtrl = static_cast<GUI::CPanelCtrl*>(pCharaPanel_);
		for(int i=0; i<nDiv_; i++)
			pCtrl->getWidgetCast<GUI::CPanel>(i)->emptyWidget();
	}
}*/

//////////////////////////////////////////
// ソート
//////////////////////////////////////////
void CSally_view::sortChara(int nKey,int nOrder, CSLGContext& p)
{// フラグに合わせてソートする
	if(nOrder==Unit::CSortUnit::UP)
	{// 昇順
		switch(nKey)
		{
		case Unit::CSortUnit::LV:	listChara_.sort(sort_LvUp(p.getCharaMap()));		break;
		case Unit::CSortUnit::HP:	listChara_.sort(sort_HpUp(p.getCharaMap()));		break;
		case Unit::CSortUnit::EN:	listChara_.sort(sort_EnUp(p.getCharaMap()));		break;
		case Unit::CSortUnit::SP:	listChara_.sort(sort_SpUp(p.getCharaMap()));		break;
		case Unit::CSortUnit::NEXT: listChara_.sort(sort_NextUp(p.getCharaMap()));		break;
		case Unit::CSortUnit::KI:	listChara_.sort(sort_MentalUp(p.getCharaMap()));	break;
		default:					listChara_.sort(sort_IdUp());						break;
		}
	}
	else
	{//	降順
		switch(nKey)
		{
		case Unit::CSortUnit::LV:	listChara_.sort(sort_LvDown(p.getCharaMap()));		break;
		case Unit::CSortUnit::HP:	listChara_.sort(sort_HpDown(p.getCharaMap()));		break;
		case Unit::CSortUnit::EN:	listChara_.sort(sort_EnDown(p.getCharaMap()));		break;
		case Unit::CSortUnit::SP:	listChara_.sort(sort_SpDown(p.getCharaMap()));		break;
		case Unit::CSortUnit::NEXT: listChara_.sort(sort_NextDown(p.getCharaMap()));	break;
		case Unit::CSortUnit::KI:	listChara_.sort(sort_MentalDown(p.getCharaMap()));	break;
		default:					listChara_.sort(sort_IdDown());						break;
		}
	}
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW ene