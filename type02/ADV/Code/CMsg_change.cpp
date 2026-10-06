#include "stdafx.h"

#include "../CADVContext.h"
#include "../CMsgBoard.h"

#include "CMsg_change.h"

namespace BMW{
namespace ADV{
namespace API{

void CMsg_change::OnAction(Task::CTaskContext* pContext)
{
	// マスクフラグ
	int nMask = pContext->top();
	pContext->pop();
	// 文字列ID
	int nString = pContext->top();
	pContext->pop();
	// 顔ID
	int nFace = pContext->top();
	pContext->pop();
	// キャラID
	int nChara = pContext->top();
	pContext->pop();
	// サイド
	int nSide = pContext->top();
	pContext->pop();

	// コンテキスト変換
	CADVContext* p = static_cast<CADVContext*>(pContext);

	// 変更するメッセージボードを取得
	smart_ptr<CMsgBoard>& pMsg = p->getMsgBoard(nSide);
	
	// 顔の変更
	pMsg->changeMsg(nChara,nFace,p->getString(nString),nMask==1,p);
	// 有効化
	pMsg->msgValid(true);

	// 反対側の無効化
	p->getMsgBoard(1-nSide)->msgValid(false);

	// ログの追加
	p->addBackLog(nSide,nChara,nFace,nString,nMask);

	// 終了したのでリターン
	getTaskListCtrl()->returnTaskList();
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end