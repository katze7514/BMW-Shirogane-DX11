#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Item/ConstItem.h"
#include "CItem_ctrl.h"

namespace BMW{
namespace ADV{
namespace API{

CItem_ctrl::~CItem_ctrl()
{
	DELETE_SAFE(pPanel_);
}

void CItem_ctrl::OnReset(Task::CTaskContext* pContext)
{
	// インターフェイス取得
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_ITEMGET");
}

void CItem_ctrl::OnInit(Task::CTaskContext* pContext)
{
	// インターフェイス初期化
	pPanel_->visibleAll(false);
	pPanel_->getWidget("BACK")->visible(true);
	pPanel_->getWidget("GET")->visible(true);

	// セーブデータ取得
	Save::CExecData& data = pContext->getApp()->getExec();

	// 操作フラグ取得
	int nCtrl = pContext->top();
	pContext->pop();
	// まず、追加するアイテムの種類数
	int nSize = pContext->top();
	pContext->pop();
	// 本当は4つまで
	int nID,nValue;
	for(int i=0; i<nSize; ++i)
	{// 順番に、ID、個数と入ってる
		nID = pContext->top();
		pContext->pop();
		nValue = pContext->top();
		pContext->pop();
		if(nID==Item::BP)
		{ 
			// 減らすなら符号反転
			if(nCtrl==DEL) nValue = -nValue;
			// 計算
			data.calcBP(nValue);
		}
		else
		{ 
			for(int j=0; j<nValue; ++j) 
			{
				if(nCtrl==ADD) data.incItem(nID);
				ef(nCtrl==DEL) data.decItem(nID);
			}
		}

		// ライン設定
		createLine(nID,nValue,i+1);
	}

	pContext->getInput()->guard(false);
}

void CItem_ctrl::OnAction(Task::CTaskContext* pContext)
{
	// ボタンが押されたらリターン
	if(pContext->getInput()->getInputState(Input::IInput::OK)==Input::IInput::RELEASE
	|| pContext->getInput()->getInputState(Input::IInput::CANCEL)==Input::IInput::RELEASE
	|| pContext->getInput()->getInputState(Input::IInput::CTRL)!=Input::IInput::NO)
	{
		pContext->getInput()->guard(true);
		getTaskListCtrl()->returnTaskList();
	}
}

void CItem_ctrl::callTaskDraw(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}

void CItem_ctrl::createLine(int nID, int nValue, int nLine)
{
	GUI::CText* pText = pPanel_->getWidgetCast<GUI::CText>(Misc::linkStrAndNum("ROW",nLine));
	if(nID==Item::BP)
	// BPだったら、BP nValue
		pText->setText(Misc::linkStrAndNum("BP ",nValue));
	else
	// アイテム名
		pText->setText(Item::Const::itemName_.getValue(nID) + Misc::linkStrAndNum("×",nValue));

	pText->UpdateTextAA();
	pText->visible(true);
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end