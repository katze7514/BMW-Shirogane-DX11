/*
	katze 05/05/17
	update 06/03/17 APIに昇格
	フェーズ切り替え時の定型処理
*/
#pragma once

namespace BMW{
namespace SLG{

namespace Effect{
class CEffectMovieClip;
} // namespace Effect end

namespace Phase{
class CPhaseBall;

class CPhase_init : public Task::ITaskList
{/**
	フェーズ切り替え時の定型処理
 */
public:
	enum eState{
		EFFECT,
		END,
		PHASE_START,
		VICTORY,
		VICTORY_CHANGE,
	};
	// デストラクタ
	~CPhase_init();

	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

private:
	CPhaseBall* pBall_;
	Task::CTaskCtrl<Effect::CEffectMovieClip>* pPhaseCtrl_;

	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);
};

} // namespace Phase end
} // namespace SLG end
} // namespace BMW end