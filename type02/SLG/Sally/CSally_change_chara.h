/*
	katze 06/06/13
	キャラデータ引き継ぎ
	もとい、変更
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Sally{

class CSally_change_chara : public Task::ITaskList
{/**
	キャラデータ引き継ぎ
	もとい、変更

	スタックに
		SLG ID
		CHARA ID
		TRAIN No
		LV FLAG
		LV OFFSET
	と積まれてる。


 */
public:
	enum eLvFlag{
		NORMAL,  // 特になし
		AVERAGE, // 登場済み味方の平均LVにする
		MAX,	 // 登場済み味方の最大LVにする
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end