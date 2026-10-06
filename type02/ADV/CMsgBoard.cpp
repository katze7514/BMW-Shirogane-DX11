#include "stdafx.h"

#include "../Scene/IScene.h"
#include "../Scene/GUI/CGraphicName.h"
#include "../Scene/GUI/CGraphicFace.h"

#include "CMsgBoard.h"

namespace BMW{
namespace ADV{

CMsgBoard::CMsgBoard():nSide_(LEFT),nChara_(-1),nFace_(0)
{ 
	//デフォで非表示
	visible(false);
}

CMsgBoard::~CMsgBoard()
{
	DELETE_SAFE(pPanel_);
}

void CMsgBoard::Task(Task::CTaskContext* pContext)
{
	if(!pContext->IsAction())
	{
		if(IsVisible())
		{
			pPanel_->Task(pContext);
		}
	}
}

void CMsgBoard::OnInit(Task::CTaskContext* pContext)
{
	// ある意味省略記号ｗ
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// 一括読み込み
	pPanel_ = db.createInterfaceCast<GUI::CPanel>(getSide()==LEFT ? "MSG_LEFT" : "MSG_RIGHT");
	pPanel_->setParent(smart_ptr<Task::ITaskBase>(this,false));

	// 顔グラフィック
	pFace_ = pPanel_->getWidgetCast<GUI::CGraphicFace>("FACE");
	// 名前グラフィック
	pName_ = pPanel_->getWidgetCast<GUI::CGraphicName>("NAME");
	// メッセージ
	pMsg_ = pPanel_->getWidgetCast<GUI::CText>("MSG_TEXT");
	pMsg_->getFontConf().SetHeight(22);
}

const Draw::CDrawInfo CMsgBoard::getDrawInfo(bool bRela)
{
	Draw::CDrawInfo draw;
	draw.setAlpha(IsMsgValid() ? 255 : ((50<<8)-50)/100);
	return draw;
}

void CMsgBoard::msgValid(bool bMsgValid)
{
	bMsgValid_=bMsgValid;
	// 無効だったら、alphaを下げる
	//setAlpha(bMsgValid?255:((50<<8)-50)/100);
}

void CMsgBoard::changeMsg(int nChara,int nFace, const string& sMsg, bool bMask, Task::CTaskContext* pContext)
{
	//CDbg().Out("ChangeMsg %d %d %s",nChara,nFace,sMsg.c_str());
	// 顔の変更
	if(nChara_!=nChara || nFace_!=nFace)
	{
		pFace_->setFace(nChara,nFace,pContext);
		nFace_=nFace;
	}

	// 名前の変更
	if(nChara_!=nChara)
	{
		pName_->setCharaName(bMask ? -1 : nChara,pContext);
	}

	// メッセージの変更
	pMsg_->setText(sMsg);
	pMsg_->UpdateTextAA();
}

} // namespace ADV end
} // namespace BMW end