/**
	katze 08/08/19
	キャラ辞典のキャラ選択
*/
#pragma once

namespace BMW{

namespace GUI{
class CGraphicFace;
class CGraphicName;
} // namespace GUI end

namespace Dict{

class CDictCharaSelect : public Task::ITaskList
{
public:
	enum eButton{
		CHANGE=-1,
		// 0以上はキャラボタン
	};
	// デストラクタ
	~CDictCharaSelect();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントリスナー
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);

private:
	// パネル分割数
	int nDiv_;
	// インターフェイス
	GUI::CPanel* pPanel_;
	// 処理簡単のため下
	GUI::CPanelCtrl*	pSelect_;
	GUI::CPanel*		pChange_;
	GUI::CNum*			pPage_;
	GUI::CGraphicFace*	pFace_;
	GUI::CGraphicName*	pName_;
};

} // namespace Dict end
} // namespace BMW end
