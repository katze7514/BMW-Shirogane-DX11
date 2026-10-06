#include "stdafx.h"

#include "../Weapon/ConstWeapon.h"
#include "../Weapon/CDataWeaponBattle.h"
#include "../Status/status_fun.h"
#include "../Scene/IScene.h"
#include "../Scene/GUI/CNumCtrl.h"


#include "train_fun.h"
#include "CWeaponFactory.h"
#include "CInterContext.h"
#include "CInterChara.h"

#include "CChara.h"
#include "CTrainWeapon.h"

namespace BMW{
namespace Inter{
namespace Chara{

/////////////////////////////////////////
// タスク
/////////////////////////////////////////
void CTrainWeapon::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_YOUSEI_WEAPON");
	pPanel_->setParent(getParent());
	// インターフェイス展開
	pBar_		= pPanel_->getWidgetCast<GUI::CPanel>("YOUSEI_LEVEL");
	pPercent_	= pPanel_->getWidgetCast<GUI::CNum>("YOUSEI_PERCENT");
	pSum_		= pPanel_->getWidgetCast<GUI::CNum>("BP_SPEND");
	pRemain_	= pPanel_->getWidgetCast<GUI::CNumCtrl>("BP_REMAINS");
	pChange_	= pPanel_->getWidgetCast<GUI::CPanel>("CHANGE");;
	pPage_		= pChange_->getWidgetCast<GUI::CNum>("PAGE");
	// とりあえず、Change止め
	pChange_->valid(false);
	pChange_->visible(false);

	// イベントハンドラ設定
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CTrainWeapon::eventButton);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("DOWN"),fun,DOWN);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("UP"),fun,UP);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("OK"),fun,OK);
	GUI::CButton::setButtonEvent(pChange_->getWidgetCast<GUI::CButton>("BUTTON"),fun,CHANGE);
}

void CTrainWeapon::OnReset(Task::CTaskContext* pContext)
{
	// 対象キャラデータ取得
	BMW::Chara::CDataCharaInter* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData();
	nCost_ = pChara->getWeaponCost();
	nTrain_ = pChara->getWeaponTrain();
	nUp_=0;
	// 養成％
	pPercent_->setNum(nTrain_*10);
	// 武器パネル生成
	createWeaponPanel(*pChara,*pContext);
	// 必要BP
	pSum_->setNum(0);
	// 残BP
	pRemain_->setNum(pContext->getApp()->getExec().getBP());
	pRemain_->validNumGui(0/*白*/);
	// 一端、アップデート
	updateWeaponPanel();
}

// アクション
void CTrainWeapon::actionUpdateBP(Task::CTaskContext* pContext)
{
	pRemain_->setNum(pContext->getApp()->getExec().getBP() - pSum_->getNum());
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

/////////////////////////////////////////
// イベントハンドラ
/////////////////////////////////////////
void CTrainWeapon::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		switch(pButton->getValue())
		{
		case DOWN:
			if(--nUp_<0) nUp_=0;
			updateWeaponPanel();
		break;

		case UP:
			if(++nUp_>10-nTrain_) nUp_=10-nTrain_;
			updateWeaponPanel();
		break;

		case OK:
			if(pRemain_->getNum()>=0)
			{// BP足りてるよ
				BMW::Chara::CDataCharaInter* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData();
				// 反映
				pChara->calcWeaponTrain(nUp_);
				//pContext->getApp()->getExec().getTrainData(pChara->getID(),false)->back(pChara);
				// BP
				pContext->getApp()->getExec().setBP(pRemain_->getNum());
				// ステータス
				fun_(CChara::S_HEAD,pContext);
				fun_(CChara::S_WEAPON,pContext);
				fun_(CChara::S_BP,pContext);
				OnReset(pContext);
			}
		break;

		case CHANGE:
		{// ページを進める
			int nPage = pPage_->getNum();
			if(++nPage>nDiv_) nPage=1;
			pPage_->setNum(nPage);
			static_cast<GUI::CPanelCtrl*>(pWeapon_)->validWidget(nPage-1);
		}
		break;

		default: break;
		}
	}
}

////////////////////////////////////////
// インターフェイス
////////////////////////////////////////
void CTrainWeapon::createWeaponPanel(BMW::Chara::CDataCharaInter& chara, Task::CTaskContext& p)
{
	// 本当は持っている武器の数に合わせて生成
	// というか合体武器はのぞくので、武器数カウントが必要か・・・
	CWeaponFactory fct;
	fct.setOut(0);
	Weapon::CDataWeaponBattle* pWeapon;
	// ランク付けのため一回し
	chara.beginWeapon();
	while(!chara.endWeapon()) pWeapon = fct.createWeapon(*chara.nextWeapon(),&chara,p);
	// ランク付け
	fct.updateRank();
	// 養成に関係ない武器数ゲット
	nOut_ = fct.getOut();
	// 養成に関係のある武器数
	int nSize = (int)chara.sizeWeapon()-nOut_;
	// 武器の養成ID配列設定
	vecWeaponType_.clear();
	vecWeaponType_.resize(nSize);
	// 必要な武器数に合わせて選択パネルを生成
	// 武器パネル生成
	nDiv_ = (int)ceil((double)nSize / 3.0);

	GUI::CPanel* pLine;
	GUI::CText*	 pText;
	int nPos=0;

	if(nDiv_==1)
	{// 1枚しか必要無い時
		GUI::CPanel* pPanel = p.getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("WEAPON_PANEL_T");
		pWeapon_ = pPanel;

		// ボタンいらない
		pChange_->valid(false);
		pChange_->visible(false);

		// 武器ライン設定
		pPanel->validAll(false);
		pPanel->visibleAll(false);
		
		chara.beginWeapon();
		while(!chara.endWeapon())
		{
			pWeapon = fct.createWeapon(*chara.nextWeapon(),&chara,p);

			// 養成に関係の無い武器はスルー
			if(pWeapon==NULL
			|| pWeapon->getKind()==Weapon::Kind::CURE
			|| pWeapon->getKind()==Weapon::Kind::REFILL
			|| pWeapon->getKind()==Weapon::Kind::STATUS)
				continue;

			vecWeaponType_[nPos]=pWeapon->getTrainingType();
		
			pLine = pWeapon_->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("LINE",nPos+1));
			pLine->visible(true);
			// アイコン設定
			Status::setWeaponIcon(pLine->getWidgetCast<GUI::CPanel>("ICON"), *pWeapon);
			// 名前
			pText = pLine->getWidgetCast<GUI::CText>("WEAPONNAME");
			pText->setText(pWeapon->getName());
			pText->UpdateTextAA();
			// 攻撃力
			pLine->getWidgetCast<GUI::INum>("ATK")->setNum(pWeapon->getAttack());
			pLine->getWidgetCast<GUI::INum>("ATK_UP")->setNum(pWeapon->getAttack());
			++nPos;
		}
	}
	else
	{// 複数枚ある時
		// CHANGEボタン使用する
		pChange_->valid(true);
		pChange_->visible(true);

		int nDiv=0;
		GUI::CPanelCtrl* pCtrl = new GUI::CPanelCtrl();
		pWeapon_ = pCtrl;

		// とりあえず、1枚
		GUI::CPanel* pPanel = p.getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("WEAPON_PANEL_T");
		pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));
		// 武器ライン設定
		pPanel->validAll(false);
		pPanel->visibleAll(false);
		
		chara.beginWeapon();
		while(!chara.endWeapon())
		{
			if(nPos>=3)
			{// 4枚以上になたよ
				pPanel = p.getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("WEAPON_PANEL_T");
				pCtrl->addWidget(pPanel, CStringScanner::NumToString(nDiv++));
				// 武器ライン設定
				pPanel->validAll(false);
				pPanel->visibleAll(false);
				nPos=0;
			}
			// 武器データ取得
			pWeapon = fct.createWeapon(*chara.nextWeapon(),&chara,p);

			// 養成に関係の無い武器はスルー
			if(pWeapon==NULL
			|| pWeapon->getKind()==Weapon::Kind::CURE
			|| pWeapon->getKind()==Weapon::Kind::REFILL
			|| pWeapon->getKind()==Weapon::Kind::STATUS)
				continue;

			vecWeaponType_[nPos+(nDiv-1)*3]=pWeapon->getTrainingType();
		
			pLine = pPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("LINE",nPos+1));
			pLine->visible(true);
			// アイコン設定
			Status::setWeaponIcon(pLine->getWidgetCast<GUI::CPanel>("ICON"), *pWeapon);
			// 名前
			pText = pLine->getWidgetCast<GUI::CText>("WEAPONNAME");
			pText->setText(pWeapon->getName());
			pText->UpdateTextAA();
			// 攻撃力
			pLine->getWidgetCast<GUI::INum>("ATK")->setNum(pWeapon->getAttack());
			pLine->getWidgetCast<GUI::INum>("ATK_UP")->setNum(pWeapon->getAttack());
			++nPos;
		}
		// とりあえず、1を表示
		pPage_->setNum(1);
		pCtrl->validWidget(0);
	}

	// パネル設定
	pPanel_->swapWidget(pWeapon_,"WEAPON_PANEL");
}

namespace{
__inline int getWeaponUpAtk(int nTrain, int nUp, int nType)
{
	int nAtk=0;
	for(int i=nTrain+1; i<=nTrain+nUp; ++i)
		nAtk+=Weapon::Const::WEAPON_TRAINING_VALUE[nType][i];

	return nAtk;
}

__inline int getWeaponCost(int nTrain, int nUp, int nType)
{
	int nAtk=0;
	for(int i=nTrain+1; i<=nTrain+nUp; ++i)
		nAtk+=Weapon::Const::WEAPON_TRAINING_COST[nType][i];

	return nAtk;
}

} // namespace end
void CTrainWeapon::updateWeaponPanel()
{
	updateTrainBar(pBar_,nTrain_,nUp_);
	// 現在の養成段階にあわせて、攻撃力をあげる
	int nSize = (int)vecWeaponType_.size();
	GUI::CPanel* pLine;
	GUI::CNumCtrl* pUp;
	for(int i=0; i<nSize; ++i)
	{
		//if(i>=3) break;
		if(nDiv_==1)
		{	pLine = pWeapon_->getWidgetCast<GUI::CPanel>(i); }
		else
		{// 1枚以上の時はこっち
			int nDiv = (int)floor((double)i/3.0);
			GUI::CPanel* pPanel = (static_cast<GUI::CPanelCtrl*>(pWeapon_))->getWidgetCast<GUI::CPanel>(nDiv);
			pLine = pPanel->getWidgetCast<GUI::CPanel>(i-nDiv*3);
		}
		// 攻撃力UP
		pUp = pLine->getWidgetCast<GUI::CNumCtrl>("ATK_UP");
		pUp->setNum(pLine->getWidgetCast<GUI::INum>("ATK")->getNum() + getWeaponUpAtk(nTrain_,nUp_,vecWeaponType_[i]));
		pUp->validNumGui(nUp_>0?1/*緑*/:0/*白*/);
	}

	// ボタン
	Task::ITaskBase* pButton = pPanel_->getWidgetCast<GUI::CButton>("DOWN");
	pButton->valid(nUp_!=0);
	pButton->visible(nUp_!=0);
	pButton = pPanel_->getWidgetCast<GUI::CButton>("UP");
	pButton->valid(nUp_!=10);
	pButton->visible(nUp_!=10);

	// BP
	int nBP = getWeaponCost(nTrain_,nUp_,nCost_);
	pRemain_->setNum(pRemain_->getNum()+pSum_->getNum());
	pSum_->setNum(nBP);
	pRemain_->setNum(pRemain_->getNum()-nBP);
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end