/*
	katze 05/06/25
	update 06/03/02
	データシーン
*/
#pragma once

#include "../Scene/CScene.h"
#include "CDataContext.h"

namespace BMW{

namespace Unit{
class CYesNoUnit;
} // namespace Unit end

namespace Data{

class CDataScene : public Scene::CScene<CDataContext>
{/**
	データシーン
 */
public:
	enum eState{
		NORMAL,
		FADE,
		FADE_END,
		DIALOG_INTRO,
		DIALOG_S,
		DIALOG_STOP,
		DIALOG_END,
		GAME,
	};
	enum ePriority{
		PANEL,
		DIALOG
	};
	enum eArrow{
		DOWN,
		UP,
	};
	enum eDialog{
		SAVE,
		LOAD,
	};
	enum eYesNo{
		YES,NO,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// セーブデータの取得
	void setData();
	// セーブデータファイル名を生成
	void getDataFile(string& sFile);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventArrow(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventDialog(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventCursol();
	void eventYesNo(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventCancel(Task::CTaskContext* pContext);

	// アクション
	void actionSave();
	bool actionLoad();

private:
	GUI::CPanel*	getDataLine(int nID);
	void			updateDataFooter(Save::CExecDataHead* pHead);
	// 一時的にキャラアイコンデータを保持する
	Movie::CSymbolDB	symbol_;
	// インターフェイス
	GUI::CPanel*		pPanel_;
	GUI::CPanelCtrl*	pCtrl_;
	GUI::CText*			pPage_;
	GUI::CPanel*		pFooter_;
	GUI::CGraphic*		pBlack_;
	// ダイアログ
	GUI::CPanel*		pDialog_;
	CInteriorCounter	c_;
	Unit::CYesNoUnit*	pYesNo_;

	int getMode();
};

} // namespace Data end
} // namespace BMW end