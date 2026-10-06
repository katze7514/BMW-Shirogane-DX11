/*
	katze 06/06/09
	攻撃対象選択Ver.2
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Attack{

class CAttack_select2 : public BMW::Rule::CRuleList
{/*
	攻撃対象選択

	選択されると対象キャラに設定され、
	そのSLGIDがスタックに積まれる。
	キャンセルの場合は、-1を積む
 */
public:
	enum eState{
		NORMAL,
		OK,
		CANCEL,
	};
	enum ePriority{
		OK_T,
		CANCEL_T,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void actionEnd(Task::CTaskContext*);

private:
	// 現在表示しているキャラID
	int nChara_;
	int nSide_;

	CSLGContext* p;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end