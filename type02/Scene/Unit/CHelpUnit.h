/**
	katze 07/03/14
	ヘルプユニット
*/
#pragma once

namespace BMW{

namespace GUI{
class CGuiDefDB;
} // namespace GUI end

namespace Unit{

class CHelpUnit : public Task::ITaskBase
{/**
	ヘルプモードを表現するクラス

	内部に持ってるデータを前景に表示するだけ。
	こいつが表示中はシーンは動作を止める。

	実験的にnew/deleteを動的にしてみようと思う
 */
public:
	enum eGUI{
		TOP,
		BOTTOM,
	};
	enum eState{
		NORMAL,
		INTRO,
		WAIT,
		END,
	};

	// コンストラクタ・デストラクタ
	CHelpUnit();
	~CHelpUnit();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// インターフェイス
	GUI::CPanel*	pPanel_[2];
	// GUIそのものをトゥイーンできないので、
	// 仕方ないのでハードコード
	// モーション
	Movie::CMotion	motion_[2];

	// GUI実体
	GUI::CGuiDefDB*	pGuiDef_;

	// 現在の入力フラグ
	bool bInputGuard_;
	bool bCursolVisible_;

	// モーション動作とか
	void setMotion(bool bIntro);
};

} // namespace Unit end
} // namespace BMW end