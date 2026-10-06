#include "stdafx.h"

#include "../../ADV/CMsgBoard.h"
#include "../CADVContext.h"
#include "../CMsgBoard.h"

#include "CMsg_back.h"

namespace BMW{
namespace ADV{
namespace API{

void CMsg_back::OnReset(Task::CTaskContext* pContext)
{
	p = static_cast<CADVContext*>(pContext);
}

void CMsg_back::OnInit(Task::CTaskContext* pContext)
{
	// イテレータ初期化
	it_ = p->getBackLogList().begin();
	// 一番先頭は最新なので、一つ次のやつからスタート
	++it_;
	if(it_!=p->getBackLogList().end())
	{// 最後に到達してなかったら、一つ進めて更新！
		// 現在の状況を保存
		// 文字色
		rgb_ = p->getMsgBoard(0)->getTextColor();
		// ボードをバックログ仕様へ
		for(int nSide=0; nSide<=1; ++nSide)
		{
			smart_ptr<CMsgBoard>& pMsg = p->getMsgBoard(nSide);
			// 状況を変更
			pMsg->setTextColor(RGB(13,114,126));
			pMsg->setTextFont(GUI::CText::FONT_GOTHIC);
			if(!pMsg->IsVisible()) // 非表示
				nMsgState_[nSide]=-1;
			ef(pMsg->IsMsgValid()) // 有効
				nMsgState_[nSide]=1;
			else // 無効
				nMsgState_[nSide]=0;

			pMsg->visible(true);
		}

		changeMsg();
		setState(WAIT);
		p->getInput()->guard(false);
	}
	else
	{// 実はログがない！？
	 // だったら、このままEND
		setState(END);
	}
}

void CMsg_back::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case END:
		getTaskListCtrl()->returnTaskList();
	break;

	case WAIT: // 入力待ち
		if(Input::releaseOK(p)
		|| p->getInput()->getInputState(Input::IInput::CTRL)!=Input::IInput::NO)
		{// OKが押されたら次へ
			--it_;
			if(it_!=p->getBackLogList().begin())
			{// 先頭に来てなければ、一つ戻して更新！
				changeMsg();
			}
			else
			{// 先頭に到達したら、エンド
				// 変更するメッセージボードを取得
				for(int i=0; i<2; ++i)
				{// 状況を戻す
					smart_ptr<CMsgBoard>& pMsg = p->getMsgBoard(i);
					pMsg->setTextColor(rgb_);
					pMsg->setTextFont(GUI::CText::FONT_MINCHO);
					pMsg->visible(true);
					// ボード状態復元
					if(nMsgState_[i]==-1)
						pMsg->visible(false);
					ef(nMsgState_[i]==1)
						pMsg->msgValid(true);
					else
						pMsg->msgValid(false);
				}
				// メッセージ変更
				changeMsg();
				// 終了～
				p->getInput()->guard(true);
				setState(END);
			}
		}
		ef(Input::releaseCancel(p))
		{// キャンセルが押されたら一つ戻る
			++it_;
			if(it_!=p->getBackLogList().end())
			// 最後に到達してなかったら、一つ進めて更新！
				changeMsg();
			else
				--it_;
		}
	break;
	}
}

void CMsg_back::changeMsg()
{
	smart_ptr<CMsgBoard>& pMsg = p->getMsgBoard(it_->getSide());
	
	// 顔の変更
	pMsg->changeMsg(it_->getChara(),it_->getFace(),p->getString(it_->getString()),it_->getMask()==1,p);
	// 有効化
	pMsg->msgValid(true);
	// 反対側の無効化
	p->getMsgBoard(1-it_->getSide())->msgValid(false);
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end