#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CGage.h"
#include "../../Scene/GUI/CNumCtrl.h"

#include "../Context/CDataCharaSLG.h"

#include "CStatusCharaVeryEasy.h"

namespace BMW{
namespace SLG{

CStatusCharaVeryEasy::~CStatusCharaVeryEasy()
{
	DELETE_SAFE(pPanel_);
}

void CStatusCharaVeryEasy::Task(Task::CTaskContext* pContext)
{
	if(!pContext->IsAction() && IsVisible())
	{
		pPanel_->Task(pContext);
	}
}

void CStatusCharaVeryEasy::OnInit(Task::CTaskContext* pContext)
{// テキストをのぞいてnewして設定しておく
	// GuiDef
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();
	// レイアウト取得
	pPanel_ = db.createInterfaceCast<GUI::CPanel>(getSide()==LEFT ? "EASY_STATUS2_L" : "EASY_STATUS2_R");
}

///////////////////////////////////////////////////////////
// アクション
///////////////////////////////////////////////////////////
void CStatusCharaVeryEasy::actionReset(const CDataCharaSLG& chara, int nInfo, int nValue,int nHP, int nEN)
{	
	// とりあえず、フェーズ・HP・ENは設定
	// フェーズ
	pPanel_->getWidgetCast<GUI::CPanelCtrl>("CRAN")->validWidget(chara.getPhase());
	// キャラが出現してないなら？？？状態
	bool bApper=chara.getState().IsApper();
	pPanel_->getWidget("UNKNOWN")->visible(!bApper);

	// HP
	GUI::CGage*	pGage;
	pGage = pPanel_->getWidgetCast<GUI::CGage>("HP");
	pGage->getCurrentNumGui()->visible(bApper);
	pGage->getMaxNumGui()->visible(bApper);
	if(chara.getState().IsApper())
		pGage->actionChangeNum(chara.getBattle().getHP()-nHP, chara.getBattle().getMaxHP());
	else
		pGage->actionChangeNum(chara.getBattle().getMaxHP(),chara.getBattle().getMaxHP());

	// EN
	pGage = pPanel_->getWidgetCast<GUI::CGage>("EN");
	pGage->getCurrentNumGui()->visible(bApper);
	pGage->getMaxNumGui()->visible(bApper);
	if(chara.getState().IsApper())
		pGage->actionChangeNum(chara.getBattle().getEN()-nEN, chara.getBattle().getMaxEN());
	else
		pGage->actionChangeNum(chara.getBattle().getMaxEN(),chara.getBattle().getMaxEN());
	

	// INFOMATION設定
	GUI::CPanelCtrl*	pInfo = pPanel_->getWidgetCast<GUI::CPanelCtrl>("INFORMATION");
	GUI::CPanel*		pPanel;
	// 表示設定
	pInfo->validWidget(nInfo);
	pInfo->visible(true);
	switch(nInfo)
	{// 表示する内容によって設定
	case HIT: // 命中率
		pPanel = pInfo->getWidgetCast<GUI::CPanel>("HIT_PERCENT");
		
		pPanel->getWidgetCast<GUI::INum>("HIT_PERCENT")->setNum(nValue);
	break;

	case ACTION: // 攻撃行動
		pInfo->getWidgetCast<GUI::CPanelCtrl>("COUNTER_ACTION")->validWidget(nValue);
	break;

	default: // 気力
		pPanel = pInfo->getWidgetCast<GUI::CPanel>("MENTAL");
		if(bApper)
			pPanel->getWidgetCast<GUI::INum>("KI_PREVIEW")->setNum(chara.getBattle().getMental());
		else
			pInfo->visible(false);
	break;
	}
}

} // namespace SLG end
} // namespace BMW end