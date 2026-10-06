/*
	katze 06/06/08
	精神検索
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Sally{

class CSally_spirit : public BMW::Rule::CRuleList
{/**
	精神検索
 */
public:
	enum eState
	{
		NORMAL,
		END,
	};
	enum ePriority
	{
		INTERFACE,
	};
	enum eButton
	{
		CHANGE=-1,
	};
	// コンストラクタ
	CSally_spirit():pCharaPanel_(NULL){}
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// イベントリスナー
	// ボタン
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// キャラ
	void eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	// インターフェイス
	GUI::CPanel*	pPanel_;
	GUI::IPanel*	pCharaPanel_;
	GUI::CButton*	pChange_;
	GUI::INum*		pPage_;
	int nDiv_;
	
	// 精神初期化
	void initSpirit(GUI::CPanel* pPanel, CSLGContext*);
	// キャラインターフェイス生成
	void createChipPanel(int nSpirit , CSLGContext*);
	void setCharaChip(int nID, int nPos, int nSpirit, const GUI::CButton::ButtonEvent& fun, GUI::CPanel* pPanel, CSLGContext* p);
};

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end