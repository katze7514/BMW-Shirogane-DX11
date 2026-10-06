/*
	katze 06/03/09
	ムービー用の停止を伝えるコード
*/
#pragma once

namespace BMW{
namespace Movie{
namespace Code{

class CCode_movie_stop : public Rule::IRuleTask
{/**
	ムービー用の停止を伝えるコード
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Movie end
} // namespace BMW end