/*
	katze 06/02/12
	サークルメニューリスナ
*/
#pragma once

namespace BMW{

namespace GUI{
class CEventButton;
class CCircleMenu;
} // namesapce GUI end

namespace Event{

class IListenerCircleMenu
{/**
	サークルメニューリスナ
 */
public:
	// デストラクタ
	virtual ~IListenerCircleMenu(){}
	// サークルメニューリスナ
	// 動き終了時に呼ばれる
	virtual void eventCircle(int nState, Task::CTaskContext* pContext)=0;
	// ボタンイベントリスナ
	virtual void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)=0;

	// アクション
	virtual void actionMenu(int nState, Task::CTaskContext* pContext);

protected:
	GUI::CCircleMenu* pMenu_;
};

} // namespace Event end
} // namespace BMW end