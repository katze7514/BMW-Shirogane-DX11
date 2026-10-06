/*
	katze 0/02/18
	ボタンタスクホルダー
*/
#pragma once

#include "CButton.h"

namespace BMW{
namespace GUI{

class CButtonHolder : public CButton
{/**
	ボタンホルダー
	内側にボタンを持ち動作をすべてそれにスルーする
 */
public:
	// コンストラクタ・デストラクタ
	CButtonHolder():pButton_(NULL){}
	virtual ~CButtonHolder(){ DELETE_SAFE(pButton_); }
	// タスク
	void Task(Task::CTaskContext* pContext){ pButton_->Task(pContext); }

	// アクセッサ
	bool IsValid() const { return pButton_->IsValid(); }
	void valid(bool bV){ pButton_->valid(bV); }
	int  getState() const { return pButton_->getState(); }
	void setState(int nState){ pButton_->setState(nState); }

	bool IsVisible() const { return pButton_->IsVisible(); }
	void visible(bool bV){ pButton_->visible(bV); }

	// サイズの取得
	void getSize(LONG& lWidth,LONG& lHeight) const { pButton_->getSize(lWidth,lHeight); }
	void getDrawSize(LONG& lWidth,LONG& lHeight) const { pButton_->getDrawSize(lWidth,lHeight); }

	// 設定
	GUI::CButton*	getHoldButton(){ return pButton_; }
	void			setHoldButton(GUI::CButton* pButton)
	{ 
		DELETE_SAFE(pButton_);
		pButton_=pButton; 
		pButton_->setParent(smart_ptr<Task::ITaskBase>(this,false));
		pButton_->setEvent(getEvent());
		pButton_->setEventHandler(getEventHandler());
	}

private:
	GUI::CButton* pButton_;
};

} // namespace GUI end
} // namespace BMW end