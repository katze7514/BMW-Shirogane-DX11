/*
	katze 05/06/16
	インターミッションキャラセレクト
*/
#pragma once

namespace BMW{
namespace Unit{
class CSortUnit;
class CExitUnit;
} // namespace Unit end

namespace Inter{
class CInterContext;

namespace Select{

class CSelect : public Task::ITaskList
{/**
	インターミッションキャラセレクト
 */
public:
	enum eState{
		NORMAL,
		SORT,
	};
	enum eButton{
		CHANGE,
		DATA,
		SEARCH,
		EXIT,
		NEXT,
	};
	
	// デストラクタ
	~CSelect();
	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	// キャラ選択などが行われた時
	void eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// セーブとかEXITボタン動作
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// ソートユニット動作
	void eventSort(int nKey, int nOrder, Task::CTaskContext* pContext);
	// Exitユニット動作
	void eventExit(int nState, Task::CTaskContext* pContext);

private:
	// チップキャラ生成
	void createChipPanel(int nKey, int nOrder,Task::CTaskContext*);
	void clearChipPanel();

	// キャラ選択パネルの分割数
	int nDiv_;
	// インターフェイス
	GUI::CPanel*		pPanel_;
	Unit::CExitUnit*	pExitUnit_;
	// 処理簡単化のため↓
	GUI::CPanel*		pHeader_;
	GUI::IPanel*		pCharaPanel_;
	GUI::CPanel*		pChange_;
	GUI::CNum*			pPage_;
	Unit::CSortUnit*	pSort_;
	GUI::CPanel*		pStatus_;
};

} // namespace Select end
} // namespace Inter end
} // namespace BMW end