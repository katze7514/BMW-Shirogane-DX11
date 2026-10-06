/*
	katze 05/05/10
	ムービー用の終了を伝えるコード
*/
#pragma once

namespace BMW{
namespace Movie{
namespace Code{

class CCode_movie_end : public Rule::IRuleTask
{/**
	ムービー用の終了を伝えるコード
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Movie end
} // namespace BMW end