/*
	katze 05/07/20
	トラップエフェクト
*/
#pragma once

namespace BMW{
namespace SLG{

namespace Map{
class CMapChipChara;
} // namespace Map end

namespace Effect{
class CEffectMoveiClip;
} // namespace Effect end

namespace C_06{

class CEvent_trap : public Task::ITaskList
{/**
	トラップエフェクト
 */
public:
	enum eState{
		WAIT,
		END,
	};
	// デストラクタ
	~CEvent_trap();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	Effect::CEffectMovieClip*	pEffect_[7];
	int nPos_;
};

} // namespace C_06 end
} // namespace SLG end
} // namespace BMW end