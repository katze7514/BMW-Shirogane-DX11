#include "stdafx.h"

#include "../CSLGScene.h"

#include "CMapChip.h"
#include "CMapChipState.h"

#include "CMap.h"

namespace BMW{
namespace SLG{
namespace Map{

CMap::~CMap()
{
	vector<CMapChip*>::iterator it;
	for(it=vecMap_.begin(); it!=vecMap_.end(); it++)
		DELETE_SAFE(*it);

	vecMap_.clear();
}

void CMap::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
		{
			OnAction(pContext);
			back_.Task(pContext);
			for(int i=0; i<nSize_; ++i)
				vecMap_[i]->Task(pContext);
		}
	}
	else
	{
		if(IsVisible())
		{
			back_.Task(pContext);
			for(int i=0; i<nSize_; ++i)
				vecMap_[i]->Task(pContext);
		}
	}
}

void CMap::OnInit(const string& sFile)
{// 現在のマップIDからマップを生成
#ifdef BMW_DEBUG
	CDbg().Out("MAP %s",sFile.c_str());
#endif
	loader_.setMap(this,sFile);
}

void CMap::OnAction(Task::CTaskContext* pContext)
{// マウスの位置から、マップをスクロール
	using namespace Input;
	switch(getState())
	{
	case NORMAL:
		if(CMapChipState::IsAction())
		{// マウスのアクションが有効の時に判定
			if(pContext->getInput()->getInputState(IInput::OK)==IInput::DRAG)
			{// OKボタンでドラッグされたら、スクロールモードへ
				setState(SCROLL);
				pContext->getInput()->getCursolPos(nX_,nY_);
				// マップの動作停止
				CMapChipState::action(false);
				// VMの動きも止めておく
				(pContext->getTaskList()->getTask(CSLGScene::VM))->valid(false);
			}
		}
	break;

	case SCROLL:
		// ドラッグ状態なら
		if(pContext->getInput()->getInputState(IInput::OK)==IInput::DRAG)
		{
			scroll(pContext);
		}
		else
		{// DRAGを離したら、通常モードへ
			setState(NORMAL);
			// マップの動作を戻す
			CMapChipState::action(true); 
			// VMの動きを元に戻す
			(pContext->getTaskList()->getTask(CSLGScene::VM))->valid(true);
		}
	break;

	default: break;
	}
	
}

/////////////////////////////////////////////////////////////
// マップチップ操作系
/////////////////////////////////////////////////////////////
void CMap::setMapChip(CMapChip* pChip, int nIndex)
{
	pChip->setIndex(nIndex);
	pChip->setTaskPriority(nIndex);
	pChip->setParent(smart_ptr<Task::ITaskBase>(this,false));
	vecMap_[nIndex]=pChip;
}

/////////////////////////////////////////////////////////////
// マップスクロール
/////////////////////////////////////////////////////////////
void CMap::scroll(Task::CTaskContext* pContext)
{
	int nX,nY;
	// 現在のカーソル位置を取得
	pContext->getInput()->getCursolPos(nX,nY);
	// 差分だけマップをずらす
	setX(drawInfo_.getX()-(nX_-nX));
	setY(drawInfo_.getY()-(nY_-nY));
	// 限界位置は、各端が中央に来る位置
	// 左限
	if(drawInfo_.getX()+rect_.left>=288)
		setX(288-rect_.left);
	// 右限
	ef(drawInfo_.getX()+rect_.right<=288)
		setX(288-rect_.right);

	// 上限
	if(drawInfo_.getY()+rect_.top>=224)
		setY(224-rect_.top);
	// 下限
	ef(drawInfo_.getY()+rect_.bottom<=224)
		setY(224-rect_.bottom);

	// 値更新
	nX_=nX;
	nY_=nY;
}

void CMap::scrollIndex(int nIndex)
{// 指定されたマップインデックスがマップ中央に来るようにずらす
	int nX,nY;
	if(!getMapChipPos(nIndex,nX,nY,false)) return;

	setX(288-nX);
	setY(224-nY);
}

void CMap::scrollPos(int nX, int nY)
{// 指定された位置が画面中央に来るようにマップ位置をずらす
	int x = nX;
	int y = nY;

	// X位置確認
	if(x<rect_.left)
		x=rect_.left;
	ef(x>rect_.right)
		x=rect_.right;

	// Y位置確認
	if(y<rect_.top)
		y=rect_.top;
	ef(y>rect_.bottom)
		y=rect_.bottom;

	setX(320-x);
	setY(240-y);
}

void CMap::getMapPos(int& nX, int& nY)
{
	nX = drawInfo_.getX();
	nY = drawInfo_.getY();
}

bool CMap::getMapChipPos(int nIndex, int& nX, int& nY, bool bP)
{
	CMapChip* pChip = getMapChip(nIndex);
	if(pChip==NULL) return false;

	Draw::CDrawInfo info = pChip->getDrawInfo(bP);
	nX = info.getX();
	nY = info.getY();
	return true;
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end