#include "stdafx.h"

#include "../Scene/GUI/CGraphicFace.h"
#include "../Scene/GUI/CGraphicName.h"
#include "../Scene/GUI/CGage.h"
#include "../Scene/GUI/CGage.h"
#include "../Inter/CInterChara.h"

#include "status_fun.h"

namespace BMW{
namespace Status{
///////////////////////////////////////////////////
// クリアヘッダ
//////////////////////////////////////////////////
void setClearHeaderInter(GUI::CPanel* pPanel, Task::CTaskContext& p)
{
	// 共通
	setClearHeaderBase(pPanel,p,true);

	// クリア！
	// BP
	pPanel->getWidgetCast<GUI::INum>("BP_ALL")->setNum(p.getApp()->getExec().getBP());
	// FP
	pPanel->getWidgetCast<GUI::INum>("FP_ALL")->setNum(p.getApp()->getExec().getFP());
	// ターン数
	pPanel->getWidgetCast<GUI::INum>("TURN_ALL")->setNum(p.getApp()->getExec().getTurn());
	// 熟練度
	pPanel->getWidgetCast<GUI::INum>("JUKUREN_ALL")->setNum(p.getApp()->getExec().getExpert());
}

//////////////////////////////////////////////////
//	簡易ステータス
//////////////////////////////////////////////////
void setEasyStatus(GUI::CPanel* pPanel, Inter::CInterChara& chara, Task::CTaskContext& p)
{
	 Chara::CDataCharaInter* pInter = chara.getData();
	// 顔
	GUI::CGraphicFace* pFace = pPanel->getWidgetCast<GUI::CGraphicFace>("FACE");
	pFace->setFace(pInter->getFaceID(), "DEFAULT", &p);

	GUI::CPanel* pStatus = pPanel->getWidgetCast<GUI::CPanel>("STATUS");
	// 識別
	pStatus->getWidgetCast<GUI::CPanelCtrl>("CRAN")->validWidget("PLAYER");
	// 名前
	GUI::CGraphicName* pName = pStatus->getWidgetCast<GUI::CGraphicName>("NAME");
	pName->setCharaName(pInter->getFaceID(),&p);
	// HP
	GUI::CGage* pGage;
	pGage = pStatus->getWidgetCast<GUI::CGage>("HP");
	pGage->actionChangeNum(pInter->getHP(),pInter->getHP());
	// EN
	pGage = pStatus->getWidgetCast<GUI::CGage>("EN");
	pGage->actionChangeNum(pInter->getEN(),pInter->getEN());
	// SP
	pGage = pStatus->getWidgetCast<GUI::CGage>("SP");
	pGage->actionChangeNum(pInter->getSP(),pInter->getSP());
	// LV
	GUI::INum* pNum;
	pNum = pStatus->getWidgetCast<GUI::INum>("LV");
	pNum->setNum(pInter->getLv());
	// NEXT
	pNum = pStatus->getWidgetCast<GUI::INum>("NEXT");
	pNum->setNum(pInter->getExp());
	// MENTAL
	pNum = pStatus->getWidgetCast<GUI::INum>("KIRYOKU");
	pNum->setNum(pInter->getMental());
}
//////////////////////////////////////////////////
// チップソート設定
//////////////////////////////////////////////////
void setChipSort(GUI::CPanel* pPanel, Inter::CInterChara& chara)
{
	// チップボタン
	chara.getSymbol().setButtonGui(pPanel->getWidgetCast<GUI::CButtonSymbol>("CHIP"),"BUTTON_INTER");

	// データ表示
	setCharaSort(pPanel->getWidgetCast<GUI::CPanelCtrl>("SORT"),chara.getID(),*chara.getData());
}

void setCharaSort(GUI::CPanelCtrl* pCtrl, int nID, Chara::CDataCharaInter& chara)
{
	GUI::CPanel* pPanel;
	// ID
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("ID");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(nID);
	// LV
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("LV");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getLv());
	// HP
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("HP");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getHP());
	// EN
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("EN");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getEN());
	// SP
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("SP");
	setChipGage(pPanel->getWidgetCast<GUI::CGage>("GAGE"),chara.getSP());
	// NEXT
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("NEXT");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getExp());
	// MENTAL
	pPanel = pCtrl->getWidgetCast<GUI::CPanel>("MENTAL");
	pPanel->getWidgetCast<GUI::INum>("NUM")->setNum(chara.getMental());
}

void setChipGage(GUI::CGage* pGage, int nValue)
{
	pGage->getSlashGui()->visible(false);
	pGage->getMaxNumGui()->visible(false);
	pGage->actionChangeNum(nValue,nValue);
}

///////////////////////////////////////////////////
// 基本ステータス
///////////////////////////////////////////////////
void setStatusBasic(GUI::CPanel* pPanel, const Inter::CInterChara& chara, Task::CTaskContext& p)
{
	// 基礎能力
	setStatusBasicFund(pPanel->getWidgetCast<GUI::CPanel>("BASIC"),chara.getData());
	// 技能
	setStatusBasicSkill(pPanel->getWidgetCast<GUI::CPanel>("SKILL"),chara.getData(),p);
	// 固有能力
	setStatusBasicTalent(pPanel->getWidgetCast<GUI::CPanel>("TALENT"),chara.getData(),p);
	// 精神
	setStatusBasicSpirit(pPanel->getWidgetCast<GUI::CPanel>("SPIRITS"),chara.getData(),p);
	// アイテム
	setStatusBasicItem(pPanel->getWidgetCast<GUI::CPanel>("ITEM"),chara.getData(),p);
}

void setStatusBasicSkill(GUI::CPanel* pPanel, const Chara::CDataCharaInter& chara, Task::CTaskContext& p)
{// 技能
	pPanel->validAll(false);
	pPanel->visibleAll(false);

	int nCount=0;
	Ability::CAbilityDB& aDB = p.getApp()->getAbility();
	GUI::CTextPopUp* pText;
	chara.beginSkill();
	while(!chara.endSkill())
	{// 先天技能
		const Chara::CStatusAbility& skill= *chara.nextSkill();
		pText = pPanel->getWidgetCast<GUI::CTextPopUp>(nCount++);
		aDB.setTextPopUpHolder(pText, skill.getID());
		// レベル制だったらそれを追記
		addLvStr(skill,pText->getText());
		pText->UpdateTextAA();
		pText->valid(true);
		pText->visible(true);
	}

	chara.beginSkillAcqu();
	while(!chara.endSkillAcqu())
	{// 後天技能
		const Chara::CStatusAbility& skill= *chara.nextSkillAcqu();
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
} // naemspace BMW end
