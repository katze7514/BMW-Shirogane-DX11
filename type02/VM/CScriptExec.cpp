#include "stdafx.h"

#include "CScript.h"
#include "CScriptExec.h"

namespace BMW{
namespace VM{

void CScriptExec::Task(Task::CTaskContext* pContext)
{
	ITaskBase::Task(pContext);
}
	
void CScriptExec::setScript(const smart_ptr<CScript>& s)
{
	pScript_=s;
	pScript_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

void CScriptExec::OnInit(Task::CTaskContext* pContext)
{
	nPc_=0;
	setState(0);
}

void CScriptExec::OnReset(Task::CTaskContext* pContext)
{
	nPc_=0;
	setState(0);
}

void CScriptExec::OnAction(Task::CTaskContext* pContext)
{
	// タスクリストの入れ替え
	Task::ITaskList* pList = pContext->getTaskList();
	pContext->setTaskList(this);

	do{
		// コード実行
		nPc_=nState_++;
		//CDbg().Out("exec code %d", nPc_);
		pScript_->getCode(nPc_)->Task(pContext);

	// フレームを経過してOKになるまで、コードを実行する
	}while(!IsKill());
	bKill_=false;

	// 入れ替えたタスクリストを元に戻す
	pContext->setTaskList(pList);
}

void CScriptExec::OnDraw(Task::CTaskContext* pContext)
{
	pScript_->getCode(nPc_)->Task(pContext);
}

} // namespace VM end
} // namespace BMW end