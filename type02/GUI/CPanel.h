/*
	katze 06/01/20
	パネル
*/
#pragma once

#include "IPanel.h"
#include "../Rule/CRuleList.h"

namespace BMW{
namespace GUI{

class CPanelList : public CTaskList
{/**
	こいつで管理するタスクの親は
	これ自身の親になる。
 */
public:
	virtual ~CPanelList(){}
	virtual void addTask(ITaskBase* pBase, int nPri);
};

class CPanel : public IPanel
{/**
	パネル
	つまり、GUI置き場
	プライオリティを文字列(名前)で管理しているTaskListだとおもいねぇ
 */
public:
	// コンストラクタ・デストラクタ
	CPanel();
	virtual ~CPanel(){}

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnReset(Task::CTaskContext*);

	// 設定・取得
	virtual void valid(bool bV){ ITaskBase::valid(bV); listWidget_.valid(bV); }
	virtual void visible(bool bV){ listWidget_.visible(bV); }
	virtual bool IsVisible(){ return listWidget_.IsVisible(); }

	// 描画サイズの計算も実装すべきかもしれないけど、とりあえず、必要ないので未実装
	//virtual void getSize(LONG& lWidth, LONG lHeight);
	//virtual void getDrawSize(LONG& lWidth, LONG lHeight);

	// 設定・取得
	virtual Task::ITaskBase*	getWidget(int nID){ return listWidget_.getTask(nID); }
	virtual Task::ITaskBase*	getWidget(const string& sID){ return listWidget_.getTask(getID(sID)); }
	virtual void				addWidget(Task::ITaskBase* pBase, const string& sID);
	virtual void				delWidget(const string& sID){ listWidget_.killTask(getID(sID)); }
	virtual Task::ITaskBase*	removeWidget(const string& sID){ return listWidget_.removeTask(getID(sID)); }
	virtual Task::ITaskBase*	swapWidget(Task::ITaskBase* pBase, const string& sID, bool bDelete=true);
	virtual void				emptyWidget(){ listWidget_.getTaskList().clear(); }

	// 操作
	// 子供に対する全操作
	virtual void validAll(bool bV);
	virtual void visibleAll(bool bV);

	// リスト取得
	CPanelList& getWidgetList(){ return listWidget_; }

protected:
	CPanelList listWidget_;
};

} // namespace GUI end
} // namespace BMW end