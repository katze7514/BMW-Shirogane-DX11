#include "stdafx.h"

#include "../../ADV/CMsgBoard.h"
#include "../Context/CSLGContext.h"

#include "CEvent.h"
#include "CEvent_Msg_back.h"

namespace BMW{
namespace SLG{
namespace Event{

void CEvent_Msg_back::OnReset(Task::CTaskContext* pContext)
{
	p = static_cast<CSLGContext*>(pContext);
}

void CEvent_Msg_back::OnInit(Task::CTaskContext* pContext)
{
	// 現在の状況を保存
	Task::CTaskCtrl<ADV::CMsgBoard>& msgCtrl = p->getEvent()->getBoardCtrl();
	// 文字色
	rgb_ = msgCtrl.getCurrentTask()->getTextColor();

	// イテレータ初期化
	it_ = p->getBackLogList().begin();
	++it_;
	// 一番先頭は最新なので、一つ次のやつからスタート
	if(it_!=p->getBackLogList().end())
	{// 最後に到達してなかったら、一つ進めて更新！
		// ボードをバックログ仕様へ
		for(int nSide=0; nSide<=1; ++nSide)
		{
			ADV::CMsgBoard* pMsg = msgCtrl.getTask(nSide);
			// 状況を変更
			pMsg->setTextColor(RGB(13,114,126));
			pMsg->setTextFont(GUI::CText::FONT_GOTHIC);
		}
		// メッセージ取得
		changeMsg();
		setState(WAIT);
		p->getInput()->guard(false);
	}
	else
	{// つうか、ログないじゃん！
		setState(END);
	}
}

void CEvent_Msg_back::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END:
		getTaskListCtrl()->returnTaskList();
	break;

	case WAIT: // 入力待ち
		if(Input::releaseOK(p)
		|| p->getInput()->getInputState(Input::IInput::CTRL)!=Input::IInput::NO)
		{// OKが押されたら
			--it_;
			if(it_!=p->getBackLogList().begin())
			{// 先頭じゃなければ、一つ戻して更新
				changeMsg();
			}
			else
			{// 先頭だったら終了～
				Task::CTaskCtrl<ADV::CMsgBoard>& msgCtrl = p->getEvent()->getBoardCtrl();
				// 先頭情報から元に戻す
				// 変更するメッセージボードを取得
				for(int i=0; i<2; ++i)
				{// 状況を戻す
					ADV::CMsgBoard* pMsg = msgCtrl.getTask(i);
					pMsg->setTextColor(rgb_);
					pMsg->setTextFont(GUI::CText::FONT_MINCHO);
					pMsg->visible(true);
				}
				// メッセージ変更
				changeMsg();
				// そしてリターン
				p->getInput()->guard(true);
				setState(END);
			}
		}
		ef(Input::releaseCancel(p))
		{// キャンセルが押された
			++it_;
			if(it_!=p->getBackLogList().end())
			// 一番後ろじゃなければ、一つ進めて更新
				changeMsg();
			else
				--it_;
		}
	break;
	}
}

void CEvent_Msg_back::changeMsg()
{
	// 変更対象のMSGボードゲット
	Task::CTaskCtrl<ADV::CMsgBoard>& msgCtrl = p->getEvent()->getBoardCtrl();
	msgCtrl.setState(it_->getSide());
	ADV::CMsgBoard* pMsg = msgCtrl.getCurrentTask();
	
	// 内容変更
	pMsg->changeMsg(it_->getChara(),it_->getFace(),p->getString(it_->getString()),it_->getMask()==1,p);
	// 有効化
	pMsg->msgValid(true);
}

} // namespace Event end
} // namespace SLG end
} // namespace BMW end