#include "stdafx.h"

#include "CScriptExecSymbol.h"

namespace BMW{
namespace Movie{


void CScriptExecSymbol::OnAction(Task::CTaskContext* pContext)
{
	// タスクリストの入れ替え
	Task::ITaskList* pList = pContext->getTaskList();
	pContext->setTaskList(this);

	do{
		// コード実行
		nPc_=nState_++;
		pScript_->getCode(nPc_)->Task(pContext);

		if(IsRemove())
		{// リムーブが引っかけられたら、プログラムカウンタを進めない
			nState_=nPc_;
			bRemove_=false;
		}

	// フレームを経過してOKになるまで、コードを実行する
	}while(!IsKill());
	bKill_=false;

	// 入れ替えたタスクリストを元に戻す
	pContext->setTaskList(pList);
}

} // namespace Movie end
} // namespace BMW end