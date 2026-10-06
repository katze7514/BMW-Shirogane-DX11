/*
	katze 05/04/27
	update 06/02/06 INumを基底に
	数字用Ctrl
*/
#pragma once

#include "../../GUI/INum.h"

namespace BMW{
namespace GUI{

class CNumCtrl : public INum
{/**
	数字用CTaskCtrl
 */
public:
	typedef Task::CTaskCtrl<INum> num_ctrl;
	typedef num_ctrl::task_map num_map;

	// コンストラクタ・デストラクタ
	CNumCtrl();
	virtual ~CNumCtrl(){}

	// タスク
	void Task(Task::CTaskContext*);

	bool IsValid(){ return ctrlNum_.IsValid(); }
	void valid(bool bV){ ctrlNum_.valid(bV); }
	bool IsVisible(){ return ctrlNum_.IsVisible(); }
	void visible(bool bV){ ctrlNum_.visible(bV); }

	// 設定・取得
	LONG getNum() const;
	void setNum(LONG lNum);
	bool IsPlus() const;
	void plus(bool bPlus);

	// 操作
	INum*	getNumGui(int nID){ return ctrlNum_.getTask(nID); }
	void	addNumGui(INum* pNum, int nID){ ctrlNum_.addTask(pNum,nID); }
	int		IsValidNumGui()const{ return ctrlNum_.getState(); }
	void	validNumGui(int nID){ ctrlNum_.setState(nID); }
	INum*	getCurrentNumGui(){ return ctrlNum_.getCurrentTask(); }

private:
	 num_ctrl ctrlNum_;

	// ヘルパ
	num_map& getNumMap(){ return ctrlNum_.getTaskMap(); }
	const num_map& getNumMap()const{ return ctrlNum_.getTaskMap(); }
};

} // namespace GUI end
} // namespace BMW end