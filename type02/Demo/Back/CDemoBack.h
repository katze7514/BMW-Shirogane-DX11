/*
	katze 06/03/23
	戦闘デモ背景
*/
#pragma once

#include "IDemoBack.h"

namespace BMW{
namespace Demo{
class IDemoBackLine;

class CDemoBack : public IDemoBack
{/**
	戦闘デモ背景
 */
public:
	typedef vector<IDemoBackLine*> back_vec;
	// デストラクタ
	virtual ~CDemoBack();

	// タスク
	void Task(Task::CTaskContext*);
	void TaskBack(Task::CTaskContext*);
	void TaskForward(Task::CTaskContext*);

	// 設定
	void setBackLine(IDemoBackLine* pLine, int nLine){ vecBack_[nLine]=pLine; }
	void resizeBack(int nSize){ vecBack_.resize(nSize); }
	void setForwardLine(IDemoBackLine* pLine, int nLine){ vecForward_[nLine]=pLine; }
	void resizeForward(int nSize){ vecForward_.resize(nSize); }
	
private:
	back_vec vecBack_;
	back_vec vecForward_;
};

} // namespace Demo end
} // namespace BMW end