#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "CMapChipChara2.h"
#include "CMapChipState.h"
#include "CMapChip.h"

namespace BMW{
namespace SLG{
namespace Map{

void CMapChip::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case OVER: actionOver(pContext); break;
	default:
	break;
	}
	#ifdef BMW_DEBUG
	{
		Input::CTaskInput* pInput = static_cast<Input::CTaskInput*>(pContext->getInput());
		if(pInput->getKeyBoard().IsKeyPush(DIK_F9))	text_->visible(!text_->IsVisible());
	}
	#endif
}

void CMapChip::actionOver(Task::CTaskContext* pContext)
{
	// SLGコンテキストにクリック情報を書き込む
#ifdef BMW_DEBUG
	//CDbg().Out("Map %d %d %d %d %d",getIndex(),info_.getOnMap(0),info_.getOnMap(1),info_.getOnMap(2),info_.getOnMap(3));
	CDbg().Out("MapState %d %d %d",getMapChipState()->getMove(),getMapChipState()->getAttack(),getMapChipState()->getAtkHeight());
	//CDbg().Out("MapField %d %d %d",getMapChipState()->getFieldAttack(),getMapChipState()->getFieldToward(0),getMapChipState()->getFieldToward(1));
	//CDbg().Out("MapField %d %d %d",getMapChipState()->getFieldAttack(),CMapChipState::getMax(),CMapChipState::getMin());
#endif
	pContext->setValue(getIndex(),Flag::TARGET_MAP);
	// キャラがいるか確認
	Task::ITaskBase* pTask = getTask(CMapChip::CHARA);
	if(pTask!=NULL)
	{// いた
		CMapChipChara2* pChara = static_cast<CMapChipChara2*>(pTask);
		// 対象キャラとして設定
		pContext->setValue(pChara->getID(),Flag::TARGET_CHARA);
		pContext->getInput()->guardDrag(true);
	}
	else
	{// いない
		pContext->setValue(-1,Flag::TARGET_CHARA);
		pContext->getInput()->guardDrag(false);
	}
	setState(NORMAL);
}

void CMapChip::createMapChipState()
{
	pState_ = new CMapChipState();
	addTask(pState_, STATE);

#ifdef BMW_DEBUG
	text_->setColor(RGB(255,255,255));
	text_->UpdateText();
#endif
}

void CMapChip::setRange(const RECT& range)
{
	pState_->setRange(range.left, range.top, range.right, range.bottom);
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end