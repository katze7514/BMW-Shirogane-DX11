/*
	katze 06/01/30
	ポップアップ
*/
#pragma once

namespace BMW{
namespace Draw{

class CPopUpBase : public Task::CTaskBase
{/**
	ポップアップ一つを表現する
 */
public:
	enum eState{
		NORMAL,
		WAIT,
		FADE_IN,
	};
	// コンストラクタ・デストラクタ
	CPopUpBase();
	CPopUpBase(const string& s);
	virtual ~CPopUpBase(){}

	// タスク
	virtual void OnReset(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);
	virtual void OnDraw(Task::CTaskContext*);

	// 設定・取得
	GUI::CText&		getText(){ return text_; }
	GUI::IButton*	getButtonTask(){ return pButton_; }
	void			setButtonTask(GUI::IButton* pButton){ pButton_=pButton; }

	// 設定されているテキストを表示するポップアップを構築する
	void createPopUp();

	// サイズ
	virtual void getSize(LONG &lWidth, LONG& lHeight);
	virtual void getDrawSize(LONG &lWidth, LONG& lHeight){ getSize(lWidth,lHeight); }

protected:
	void setPos(Task::CTaskContext*);

	// 表示テキスト
	//CTextFastPlane	text_;
	GUI::CText	text_;
	// 枠
	CFastPlane	rect_;
	// フェードインで使うカウンタ
	CInteriorCounter counter_;
	int nFrame_;
	// こいつがになってるボタンタスク
	GUI::IButton* pButton_;
};

// set<CPopUpBase*>を行うための比較子
// テキスト内容をIDとするのである！
/*struct CPopUpBasePtrLess : public binary_function<const CPopUpBase*,const CPopUpBase*,bool>
{
	bool operator()(const CPopUpBase* ps1, const CPopUpBase* ps2) const
	{
		return const_cast<CPopUpBase*>(ps1)->getText().getText() < const_cast<CPopUpBase*>(ps2)->getText().getText();
	}
};*/

} // namespace Draw end
} // namespace BMW end