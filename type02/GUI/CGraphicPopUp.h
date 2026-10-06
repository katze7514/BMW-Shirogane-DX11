/*
	katze 06/02/20
	ポップアップ機能を持ったグラフィック
*/
#pragma once

namespace BMW{
namespace GUI{
class IButton;

class CGraphicPopUp : public CGraphic
{/**
	ポップアップ機能を持ったテキスト
 */
public:
	// コンストラクタ・デストラクタ
	CGraphicPopUp();
	virtual ~CGraphicPopUp();

	// タスク
	virtual void Task(Task::CTaskContext*);

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