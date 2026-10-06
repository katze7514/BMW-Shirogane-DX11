/*
	katze 05/12/23
	チュートリアルメニュー
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace Tutorial{

class CTutorialScene : public BMW::Rule::CRuleList
{/*
	チュートリアルメニューを表現するシーン
 */
public:
	enum eState{
		FADE=-3,
		FADE_END,
		NORMAL,
	};
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
};

} // namespace Tutorial end
} // namespace BMW end
