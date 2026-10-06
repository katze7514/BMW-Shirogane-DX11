/*
	katze 05/07/18
	有効なキャラリスト操作
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CCode_valid : public BMW::Rule::IRuleTask
{/**
	有効なキャラリスト操作
	操作するキャラIDをスタックに積んでおく
	-1にぶつかるまで処理が続けられれる
 */
public:
	enum eState{
		ADD,
		DEL,
		CLEAR,
	};
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end