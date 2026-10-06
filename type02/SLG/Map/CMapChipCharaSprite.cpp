#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CCharaState.h"
#include "CMapChipCharaSprite.h"

namespace BMW{
namespace SLG{
namespace Map{

CMapChipCharaSprite::CMapChipCharaSprite()
{// 親設定
	for(int i=0; i<2; i++)
		for(int j=0; j<4; j++)
			sprite_[i][j].setParent(smart_ptr<Task::ITaskBase>(this,false));

	// マップ中央
	setX(32);
	setY(16);
}

void CMapChipCharaSprite::Task(Task::CTaskContext* pContext)
{
	if(!pContext->IsAction() && IsVisible()) OnDraw(pContext);
}

void CMapChipCharaSprite::OnDraw(Task::CTaskContext* pContext)
{// 現在の状態に合わせて描画
	sprite_[getAct()][pState_->getWay()].OnDraw(pContext);
}

void CMapChipCharaSprite::getSize(LONG& lWidth, LONG& lHeight)
{
	sprite_[getAct()][pState_->getWay()].getSize(lWidth, lHeight);
}

void CMapChipCharaSprite::getDrawSize(LONG& lWidth, LONG& lHeight)
{
	sprite_[getAct()][pState_->getWay()].getDrawSize(lWidth, lHeight);
}

int CMapChipCharaSprite::getAct()
{
	switch(pState_->getAct())
	{
	case Act::BEFORE:
	case Act::MOVE:
	case Act::HIT_AWAY:
		return 0;

	default: 
		return 1;
	}
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end