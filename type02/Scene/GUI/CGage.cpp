#include "stdafx.h"

#include "CNumCtrl.h"
#include "CNumRemain.h"
#include "CGage.h"

namespace BMW{
namespace GUI{
// using宣言
using Draw::CDrawInfo;

CGage::CGage():bLeft_(true)
{
	//pRemain_ = new CNumRemain();
	//pRemain_->setParent(smart_ptr<Task::ITaskBase>(this,false));

	pGage_ = new CGraphicSize();
	pGage_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CGage::~CGage()
{
	DELETE_SAFE(pRemain_);
	DELETE_SAFE(pGage_);
}

/////////////////////////////////////////////////////
// タスク
/////////////////////////////////////////////////////
void CGage::OnDraw(Task::CTaskContext* pContext)
{// 基本的にゲージが一番下でいいよね
	pGage_->Task(pContext);
	pRemain_->Task(pContext);
}
///////////////////////////////////////////////////////
// アクション
///////////////////////////////////////////////////////
void CGage::actionChangeNum()
{// 描画サイズ調整
	double dWidthRate = pRemain_->getRate();
	pGage_->setWidth(dWidthRate*Draw::CDrawInfo::SCALE_RATE);

	if(!IsLeft())
	{// 左から右に減るなら、描画位置をずらす
		LONG lWidth,lHeight;
		pGage_->getSize(lWidth,lHeight);
		lWidth-=lWidth*dWidthRate;
		pGage_->setX(lWidth);
	}
}
void CGage::actionChangeNum(int nCurrent, int nMax)
{
	setMaxNum(nMax);
	setCurrentNum(nCurrent);
	actionChangeNum();
}

//////////////////////////////////////////////////////////
// 取得
//////////////////////////////////////////////////////////
LONG CGage::getCurrentNum() const
{ 
	return pRemain_->getCurrentNum();
}

void CGage::setCurrentNum(LONG lNum)
{ 
	pRemain_->setCurrentNum(lNum);
	//actionChangeNum();
}

LONG CGage::getMaxNum() const
{
	return pRemain_->getMaxNum();
}

void CGage::setMaxNum(LONG lNum)
{ 
	pRemain_->setMaxNum(lNum);
	//actionChangeNum(); 
}

void CGage::setRemain(CNumRemain* pRemain)
{
	pRemain_=pRemain;
	pRemain_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

CNumCtrl* CGage::getCurrentNumGui(){ return pRemain_->getCurrentNumGui(); }
CNum*	  CGage::getMaxNumGui(){ return pRemain_->getMaxNumGui(); }
CGraphic* CGage::getSlashGui(){ return pRemain_->getSlashGui(); }

void CGage::visibleNum(bool bV)
{
	getCurrentNumGui()->visible(bV);
	getMaxNumGui()->visible(bV);
}

} // namespace GUI end
} // namespace BMW end