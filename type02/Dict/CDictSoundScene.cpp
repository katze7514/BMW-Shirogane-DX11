#include "stdafx.h"

#include "../Scene/IDScene.h"

#include "DictFunction.h"
#include "CDictSoundParser.h"
#include "CDictSoundScene.h"

namespace BMW{
namespace Dict{

CDictSoundScene::~CDictSoundScene()
{
	DELETE_SAFE(pPanel_);
}

void CDictSoundScene::Task(Task::CTaskContext* pContext)
{
	context_.action(pContext->IsAction());

	pPanel_->Task(&context_);

	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

namespace{
__inline void setButtonEventVolume(GUI::CPanel* pPanel, GUI::CButton::ButtonEvent& fun, int nStart)
{
	for(int i=0; i<=10; ++i)
		GUI::CButton::setButtonEvent(pPanel->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_",i)),fun,nStart+i);
}

__inline void setButtonEventSoundColomn(GUI::CPanel* pPanel, GUI::CButton::ButtonEvent& fun, int nStart)
{
	GUI::CPanel* pRow;
	for(int i=1; i<=12; ++i)
	{
		pRow = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("PANEL_SOUND_ROW",i));
		GUI::CButton::setButtonEvent(pRow->getWidgetCast<GUI::CButton>("PLAY_BUTTON"),fun,nStart+(i-1));
	}
}
} // namespace end

void CDictSoundScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト設定
	setContext(pContext);

	// 辞書ファイル読み込み
	setDictSound();

	// インターフェイス読み込み
	setGuiDefDB("DICT");

	// サウンド辞典取得
	pPanel_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_SOUND_JITEN");

	// ボリューム
	pVol_ = pPanel_->getWidgetCast<GUI::CPanel>("VOLUME");
	setVolume(pContext->getApp()->getGlobal().getBGM()/10, pContext);

	// サウンド項目
	pLeft_ = pPanel_->getWidgetCast<GUI::CPanel>("PANEL_SOUND_COLOMN1");
	pRight_ = pPanel_->getWidgetCast<GUI::CPanel>("PANEL_SOUND_COLOMN2");

	// タイトル
	pTitleNum_ = pPanel_->getWidgetCast<GUI::CText>("TITLE_NUM");
	pTitle_ = pPanel_->getWidgetCast<GUI::CText>("TITLE");

	// 作曲者とか
	GUI::CPanel* pData = pPanel_->getWidgetCast<GUI::CPanel>("DATA_SOUND");
	pData->getWidgetCast<GUI::CText>("COMPOSE")->UpdateText();
	pComposer_ = pData->getWidgetCast<GUI::CText>("NAME");
	pUse_ = pData->getWidgetCast<GUI::CText>("USE");

	// コメント
	pComment_ = pPanel_->getWidgetCast<GUI::CText>("TEXT");
	pComment_->getFontConf().SetHeight(18);

	// コメントページ替え
	pChange_ = pPanel_->getWidgetCast<GUI::CPanel>("CHANGE_BUTTON");
	pChangePage_ = pChange_->getWidgetCast<GUI::INum>("PAGE");
	pChange_->valid(false);
	pChange_->visible(false);

	// サウンドページ替え
	GUI::CPanel* pChangeSound = pPanel_->getWidgetCast<GUI::CPanel>("PAGE_CHANGE_SOUND");
	pChangeSound->getWidgetCast<GUI::CText>("PAGE_SLASH")->UpdateText();
	pNow_ = pChangeSound->getWidgetCast<GUI::CText>("PAGE_NOW");
	pNow_->setText(CStringScanner::NumToString(1));
	pNow_->UpdateText();
	// 最大ページ
	GUI::CText* pMax = pChangeSound->getWidgetCast<GUI::CText>("PAGE_MAX");
	nMaxPage_ = (int)(ceil((double)context_.getSoundItemMap().size()/24.0));
	pMax->setText(CStringScanner::NumToString(nMaxPage_));
	pMax->UpdateText();

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent funButton(this,&CDictSoundScene::eventButton);
	GUI::CButton::setButtonEvent(pChange_->getWidgetCast<GUI::CButton>("BUTTON"), funButton, CHANGE);
	GUI::CButton::setButtonEvent(pChangeSound->getWidgetCast<GUI::CButton>("CHARALEFT"), funButton, LEFT);
	GUI::CButton::setButtonEvent(pChangeSound->getWidgetCast<GUI::CButton>("CHARARIGHT"), funButton, RIGHT);
	setButtonEventSoundColomn(pLeft_, funButton, 0);
	setButtonEventSoundColomn(pRight_, funButton, 12);
	setButtonEventVolume(pVol_, funButton, VOLUME_0);

	// 1ページ目を表示
	setSoundPage(0);

	// フェード
	Scene::CFoward::FaderEvent fun;
	fun.set(this,&CDictSoundScene::eventFade);
	context_.getApp()->getFoward()->setFaderHandler(fun);
	// FadeOut
	context_.getApp()->getFoward()->fadeOut();
	// BGM
	context_.getBgmSound()->change("STATUS");
	context_.getBgmSound()->FadeIn(30);

	setState(FADE);
}

void CDictSoundScene::OnAction(Task::CTaskContext* pContext)
{
	// キャンセルしたら、タイトルへ
	if(Input::releaseCancel(pContext))
	{
		context_.getInput()->cursolVisible(false);
		context_.getInput()->guard(true);
		context_.getApp()->getFoward()->fadeIn();
		context_.getBgmSound()->FadeOut(30);

		setState(FADE_END);
	}
}

void CDictSoundScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		context_.getInput()->cursolVisible(true);
		context_.getInput()->guard(false);
		setState(NORMAL);
	}
	else
	{
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->jumpTaskList(Scene::ID::TITLE);
		setState(NORMAL);
	}
}

void CDictSoundScene::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// 押された
		switch(pButton->getValue())
		{
		// 音量変更
		case VOLUME_0:
		case VOLUME_1:
		case VOLUME_2:
		case VOLUME_3:
		case VOLUME_4:
		case VOLUME_5:
		case VOLUME_6:
		case VOLUME_7:
		case VOLUME_8:
		case VOLUME_9:
		case VOLUME_10:
			setVolume(pButton->getValue()-VOLUME_0, pContext);
		break;

		case CHANGE:
			pChangePage_->setNum(pChangePage_->getNum()+1);
			if(++page_it_==pageComment_.end())
			{
				page_it_=pageComment_.begin();
				pChangePage_->setNum(1);
			}
			pComment_->setText(*page_it_);
			pComment_->UpdateText();
		break;

		case LEFT:
			if(--nPage_<0) nPage_=nMaxPage_-1;
			setSoundPage(nPage_);
		break;

		case RIGHT:
			if(++nPage_>=nMaxPage_) nPage_=0;
			setSoundPage(nPage_);
		break;

		default: // サウンド再生！！
			context_.setCurrentSoundItemID(nSoundNo_[pButton->getValue()]);
			createCommentPage();
			// 再生
			context_.getBgmSound()->change(context_.getCurrentSoundItem()->getID());
			context_.getBgmSound()->FadeIn(30);
		break;
		}
	}
}

void CDictSoundScene::setDictSound()
{
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read(Config::Const::configDB_.getConfigFileStr("DICT_SOUND"));
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	using namespace boost::spirit;
	using namespace phoenix;

	// 構文解析
	CDictSoundParser ps(&context_);
	Parser::skip_comment skip;
#ifdef BMW_DEBUG
	parse_info<> r = 
#endif
	parse(p.c_str(), ps, skip);
#ifdef BMW_DEBUG
	if(!r.full) CDbg().Out("%s 読み込み失敗！！", r.stop);
#endif
}

void CDictSoundScene::setSoundPage(int nPage)
{
	// 現在ページ更新
	nPage_ = nPage;
	pNow_->setText(CStringScanner::NumToString(nPage_+1));
	pNow_->UpdateText();

	CDictSoundContext::sound_item_map& mapSoundItem = context_.getSoundItemMap();
	CDictSoundContext::sound_item_map::iterator it = mapSoundItem.begin();
	// ページ分、進める
	int i;
	for(i=0; i<24*nPage && it!=mapSoundItem.end(); ++i) ++it;

	GUI::CPanel* pRow;
	GUI::CText* pText;
	// そこから、まずは左側12項目取ってくる
	for(i=0; i<12 && it!=mapSoundItem.end(); ++i)
	{
		pRow = pLeft_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("PANEL_SOUND_ROW",i+1));
		pRow->valid(true);
		pRow->visible(true);

		pText = pRow->getWidgetCast<GUI::CText>("TITLE_NUM");
		CStringScanner::NumToStringZ(it->first+1, pText->getText(), 2);
		pText->UpdateTextAA();
		pText = pRow->getWidgetCast<GUI::CText>("TITLE");
		pText->setText(it->second->getTitle());
		pText->UpdateTextAA();

	#ifdef BMW_DEBUG
		// POPUPにIDを設定
		pRow->getWidgetCast<GUI::CButton>("PLAY_BUTTON")->setPopUp(it->second->getID());
	#endif

		nSoundNo_[i] = it->first;
		++it;
	}

	if(i<12)
	{// 12以下ってことは途中で終了してるので、残りは無効にする
		for(int j=i; j<12; ++j)
		{
			pRow = pLeft_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("PANEL_SOUND_ROW",j+1));
			pRow->valid(false);
			pRow->visible(false);
		}
		// 右側までいってないみたい
		pRight_->valid(false);
		pRight_->visible(false);
	}
	else
	{
		// そこから、まずは右側12項目取ってくる
		for(i=0; i<12 && it!=mapSoundItem.end(); ++i)
		{
			pRow = pRight_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("PANEL_SOUND_ROW",i+1));
			pRow->valid(true);
			pRow->visible(true);

			pText = pRow->getWidgetCast<GUI::CText>("TITLE_NUM");
			CStringScanner::NumToStringZ(it->first+1, pText->getText(), 2);
			pText->UpdateTextAA();
			pText = pRow->getWidgetCast<GUI::CText>("TITLE");
			pText->setText(it->second->getTitle());
			pText->UpdateTextAA();
		#ifdef BMW_DEBUG
			// POPUPにIDを設定
			pRow->getWidgetCast<GUI::CButton>("PLAY_BUTTON")->setPopUp(it->second->getID());
		#endif

			nSoundNo_[i+12] = it->first;
			++it;
		}

		pRight_->valid(true);
		pRight_->visible(true);

		if(i<12)
		{// 12以下ってことは途中で終了してるので、残りは無効にする
			for(int j=i; j<12; ++j)
			{
				pRow = pRight_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("PANEL_SOUND_ROW",j+1));
				pRow->valid(false);
				pRow->visible(false);
			}
		}
	}
}

void CDictSoundScene::createCommentPage()
{
	// コメント
	pageComment_.clear();
	createPageHelper(context_.getCurrentSoundItem()->getComment(),pageComment_);
	page_it_ = pageComment_.begin();

	pComment_->setText(*page_it_);
	pComment_->UpdateText();

	pChangePage_->setNum(1);

	// 1ページ以上なかったら、ページチェンジは動かなくて良い
	pChange_->valid(pageComment_.size()>1);
	pChange_->visible(pageComment_.size()>1);

	// タイトル
	CDictSoundItem* pItem = context_.getCurrentSoundItem();
	CStringScanner::NumToStringZ(pItem->getNo()+1, pTitleNum_->getText(), 2);
	pTitleNum_->UpdateTextAA();
	pTitle_->setText(pItem->getTitle());
	pTitle_->UpdateTextAA();

	// 作曲者
	pComposer_->setText(pItem->getComposer());
	pComposer_->UpdateText();

	// 使う場所
	pUse_->setText(pItem->getUse());
	pUse_->UpdateText();
}

void CDictSoundScene::setVolume(int nVol, Task::CTaskContext* pContext)
{
	// 音量保存
	pContext->getApp()->getGlobal().setBGM(nVol*10);
	// 音量設定
	pContext->getBgmSound()->setVolume(percentVol2dB(nVol*10));
	if(pCurVol_!=NULL)
	{// 動作を元に戻す
		pCurVol_->valid(true);
		pCurVol_->setState(GUI::IButton::NORMAL);
	}
	// 現在の音量表示を変更
	pCurVol_ = pVol_->getWidgetCast<GUI::CButton>(Misc::linkStrAndNum("VOLUME_BUTTON_", nVol));
	pCurVol_->valid(false);
	pCurVol_->setState(GUI::IButton::PRESS);
}

} // namespace Dict end
} // namespace BMW end
