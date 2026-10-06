#include "stdafx.h"

#ifdef BMW_DEBUG
#include "../Input/CTaskInput.h"
#endif

#include "../Weapon/IDWeapon.h"
#include "../Weapon/CalcWeapon.h"
#include "../Weapon/CDataWeaponBattle.h"

#include "../Scene/IScene.h"

#include "../SLG/IDSLG.h"

#include "DictFunction.h"

#include "CDictCharaScene.h"
#include "CDictCharaView.h"

namespace BMW{
namespace Dict{

CDictCharaView::CDictCharaView()
{
	pWeaponData_ = new Weapon::CDataWeaponBattle();
}

CDictCharaView::~CDictCharaView()
{
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(pWeaponData_);
}

void CDictCharaView::Task(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);

	if(pContext->IsAction() && IsValid())
		OnAction(pContext);
}

namespace{
// 武器選択のボタンイベントハンドラ設定関数
__inline void setWeaponButtonEvent(GUI::CPanelCtrl* pPanel, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	GUI::CButton* pButton;
	for(int i=0; i<14; ++i)
	{
		pPanel->validWidget(i);
		pButton = pPanel->getValidWidgetCast<GUI::CButton>();
		pButton->setEventHandler(fun);
		if(nValue>=0) pButton->getEvent()->setValue(nValue);
	}
}
} // namesapce end

void CDictCharaView::OnInit(Task::CTaskContext* pContext)
{
	// 戦闘背景リスト読み込み
	battleBackMap_.readMapFile(Config::Const::configDB_.getConfigFileStr("BATTLE_BACK"));
	// インターフェイス生成
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARA_PAGE");

	// ふりがなとか
	GUI::CPanel* pData = pPanel_->getWidgetCast<GUI::CPanel>("DATA_CHARA");
	pData->getWidgetCast<GUI::CText>("NCV")->UpdateText();
	pData->getWidgetCast<GUI::CText>("ORIGIN")->UpdateText();

	// テキスト
	pText_ = pPanel_->getWidgetCast<GUI::CText>("TEXT");
	pText_->getFontConf().SetHeight(18);

	// 武器アイコン
	GUI::CPanel* pWeapon = pPanel_->getWidgetCast<GUI::CPanel>("WEAPON_ICON");
	apWeapon_[0] = pWeapon->getWidgetCast<GUI::CPanelCtrl>("WEAPON1");
	apWeapon_[1] = pWeapon->getWidgetCast<GUI::CPanelCtrl>("WEAPON2");
	apWeapon_[2] = pWeapon->getWidgetCast<GUI::CPanelCtrl>("WEAPON3");
	apWeapon_[3] = pWeapon->getWidgetCast<GUI::CPanelCtrl>("WEAPON4");
	
	// キャラ替え
	GUI::CPanel* pChangeChara = pPanel_->getWidgetCast<GUI::CPanel>("PAGE_CHANGE_CHARA");
	pChangeChara->getWidgetCast<GUI::CText>("PAGE_SLASH")->UpdateText();
	pCurrentNum_ = pChangeChara->getWidgetCast<GUI::CText>("PAGE_NOW");
	// 最大人数は確定しているはず
	GUI::CText* pMax = pChangeChara->getWidgetCast<GUI::CText>("PAGE_MAX");
	CStringScanner::NumToStringZ(static_cast<CDictCharaContext*>(pContext)->getCharaItemMap().size(),pMax->getText(),3);
	pMax->UpdateText();

	// ページ替え
	pChange_ = pPanel_->getWidgetCast<GUI::CPanel>("CHANGE_BUTTON");
	pTextPage_ = pChange_->getWidgetCast<GUI::INum>("PAGE");

	// コメント・設定タブボタン
	GUI::CPanel* pPlot = pPanel_->getWidgetCast<GUI::CPanel>("PANEL_PLOT");

	GUI::CPanel* pOrigin = pPlot->getWidgetCast<GUI::CPanel>("PLOT_ORIJINAL");
	pOrigin->getWidgetCast<GUI::CText>("ORIJINAL")->UpdateText();
	pPlotButton_[0]=pOrigin->getWidgetCast<GUI::CButton>("BAR_CHARA");

	GUI::CPanel* pBmw = pPlot->getWidgetCast<GUI::CPanel>("PLOT_BMW");
	pBmw->getWidgetCast<GUI::CText>("BMW")->UpdateText();
	pPlotButton_[1]=pBmw->getWidgetCast<GUI::CButton>("BAR_CHARA");

	GUI::CPanel* pComment = pPlot->getWidgetCast<GUI::CPanel>("PLOT_COMMENT");
	pComment->getWidgetCast<GUI::CText>("COMMENT")->UpdateText();
	pPlotButton_[2]=pComment->getWidgetCast<GUI::CButton>("BAR_CHARA");

	// 匠・陽菜両ルートクリアしてないと出現しない
	bool bComment =
#ifdef BMW_DEBUG
	true;
#else
	pContext->getApp()->getGlobal().IsTakumiClear() && pContext->getApp()->getGlobal().IsHarunaClear();
#endif
	pComment->valid(bComment);
	pComment->visible(bComment);

	// ボタンハンドラ設定
	GUI::CButton::ButtonEvent funButton(this,&CDictCharaView::eventButton);
	GUI::CButton::setButtonEvent(pChange_->getWidgetCast<GUI::CButton>("BUTTON"), funButton, CHANGE);
	GUI::CButton::setButtonEvent(pChangeChara->getWidgetCast<GUI::CButton>("CHARALEFT"), funButton, LEFT);
	GUI::CButton::setButtonEvent(pChangeChara->getWidgetCast<GUI::CButton>("CHARARIGHT"), funButton, RIGHT);
	for(int i=0; i<4; ++i) setWeaponButtonEvent(apWeapon_[i], funButton, WEAPON1 + i);
	GUI::CButton::setButtonEvent(pPlotButton_[0], funButton, ORIGINAL);
	GUI::CButton::setButtonEvent(pPlotButton_[1], funButton, BMW);
	GUI::CButton::setButtonEvent(pPlotButton_[2], funButton, COMMENT);
}

void CDictCharaView::OnReset(Task::CTaskContext* pContext)
{
	// 現在のキャラデータを取得
	pCurrentItem_ = static_cast<CDictCharaContext*>(pContext)->getCurrentCharaItem();

	// キャラデフォ
	GUI::CGraphic* pDef = pPanel_->getWidgetCast<GUI::CGraphic>("CHARA_DEFAULT");
	pCurrentItem_->getSymbolDB().setGraphic(pDef, "DEFO");

	// キャラ名前
	GUI::CText* pName = pPanel_->getWidgetCast<GUI::CText>("CHARA_NAME");
	pName->setText(pCurrentItem_->getName());
	pName->UpdateTextAA();

	// ふりがなとか原作とかそういうの
	GUI::CPanel* pData = pPanel_->getWidgetCast<GUI::CPanel>("DATA_CHARA");
	GUI::CText* pText;
	// ふりがな
	pText = pData->getWidgetCast<GUI::CText>("RUBY");
	pText->setText(pCurrentItem_->getRuby());
	pText->UpdateText();
	// NCV
	pText = pData->getWidgetCast<GUI::CText>("NCV_NAME");
	pText->setText(pCurrentItem_->getNcv());
	pText->UpdateText();
	// GAME_NAME
	pText = pData->getWidgetCast<GUI::CText>("GAME_NAME");
	string sOrigin;
	if(pCurrentItem_->getOrigin()=="TSUKIHIME")		sOrigin="月姫";
	ef(pCurrentItem_->getOrigin()=="MELTY")			sOrigin="Melty Blood";
	ef(pCurrentItem_->getOrigin()=="KARA")			sOrigin="空の境界";
	ef(pCurrentItem_->getOrigin()=="FATE")			sOrigin="Fate/stay night";
	ef(pCurrentItem_->getOrigin()=="HOLLOW")		sOrigin="Fate/hollow ataraxia";
	ef(pCurrentItem_->getOrigin()=="ZERO")			sOrigin="Fate/Zero";
	ef(pCurrentItem_->getOrigin()=="UNLIMIT")		sOrigin="Fate/unlimited codes";
	ef(pCurrentItem_->getOrigin()=="MATERIAL")		sOrigin="character material";
	ef(pCurrentItem_->getOrigin()=="TAKE")			sOrigin="TAKE MOON";
	else											sOrigin="Werk";
	pText->setText(sOrigin);
	pText->UpdateText();

	// 武器アイコン設定
	for(int i=0; i<4; ++i)
	{
		apWeapon_[i]->valid(false);
		apWeapon_[i]->visible(false);
	}
	setWeaponIcon(pContext);

	// ページ構築（7行1ページ）
	createPage();

	// まずは原作設定1ページ目から
	setPlot(PLOT_ORIJINAL);

	// キャラ変更人数設定
	CStringScanner::NumToStringZ(pCurrentItem_->getID(),pCurrentNum_->getText(),3);
	pCurrentNum_->UpdateText();

	pContext->getInput()->cursolVisible(true);
	pContext->getInput()->guard(false);
}

void CDictCharaView::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case DEMO:
		pContext->getScene()->setState(CDictCharaScene::DEMO);
		setState(NORMAL);
	break;

	case NORMAL:
		// キャンセルしたら、キャラセレへ
		if(Input::releaseCancel(pContext))
		{
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			pContext->getApp()->getFoward()->clearPopUp();
			getTaskListCtrl()->returnTaskList();
		}
	break;
	
	default: break;
	}
}

////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////
void CDictCharaView::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{
		switch(pButton->getValue())
		{
		case LEFT:
			static_cast<CDictCharaContext*>(pContext)->decCharaIterator();
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			OnReset(pContext);
		break;

		case RIGHT:
			static_cast<CDictCharaContext*>(pContext)->incCharaIterator();
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			OnReset(pContext);
		break;

		case CHANGE:
		{
			if(++nCurrentPlotPage_>nPlotMaxPage_[nCurrentPlot_])
			{
				setPlot(nCurrentPlot_);	
			}
			else
			{
				pTextPage_->setNum(nCurrentPlotPage_);
				++page_it_;
			}
			string& str = pText_->getText();
			str = *page_it_;
			pText_->UpdateText();
		}
		break;

		case ORIGINAL:
			setPlot(PLOT_ORIJINAL);
		break;

		case BMW:
			setPlot(PLOT_BMW);
		break;

		case COMMENT:
			setPlot(PLOT_COMMENT);
		break;

		case WEAPON1:
		case WEAPON2:
		case WEAPON3:
		case WEAPON4:
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			setDemoData(pButton->getValue(),pContext);
		break;
		}
	}
}

///////////////////////////////////////
// 設定ヘルパ
///////////////////////////////////////
void CDictCharaView::setWeaponIcon(Task::CTaskContext* pContext)
{
	GUI::CPanel* pWeaponPanel = pPanel_->getWidgetCast<GUI::CPanel>("WEAPON_ICON");
	pWeaponPanel->validAll(false);
	pWeaponPanel->visibleAll(false);

	// 武器リストを回しつつIDなどGUI設定に必要な情報を取得
	Weapon::CWeaponDB& db = const_cast<Weapon::CWeaponDB&>(pContext->getApp()->getWeapon());
	Weapon::CDataWeaponInit* pWeapon;
	map<int,int> strRank,mgcRank;
	int j=0;
	Chara::CDataCharaBattle& battle = pCurrentItem_->getCharaData().getBattle();
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		int nID = *battle.nextWeapon();
		pWeapon = db.getData(nID);

		// 治癒・補給武器はアニメが無いのでスルー
		if(pWeapon->getKind()==Weapon::Kind::CURE || pWeapon->getKind()==Weapon::Kind::REFILL)
			continue;
		
		// ランク付け
		if(pWeapon->getKind()==Weapon::Kind::FIGHT)
			strRank.insert(pair<int,int>(pWeapon->getAttack(),j));
		ef(pWeapon->getKind()==Weapon::Kind::MAGIC)
			mgcRank.insert(pair<int,int>(pWeapon->getAttack(),j));

		apWeapon_[j]->valid(true);
		apWeapon_[j]->visible(true);
				
		// ID設定
		nWeaponID_[j++] = nID;
	}
	// ランク付け確定
	int nRank[4]={0,0,0,0};
	int n=0;
	map<int,int>::iterator it;
	// 腕力
	for(it=strRank.begin(); it!=strRank.end(); ++it) nRank[it->second]=n++;

	n=0;
	// 魔力
	for(it=mgcRank.begin(); it!=mgcRank.end(); ++it) nRank[it->second]=n++;

	// アイコン選択
	for(int i=0; i<j; ++i)
	{
		pWeapon = db.getData(nWeaponID_[i]);
		apWeapon_[i]->validWidget(Weapon::getIconID(pWeapon->getKind(), apWeapon_[i]) + nRank[i]);
		apWeapon_[i]->getValidWidgetCast<GUI::CButton>()->setPopUp(pWeapon->getName());
	}
}

void CDictCharaView::setPlot(int nPlot)
{
	nCurrentPlot_=nPlot;
	nCurrentPlotPage_=1;
	// ページ数更新
	pTextPage_->setNum(nCurrentPlotPage_);
	// 対象となるリストを取得
	switch(nPlot)
	{
	case PLOT_COMMENT:	page_it_ = pageComment_.begin(); break;
	case PLOT_BMW:		page_it_ = pageIntro_.begin(); break;
	default:			page_it_ = pageProfile_.begin(); break;
	}
	// ページ構築
	string& str = pText_->getText();
	str = *page_it_;
	pText_->UpdateText();

	// 最大ページ数が1だったらchangeは表示しない
	pChange_->valid(1<nPlotMaxPage_[nPlot]);
	pChange_->visible(1<nPlotMaxPage_[nPlot]);
}

void CDictCharaView::createPage()
{
	pageProfile_.clear();
	createPageHelper(pCurrentItem_->getProfile(),pageProfile_);
	nPlotMaxPage_[PLOT_ORIJINAL] = pageProfile_.size();

	pageIntro_.clear();
	createPageHelper(pCurrentItem_->getIntro(),pageIntro_);
	nPlotMaxPage_[PLOT_BMW] = pageIntro_.size();

	pageComment_.clear();
	createPageHelper(pCurrentItem_->getComment(),pageComment_);
	nPlotMaxPage_[PLOT_COMMENT] = pageComment_.size();
}

void CDictCharaView::setDemoData(int nWeaponSelect, Task::CTaskContext* pContext)
{
	// 攻撃側SLGデータ
	SLG::CDataCharaSLG* pAttack = &pCurrentItem_->getCharaData();
	// 戦闘データ
	// デモ再生に必要データを設定
	smart_ptr<SLG::CDataBattle>& pBattle = pContext->getBattleData();
	pBattle->clearBattleData();
	// 攻撃スタートサイド
	pBattle->setSide(pAttack->getPhase()==SLG::Phase::PLAYER ? SLG::CDataBattle::RIGHT : SLG::CDataBattle::LEFT);
	// 戦闘背景ランダム選択
	pBattle->setBack(battleBackMap_.getValue(CApp::rand_.Get(battleBackMap_.getMapSize())));
	// イベントモードでは無い
	pBattle->event(false);

	// 攻撃
	SLG::CDataBattleBase& attack = pBattle->getBattleData(SLG::CDataBattle::ATTACK);
	attack.setChara(smart_ptr<SLG::CDataCharaSLG>(pAttack,false));
	pWeaponData_->setStatus(
		*(const_cast<Weapon::CWeaponDB&>(pContext->getApp()->getWeapon()).getData(nWeaponID_[nWeaponSelect])
		));
	attack.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(pWeaponData_,false));
	attack.getAttack().setEN(pWeaponData_->getEN());

	// 敵キャラ選択
	int nAgainstPhase =  pAttack->getPhase();
	if(pWeaponData_->getKind()==Weapon::Kind::STATUS) 
	{// STATUSだったら、ひっくり返す
		if(nAgainstPhase==SLG::Phase::PLAYER) nAgainstPhase=SLG::Phase::ENEMY;
		else								  nAgainstPhase=SLG::Phase::PLAYER;
	}
	CDictCharaItem* pAgainst = static_cast<CDictCharaContext*>(pContext)->getAgainstCharaData(nAgainstPhase);
	// 反撃側SLGデータ
	SLG::CDataCharaSLG* pCounter = &pAgainst->getCharaData();
	// 反撃
	SLG::CDataBattleBase& def = pBattle->getBattleData(SLG::CDataBattle::COUNTER);
	def.setChara(smart_ptr<SLG::CDataCharaSLG>(pCounter,false));

	// ダメージ計算
	// 技能効果は完全に無視
	// 能力値のみ反映

	// 武器種別に合わせて腕力/魔力の選択
	// 基本攻撃力計算
	int nStatus;
	switch(pWeaponData_->getKind())
	{
	case Weapon::Kind::FIGHT:
	case Weapon::Kind::FIGHT_COLLAB:
	case Weapon::Kind::FIGHT_COND:
		nStatus = pAttack->getBattle().getStrength();
	break;

	case Weapon::Kind::MAGIC:
	case Weapon::Kind::MAGIC_COLLAB:
	case Weapon::Kind::MAGIC_COND:
		nStatus = pAttack->getBattle().getMagic();
	break;

	default: nStatus=100; break;
	}
	int nAttack = pWeaponData_->getAttack() * ( nStatus + 100) / 200;
	// 基本防御計算
	int nDefence = pCounter->getBattle().getTough() * (pCounter->getBattle().getDefence() + 100) / 200;
	// ダメージ
	int nDamage = nAttack - nDefence;
	attack.getAttack().setDamage(nDamage<10?10:nDamage);

#ifndef BMW_DEBUG
	def.getDefence().setAction(SLG::Battle::HIT);
#else
	// 押してるキーによって防御行動が変化
	CKeyInput& key = static_cast<Input::CTaskInput*>(pContext->getInput())->getKeyBoard();
	int nState;
	if(key.IsKeyPress(DIK_D))
	{
		nState=SLG::Battle::DEFENCE;
		attack.getAttack().setDamage(attack.getAttack().getDamage()/2);
	}
	ef(key.IsKeyPress(DIK_A))
	{
		nState=SLG::Battle::AVOID;
		attack.getAttack().setDamage(0);
	}
	else
	{	nState=SLG::Battle::HIT; }

	def.getDefence().setAction(nState);
#endif



	// 音楽
	// 武器に設定されてるのも考慮しないと：TODO
	int nBgm;
	if(pWeaponData_->getBgmID()>=0)
		nBgm = pWeaponData_->getBgmID();
	else 
		nBgm = (pAttack->getBattle().getBgmID()<0 ? pCounter->getBattle().getBgmID() : pAttack->getBattle().getBgmID());

	pContext->getBgmSound()->change(nBgm);
	pContext->getBgmSound()->FadeIn(15);

	// se読み直し
	pContext->getApp()->clearSeCache();

	// デモを呼ぶ！
	setState(DEMO);
}
} // namespace Dict end
} // namespace BMW end
