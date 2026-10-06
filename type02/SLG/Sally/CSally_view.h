/*
	katze 06/06/08
	出撃キャラ一覧
*/
#pragma once

namespace BMW{

namespace Unit{
class CSortUnit;
} // namepsace Unit end

namespace SLG{
class CSLGContext;

namespace Sally{

class CSally_view : public BMW::Rule::CRuleList
{/**
	出撃キャラ一覧
 */
public:
	enum eState{
		NORMAL,
		WAIT,
		END,
	};
	enum ePriority{
		CHARA,
	};
	enum eButton{
		CHANGE,
		SP,
	};
	enum eMode{
		NORAML,
		SALLY,
	};
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	// キャラ選択などが行われた時
	void eventChara(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// ソートユニット動作
	void eventSort(int nKey, int nOrder, Task::CTaskContext* pContext);
	// パネルボタン
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	// カーソル移動
	void eventCursol();

private:
	// インターフェイス
	GUI::CPanel*		pPanel_;
	GUI::IPanel*		pCharaPanel_;
	Unit::CSortUnit*	pSort_;
	GUI::CButton*		pPageButton_;		
	GUI::INum*			pPage_;
	GUI::CButton*		pSpButton_;

	// モード
	int nMode_;
	// 分割数
	int nDiv_;

	// 管理用リスト
	// こいつを対象にソートとかが実行される
	// first:Sally_viewID second:SLG ID
	list<pair<int,int> > listChara_;
	void sortChara(int nKey,int nOrder, CSLGContext& p);

	// チップキャラ生成
	void createChipPanel(int nKey, int nOrder, CSLGContext*);
	//void clearChipPanel();
};

} // namespace Sally end
} // namespace SLG end
} // namepsace BMW end