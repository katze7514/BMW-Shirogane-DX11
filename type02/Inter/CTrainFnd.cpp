#include "stdafx.h"

#include "../Chara/ConstChara.h"

#include "../Scene/IScene.h"
#include "../Scene/GUI/CNumCtrl.h"

#include "CInterContext.h"
#include "CInterChara.h"

#include "CChara.h"
#include "CTrainFnd.h"

namespace BMW{
namespace Inter{
namespace Chara{

using BMW::Chara::CStatusFund;

void CTrainFnd::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_YOUSEI_FND");
	pPanel_->setParent(getParent());

	// イベントハンドラ
	GUI::CButton::ButtonEvent fun;
	fun.set(this,&CTrainFnd::eventButton);
	// 腕力
	pStr_ = pPanel_->getWidgetCast<GUI::CPanel>("STR");
	GUI::CButton::setButtonEvent(pStr_->getWidgetCast<GUI::CButton>("DOWN"),fun,STR_DOWN);
	GUI::CButton::setButtonEvent(pStr_->getWidgetCast<GUI::CButton>("UP"),fun,STR_UP);
	// 魔力
	pMgc_ = pPanel_->getWidgetCast<GUI::CPanel>("MGC");
	GUI::CButton::setButtonEvent(pMgc_->getWidgetCast<GUI::CButton>("DOWN"),fun,MGC_DOWN);
	GUI::CButton::setButtonEvent(pMgc_->getWidgetCast<GUI::CButton>("UP"),fun,MGC_UP);
	// 命中
	pHit_ = pPanel_->getWidgetCast<GUI::CPanel>("HIT");
	GUI::CButton::setButtonEvent(pHit_->getWidgetCast<GUI::CButton>("DOWN"),fun,HIT_DOWN);
	GUI::CButton::setButtonEvent(pHit_->getWidgetCast<GUI::CButton>("UP"),fun,HIT_UP);
	// 回避
	pAvo_ = pPanel_->getWidgetCast<GUI::CPanel>("AVO");
	GUI::CButton::setButtonEvent(pAvo_->getWidgetCast<GUI::CButton>("DOWN"),fun,AVO_DOWN);
	GUI::CButton::setButtonEvent(pAvo_->getWidgetCast<GUI::CButton>("UP"),fun,AVO_UP);
	// 防御
	pDef_ = pPanel_->getWidgetCast<GUI::CPanel>("DEF");
	GUI::CButton::setButtonEvent(pDef_->getWidgetCast<GUI::CButton>("DOWN"),fun,DEF_DOWN);
	GUI::CButton::setButtonEvent(pDef_->getWidgetCast<GUI::CButton>("UP"),fun,DEF_UP);
	// 技量
	pSkl_ = pPanel_->getWidgetCast<GUI::CPanel>("SKL");
	GUI::CButton::setButtonEvent(pSkl_->getWidgetCast<GUI::CButton>("DOWN"),fun,SKL_DOWN);
	GUI::CButton::setButtonEvent(pSkl_->getWidgetCast<GUI::CButton>("UP"),fun,SKL_UP);
	// リセット
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("RESET"),fun,RESET);
	// OK
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("OK"),fun,OK);

	// FP計
	pSum_ = pPanel_->getWidgetCast<GUI::CNum>("FP_SUM");
	// FP残
	pRemain_ = pPanel_->getWidgetCast<GUI::CNumCtrl>("FP_REMAIN");
}

void CTrainFnd::OnReset(Task::CTaskContext* pContext)
{
	CInterChara* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData();
	// 腕力
	initBar(pStr_, pChara->getData()->getStrength());
	// 魔力
	initBar(pMgc_, pChara->getData()->getMagic());
	// 命中
	initBar(pHit_, pChara->getData()->getHit());
	// 回避
	initBar(pAvo_, pChara->getData()->getAvoid());
	// 防御
	initBar(pDef_, pChara->getData()->getDefence());
	// 技量
	initBar(pSkl_, pChara->getData()->getSkill());
	// FP計
	pSum_->setNum(0);
	// FP残
	pRemain_->setNum(pContext->getApp()->getExec().getFP());
	pRemain_->validNumGui(0); // 白
}

// アクション
void CTrainFnd::actionUpdateFP(Task::CTaskContext* pContext)
{
	pRemain_->setNum(pContext->getApp()->getExec().getFP() - pSum_->getNum());
	pRemain_->validNumGui(pRemain_->getNum()>=0?0/*白*/:1/*赤*/);
}

///////////////////////////////////////////
// イベントハンドラ
///////////////////////////////////////////
void CTrainFnd::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(GUI::IsRelease(pButton))
	{
		switch(pButton->getValue())
		{
		case STR_DOWN:	updateBar(pStr_,false); break;
		case STR_UP:	updateBar(pStr_,true);	break;
		case MGC_DOWN:	updateBar(pMgc_,false); break;
		case MGC_UP:	updateBar(pMgc_,true);	break;
		case HIT_DOWN:	updateBar(pHit_,false); break;
		case HIT_UP:	updateBar(pHit_,true);	break;
		case AVO_DOWN:	updateBar(pAvo_,false); break;
		case AVO_UP:	updateBar(pAvo_,true);	break;
		case DEF_DOWN:	updateBar(pDef_,false); break;
		case DEF_UP:	updateBar(pDef_,true);	break;
		case SKL_DOWN:	updateBar(pSkl_,false); break;
		case SKL_UP:	updateBar(pSkl_,true);	break;
		case RESET:		OnReset(pContext);		break;

		case OK: // 決定！
			if(pRemain_->getNum()>=0)
			{// FP残があれば反映可能
				// 能力反映
				BMW::Chara::CDataCharaInter* pChara = static_cast<CInterContext*>(pContext)->getTargetCharaData()->getData();
				pChara->calcStrength(pStr_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				pChara->calcMagic(pMgc_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				pChara->calcHit(pHit_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				pChara->calcAvoid(pAvo_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				pChara->calcDefence(pDef_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				pChara->calcSkill(pSkl_->getWidgetCast<GUI::INum>("PLUS")->getNum());
				//　セーブデータへ反映
				//pContext->getApp()->getExec().getTrainData(pChara->getID(),false)->back(pChara);
				// FP減らす
				pContext->getApp()->getExec().setFP(pRemain_->getNum());
				// ヘッダの反映
				fun_(CChara::S_HEAD,pContext);
				fun_(CChara::S_BASE,pContext);
				fun_(CChara::S_FP,pContext);
				OnReset(pContext);
			}
		break;
		}
	}
}

//////////////////////////////////////////
// インターフェイスアップデート
//////////////////////////////////////////
void CTrainFnd::initBar(GUI::CPanel* pPanel, int nValue)
{
	// 現在値
	pPanel->getWidgetCast<GUI::INum>("NOW")->setNum(nValue);
	// 養成後
	GUI::CNumCtrl* pNumCtrl = pPanel->getWidgetCast<GUI::CNumCtrl>("TRAIN");
	pNumCtrl->validNumGui(0); // 白
	pNumCtrl->setNum(nValue);
	// プラス
	pPanel->getWidgetCast<GUI::INum>("PLUS")->setNum(0);
	// 消費FP
	pPanel->getWidgetCast<GUI::INum>("SPEND_FP")->setNum(0);
	// ボタン
	// DOWNは非表示
	Task::ITaskBase* pBase = pPanel->getWidget("DOWN");
	pBase->valid(false);
	pBase->visible(false);
	// UPは表示
	pBase = pPanel->getWidget("UP");
	pBase->valid(nValue<CStatusFund::FUND_MAX);
	pBase->visible(nValue<CStatusFund::FUND_MAX);
}

void CTrainFnd::updateBar(GUI::CPanel* pPanel, bool bUp)
{
	int nNow = pPanel->getWidgetCast<GUI::INum>("NOW")->getNum();
	// 上昇値計算
	GUI::INum* pNum = pPanel->getWidgetCast<GUI::INum>("PLUS");
	int nPlus = pNum->getNum();
	bool bLimit;
	if(bUp)
	{// 能力アップ！
		++nPlus;
		bLimit = nNow+nPlus>= Chara::CStatusFund::FUND_MAX;
		// 限界値越えた
		if(bLimit) nPlus = CStatusFund::FUND_MAX-nNow;
	}
	else
	{// ダウソ
		--nPlus;
		bLimit = nPlus<=0;
		// 限界値越えた
		if(bLimit) nPlus = 0;
	}
	// プラス値設定
	pNum->setNum(nPlus);

	// ボタンの出し入れ
	// DOWN
	Task::ITaskBase* pButton = pPanel->getWidget("DOWN");
	pButton->valid(!(!bUp&bLimit));
	pButton->visible(!(!bUp&bLimit));
	// UP
	pButton = pPanel->getWidget("UP");
	pButton->valid(!(bUp&bLimit));
	pButton->visible(!(bUp&bLimit));

	// 残りは補正値から算出
	// 養成後値
	GUI::CNumCtrl* pCtrl = pPanel->getWidgetCast<GUI::CNumCtrl>("TRAIN");
	pCtrl->setNum(nNow+nPlus);
	pCtrl->validNumGui(nPlus!=0 ? 1/*緑*/ : 0/*白*/);
	// 必要FP
	pNum = pPanel->getWidgetCast<GUI::INum>("SPEND_FP");
	int nFP = pNum->getNum();
	// 養成値1あたり6使うでー
	pNum->setNum(nPlus * BMW::Chara::Const::FUND_TRAINING_FP);

	// FP計
	pSum_->setNum(pSum_->getNum() - nFP + pNum->getNum());
	// FP残
	pRemain_->setNum(pRemain_->getNum() + nFP - pNum->getNum());
	pRemain_->validNumGui(pRemain_->getNum()<0 ? 1/*赤*/ : 0/*白*/);
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end