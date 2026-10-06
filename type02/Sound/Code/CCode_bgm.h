/*
	katze 05/04/25
	genereted by code_gen.rb
	bgm
*/
#pragma once

#include "../../Rule/IRuleTask.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Sound{
namespace Code{

class CCode_bgm : public Rule::IRuleTask
{/**
	bgm

	BGM操作
	Stateに操作IDが設定される。
	 再生の時は、スタックトップに変更するBGM IDを積んでおく
	 フェードの際は、フェードするフレーム数を、スタックに積んでおく
	 フェードインの時は、ならすBGMIDもスタックに積む
 */
public:
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace Sound end
} // namespace BMW end
