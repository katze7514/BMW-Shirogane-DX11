/*
	katze 05/05/24
	フェードコントロール
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CCode_fade : public BMW::Rule::IRuleTask
{/**
	フェードコントロール

	制御
	フレーム数
	色
	スタックに積む
 */
public:
	enum eColor{
		BLACK,
		WHITE,
		RED,
	};
	enum eCtrl{
		FADE_IN,
		FADE_OUT,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end