/*
	katze 06/05/23
	update 08/7/22
	マップ上のチップ操作
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CCode_ctrl_map_chip : public BMW::Rule::IRuleTask
{/*
	マップ上のチップ操作

	タイプ
	操作

	// キャラチップ対象なら
	対象ID
	フラグ

	// マップなら
	ロードするSymobol ID
	対象のマップインデックス
	対象のTaskIndex
	
	が積まれている
 */
public:
	enum eType{
		CHARA,
		MAP
	};
	enum eCtrl{
		VISIBLE,
		ADD,
		SWAP,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Effect end
} // namepsace SLG end
} // namepsace BMW end