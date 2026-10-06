/*
	katze 07/01/20
	無条件SLG終了
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_slg_end : public BMW::Rule::IRuleTask
{/**
	無条件SLG終了
 */
public:
	enum eState{
		NO_SAVE,
		SAVE,
		END,
	};
	// コンストラクタ
	CCode_slg_end(bool bSave=false){ setState(bSave?SAVE:NO_SAVE); }
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
