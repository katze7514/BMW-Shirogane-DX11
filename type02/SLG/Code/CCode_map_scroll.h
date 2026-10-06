/*
	katze 05/05/22
	update 06/05/24

	マップを指定されたマップインデックスが
	中央に来るようにするコード
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Code{

class CCode_map_scroll : public BMW::Rule::IRuleTask
{/**
	マップを指定されたマップインデックスが
	中央に来るようにするコード

	タイプ
	対象のマップインデックス or キャラID

	と積んでおく
 */
public:
	enum eType{
		MAP,
		CHARA,
		TWEEN,
	};
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end