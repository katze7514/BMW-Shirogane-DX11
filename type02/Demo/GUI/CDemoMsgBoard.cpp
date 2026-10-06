#include "stdafx.h"

#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CGraphicName.h"
#include "../../Scene/GUI/CGraphicFace.h"

#include "../Msg/CDemoMsg.h"
#include "CDemoMsgBoard.h"

namespace BMW{
namespace Demo{

CDemoMsgBoard::~CDemoMsgBoard()
{
	DELETE_SAFE(pPanel_);
}

void CDemoMsgBoard::OnInit(Task::CTaskContext* pContext)
{
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("WINDOW");
	
	pFace_ = pPanel_->getWidgetCast<GUI::CGraphicFace>("FACE");
	pName_ = pPanel_->getWidgetCast<GUI::CGraphicName>("NAME");
	pMsg_ = pPanel_->getWidgetCast<GUI::CText>("MSG");
}

void CDemoMsgBoard::Task(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}

void CDemoMsgBoard::changeMsg(int nSide, int nChara,int nFace,const string& sMsg,bool bMask,Task::CTaskContext* pContext)
{
	// 顔の変更
	pFace_->setToward(nSide);
	pFace_->setFace(nChara,nFace,pContext);
	
	// 名前の変更
	pName_->setCharaName(bMask?-1:nChara,pContext);

	// メッセージの変更
	LPSTR pStr = const_cast<LPSTR>(sMsg.c_str());
	CLineParser::ConvertCR(pStr);
	pMsg_->setText(pStr);
	pMsg_->UpdateTextA();
}

void CDemoMsgBoard::changeMsg(int nID, Task::CTaskContext* pContext)
{
	if(0<=nID && nID<static_cast<int>(vecMsg_.size()))
		changeMsg(vecMsg_[nID]->getSide(),
				  vecMsg_[nID]->getChara(),
				  vecMsg_[nID]->getFace(),
				  vecMsg_[nID]->getMsg(),
				  vecMsg_[nID]->IsMask(),
				  pContext);
}

} // namespace Demo end
} // namespace BMW end