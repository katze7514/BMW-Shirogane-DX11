/*
	SLG関係の設定子定義
*/
#include "stdafx.h"

#include "../GUI/DB/CGuiDefDB.h"

#include "../Scene/GUI/CGraphicFace.h"
#include "../Scene/GUI/CGraphicName.h"
#include "../Scene/GUI/CGage.h"

#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "status_fun.h"

namespace BMW{
namespace Status{
///////////////////////////////////////////////////
// クリアヘッダ
///////////////////////////////////////////////////
void setClearHeader(GUI::CPanel* pPanel, SLG::CSLGContext& p)
{
	// 共通
	setClearHeaderBase(pPanel,p,false);

	// BP
	pPanel->getWidgetCast<GUI::INum>("BP_ALL")->setNum(p.getBP());
	// FP
	pPanel->getWidgetCast<GUI::INum>("FP_ALL")->setNum(p.getFP());
	// ターン数
	pPanel->getWidgetCast<GUI::INum>("TURN_ALL")->setNum(p.getTurn());
	// 熟練度
	pPanel->getWidgetCast<GUI::INum>("JUKUREN_ALL")->setNum(p.getExpert());
}
///////////////////////////////////////////////////
// 簡易ステータス
///////////////////////////////////////////////////
void setEasyStatus(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p, bool bApper)
{// 共通部分
	const Chara::CDataCharaBattle& battle = chara.getBattle();
	// 顔
	GUI::CGraphicFace* pFace = pPanel->getWidgetCast<GUI::CGraphicFace>("FACE");
	pFace->setFace(battle.getFaceID(), "DEFAULT", &p);

	GUI::CPanel* pStatus = pPanel->getWidgetCast<GUI::CPanel>("STATUS");
	// 識別
	pStatus->getWidgetCast<GUI::CPanelCtrl>("CRAN")->validWidget(chara.getPhase());
	// 名前
	GUI::CGraphicName* pName = pStatus->getWidgetCast<GUI::CGraphicName>("NAME");
	pName->setCharaName(bApper ? battle.getFaceID() : -1 ,&p);
	// HP
	GUI::CGage* pGage;
	pGage = pStatus->getWidgetCast<GUI::CGage>("HP");
	pGage->visibleNum(bApper);
	if(bApper)
		pGage->actionChangeNum(battle.getHP(),battle.getMaxHP());
	else
		pGage->actionChangeNum(battle.getMaxHP(),battle.getMaxHP());
	// EN
	pGage = pStatus->getWidgetCast<GUI::CGage>("EN");
	pGage->visibleNum(bApper);
	if(bApper)
		pGage->actionChangeNum(battle.getEN(),battle.getMaxEN());
	else
		pGage->actionChangeNum(battle.getMaxEN(),battle.getMaxEN());
	// SP
	pGage = pStatus->getWidgetCast<GUI::CGage>("SP");
	pGage->visibleNum(bApper);
	if(bApper)
		pGage->actionChangeNum(battle.getSP(),battle.getMaxSP());
	else
		pGage->actionChangeNum(battle.getMaxSP(),battle.getMaxSP());
	// LV
	GUI::INum* pNum;
	pNum = pStatus->getWidgetCast<GUI::INum>("LV");
	pNum->visible(bApper);
	if(bApper) pNum->setNum(battle.getLv());
	// NEXT
	pNum = pStatus->getWidgetCast<GUI::INum>("NEXT");
	pNum->visible(bApper);
	if(bApper) pNum->setNum(battle.getExp());
	// MENTAL
	pNum = pStatus->getWidgetCast<GUI::INum>("KIRYOKU");
	pNum->visible(bApper);
	if(bApper) pNum->setNum(battle.getMental());
}
//////////////////////////////////////////////////
// 戦闘ステータス設定
//////////////////////////////////////////////////
void setBattleStatus(GUI::CPanel* pPanel, const Chara::CDataCharaBase& chara)
{
	// 移動
	pPanel->getWidgetCast<GUI::INum>("MOVE")->setNum(chara.getMove());
	// ジャンプ
	pPanel->getWidgetCast<GUI::INum>("JUMP")->setNum(chara.getJump());
	// 敏捷
	pPanel->getWidgetCast<GUI::INum>("QUICK")->setNum(chara.getQuick());
	// 耐久
	pPanel->getWidgetCast<GUI::INum>("TOUGH")->setNum(chara.getTough());
	// ペナルティ
	pPanel->getWidgetCast<GUI::INum>("PENALTY")->setNum(chara.getPena());
	// 性格
	pPanel->getWidgetCast<GUI::CPanelCtrl>("CHARACTER")->validWidget(chara.getChara());
	// 撃墜
	pPanel->getWidgetCast<GUI::INum>("GEKITSUI")->setNum(chara.getKill());
	// きのこ
	pPanel->getWidget("GOLD_KINOKO")->visible(chara.getKill()>=50);
}

///////////////////////////////////////////////////
// チップソート
///////////////////////////////////////////////////
void setCharaSort(GUI::CPanelCtrl* pCtrl, int nID, SLG::CDataCharaSLG& chara)
{
	GUI::CPanel* pPanel;
	// ID
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("ID");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(nID);
	// LV
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("LV");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getBattle().getLv());
	// HP
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("HP");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getBattle().getHP(),chara.getBattle().getMaxHP());
	// EN
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("EN");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getBattle().getEN(),chara.getBattle().getMaxEN());
	// SP
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("SP");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getBattle().getSP(),chara.getBattle().getMaxSP());
	// NEXT
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("NEXT");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getBattle().getExp());
	// MENTAL
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("MENTAL");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getBattle().getMental());
}

void setChipGage(GUI::CGage* pGage, int nValue, int nMax)
{
	pGage->getSlashGui()->visible(false);
	pGage->getMaxNumGui()->visible(false);
	pGage->actionChangeNum(nValue,nMax);
}

///////////////////////////////////////////////////
// 基本ステータス
///////////////////////////////////////////////////
void setStatusBasic(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p)
{
	// 基礎能力
	setStatusBasicFund(pPanel->getWidgetCast<GUI::CPanel>("BASIC"),chara.getBattle());
	// 技能
	setStatusBasicSkill(pPanel->getWidgetCast<GUI::CPanel>("SKILL"),chara,p);
	// 固有能力
	setStatusBasicTalent(pPanel->getWidgetCast<GUI::CPanel>("TALENT"),chara.getBattle(),p);
	// 精神
	setStatusBasicSpirit(pPanel->getWidgetCast<GUI::CPanel>("SPIRITS"),chara.getBattle(),p);
	// アイテム
	setStatusBasicItem(pPanel->getWidgetCast<GUI::CPanel>("ITEM"),chara.getBattle(),p);
}

void setStatusBasicSkill(GUI::CPanel* pPanel, const SLG::CDataCharaSLG& chara, SLG::CSLGContext& p)
{// 技能
	pPanel->validAll(false);
	pPanel->visibleAll(false);

	int nCount=0;
	Ability::CAbilityDB& aDB = p.getApp()->getAbility();
	GUI::CTextPopUp* pText;
	chara.getBattle().beginSkill();
	while(!chara.getBattle().endSkill())
	{
		const Chara::CStatusAbility& skill= *chara.getBattle().nextSkill();
		pText = pPanel->getWidgetCast<GUI::CTextPopUp>(nCount++);
		aDB.setTextPopUpHolder(pText, skill.getID());
		// レベル制だったらそれを追記
		addLvStr(skill,pText->getText());
		pText->UpdateTextAA();
		pText->valid(true);
		pText->visible(true);
	}
}

} // namespace Status end
} // namespace BMW end