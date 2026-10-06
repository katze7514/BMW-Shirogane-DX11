/*
	katze 06/06/17
	混沌生成
*/
#pragma once

namespace BMW{
namespace SLG{
namespace C_11{

class CCaosCreate : public Task::ITaskList
{/**
	混沌生成
*/
public:
	enum eState{
		NORMAL,
		END,
	};
	// コンストラクタ・デストラクタ
	CCaosCreate();
	~CCaosCreate();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	VM::CScriptExec* pExec_;

	void createAddCaos(CSLGContext*);
};

} // namespace C_11 end
} // namespace SLG end
} // namespace BMW end
