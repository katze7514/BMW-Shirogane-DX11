/*
	katze 06/02/24
	Exitルール
*/
#pragma once

namespace BMW{

namespace Unit{
class CExitUnit;
} // namespace Unit end

namespace SLG{
namespace Exit{

class CExit_rule : public Task::ITaskList
{/**
	Exitルール
 */
public:
	enum eState{
		NORMAL,
		INTRO,
		CANCEL,
	};
	// デストラクタ
	~CExit_rule();
	// タスク
	void Task(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	
	// イベントハンドラ
	void eventExit(int nState, Task::CTaskContext*);

	// 設定・取得
	bool IsValid()const;
	void valid(bool bV);
	bool IsVisible()const;
	void visible(bool bV);

private:
	Unit::CExitUnit* pUnit_;
};

} // namespace Exit end
} // namespace SLG end
} // namespace BMW end