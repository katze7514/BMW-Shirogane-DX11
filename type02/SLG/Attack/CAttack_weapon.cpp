#include "stdafx.h"

#include "../../Weapon/CalcWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Status/status_fun.h"
#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CCircleMenu.h"
#include "../../Scene/GUI/CCircleMenuButton.h"
#include "../../Scene/Unit/IDHelp.h"

#include "../slg_fun.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/COffsetWeapon.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CAttack_calc.h"
#include "CAttack_range2.h"
#include "CAttack_weapon.h"

namespace BMW{
namespace SLG{
namespace Attack{
namespace{
// ボタンタスク取得関数
__inline GUI::CPanelCtrl* getCircleButtonTask(GUI::CCircleMenu* pMenu, const string& sID)
{
	return pMenu->getButton(sID)->getTaskCast<GUI::CPanel>()->getWidgetCast<GUI::CPanelCtrl>("ICON");
}

// 武器選択のボタンイベントハンドラ設定関数
__inline void setEventButtonCtrl(GUI::CPanelCtrl* pPanel, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	//Task::CTaskCtrl<>::task_map::iterator it;
	//Task::CTaskCtrl<>::task_map& mapTask = pPanel->getWidgetCtrl().getTaskMap();
	GUI::CButton* pButton;
	//for(it=mapTask.begin(); it!=mapTask.end(); it++)
	//{
		for(int i=0; i<14; ++i)
		{
			pPanel->validWidget(i);
			pButton = pPanel->getValidWidgetCast<GUI::CButton>();
			pButton->setEventHandler(fun);
			if(nValue>=0) pButton->getEvent()->setValue(nValue);
		}
	//}
}

// 上記二つを組み合わせたヘルパ
__inline void setEventButtonMenuCtrl(GUI::CCircleMenu* pMenu, const string& sID, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	setEventButtonCtrl(getCircleButtonTask(pMenu, sID), fun, nValue);
}

} // namepsace end
void CAttack_weapon::OnReset(Task::CTaskContext* pContext)
{// 受け入れ口を作るだけ作っておく
	// ある意味省略記号ｗ
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// キャンセル
	addTask(new BMW::Rule::CRuleCancel(CANCEL),CANCEL_T);

	// サークルメニュー取得
	pMenu_ = db.createInterfaceCast<GUI::CCircleMenu>("WEAPON_CIRCLE");
	addTask(pMenu_, MENU);
	// メニューイベントハンドラ設定
	GUI::CCircleMenu::CircleEvent funCircle;
	funCircle.set(this,&CAttack_weapon::eventCircle);
	pMenu_->setEventHandler(funCircle);

	// ボタンイベントハンドラ設定
	GUI::CButton::ButtonEvent funButton;
	funButton.set(this,&CAttack_weapon::eventButton);
	for(int i=1; i<=6; i++) // 全ボタンにイベントハンドラを設定
		setEventButtonMenuCtrl(pMenu_, Misc::linkStrAndNum("WEAPON",i), funButton, i-1);

	// 武器パネル取得
	// 援護用か通常用で生成するパネルが変わる
	pWeapon_ = db.createInterfaceCast<GUI::CPanel>(pContext->top()==1 ? "PANEL_WEAPONSELECT" : "PANEL_WEAPONSELECT2");
	addTask(pWeapon_, WEAPON);

	if(pContext->top()!=1)
	{// 通常用だったら、武器文字変更
		GUI::CText* pText = pWeapon_->getWidgetCast<GUI::CText>("WEAPONNAME");
		// 影付き
		pText->getFont()->SetBackColor(RGB(0,0,0));
		pText->getFont()->SetShadowOffset(2,2);
	}
}

void CAttack_weapon::OnInit(Task::CTaskContext* pContext)
{// 武器データを設定する
	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 表示座標設定
	pMenu_->setX(p->getCircleX());
	pMenu_->setY(p->getCircleY());

	// スタックトップに援護フラグ・必要距離・実距離・高さと積まれている
	bBack_ = p->top()==1;
	p->pop();
	int nDist = p->top();
	p->pop();
	int nRealDist = p->top();
	p->pop();
	int nHeight = p->top();
	p->pop();
	// 対象キャラデータの取得
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	Chara::CDataCharaBattle& battle = pChara->getBattle();

	// 武器データ設定
	// 設定前に状態をクリア
	mapWeaponID_.clear();
	pMenu_->resetButton();

	int							i=1;
	int							nID;
	string						sID;
	Weapon::CDataWeaponBattle*	pWeapon;
	bool						bEnable;
	GUI::CPanel*				pPanel;
	GUI::CPanelCtrl*			pCtrl;
	// 設定ループ
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		nID = *battle.nextWeapon();

		// 基本
		pWeapon = p->getWeaponData(nID);

		// 武器選択には、治療と補給武器は表示しない
		if(pWeapon->getKind()==Weapon::Kind::CURE
		|| pWeapon->getKind()==Weapon::Kind::REFILL)
			continue;

		// 援護攻撃時は、能力変化も合体攻撃もフィールド属性武器も表示しない
		if(bBack_
		&& (pWeapon->IsF()
			|| pWeapon->getKind()==Weapon::Kind::STATUS
			|| pWeapon->getKind()==Weapon::Kind::FIGHT_COLLAB
			|| pWeapon->getKind()==Weapon::Kind::MAGIC_COLLAB))
			continue;

		// 使用可能か？
		bEnable = pWeapon->enable(*pChara, 
								  pChara->getState().getAct()!=Act::BEFORE,
								  *p, 
								  nDist,
								  nRealDist,
								  nHeight);
		// 合体武器の場合は、ちょいと状況が戻ってくる
		if(pWeapon->getKind()==Weapon::Kind::FIGHT_COLLAB
		|| pWeapon->getKind()==Weapon::Kind::MAGIC_COLLAB)
		{// スタックトップが1だったら、使用できなく、かつ選択に出さない
			int nR=p->top();
			p->pop();
			if(nR) continue;
		}

	#ifdef BMW_DEBUG
		CDbg().Out("EndWeapon2 %d",battle.endWeapon());
	#endif

		// 武器IDマップへ追加
		weapon_state state(nID,bEnable);
		mapWeaponID_.insert(pair<int,weapon_state>(i-1, state));

		// インターフェイスの設定
		sID = "WEAPON" + CStringScanner::NumToString(i);
		// ボタンの有効化
		pMenu_->validButton(true,sID,p);
		pPanel = pMenu_->getButton(sID)->getTaskCast<GUI::CPanel>();
		// アイコンの設定
		pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("ICON");
		pCtrl->validWidget(Weapon::getIconID(pWeapon->getKind(),pCtrl) + pWeapon->getRank());
		// 属性の設定
		pCtrl = pPanel->getWidgetCast<GUI::CPanelCtrl>("PMT");
		pCtrl->validWidget(Weapon::getAttrID(pWeapon->IsP(),
											  pWeapon->IsM(),
											  pWeapon->IsT(),
											  pWeapon->IsF(),
											  pCtrl));
		// ペケの設定
		pPanel->getWidget("PEKE")->visible(!bEnable);
		
		++i;
	}

	// 中央パネルはとりあえず非表示
	pWeapon_->visible(false);

	// メニュー動作
	pMenu_->setState(GUI::CCircleMenu::INTRO);
	pMenu_->OnReset(p);
}

void CAttack_weapon::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CANCEL:
	// キャンセルされたら、スタックに-1を積んでリターン
		pContext->push(-1);
		if(!bBack_) Map::CMapChipState::attack(false);
	case OK:
		// 決定されたら、決定した武器IDがスタックトップに積まれているので、
		// そのままリターン
		actionMenu(GUI::CCircleMenu::EXIT,pContext);
		pWeapon_->visible(false);
		Map::CMapChipState::mapValid(true);
	break;

	default: break;
	}
}
////////////////////////////////////////////////
// アクション
////////////////////////////////////////////////
void CAttack_weapon::actionMenu(int nState, Task::CTaskContext* pContext)
{
	IListenerCircleMenu::actionMenu(nState,pContext);
	setState(NORMAL);
}

////////////////////////////////////////////////
// イベントハンドラ
////////////////////////////////////////////////
void CAttack_weapon::eventCircle(int nState, Task::CTaskContext* pContext)
{
	if(nState==GUI::CCircleMenu::EXIT)
	{// 動作終了したら、リターン
		getTaskListCtrl()->returnTaskList();
	}
	else
	{// 登場動作終了
		pContext->getInput()->guard(false);

		// ヘルプモード
		if(!bBack_) callHelp(Unit::Help::SLG_ATK, "SLG_ATK", pContext);
	}
}

void CAttack_weapon::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(pButton->getState()==GUI::CButton::RELEASE)
	{// ボタンが押されたらスタックに武器IDを積む
		w_state_map::iterator it=mapWeaponID_.find(pButton->getValue());
		if(it->second.bEnable_)
		{// 有効な武器のみ反応する
			pContext->push(it->second.nWeaponID_);
			// ボタン押された！
			setState(OK);
			// 射程を設定する
			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			Weapon::CDataWeaponBattle* pWeapon = p->getWeaponData(it->second.nWeaponID_);
			// 攻撃可能範囲
			Map::CMapChipState::setRangeData(pWeapon,p->getCtrlCharaData()->getState().getWay());
		}
	}
	ef(pButton->getState()==GUI::CButton::OVER_IN)
	{// ボタンに乗ったら
	 //  中央の表示変更
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		w_state_map::iterator it=mapWeaponID_.find(pButton->getValue());
		Weapon::CDataWeaponBattle* pWeapon = p->getWeaponData(it->second.nWeaponID_);
		setWeaponDataGui(pWeapon_,
						 *pWeapon,
						 *p->getCtrlCharaData());
		// 位置に合わせて表示位置変更
		setWeaponDataGuiPos(pButton->getValue());

		pWeapon_->visible(true);

		if(!bBack_) // 援護攻撃選択でなければ
		{// 攻撃可能範囲表示
			Map::CMapChipState::setRangeData(pWeapon,Way::ALL); // ←としておくとLINE型フィールド兵器の時に全部表示される
			Map::CMapChipState::attack(true);
			Map::CMapChipState::mapValid(it->second.bEnable_);
		}
	}
	ef(pButton->getState()==GUI::CButton::OVER_OUT)
	{// ボタンがはずれたら
	 //  中央の表示変更
		pWeapon_->visible(false);
		// 攻撃可能範囲表示消し
		if(!bBack_) Map::CMapChipState::attack(false);
	}
}

///////////////////////////////////////////////////
// 武器選択設定
///////////////////////////////////////////////////
namespace{
// 数字～数字という文字列を生成する
__inline void createRange(int nMin, int nMax, string& sPopUp)
{
	if(nMin==nMax)
		sPopUp += CStringScanner::NumToString(nMin);
	else
		sPopUp += CStringScanner::NumToString(nMin) + "～" + CStringScanner::NumToString(nMax);
}

// テキスト設定
__inline void setTextGui(BMW::GUI::CText* pText, const string& sText, COLORREF color=CLR_INVALID)
{
	pText->setText(sText);
	if(color!=CLR_INVALID) pText->setColor(color);
	pText->UpdateTextAA();
}

}// namespace end

// 武器のポップアップ文字列生成
/**
	名前
	射程（中心）：
	到達：
*/
void CAttack_weapon::createWeaponPopUp(const string& sName,
									   int nMin,		int nMax,
									   int nCoreMin,	int nCoreMax,
									   int nReach,
									   string& sPopUp)
{
	sPopUp.clear();
	// 名前
	sPopUp = sName + "\n";
	// 射程（中心）：
	sPopUp += "射程（中心）：";
	createRange(nMin,nMax,sPopUp);
	sPopUp += "（";
	createRange(nCoreMin,nCoreMax,sPopUp);
	sPopUp += "）\n";
	// 到達
	sPopUp += "到達：" + CStringScanner::NumToString(nReach);
}

/////////////////////////////////////////////
// 武器選択時に真ん中に表示されるやつの設定
/////////////////////////////////////////////
void CAttack_weapon::setWeaponDataGui(GUI::CPanel* pWeapon, Weapon::CDataWeaponBattle& weapon, SLG::CDataCharaSLG& chara)
{// データ設定
	// 設定になげる
	Status::setStatusWeaponLine(pWeapon,weapon,false);
	// 命中補正
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("HIT_REVISION"),
			   (weapon.getHit()>=0 ? "+" : "-") + CStringScanner::NumToString(weapon.getHit()),
			   bBack_ ? RGB(63,57,54) : RGB(255,255,255));
	// クリティカル補正
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("CT_REVISION"),
			  "+" + CStringScanner::NumToString(weapon.getCT()),
			  bBack_ ? RGB(63,57,54) : RGB(255,255,255));
	// 消費エネルギー
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("EN_SPEND"),
			   weapon.getEN()==0?"---":CStringScanner::NumToString(weapon.getEN()),
			   bBack_ ? RGB(63,57,54) : RGB(255,255,255));
	// 現在エネルギー
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("EN_MAX"),
				CStringScanner::NumToString(chara.getBattle().getEN()),
				weapon.getEN()<=chara.getBattle().getEN() ? (bBack_ ? RGB(63,57,54) : RGB(255,255,255))
														  : RGB(226,6,83));
	// 全弾
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("TAMA_ALL"),
			   weapon.getBallet()==0?"--":CStringScanner::NumToString(weapon.getBallet()),
			   bBack_ ? RGB(63,57,54) : RGB(255,255,255));
	// 残弾
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("TAMA_NOW"),
				weapon.getBallet()==0?"--":CStringScanner::NumToString(weapon.getBalletRest()),
			   (weapon.getBallet()==0 || weapon.getBalletRest()>0) ? (bBack_ ? RGB(63,57,54) : RGB(255,255,255))
																   : RGB(226,6,83));
	// 必要気力
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("KIRYOKU_WANT"),
			   weapon.getMental()==0?"---":CStringScanner::NumToString(weapon.getMental()),
			   bBack_ ? RGB(63,57,54) : RGB(255,255,255));
	// 現在気力
	setTextGui(pWeapon->getWidgetCast<GUI::CText>("KIRYOKU_NOW"),
			   CStringScanner::NumToString(chara.getBattle().getMental()),
			   weapon.getMental()<=chara.getBattle().getMental() ? (bBack_ ? RGB(63,57,54) : RGB(255,255,255))
																 : RGB(226,6,83));

	// 特殊マーク欄
	GUI::CPanelCtrl* pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC1");
	// とりあえず、非表示
	pCtrl->visible(false);
	pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2")->visible(false);

	if(weapon.getKind()==Weapon::Kind::FIGHT_COND
	|| weapon.getKind()==Weapon::Kind::MAGIC_COND)
	{// 状態変化
		pCtrl->validWidget(weapon.getCond());
		pCtrl->visible(true);
		pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2")->visible(false);
	}
	ef(weapon.getKind()==Weapon::Kind::STATUS)
	{// ステータスアップ
		if(weapon.getStrengthAid()>0)
		{// 腕力アップ
			pCtrl->validWidget("UP_STR");
			pCtrl->visible(true);
			pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
		}
		if(weapon.getMagicAid()>0)
		{// 魔力アップ
			pCtrl->validWidget("UP_MGC");
			pCtrl->visible(true);
			if(pCtrl != pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2"))
				pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
			else
				return;
		}
		if(weapon.getHitAid()>0)
		{// 命中アップ
			pCtrl->validWidget("UP_HIT");
			pCtrl->visible(true);
			if(pCtrl != pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2"))
				pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
			else
				return;
		}
		if(weapon.getAvoidAid()>0)
		{// 回避アップ
			pCtrl->validWidget("UP_AVOID");
			pCtrl->visible(true);
			if(pCtrl != pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2"))
				pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
			else
				return;
		}
		if(weapon.getDefenceAid()>0)
		{// 防御アップ
			pCtrl->validWidget("UP_TOUGH");
			pCtrl->visible(true);
			if(pCtrl != pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2"))
				pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
			else
				return;
		}
		if(weapon.getSkillAid()>0)
		{// 技量アップ
			pCtrl->validWidget("UP_SKILL");
			pCtrl->visible(true);
			if(pCtrl != pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2"))
				pCtrl = pWeapon->getWidgetCast<GUI::CPanelCtrl>("ETC2");
			else
				return;
		}
		if(weapon.getMentalAid()>0)
		{// 気力アップ
			pCtrl->validWidget("UP_KI");
			pCtrl->visible(true);
		}
	}
}

void CAttack_weapon::setWeaponDataGuiPos(int nPos)
{
	if(bBack_)
	{// 援護位置
		// 表示されてるボタン取得
		GUI::CCircleMenuButton* pButton = pMenu_->getButton(Misc::linkStrAndNum("WEAPON",nPos+1));
		Draw::CDrawInfo info = pButton->getDrawInfo();
		pWeapon_->setX(info.getX());
		pWeapon_->setY(info.getY()-68);
	}
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end