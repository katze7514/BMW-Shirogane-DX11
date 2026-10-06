/*
	katze 05/05/22
	メッセージボードの状態を変更する
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CCode_msg_state : public BMW::Rule::IRuleTask
{/**
	メッセージボードの状態を変更する

	状態とは、有効・無効・表示、の三つ
	表示の場合のフラグは、visibleに設定されている
	変更フラグは、stateに、サイドはスタックに積まれている
 */
public:
	enum eState{
		VALID,
		INVALID,
		VISIBLE,
	};
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end