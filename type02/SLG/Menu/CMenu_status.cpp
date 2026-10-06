#include "stdafx.h"

#include "../../Status/status_fun.h"
#include "../../Scene/IScene.h"

#include "../IDRule.h"
#include "../IDSLG.h"
#include "../slg_fun.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Map/CMapChip.h"

#include "CMenu_status.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_status::OnReset(Task::CTaskContext* pContext)
{// タスクの生成

	// OK
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	pOK->setValue(OK);
	addTask(pOK, OK_T);
	// CANCEL
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CANCEL);
	addTask(pCancel, CANCEL_T);

	// ステータス
	pStatus_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("EASYSTATUS_DETAIL");
	addTask(pStatus_,STATUS);
	// キャラ部分
	pChara_ = pStatus_->getWidgetCast<GUI::CPanel>("STATUS");
	// ↑の？？？表示
	pUnKnown_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("UNKNOWN_STATUS1");
	addTask(pUnKnown_,UNKNOWN);
	// 状態
	pJotai_ = pStatus_->getWidgetCast<GUI::CPanel>("JOUTAI");
	// 援護
	pEngo_ = pStatus_->getWidgetCast<GUI::CPanel>("ENGO");
	// 精神
	pSpirit_ = pStatus_->getWidgetCast<GUI::CPanel>("SPIRITS");
	// 透明度50%
	pSpirit_->setAlpha(255/2);
	// 地形
	pLand_ = pStatus_->getWidgetRecCast<GUI::CPanel>("CHIKEI/VALUES");
	// +をON
	pLand_->getWidgetCast<GUI::INum>("HIT")->plus(true);
	pLand_->getWidgetCast<GUI::INum>("DEF")->plus(true);
	pLand_->getWidgetCast<GUI::INum>("HP")->plus(true);
	pLand_->getWidgetCast<GUI::INum>("EN")->plus(true);
}

void CMenu_status::OnInit(Task::CTaskContext* pContext)
{// 対象を調べて、ステータス表示を切り替える
	setState(NORMAL);
	pContext->getInput()->guard(false);

	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 対象チェック
	// 地形
	updateLand(p);

	// キャラはいるかいな？
	if(p->getTargetChara()>=0)
	{// おった
		updateChara(p);
		pChara_->visible(true);

		// キャラは出現扱い？
		if(p->getTargetCharaData()->getState().IsApper())
		{		
			pJotai_->visible(true);
			pEngo_->visible(true);
			pSpirit_->visible(true);
			pUnKnown_->visible(false);
			getTask(OK_T)->valid(true);
		}
		else
		{// してない
			pJotai_->visible(false);
			pEngo_->visible(false);
			pSpirit_->visible(false);
			pUnKnown_->visible(true);
			getTask(OK_T)->valid(false);
		}
	}
	else
	{// いない
		pChara_->visible(false);
		pJotai_->visible(false);
		pEngo_->visible(false);
		pSpirit_->visible(false);
		pUnKnown_->visible(false);
		getTask(OK_T)->valid(false);
	}
}

void CMenu_status::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OK:
		pContext->setValue(pContext->getValue(Flag::TARGET_CHARA),Flag::CTRL_CHARA);
		getTaskListCtrl()->callTaskList(Rule::STATUS_RULE,true);
		pContext->getInput()->guard(true);
	break;

	case CANCEL:
		getTaskListCtrl()->returnTaskList();
		pContext->getInput()->guard(true);
	break;

	default: break;
	}
}

void CMenu_status::OnComeBack(int nID, Task::CTaskContext* pContext)
{// ステータスシーンから戻って来た時の処理
	// 普通にMenu_Selectに戻っちゃってOKだと思われ
	setState(NORMAL);
	getTaskListCtrl()->returnTaskList();
}

////////////////////////////////////////////
// アップデート
////////////////////////////////////////////
void CMenu_status::updateLand(CSLGContext* p)
{// 地形アップデート
	const Map::CMapChipInfo& info = p->getTargetMapChip()->getMapInfo();

	// 命中
	pLand_->getWidgetCast<GUI::INum>("HIT")->setNum(info.getHit());
	// 防御
	pLand_->getWidgetCast<GUI::INum>("DEF")->setNum(info.getDefence());
	// HP
	pLand_->getWidgetCast<GUI::INum>("HP")->setNum(info.getHP());
	// EN
	pLand_->getWidgetCast<GUI::INum>("EN")->setNum(info.getEN());
	// 障害
	pLand_->getWidgetCast<GUI::INum>("SYOUGAI")->setNum(info.getHeight());
}

void CMenu_status::updateChara(CSLGContext* p)
{
	CDataCharaSLG* pChara = p->getTargetCharaData();
	bool bApper = pChara->getState().IsApper();
	// 簡易ステ
	BMW::Status::setEasyStatus(pChara_,*pChara,*p,bApper);
	// 状態
	setJotai(pJotai_,pChara->getBattle().getValidCond(),bApper);
	// 援護
	setEngo(pEngo_, pChara->getBattle().getBackUpAttack(),pChara->getBattle().getBackUpDefence(),bApper);
	// 精神
	setSpirits(pSpirit_, pChara->getBattle().getValidSpirit(),p->getApp()->getSpirit());
}

} // namespace Menu end
} // namespace SLG end
} // namespaec BMW end