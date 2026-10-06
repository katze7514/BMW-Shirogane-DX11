#include "stdafx.h"

#include "../../Scene/IScene.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "CPenalty_rule.h"

namespace BMW{
namespace SLG{
namespace Penalty{

void CPenalty_rule::OnReset(Task::CTaskContext* pContext)
{
	// OK_T
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	addTask(pOK,OK_T);
	pOK->setValue(OK);

	addTask(pContext->getScene()->getGuiDefDB().createInterface("PANEL_PENALTY"),PANEL);
}

void CPenalty_rule::OnInit(Task::CTaskContext* pContext)
{
	if(pContext->top()==Victory::LOSE)
	{// 敗北してたら、スルー
		getTaskListCtrl()->returnTaskList();
		setState(NORMAL);
	}
	else
	{
		CSLGContext* p = static_cast<CSLGContext*>(pContext);
		set<int>& Set = p->getDeathSet();
		if(Set.empty())
		{// 誰も死んでなかったら、すぐに終了
			getTaskListCtrl()->returnTaskList();
			setState(NORMAL);
		}
		else
		{// 死んでるやつがいるよ・・・
			GUI::CPanel* pPanel = static_cast<GUI::CPanel*>(getTask(PANEL));
			GUI::CPanel* pCharaPanel = pPanel->getWidgetCast<GUI::CPanel>("DEATH_CHARAS");
			pCharaPanel->visibleAll(false);
			int nPena=0;

			GUI::CPanel* pPena;
			CDataCharaSLG* pChara;
			int nPos=1;
			set<int>::iterator it;
			for(it=Set.begin(); it!=Set.end(); ++it)
			{// キャラの設定
			#ifdef BMW_DEBUG
				CDbg().Out("Pena %d %d",*it,nPos);
			#endif
				pChara = p->getCharaData(*it);
				if(pChara==NULL) continue;

				if(nPos<=9)
				{// インターフェイスには9人まで表示
					pPena = pCharaPanel->getWidgetCast<GUI::CPanel>(Misc::linkStrAndNum("CHARA",nPos++));
					pPena->visible(true);
					// アイコン
					pChara->getMapSymbol()->setGraphicGui(pPena->getWidgetCast<GUI::CGraphic>("CHARA"),"BEFORE_LEFT");
					// ペナ値
					pPena->getWidgetCast<GUI::INum>("NUM")->setNum(pChara->getBattle().getPena());
				}
				// 合計値を計算
				nPena+=pChara->getBattle().getPena();
			}
			pPanel->getWidgetCast<GUI::INum>("BP_SUM")->setNum(nPena);

			// BPを減らす
			pContext->setValue(pContext->getValue(Flag::BP)-nPena,Flag::BP);

			setState(NORMAL);
			pContext->getInput()->guard(false);
		}
	}
}

void CPenalty_rule::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
		getTaskListCtrl()->returnTaskList();
		setState(NORMAL);
	break;

	default: break;
	}
}

} // namespace Penalty end
} // namespace SLG end
} // namespace BMW end