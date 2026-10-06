/*
	katze 06/05/23
	シンボル再生終了待ちAPI
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CWait_symbol : public Task::ITaskList
{/**
	シンボル再生待ちAPI

	No
	
	が積まれてる
 */
public:
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	Movie::CMovieClip* pMovie_;
};

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end
