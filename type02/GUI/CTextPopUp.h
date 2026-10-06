/*
	katze 06/02/20
	ポップアップ機能を持ったテキスト
*/
#pragma once

namespace BMW{
namespace GUI{
class IButton;

class CTextPopUp : public CText
{/**
	ポップアップ機能を持ったテキスト
 */
public:
	// コンストラクタ・デストラクタ
	CTextPopUp();
	virtual ~CTextPopUp();

	// タスク
	virtual void Task(Task::CTaskContext*);

	// テキスト生成
	virtual void UpdateText();
	virtual void UpdateTextA();
	virtual void UpdateTextAA();

	// 設定
	void setRange(const RECT& rect);
	const string&	getPopUp()const;
	string&			getPopUp();
	void			setPopUp(const string& sPopUp);

protected:
	// ポップアップ能力を持たすため
	IButton* pButton_;
};

} // namespace GUI end
} // namespace BMW end