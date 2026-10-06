/*
	katze 06/01/20
	ボタンイベント
*/
#pragma once

#include "../IDGui.h"
#include "IEvent.h"

namespace BMW{
namespace GUI{

class CEventButton : public IEvent
{/**
	ボタンイベント
 */
public:
	// コンストラクタ
	CEventButton():nState_(-1){}
	virtual ~CEventButton(){}
	// 種別の取得
	int		getKind()const{ return Event::Kind::BUTTON; }
	// 状態の取得
	int		getState()const{ return nState_; }
	void	setState(int nState){ nState_=nState; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

protected:
	int nState_;	// ボタンの状態が入ってくる
	int nValue_;	// ちょっとしたデータのやりとりを行う
};

// よく使うユーティリティだとおもいね
extern bool IsOverIn(const smart_ptr<CEventButton>& pButton);
extern bool IsOverOut(const smart_ptr<CEventButton>& pButton);
extern bool IsPress(const smart_ptr<CEventButton>& pButton);
extern bool IsRelease(const smart_ptr<CEventButton>& pButton);
extern bool IsCancel(const smart_ptr<CEventButton>& pButton);

} // namespace GUI end
} // namespace BMW end