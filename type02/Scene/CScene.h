/*
	katze 05/03/07
	トップレベルシーンのテンプレート基底クラス
*/
#pragma once

#include "../Task/CTaskContext.h"
#include "../Task/CTaskList.h"

#include "IScene.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Scene{

template<class ContextT = Task::CTaskContext>
class CScene : public IScene
{/**
	コンテキストを抽象化する
 */
public:
	// デストラクタ
	virtual ~CScene(){}
	// タスク処理
	virtual void Task(Task::CTaskContext*);

protected:
	// コンテキストクラスを指定する
	ContextT context_;

	// コンテキストを持っているので、Listの入れ替えが不要
	virtual void callTaskAction(Task::CTaskContext*);	// 動作用

	// コンテキストの基本設定
	virtual void setContext(Task::CTaskContext* pContext)
	{
		context_.setApp(pContext->getApp());
		context_.setDrawPlane(pContext->getDrawPlane());
		context_.setBgmSound(pContext->getBgmSound());
		context_.setInput(pContext->getInput());
		context_.setScenarioData(pContext->getScenarioData());
		context_.setTaskList(this);
		context_.setScene(smart_ptr<IScene>(this,false));
	}
};

template<class ContextT>
__inline void CScene<ContextT>::Task(Task::CTaskContext* pContext)
{// 子は渡ってきたコンテキストの代わりに
 // 親のコンテキストを渡す
	context_.action(pContext->IsAction());
	if(pContext->IsAction())
	{
		OnAction(pContext);
		callTaskAction(&context_);
	}
	else
	{
		callTaskDraw(&context_);
	}
}

template<class ContextT>
__inline void CScene<ContextT>::callTaskAction(Task::CTaskContext* pContext)
{// コンテキスト内のTaskList入れ替えが必要ない
	tasklist::iterator it=listTask_.begin();
	while(it!=listTask_.end())
	{
		(*it)->Task(pContext);
		if(IsKill() || IsRemove()){
			// killフラグだったらdelete
			if(IsKill()){ DELETE_SAFE(*it); }
			it=listTask_.erase(it);

			// フラグリセット
			bKill_=false;
			bRemove_=false;
		}
		else
		{
			++it;
		}
	}
}

} // namespace Scene end
} // namespace BMW end