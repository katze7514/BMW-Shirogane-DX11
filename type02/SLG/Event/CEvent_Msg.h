/*
	katze 05/05/23
	SLG上でのメッセージイベント
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Event{

class CEvent_Msg : public BMW::Rule::IRuleTask
{/**
	SLG上でのメッセージイベント

	順番に
		サイド
		キャラIDがSLGなのかFaceフラグ
		キャラID
		顔ID
		文字列プールID
		マスクフラグ
	と積んでおく

	MSGに状況を設定しておく
	よって、こいつはニーモニック扱い
	表示や、入力待ちは別
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end