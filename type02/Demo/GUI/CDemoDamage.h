/*
	katze 05/05/24
	update 06/03/24
	ダメージ数値
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoDamage : public Task::ITaskBase
{/**
	ダメージ数値
 */
public:
	enum eState{
		NORMAL,
		VIEW,
		WAIT,
	};
	// デストラクタ
	virtual ~CDemoDamage();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void action(int nSide, int nValue);
	void ct();

private:
	GUI::CNum*			pNum_;	// ダメージ値
	CInteriorCounter	counter_;
	Task::ITaskBase*	pCritical_; // クリティカル表示
};

} // namespace Demo end
} // namespace BMW end