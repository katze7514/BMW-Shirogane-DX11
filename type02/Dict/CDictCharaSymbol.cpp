#include "stdafx.h"

#ifdef BMW_DEBUG

#include "../Movie/DB/CSymbolDB.h"

#include "CDictCharaContext.h"

#include "CDictCharaSymbol.h"

namespace BMW{
namespace Dict{

CDictCharaSymbol::CDictCharaSymbol()
{
	pBack_ = new CFastPlane();
}

CDictCharaSymbol::~CDictCharaSymbol()
{
	DELETE_SAFE(pBack_);
}

void CDictCharaSymbol::Task(Task::CTaskContext* pContext)
{
	if(!pContext->IsAction())
		(*pContext->getDrawPlane())->BltNatural(pBack_,0,0);

	CTaskList::Task(pContext);
}

void CDictCharaSymbol::OnInit(Task::CTaskContext* pContext)
{
	// ウインドウサイズ取得
	int nWidth,nHeight;
	pContext->getApp()->getWindowSize(nWidth, nHeight);
	// 背景だけ作る
	pBack_->CreateSurface(nWidth,nHeight,false);
	pBack_->SetFillColor(RGB(255,255,255));
	pBack_->Clear();
}

void CDictCharaSymbol::OnReset(Task::CTaskContext* pContext)
{
	clearTask();

	// 現在のキャラデータを取得
	pCurrentItem_ = static_cast<CDictCharaContext*>(pContext)->getCurrentCharaItem();

	// 位置合わせ
	GUI::CGraphic* pGraphic[3];
	pGraphic[0] = new GUI::CGraphic();
	addTask(pGraphic[0], SYMBOL_DEFAULT);
	pGraphic[1] = new GUI::CGraphic();
	addTask(pGraphic[1], SYMBOL_DEFENCE);
	pGraphic[2] = new GUI::CGraphic();
	addTask(pGraphic[2], SYMBOL_DAMAGE);

	// シンボル設定
	pCurrentItem_->getSymbolDB().setGraphic(pGraphic[0], "DEFO");
	pCurrentItem_->getSymbolDB().setGraphic(pGraphic[1], "DEFENCE");
	pCurrentItem_->getSymbolDB().setGraphic(pGraphic[2], "DAMAGE");

	const GUI::CSpriteInfo& info = pGraphic[0]->getSpriteInfo();
	pGraphic[0]->setX(info.getX());

	int nY = info.getY();

	const GUI::CSpriteInfo& info1 = pGraphic[1]->getSpriteInfo();
	pGraphic[1]->setX(info.getWidth()+5+info1.getX());

	if(nY<=info1.getY()) nY=info1.getY();

	const GUI::CSpriteInfo& info2 = pGraphic[2]->getSpriteInfo();
	pGraphic[2]->setX(info.getWidth()+5 + info1.getWidth()+5 + info2.getX());

	if(nY<=info2.getY()) nY=info2.getY();

	pGraphic[0]->setY(nY);
	pGraphic[1]->setY(nY);
	pGraphic[2]->setY(nY);

	// チップ
	nY += 80;
	int nX = 32;
	LONG nWidth,nHeight;
	int nMaxHeight=0;
	
	// BEFORE_TOP
	addChip("BEFORE_TOP",CHIP_T, nX, nY, nMaxHeight);
	
	// BEFORE_LEFT
	addChip("BEFORE_LEFT",CHIP_L, nX, nY, nMaxHeight);

	// BEFORE_BOTTOM
	addChip("BEFORE_BOTTOM",CHIP_B, nX, nY, nMaxHeight);

	// BEFORE_RIGHT
	addChip("BEFORE_RIGHT",CHIP_R, nX, nY, nMaxHeight);

	// AFTER_TOP
	addChip("AFTER_TOP",CHIP_AT, nX, nY, nMaxHeight);
	
	// AFTER_LEFT
	addChip("AFTER_LEFT",CHIP_AL, nX, nY, nMaxHeight);

	// AFTER_BOTTOM
	addChip("AFTER_BOTTOM",CHIP_AB, nX, nY, nMaxHeight);

	// AFTER_RIGHT
	addChip("AFTER_RIGHT",CHIP_AR, nX, nY, nMaxHeight);

	nX = 32;
	nY += nMaxHeight;
	nMaxHeight=0;

	// BEFORE_PINCH_TOP
	addChip("BEFORE_PINCH_TOP",CHIP_PINCH_T, nX, nY, nMaxHeight);
	
	// BEFORE_PINCH_LEFT
	addChip("BEFORE_PINCH_LEFT",CHIP_PINCH_L, nX, nY, nMaxHeight);

	// BEFORE_PINCH_BOTTOM
	addChip("BEFORE_PINCH_BOTTOM",CHIP_PINCH_B, nX, nY, nMaxHeight);

	// BEFORE_PINCH_RIGHT
	addChip("BEFORE_PINCH_RIGHT",CHIP_PINCH_R, nX, nY, nMaxHeight);

	// AFTER_PINCH_TOP
	addChip("AFTER_PINCH_TOP",CHIP_PINCH_AT, nX, nY, nMaxHeight);
	
	// AFTER_PINCH_LEFT
	addChip("AFTER_PINCH_LEFT",CHIP_PINCH_AL, nX, nY, nMaxHeight);

	// AFTER_PINCH_BOTTOM
	addChip("AFTER_PINCH_BOTTOM",CHIP_PINCH_AB, nX, nY, nMaxHeight);

	// AFTER_PINCH_RIGHT
	addChip("AFTER_PINCH_RIGHT",CHIP_PINCH_AR, nX, nY, nMaxHeight);

	nX = 32;
	nY += nMaxHeight + 32;
	nMaxHeight=0;

	// JUMP_TOP1
	addChip("JUMP_TOP1",CHIP_JUMP1_T, nX, nY, nMaxHeight);
	
	// JUMP_TOP2
	addChip("JUMP_TOP2",CHIP_JUMP2_T, nX, nY, nMaxHeight);

	// JUMP_TOP3
	addChip("JUMP_TOP3",CHIP_JUMP3_T, nX, nY, nMaxHeight);

	// JUMP_LEFT1
	addChip("JUMP_LEFT1",CHIP_JUMP1_L, nX, nY, nMaxHeight);
	
	// JUMP_LEFT2
	addChip("JUMP_LEFT2",CHIP_JUMP2_L, nX, nY, nMaxHeight);

	// JUMP_LEFT3
	addChip("JUMP_LEFT3",CHIP_JUMP3_L, nX, nY, nMaxHeight);

	// JUMP_BOTTOM1
	addChip("JUMP_BOTTOM1",CHIP_JUMP1_B, nX, nY, nMaxHeight);
	
	// JUMP_BOTTOM2
	addChip("JUMP_BOTTOM2",CHIP_JUMP2_B, nX, nY, nMaxHeight);

	// JUMP_BOTTOM3
	addChip("JUMP_BOTTOM3",CHIP_JUMP3_B, nX, nY, nMaxHeight);

	// JUMP_RIGHT1
	addChip("JUMP_RIGHT1",CHIP_JUMP1_R, nX, nY, nMaxHeight);
	
	// JUMP_RIGHT2
	addChip("JUMP_RIGHT2",CHIP_JUMP2_R, nX, nY, nMaxHeight);

	// JUMP_RIGHT3
	addChip("JUMP_RIGHT3",CHIP_JUMP3_R, nX, nY, nMaxHeight);

	nX = 32;
	nY += nMaxHeight + 10;
	nMaxHeight=0;

	// アニメーション系

	// WALK_TOP
	addChip("WALK_TOP",CHIP_WALK_T, nX, nY, nMaxHeight);
	nX += 48;

	// WALK_LEFT
	addChip("WALK_LEFT",CHIP_WALK_L, nX, nY, nMaxHeight);
	nX += 48;

	// WALK_BOTTOM
	addChip("WALK_BOTTOM",CHIP_WALK_B, nX, nY, nMaxHeight);
	nX += 48;

	// WALK_RIGHT
	addChip("WALK_RIGHT",CHIP_WALK_R, nX, nY, nMaxHeight);
	nX += 64;

	Movie::IDataSymbol* pData;
	Task::ITaskBase* pChip;
	Movie::CSymbolDB& chipDB = pCurrentItem_->getChipDB();

	// ATTACK
	pData = chipDB.getSymbolDataStr("ATK");
	pChip = chipDB.createMovieClip(pData, INT_MAX);
	addTask(pChip,CHIP_ATTACK);
	pChip->setX(nX);
	pChip->setY(nY);
	pChip->getSize(nWidth,nHeight);
	nX += 64 + 10;
	if(nMaxHeight<nHeight) nMaxHeight=nHeight;

	// ITEM
	pData = chipDB.getSymbolDataStr("ITEM");
	if(pData!=NULL)
	{
		pChip = chipDB.createMovieClip(pData, INT_MAX);
		addTask(pChip,CHIP_ITEM);
		pChip->setX(nX);
		pChip->setY(nY);
		pChip->getSize(nWidth,nHeight);
		nX += 64 + 10;
		if(nMaxHeight<nHeight) nMaxHeight=nHeight;
	}

	// DEFENCE
	addChip("DEFENCE",CHIP_DEFENCE, nX, nY, nMaxHeight);
	// DAMAGE
	addChip("DAMAGE",CHIP_DAMAGE, nX, nY, nMaxHeight);

	pContext->getInput()->cursolVisible(true);
	pContext->getInput()->guard(false);
}

void CDictCharaSymbol::OnAction(Task::CTaskContext* pContext)
{
	// キャンセルしたら、タイトルへ
	if(Input::releaseCancel(pContext))
	{
		pContext->getInput()->cursolVisible(false);
		pContext->getInput()->guard(true);
		pContext->getApp()->getFoward()->clearPopUp();
		getTaskListCtrl()->returnTaskList();
	}
}

void CDictCharaSymbol::addChip(const string& sID, int nPri, int& nX, int& nY, int& nMaxHeight)
{
	Task::ITaskBase* pChip;
	Movie::CSymbolDB& chipDB = pCurrentItem_->getChipDB();

	pChip = chipDB.createSymbolStr(sID);

	if(pChip==NULL) return;

	addTask(pChip,nPri);
	pChip->setX(nX);
	pChip->setY(nY);
	LONG nWidth, nHeight;
	pChip->getSize(nWidth,nHeight);
	nX += nWidth + 10;
	if(nMaxHeight<nHeight) nMaxHeight=nHeight;
}

} // namespace Dict end
} // namespace BMW end

#endif // BMW_DEBUG
