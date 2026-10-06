/*
	katze 06/01/30
	update 08/09/15
	前景タスク
*/
#pragma once

namespace BMW{

namespace Movie{
class CSymbolDB;
} // namespace Movie end

namespace Draw{
class CPopUpCtrl;
} // namespace Draw end

namespace Scene{

class CFoward : public Task::CTaskList
{/**
	前景タスク
	どっちかってと戦闘デモの背景クラスに近い動作ということで
	あと、画面全体を対象にするようなエフェクトもこいつがかけるのかな。

	Fader・Loading・DictCaution・PopUp・Message表示
 */
public:
	typedef Draw::CFader::FaderEvent FaderEvent;

	enum ePriority{
		POPUP,
		HELP,
		FADE,
		LOADING,
		DICT_CAUTION,
	};
	// コンストラクタ・デストラクタ
	CFoward();
	~CFoward();

	// タスク
	void OnInit(Task::CTaskContext*);

	// ポップアップ動作
	void createPopUp(GUI::IButton* pButton, Task::CTaskContext* pContext);
	void clearPopUp();
	void clearPopUpAll();
	// ロードグラフィク
	void visibleLoad(bool bVisible);
	// コーショングラフィク
	void visibleDictCaution(bool bVisible);
	// フェーダ動作
	void setFadeColor(COLORREF rgb=RGB(0,0,0));
	void fadeIn(int nFrame=15);
	void fadeOut(int nFrame=15);
	void setFaderHandler(const FaderEvent& fun);
	bool IsFadeEnd()const;
	int	 getFadeType()const{ return fader_.getFadeType(); }
	// ヘルプ動作
	void createHelp(int nID, Task::CTaskContext*);

	Movie::CSymbolDB& getSymbolDB(){ return *pSymbolDB_; }

private:
	// ポップアップ管理
	Draw::CPopUpCtrl*	pPopUpCtrl_;
	// LOADING
	GUI::CGraphic		loading_;
	// DICT_CAUTION
	GUI::CGraphic		dict_caution_;
	// フェーダ
	Draw::CFader		fader_;
	// これで管理されるグラフィックDB
	Movie::CSymbolDB*	pSymbolDB_;
};

} // namespace Scene end
} // namespace BMW end