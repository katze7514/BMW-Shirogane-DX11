/*
	katze 06/06/27
	コンテニュー時に意味のあるイベント
*/
#pragma once

#include "CScript_base.h"

namespace BMW{
namespace SLG{

class CContinue_start : public Script::CScript_base
{/**
	コンテニュー時に意味のあるイベント
 */
public:
	enum eState
	{
		NORMAL,
		FADE,
		END,
	};
	// デストラクタ
	virtual ~CContinue_start(){}

	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID,Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
};

} // namespace SLG end
} // namespace BMW end