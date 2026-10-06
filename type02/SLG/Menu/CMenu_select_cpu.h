/*
	katze 05/05/17
	CPU時のSelectメニュー
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Action{
class CActionNormal;
} // namespace Action end

namespace Menu{

class CMenu_select_cpu : public BMW::Rule::CRuleList
{/**
	CPU時のSelectメニュー

	思考ルーチンを起動したり、
	それに合わせて、動かしたりとかする
 */
public:
	enum eState{
		CHANGE,
		EXEC,
		ACTION,
		CENTER,
		ATTACK,
		MOVE,
		PRE_WAIT,
		WAIT,
		CHARA_END,
		VICTORY,
		VICTORY_CHANGE,
		PHASE_END,
		END,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// アクション
	void actionRule(Task::CTaskContext*);
	void actionSelect(CSLGContext*);

private:
	//CDataCharaSLG* pChara_;
	list<int>::iterator it;
	Action::CActionNormal* pAction_;

	int nFrame_; // ちょっとWait

	int nActionCount_; // 複数回行動時に使う

	bool bTwiceAttack_; // 二回攻撃するかどうか
};

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end