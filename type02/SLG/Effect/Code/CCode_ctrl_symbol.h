/*
	katze 06/05/23
	シンボル操作
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_ctrl_symbol : public BMW::Rule::IRuleTask
{/*
	シンボル操作

	対象No
	タイプ
	設定内容

	が積まれている
 */
public:
	enum eType{
		VALID,
		VISIBLE,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namepsace SLG end
} // namepsace BMW end