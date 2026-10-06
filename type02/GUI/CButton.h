/*
	katze 05/02/19
	update 06/01/20 イベントハンドラ搭載
	update 06/02/19 IButtonに分離
	ボタンクラス
*/
#pragma once

#include "Event/CEventButton.h"
#include "IButton.h"

namespace BMW{
namespace GUI{
using namespace Task;

class CButton : public IButton
{/**
	ボタンを表現する基底クラス
	インターフェイス定義スクリプト化に伴い
	リスナーモデルに変更
	各アクション動作時に、リスナーを呼び出す
 */
public:
	typedef delegate<void, const smart_ptr<CEventButton>&, Task::CTaskContext*> ButtonEvent;
	// EVENT ID
	enum eButtonEvent{
		OVER_IN,
		OVER_OUT,
		PRESS_E,
		RELEASE,
		CANCEL,
	};
	// コンストラクタ・デストラクタ
	CButton();
	virtual ~CButton(){}

	// イベント
	smart_ptr<CEventButton>& getEvent(){ return pEvent_; }
	void					 setEvent(const smart_ptr<CEventButton>& pEvent){ pEvent_=pEvent; }
	const ButtonEvent&		 getEventHandler(){ return eventFun; }
	void					 setEventHandler(const ButtonEvent& fun){ eventFun=fun; }
	

	// アクション
	virtual void actionOverIn(CTaskContext*);
	virtual void actionOverOut(CTaskContext*);
	virtual void actionPress(CTaskContext*);
	virtual void actionRelease(CTaskContext*);

	// 設定子
	static void setButtonEvent(GUI::CButton* pButton, const ButtonEvent& fun, int nValue);

protected:
	// このボタンにイベントがあった時にハンドラに渡される
	smart_ptr<CEventButton>	pEvent_;
	// イベントリスナー
	ButtonEvent		eventFun;
};

} // namespace GUI end
} // namespace BMW end