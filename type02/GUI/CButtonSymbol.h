/*
	katze 06/01/16
	ボタンシンボル
*/
#pragma once

#include "CButton.h"

namespace BMW{
namespace GUI{

class CButtonSymbol : public CButton
{/**
	より、一般的なオブジェクトを持つボタン
 */
public:
	// デストラクタ
	virtual ~CButtonSymbol(){}

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnReset(Task::CTaskContext*);

	// アクション
	virtual void actionOverIn(CTaskContext*);
	virtual void actionOverOut(CTaskContext*);
	virtual void actionPress(CTaskContext*);
	virtual void actionRelease(CTaskContext*);

	// シンボルアクセッサ
	const Task::ITaskBase&	getSymbol(int nState)const{ return *pSymbol_[nState]; }
	void					setSymbol(const smart_ptr<Task::ITaskBase>& pSymbol, int nState)
	{
		pSymbol->setParent(smart_ptr<Task::ITaskBase>(this,false));
		pSymbol_[nState]=pSymbol; 
	}

	// 操作
	void getSize(LONG& lWidth,LONG& lHeight) const
	{ 
		lWidth=range_.right-range_.left; 
		lHeight=range_.bottom-range_.top; 
	}
	void getDrawSize(LONG& lWidth,LONG& lHeight) const { getSize(lWidth,lHeight); }
	
protected:
	// シンボルつうか、一番のプリミティブ
	smart_ptr<Task::ITaskBase> pSymbol_[3];
};

} // namespace GUI end
} // namespace BMW end