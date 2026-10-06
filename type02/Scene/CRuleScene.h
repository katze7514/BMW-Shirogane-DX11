/*
	katze 05/03/07
	コンテキストを持つルールのテンプレート基底クラス
*/
#pragma once

#include "CScene.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Scene{

template<class ContextT>
class CRuleScene : public CScene<ContextT>
{/**
	コンテキストを抽象化する
 */
public:
	// デストラクタ
	virtual ~CRuleScene(){}
	// タスク処理
	virtual void Task(Task::CTaskContext*);
};

template<class ContextT>
__inline void CRuleScene<ContextT>::Task(Task::CTaskContext* pContext)
{// 子は渡ってきたコンテキストの代わりに
 // 親のコンテキストを渡す
	context_.action(pContext->IsAction());
	if(pContext->IsAction())
	{
		callTaskAction(&context_);
		OnAction(pContext);
	}
	else
	{
		callTaskDraw(&context_);
	}
}

} // namespace Scene end
} // namespace BMW end