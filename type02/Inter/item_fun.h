/*
	katze 06/03/02
	アイテムステータス関数群
*/
#pragma once

#include "../Scene/GUI/CNumCtrl.h"
#include "../Scene/GUI/CNumRemain.h"

namespace BMW{
namespace Inter{
namespace Chara{

__inline void setStatus(GUI::CPanel* pPanel, BMW::Chara::CDataCharaInter* pChara)
{// ステータス設定
	pPanel->getWidgetCast<GUI::INum>("HP")->setNum(pChara->getHP());
	pPanel->getWidgetCast<GUI::INum>("EN")->setNum(pChara->getEN());
	pPanel->getWidgetCast<GUI::INum>("QUICK")->setNum(pChara->getQuick());
	pPanel->getWidgetCast<GUI::INum>("TOUGH")->setNum(pChara->getTough());
	pPanel->getWidgetCast<GUI::INum>("MOVE")->setNum(pChara->getMove());
	pPanel->getWidgetCast<GUI::INum>("JUMP")->setNum(pChara->getJump());
}

__inline void setColor(GUI::CNumCtrl* pNum, int nValue1, int nValue2)
{
	if(nValue1==nValue2)	pNum->validNumGui(0/*白*/);
	ef(nValue1<nValue2)		pNum->validNumGui(1/*緑*/);
	else					pNum->validNumGui(2/*赤*/);
}

__inline void setStatusColor(GUI::CPanel* pNowStatus, GUI::CPanel* pUpStatus)
{
	GUI::CNumCtrl* pNum;
	int nValue;
	// HP
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("HP");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("HP")->getNum();
	setColor(pNum,nValue,pNum->getNum());
	// EN
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("EN");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("EN")->getNum();
	setColor(pNum,nValue,pNum->getNum());
	// QUICK
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("QUICK");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("QUICK")->getNum();
	setColor(pNum,nValue,pNum->getNum());
	// TOUGH
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("TOUGH");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("TOUGH")->getNum();
	setColor(pNum,nValue,pNum->getNum());
	// MOVE
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("MOVE");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("MOVE")->getNum();
	setColor(pNum,nValue,pNum->getNum());
	// JUMP
	pNum = pUpStatus->getWidgetCast<GUI::CNumCtrl>("JUMP");
	nValue = pNowStatus->getWidgetCast<GUI::INum>("JUMP")->getNum();
	setColor(pNum,nValue,pNum->getNum());
}

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end