#include "stdafx.h"

#include "../../Weapon/CDataWeaponBattleCollab.h"

#include "../../Scene/IScene.h"
#include "../../Status/status_fun.h"
#include "../../Status/CStatusWeaponPanel.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Map/CMap.h"

#include "CStatus_rule.h"

namespace BMW{
namespace SLG{
namespace Status{

CStatus_rule::~CStatus_rule()
{
	pWeaponPanel_->removeWidget("PANEL");
	DELETE_SAFE(pWeapon_[0]);
	DELETE_SAFE(pWeapon_[1]);
}

void CStatus_rule::OnReset(Task::CTaskContext* pContext)
{// とりあえず、いろいろと生成はしておく
	// キャンセル
	//BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	//pCancel->setValue(CANCEL);
	//addTask(pCancel,CANCEL_T);

	// インターフェイス生成
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHARA_STATUS");
	addTask(pPanel_,PANEL);

	// 各種パネル生成
	pStatus_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("CHARAPANEL");
	pWeaponPanel_ = pStatus_->getWidgetCast<GUI::CPanel>("WEAPON");
	pWeapon_[0] = new BMW::Status::CStatusWeaponPanel();
	pWeapon_[0]->OnInit(pContext);
	pWeapon_[1] = new BMW::Status::CStatusWeaponPanel();
	pWeapon_[1]->OnInit(pContext);
	// 武器ステに挿入
	pWeaponPanel_->swapWidget(pWeapon_[0], "PANEL");

	GUI::CButton::ButtonEvent fun;
	// ボタン設定
	fun.set(this,&CStatus_rule::eventWeapon);
	GUI::CButton::setButtonEvent(pWeaponPanel_->getWidgetCast<GUI::CPanel>("CHANGE")->getWidgetCast<GUI::CButton>("BUTTON"), fun, 0);

	// イベントハンドラ設定
	GUI::CPanel* pMenu = pPanel_->getWidgetCast<GUI::CPanel>("MENU");
	fun.set(this, &CStatus_rule::eventMenu);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("FUND"), fun, BASE);
	GUI::CButton::setButtonEvent(pMenu->getWidgetCast<GUI::CButton>("WEAPON"), fun, WEAPON);
}

void CStatus_rule::OnInit(Task::CTaskContext* pContext)
{
	setState(END);
	// 描画がもったいないので、マップ動作と描画を止めておく
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	p->getMap()->valid(false);
	p->getMap()->visible(false);

	// フェーダのハンドラ設定
	//Scene::CFoward::FaderEvent fun;
	//fun.set(this, &CStatus_rule::eventFader);
	//pContext->getApp()->getFoward()->setFaderHandler(fun);	

	// キャラデータ
	CDataCharaSLG* pChara = p->getTargetCharaData();

	// ヘッダ
	BMW::Status::setClearHeader(pPanel_->getWidgetCast<GUI::CPanel>("CLEARTITLE"),*p);
	// 簡易ステータス
	BMW::Status::setEasyStatus(pPanel_->getWidgetCast<GUI::CPanel>("EASY_STATUS"),*pChara,*p);
	// 戦闘ステータス
	BMW::Status::setBattleStatus(pPanel_->getWidgetCast<GUI::CPanel>("BATTLE_STATUS"),pChara->getBattle());

	// 基礎ステ
	BMW::Status::setStatusBasic(pStatus_->getWidgetCast<GUI::CPanel>("BASIC"),*pChara,*p);
	// 武器ステ
	setStatusWeapon(*pChara,*p);

	// とりあえずは、基本ステ表示
	pStatus_->validWidget("BASIC");

	// カーソル移動
	Input::IInput::InputEvent fun;
	fun.set(this,&CStatus_rule::eventCursol);
	pContext->getInput()->actionMove(593,288,5,fun);
}

void CStatus_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INTRO:
		pContext->getInput()->guard(false);
		setState(NORMAL);
	break;

	case NORMAL:
		if(pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE)
		{// キャンセルされたら戻る
			pContext->getInput()->guard(true);
			pContext->push(1);
			setState(END_C);

			CSLGContext* p = static_cast<CSLGContext*>(pContext);
			int nX,nY;
			p->setCirclePos(Pos::CHIP,nX,nY);
			Input::IInput::InputEvent fun;
			fun.set(this,&CStatus_rule::eventCursol);
			pContext->getInput()->actionMove(nX,nY,5,fun);

			p->getMap()->valid(true);
			p->getMap()->visible(true);
		}
	break;

	default: break;
	}
}

////////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
//void CStatus_rule::eventFader(Task::CTaskContext*)
//{
//}

void CStatus_rule::eventMenu(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// ボタンが押された
		switch(pButton->getValue())
		{
		case BASE: // 基礎ステ～
			pStatus_->validWidget("BASIC");
		break;

		case WEAPON: // 武器ステ～
			pStatus_->validWidget("WEAPON");
		break;

		default: break;
		}
	}
}

void CStatus_rule::eventCursol()
{
	if(getState()!=END_C)
	{
		setState(INTRO);
	}
	else
	{
		getTaskListCtrl()->returnTaskList();
		setState(END);
	}
}

void CStatus_rule::eventWeapon(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{// 武器パネル交代ボタンが押された
		// パネルの入れ替え
		nWeapon_=1-nWeapon_;
		pWeaponPanel_->getWidgetCast<GUI::CPanel>("CHANGE")->getWidgetCast<GUI::CNum>("PAGE")->setNum(nWeapon_+1);
		pWeaponPanel_->swapWidget(pWeapon_[nWeapon_],"PANEL",false);
	}
}

/////////////////////////////////////////
// 武器ステ設定
/////////////////////////////////////////
void CStatus_rule::setStatusWeapon(const CDataCharaSLG& chara, CSLGContext& p)
{
	const Chara::CDataCharaBattle& battle = chara.getBattle();
	nWeapon_ = 0;
	// とりあえず、0番目
	GUI::CPanel* pPanel		= pWeapon_[0]->getPanel();
	GUI::CPanelCtrl* pCtrl	= pWeapon_[0]->getDetail();

	pPanel->validAll(false);
	pPanel->visibleAll(false);
	pCtrl->visible(true);

	// とりあえず、チェンジは使わない
	GUI::CPanel* pChange = pWeaponPanel_->getWidgetCast<GUI::CPanel>("CHANGE");
	pChange->valid(false);
	pChange->visible(false);

	GUI::CPanel* pLine;
	Weapon::CDataWeaponBattle* pWeapon;
	int nCount=1;
	battle.beginWeapon();
	while(!battle.endWeapon())
	{
		if(nCount>=4)
		{// 4つ以上になったら次のパネル
			pCtrl->validWidget("DETAIL1");
			nCount=1;
			pPanel = pWeapon_[1]->getPanel();
			pPanel->validAll(false);
			pPanel->visibleAll(false);
			pCtrl = pWeapon_[1]->getDetail();
			pCtrl->visible(true);
			// 切り替えボタンの設定
			pChange->valid(true);
			pChange->visible(true);
			pChange->getWidgetCast<GUI::CNum>("PAGE")->setNum(1);
			nWeapon_=0;
		}
		/*ef(nCount>=7) break;*/ // 7つ以上は対応してない

		// 武器データ取得
		int nID = *battle.nextWeapon();
		pWeapon = p.getWeaponData(nID);

		#ifdef BMW_DEBUG
			CDbg().Out("WeaponStatus %d",nID);
		#endif

		if(// 合体武器は全員揃ってないと表示しない
		(pWeapon->getKind()!=Weapon::Kind::FIGHT_COLLAB && pWeapon->getKind()!=Weapon::Kind::MAGIC_COLLAB)
		|| static_cast<Weapon::CDataWeaponBattleCollab*>(pWeapon)->IsCollabLoad())
		{
			// 武器行
			pLine = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("LINE",nCount));
			BMW::Status::setStatusWeaponLine(pLine,	*pWeapon);
			pLine->valid(true);
			pLine->visible(true);
			// 詳細
			BMW::Status::setStatusWeaponDetail(pCtrl->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("DETAIL",nCount++)),
												*pWeapon,battle.getEN(),battle.getMental());
		}
	}

	// 二枚目が有効だけど何も設定されてないなら
	if(pChange->IsValid()
	&& nCount==1)
	{// 二枚目使わない
		pChange->valid(false);
		pChange->visible(false);
	}
	// とりあえず、一枚目
	pWeaponPanel_->swapWidget(pWeapon_[0],"PANEL",false);

	// とりあえず、一番目を表示
	pCtrl->validWidget("DETAIL1");
}

} // namespace Status end
} // namespace SLG end
} // namespace BMW end