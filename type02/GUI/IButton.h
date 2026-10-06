/*
	katze 06/02/12
	ボタンの基底クラス
*/
#pragma once

namespace BMW{
namespace GUI{

class IButton : public CTaskBase
{/**
	ボタンクラスの基底
 */
public:
	// 状態ID
	enum eButtonState{
		NORMAL,
		OVER,
		PRESS,
		PUSH,
	};
	// デストラクタ
	virtual ~IButton(){}

	// タスク
	virtual void OnAction(CTaskContext*);

	// 設定・取得
	void setRange(int nLeft, int nTop, int nRight, int nBottom){ ::SetRect(&range_,nLeft,nTop,nRight,nBottom); }
	void setRange(const RECT& range){ range_=range; }


	// ポップアップ
	const string&	getPopUp()const{ return sPopUp_; }
	string&			getPopUp(){ return sPopUp_; }
	void			setPopUp(const string& sPopUp){ sPopUp_=sPopUp; }
	void			clearPopUp(){ sPopUp_.clear(); }

	// アクション
	virtual void actionOverIn(CTaskContext*);
	virtual void actionOverOut(CTaskContext*);
	virtual void actionPress(CTaskContext*);
	virtual void actionRelease(CTaskContext*);

	// 与えられた座標が反応範囲内かを調べる
	bool IsRange(int nX,int nY);

	// 設定子
	static void setButtonField(GUI::IButton* pButton, const RECT& rect, const string& sPopUp);

protected:
	// ボタンの反応範囲
	RECT range_;

	// ポップアップ用文字列
	string sPopUp_;
};

} // namespace GUI end
} // namespace BMW end