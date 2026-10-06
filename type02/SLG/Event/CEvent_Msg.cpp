#include "stdafx.h"

#include "../../ADV/CMsgBoard.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"

#include "CEvent.h"
#include "CEvent_Msg.h"

namespace BMW{
namespace SLG{
namespace Event{

void CEvent_Msg::OnAction(Task::CTaskContext* pContext)
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
	// 顔キャラID
	int nChara = pContext->top();
	pContext->pop();
	// SLG ID
	int nSlg = pContext->top();
	pContext->pop();
	// サイド
	int nSide = pContext->top();
	pContext->pop();

	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Task::CTaskCtrl<ADV::CMsgBoard>& msgCtrl_ = p->getEvent()->getBoardCtrl();

	// 顔の変更
	msgCtrl_.getTask(nSide)->changeMsg(nChara,nFace,p->getString(nString),nMask==1,p);
	// 有効化
	msgCtrl_.getTask(nSide)->msgValid(true);
	msgCtrl_.setState(nSide);

	// しゃべってるキャラをマップ中央に
	// いなかったら、スクロールしない
	if(nSlg<0) nSlg = p->searchFace2Slg(nChara);
	
#ifdef BMW_DEBUG
	//CDbg().Out("Face2Slg %d %d", nChara, nSlg);
#endif

	if(nSlg>=0)
	{	
		p->getMap()->scrollIndex(p->getCharaData(nSlg)->getIndex());
	#ifdef BMW_DEBUG
		//CDbg().Out("Index %d", p->getCharaData(nSlg)->getIndex());
	#endif
	}

	// ログ保存
	p->addBackLog(nSide,nSlg,nChara,nFace,nString,nMask);
}

} // namespace Event end
} // namespace SLG end
} // namespace BMW ends