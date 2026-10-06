/*
	katze 06/02/04
	パネルコントローラ
*/
#pragma once

#include "IPanel.h"

namespace BMW{
namespace GUI{

class CPanelCtrl : public IPanel
{/**
	パネル基底クラス
 */
public:
	// デストラクタ
	CPanelCtrl();
	virtual ~CPanelCtrl(){}

	// 描画サイズの計算も実装すべきかもしれないけど、とりあえず、必要ないので未実装
	//virtual void getSize(LONG& lWidth, LONG lHeight);
	//virtual void getDrawSize(LONG& lWidth, LONG lHeight);

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnReset(Task::CTaskContext*);

	// 表示選択とか
	// 空文字列をいれると非表示
	virtual Task::ITaskBase*	getValidWidget(){ return ctrlWidget_.getCurrentTask(); }
	virtual void				validWidget(int nID);
	virtual void				validWidget(const string& sID);

	// 設定・取得
	virtual Task::ITaskBase*	getWidget(int nID){ return ctrlWidget_.getTask(nID); }
	virtual Task::ITaskBase*	getWidget(const string& sID){ return ctrlWidget_.getTask(getID(sID)); }
	virtual void				addWidget(Task::ITaskBase* pBase, const string& sID);
	
	// Widget取得のヘルパ
	template<class T>
	T* getValidWidgetCast(){ return static_cast<T*>(ctrlWidget_.getCurrentTask()); }
	
	// コントローラ取得
	Task::CTaskCtrl<>&	getWidgetCtrl(){ return ctrlWidget_; }

protected:
	// 管理してるタスク
	Task::CTaskCtrl<> ctrlWidget_;
};

} // namespace GUI end
} // namespace BMW end