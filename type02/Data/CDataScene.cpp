#include "stdafx.h"

#include "../Save/CExecDataHead.h"
#include "../Scene/IDScene.h"
#include "../Scene/Unit/CYesNoUnit.h"
#include "../Scene/Unit/IDHelp.h"

#include "IDData.h"
#include "CDataScene.h"

namespace BMW{
namespace Data{

namespace{
__inline void updateDataLine(GUI::CPanel* pPanel, Save::CExecDataHead* pHead, int nNo, Scenario::CScenarioDB& db, bool bSave, bool bHandover)
{
	GUI::CText* pText;
	pText = pPanel->getWidgetCast<GUI::CText>("DATA_NUMBER");
	string s;
	CStringScanner::NumToStringZ(nNo+1,s,3);
	s+=".";
	pText->setText(s);
	pText->UpdateTextAA();

	if(pHead==NULL
		// 引き継ぎロードの時はクリアデータじゃないのは表示しない
	|| (bHandover 
		&& !(pHead->getStory()==db.getScenarioID("CLEAR_NORMAL")
			|| pHead->getStory()==db.getScenarioID("CLEAR_GOOD")
			|| pHead->getStory()==db.getScenarioID("CLEAR_TRUE")
			)
		)
	)
	{// データがない
		pPanel->visibleAll(false);
		pText->visible(true);
		pPanel->getWidget("DATA_BAR")->visible(bSave);

		pText = pPanel->getWidgetCast<GUI::CText>("HERONAME");
		pText->setText("--------");
		pText->UpdateTextAA();
		pText->visible(true);
	}
	else
	{// データあり
		pPanel->visibleAll(true);

		pText = pPanel->getWidgetCast<GUI::CText>("SCENARIO");
		pText->setText(CStringScanner::NumToStringZ(db.getNo(pHead->getStory()),2));
		pText->UpdateTextAA();

		pText = pPanel->getWidgetCast<GUI::CText>("HERONAME");
		pText->setText(pHead->getHero()==0?"熱田　匠":"望月陽菜");
		pText->UpdateTextAA();
		
		pPanel->getWidgetCast<GUI::INum>("LV")->setNum(pHead->getLv());
		pPanel->getWidgetCast<GUI::INum>("JUKUREN")->setNum(pHead->getExpert());
		pPanel->getWidgetCast<GUI::INum>("TURN")->setNum(pHead->getTurn());
		pPanel->getWidgetCast<GUI::INum>("BP")->setNum(pHead->getBP());
		pPanel->getWidgetCast<GUI::INum>("FP")->setNum(pHead->getFP());
	}
}

} // namespace end

int CDataScene::getMode()
{
	return context_.getValue(Flag::DATA_FLAG);
}

void CDataScene::OnInit(Task::CTaskContext* pContext)
{
	// コンテキストの設定
	setContext(pContext);
	setGuiDefDB("DATA");
	// セーブデータの取得
	setData();

	// LOADモード？
	// スタックトップにデータフラグが入っている
	// それによって、OKの時の動作が変わる
	context_.setValue(pContext->top(),Flag::DATA_FLAG);
	pContext->pop();
	
	// インターフェイス
	pPanel_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("DATA");
	addTask(pPanel_,PANEL);
	pPage_ = pPanel_->getWidgetRecCast<GUI::CText>("PANEL1/PAGE_NUMBER");

	// インターフェイス展開
	pFooter_ = pPanel_->getWidgetCast<GUI::CPanel>("PANEL2");
	// イベントハンドラ
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CDataScene::eventButton);
	pCtrl_ = pPanel_->getWidgetRecCast<GUI::CPanelCtrl>("PANEL1/PANEL");
	GUI::CPanel* pPanel;
	GUI::CPanel* pLine;
	int nID;
	for(int i=0; i<10; ++i)
	{// データパネル設定
		pPanel = pCtrl_->getWidgetCast<GUI::CPanel>(i);
		for(int j=0; j<10; ++j)
		{// 各ラインを設定
			pLine = pPanel->getWidgetCast<GUI::CPanel>(j);
			nID = 10*i+j;
			GUI::CButton::setButtonEvent(pLine->getWidgetCast<GUI::CButton>("DATA_BAR"), fun, nID);
			updateDataLine(pLine, context_.getHead(nID), nID, pContext->getApp()->getScenario(), !(getMode()==Mode::LOAD || getMode()==Mode::HANDOVER), getMode()==Mode::HANDOVER);
		}
	}
	// とりあえず、保存されてるNo表示
	int nPanelNo = context_.getApp()->getGlobal().getDataPanel();
	if(nPanelNo<0 || nPanelNo>9){ nPanelNo=0; context_.getApp()->getGlobal().setDataPanel(nPanelNo); }
	pCtrl_->validWidget(nPanelNo);
	pFooter_->visibleAll(false);
	pFooter_->getWidget("BACK")->visible(true);
	pPage_->setText(CStringScanner::NumToString(context_.getApp()->getGlobal().getDataPanel()+1));
	pPage_->UpdateText();

	// 週目
	GUI::CText* pText = pFooter_->getWidgetCast<GUI::CText>("SYUUME");
	pText->UpdateText();

	// 養成段階
	/*pText = pFooter_->getWidgetCast<GUI::CText>("TEKI_YOUSEI");
	pText->UpdateText();*/

	// 矢印
	fun.set(this,&CDataScene::eventArrow);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetRecCast<GUI::CButton>("PANEL1/PAGE_LEFT"),fun,DOWN);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetRecCast<GUI::CButton>("PANEL1/PAGE_RIGHT"),fun,UP);

	// 黒幕
	pBlack_ = pPanel_->getWidgetCast<GUI::CGraphic>("BLACK");
	pBlack_->visible(false);

	// フェードハンドラ設定
	Scene::CFoward::FaderEvent funFade;
	funFade.set(this,&CDataScene::eventFade);
	pContext->getApp()->getFoward()->setFaderHandler(funFade);
	context_.setValue(-1,Flag::NEXT);

	// モード別設定
	switch(getMode())
	{
	case Mode::SAVE:
	{// SAVEモード
		pYesNo_ = new Unit::CYesNoUnit();
		addTask(pYesNo_, DIALOG);
		pYesNo_->OnInit(&context_);
		pYesNo_->valid(false);
		pYesNo_->visible(false);
		fun.set(this,&CDataScene::eventYesNo);
		pYesNo_->setButtonHandler(fun,YES,NO);
		pYesNo_->setCancelHandler(Unit::CYesNoUnit::CancelEvent(this,&CDataScene::eventCancel));
		pYesNo_->setX(320);
		pYesNo_->setY(240);
	}
	case Mode::LOAD: // fall through
		pContext->getBgmSound()->change("STATUS");
		pContext->getBgmSound()->FadeIn(30);

	case Mode::HANDOVER:
		pContext->getApp()->getFoward()->fadeOut();
		setState(FADE);
		pPanel_->valid(false);	
	break;

	case Mode::SAVE_LOAD:
	{// SAVE_LOADモード
		pContext->getInput()->guard(false);
		setState(NORMAL);
		pPanel_->valid(true);

		// ダイアログ
		pDialog_ = getGuiDefDB().createInterfaceCast<GUI::CPanel>("SAVELOAD");
		addTask(pDialog_,DIALOG);
		pDialog_->valid(false);
		pDialog_->visible(false);
		fun.set(this,&CDataScene::eventDialog);
		GUI::CButton::setButtonEvent(pDialog_->getWidgetCast<GUI::CButton>("SAVE"),fun,SAVE);
		GUI::CButton::setButtonEvent(pDialog_->getWidgetCast<GUI::CButton>("LOAD"),fun,LOAD);
	}
	break;
	}
}

void CDataScene::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DIALOG_INTRO:
		if(getMode()==Mode::SAVE_LOAD) pDialog_->setAlpha(++c_);
		pBlack_->setAlpha(++c_/2);
		if(c_.IsEnd())
		{
			pContext->getInput()->guard(false);
			if(getMode()==Mode::SAVE_LOAD) pDialog_->valid(true);
			setState(DIALOG_S);
		}
	break;

	case DIALOG_S:
		// CANCELされた
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
			setState(DIALOG_STOP);
	break;

	case DIALOG_STOP:
		pContext->getInput()->resetInputState();
		pPanel_->valid(true);
		pBlack_->visible(false);
		if(getMode()==Mode::SAVE_LOAD)	pDialog_->visible(false);
		else							pYesNo_->visible(false);
		setState(NORMAL);
	break;

	case GAME:
		pContext->push(Scene::Mode::LOAD);
		setState(FADE_END);
	break;

	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// CANCELされた
			if(getMode()!=Mode::SAVE_LOAD)
			{
				// LOADモードの時はタイトルへ、それ以外はリターン
				context_.setValue(getMode()==Mode::LOAD ? Scene::ID::TITLE : -1, Flag::NEXT);
				// HADNOVERの時は-1を積んでおく
				if(getMode()==Mode::HANDOVER) pContext->push(-1);
				// フェード準備
				pContext->getInput()->cursolVisible(false);
				pContext->getInput()->guard(true);
				pContext->getApp()->getFoward()->fadeIn();
				setState(FADE_END);
			}
			else
			{// SAVE_LOAD
			 // スタックトップに-1を積んでリターン
				pContext->push(-1);
				getTaskListCtrl()->returnTaskList();
			}
		}
	break;

	default: break;
	}
}

void CDataScene::setData()
{// セーブデータの取得
	// 現在のマップをクリア
	context_.clearHead();
	// セーブディレクトリの取得
	CDir dir;

	string sPath;
	CFile::MakeFullName(BMW::sSaveFolder, sPath);
	dir.SetPath(sPath);
	dir.EnableSubdir(false);
	// セーブデータは、"数字".sav というファイル名
	dir.SetFindFile("*.sav");

	string sFile;
	LONGLONG j;
	CSerialize s;
	Save::CExecDataHead* pHead;
	while(dir.FindFile(sFile)==0)
	{
		// Noを取得
		CStringScanner::StringToNum(sFile.substr(sFile.size()-7,sFile.size()-4),j);
		// セーブデータは100以下まで対応
		if(j>100) break;
		// ヘッダ部分だけシリアライズ
		s.Load(sFile);
		s.SetStoring(false);
		pHead = new Save::CExecDataHead();
		s << *pHead;
		context_.setHead(j,pHead);
		s.Clear();
	}
}

///////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
void CDataScene::eventFade(Task::CTaskContext* pContext)
{
	if(getState()==FADE)
	{
		setState(NORMAL);
		pContext->getInput()->guard(false);
		pContext->getInput()->cursolVisible(true);
		pPanel_->valid(true);
	}
	ef(getState()==FADE_END)
	{	// 設定されてるシーンへジャンプ
		//CDbg().Out(context_.getValue(Flag::NEXT));
		if(context_.getValue(Flag::NEXT)>=0) getTaskListCtrl()->jumpTaskList(context_.getValue(Flag::NEXT));
		else getTaskListCtrl()->returnTaskList();
	}
	else
	{
		int n = context_.top();
		context_.pop();
		pContext->push(n);
		getTaskListCtrl()->returnTaskList();
	}
}

void CDataScene::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsOverIn(pButton))
	{// オーバー
		context_.setValue(pButton->getValue(),Flag::TARGET_DATA);
		updateDataFooter(context_.getHead(pButton->getValue()));
	}
	ef(GUI::IsRelease(pButton))
	{// 押された
		// その場所に、セーブ/ロードをするかを選択する
		// ダイアログ表示したり、しなかったり
		if(getMode()==Mode::LOAD
		|| getMode()==Mode::HANDOVER)
		{
			if(actionLoad())
			{// LOAD成功！
				// LOAD後フラグをスタックに積んでゲームシーンを呼ぶ
				if(getMode()==Mode::LOAD) context_.setValue(Scene::ID::GAME,Flag::NEXT);
				else					  context_.setValue(-1,Flag::NEXT);
				pContext->getApp()->getFoward()->fadeIn();
				pContext->getBgmSound()->FadeOut(30);
				setState(GAME);
				pContext->getInput()->cursolVisible(false);
				pContext->getInput()->guard(true);
				pPanel_->valid(false);
			}
		}
		else
		{
			if(context_.getHead(context_.getValue(Flag::TARGET_DATA))==NULL)
			{// 選択した場所にデータがなければ
				// SAVEする
				int nTarget = context_.getValue(Flag::TARGET_DATA);
				actionSave();

				// コンテキストにデータ追加
				string sFile;
				CSerialize s;
	
				getDataFile(sFile);
				// ヘッダ部分だけシリアライズ
				s.Load(sFile);
				s.SetStoring(false);
				Save::CExecDataHead* pHead = new Save::CExecDataHead();
				s << *pHead;
				context_.setHead(nTarget,pHead);
				// インターフェイスへ反映
				updateDataLine(getDataLine(nTarget),context_.getHead(nTarget),nTarget,pContext->getApp()->getScenario(),!(getMode()==Mode::LOAD || getMode()==Mode::HANDOVER), getMode()==Mode::HANDOVER);
			}
			else
			{
				pPanel_->valid(false);
				if(getMode()==Mode::SAVE_LOAD)
				{// SAVE/LOADダイアログを開く
					c_.Set(0,255,5);
					pDialog_->setAlpha(c_);
					pDialog_->visible(true);
					pBlack_->setAlpha(c_);
					pBlack_->visible(true);
					pContext->getInput()->actionMove(320,240,7,Input::IInput::InputEvent(this,&CDataScene::eventCursol));
				}
				ef(getMode()==Mode::SAVE)
				{// SAVEダイアログを開く
					c_.Set(0,255,10);
					pYesNo_->setIntro(Unit::CYesNoUnit::SAVE,pContext);
					pYesNo_->valid(true);
					pYesNo_->visible(true);
					pBlack_->setAlpha(c_);
					pBlack_->visible(true);
				}
				setState(DIALOG_INTRO);
				pContext->getInput()->guard(false);
			}
		}
	}
}

void CDataScene::eventArrow(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 押された
		switch(pButton->getValue())
		{
		case DOWN:
		{// データを一つ前に
			int i = pCtrl_->getValidWidget()->getTaskPriority();
			if(--i<0) i=9;
			pCtrl_->validWidget(i);
			context_.getApp()->getGlobal().setDataPanel(i);
			pPage_->setText(CStringScanner::NumToString(i+1));
			pPage_->UpdateText();
		}
		break;

		case UP:
		{// データを一つ次に
			int i = pCtrl_->getValidWidget()->getTaskPriority();
			if(++i>9) i=0;
			pCtrl_->validWidget(i);
			context_.getApp()->getGlobal().setDataPanel(i);
			pPage_->setText(CStringScanner::NumToString(i+1));
			pPage_->UpdateText();
		}	
		break;
		}
	}

}

void CDataScene::eventCursol()
{// 何もしない
}

////////////////////////////////////
// ダイアログ用
////////////////////////////////////
void CDataScene::eventDialog(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		switch(pButton->getValue())
		{
		case SAVE:
		{
			actionSave();
			int nTarget = context_.getValue(Flag::TARGET_DATA);
			// コンテキストにデータ追加
			string sFile;
			CSerialize s;
	
			getDataFile(sFile);
			// ヘッダ部分だけシリアライズ
			s.Load(sFile);
			s.SetStoring(false);
			Save::CExecDataHead* pHead = context_.getHead(nTarget);
			s << *pHead;

			updateDataLine(getDataLine(nTarget),context_.getHead(nTarget),nTarget,pContext->getApp()->getScenario(),!(getMode()==Mode::LOAD || getMode()==Mode::HANDOVER), getMode()==Mode::HANDOVER);
			setState(DIALOG_STOP);
			pDialog_->valid(false);
		}
		break;

		case LOAD:
			if(actionLoad())
			{// LOAD成功！
				// LOADしたら、スタックトップに0を積んで、
				// リターンする
				pContext->push(0);
				pContext->getApp()->getFoward()->fadeIn();
				pContext->getBgmSound()->FadeOut(30);
				setState(DIALOG_END);
				pContext->getInput()->cursolVisible(false);
				pContext->getInput()->guard(true);
				pPanel_->valid(false);
				pDialog_->valid(false);
			}
		break;

		default: break;
		}
	}
}

void CDataScene::eventYesNo(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		setState(DIALOG_STOP);
		switch(pButton->getValue())
		{
		case YES:
		{// SAVEするずら
			actionSave();
			int nTarget = context_.getValue(Flag::TARGET_DATA);
			// コンテキストにデータ追加
			string sFile;
			CSerialize s;
	
			getDataFile(sFile);
			// ヘッダ部分だけシリアライズ
			s.Load(sFile);
			s.SetStoring(false);
			Save::CExecDataHead* pHead = context_.getHead(nTarget);
			s << *pHead;
			updateDataLine(getDataLine(nTarget),context_.getHead(nTarget),nTarget,pContext->getApp()->getScenario(),!(getMode()==Mode::LOAD || getMode()==Mode::HANDOVER), getMode()==Mode::HANDOVER);
		}
		break;

		default: break;
		}
	}
}

void CDataScene::eventCancel(Task::CTaskContext* pContext)
{// 何もしない
}

//////////////////////////////////////////
// アクション
//////////////////////////////////////////
void CDataScene::actionSave()
{// TARGET_DATAに設定されてるデータに、セーブする
	CSerialize s;
	s << context_.getApp()->getExec();
	string sFile;
	getDataFile(sFile);
	s.Save(sFile);
}

bool CDataScene::actionLoad()
{// TARGET_DATAに設定されてるデータを、ロードする
	CSerialize s;
	string sFile;
	getDataFile(sFile);
	// ファイルが存在するかを確認
	if(!CDir().IsFileExist(sFile)) return false;
	s.Load(sFile);
	s.SetStoring(false);
	// 現在のデータを取得
	Save::CExecData& data = context_.getApp()->getExec();
	// データを初期化
	data.clear();
	// デシリアライズ
	s << data;

	//data.setExpert(50);

	// ヘルプフラグリセット
	// 第三部から追加の部分だけ
	// 定義されてないか、すでにこなしてたらリセットしない
	Unit::Help::resetLoadHelpFlag(data);

	return true;
}

void CDataScene::getDataFile(string& sFile)
{
	sFile.clear();
	sFile=BMW::sSaveFolder + "\\";
	sFile += CStringScanner::NumToStringZ(context_.getValue(Flag::TARGET_DATA),3);
	sFile += ".sav";
}

///////////////////////////////////////////////
// インターフェイス
///////////////////////////////////////////////
GUI::CPanel* CDataScene::getDataLine(int nID)
{
	return pCtrl_->getWidgetCast<GUI::CPanel>(nID/10)->getWidgetCast<GUI::CPanel>(nID%10);
}

void CDataScene::updateDataFooter(Save::CExecDataHead* pHead)
{
	Scenario::CScenarioDB& db = context_.getApp()->getScenario();
	if(pHead==NULL
	|| (getMode()==Mode::HANDOVER) 
		&& !(pHead->getStory()==db.getScenarioID("CLEAR_NORMAL")
			|| pHead->getStory()==db.getScenarioID("CLEAR_GOOD")
			|| pHead->getStory()==db.getScenarioID("CLEAR_TRUE")
			)
		)
	{// データがない
		pFooter_->visibleAll(false);
		pFooter_->visible(true);
		pFooter_->getWidget("BACK")->visible(true);
	}
	else
	{// データあり
		pFooter_->visibleAll(true);
		pFooter_->visible(true);

		Scenario::CScenarioDB& db = context_.getApp()->getScenario();
		GUI::CNum* pNum = pFooter_->getWidgetCast<GUI::CNum>("CLEARCHAPTER");

		int nStory = db.getNo(pHead->getStory());
		pNum->setNum(nStory);
		//TITLE
		GUI::CText* pText = pFooter_->getWidgetCast<GUI::CText>("CLEARTITLE");
		pText->setText(db.getTitle(pHead->getStory()));
		pText->getFontConf().SetWeight(700);
		pText->UpdateTextA();
		
		// キャラアイコン
		symbol_.clearSymbol();
		Chara::CCharaDB& chara = const_cast<Chara::CCharaDB&>(context_.getApp()->getChara());
		Chara::CDataCharaData* pData = chara.getCharaData(pHead->getAce());
		while(pData->getInit().getMapSymbolID().empty()){ pData = pData->getParentData().getPointer(); }
		
		symbol_.setSymbol(pData->getInit().getMapSymbolID());
		symbol_.setGraphicGui(pFooter_->getWidgetCast<GUI::CGraphic>("ACEPILOT"),"BEFORE_LEFT");

		// 週目
		pText = pFooter_->getWidgetCast<GUI::CText>("SYUUME");
		pText->visible(pHead->getClear()>0);
		pText = pFooter_->getWidgetCast<GUI::CText>("SHUUKAI");
		pText->visible(pHead->getClear()>0);
		pText->setText(CStringScanner::NumToString((pHead->getClear()+1)>HANDOVER_MAX ? HANDOVER_MAX : (pHead->getClear()+1)));
		pText->UpdateText();

		// 敵養成段階
		/*pText = pFooter_->getWidgetCast<GUI::CText>("TEKI_YOUSEI");
		pText->visible(pHead->getClear()>0);
		pText = pFooter_->getWidgetCast<GUI::CText>("TEKI_YOUSEI_NUM");
		pText->visible(pHead->getClear()>0);
		if(pHead->getClear()>0)
		{
			int nEnemyTrain;
			context_.getApp()->getExec().getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nEnemyTrain);
			pText->setText(CStringScanner::NumToString(nEnemyTrain));
			pText->UpdateText();
		}*/
	}
}

} // namespace Data end
} // namespace BMW end