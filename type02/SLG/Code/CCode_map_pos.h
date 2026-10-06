/*
	katze 05/05/22

	マップを指定された位置が
	中央に来るようにするコード
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_map_pos : public BMW::Rule::IRuleTask
{/**
	マップを指定された位置が
	中央に来るようにするコード

	位置をx､yの順にスタックに積んでおく
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end