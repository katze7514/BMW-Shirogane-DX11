/*
	katze 05/05/15
	update 06/03/24
	デモ用超簡易ステータス
*/
#pragma once

#include "../../SLG/GUI/CStatusCharaVeryEasy.h"

namespace BMW{
namespace GUI{
class CGage;
} // namespace GUI end
namespace Demo{

class CDemoEasyStatus : public SLG::CStatusCharaVeryEasy
{/**
	ようは、HPやENが減っていくということをする
 */
public:
	enum eState{
		NORMAL,
		HP_DOWN,
		EN_DOWN,
		HPEN_DOWN,
		INTRO,
		EXIT,
	};
	// デストラクタ
	virtual ~CDemoEasyStatus(){}
	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// アクション
	void action(int nDel, int nState, int nDel2=0);

private:
	GUI::CGage* pHP_;
	GUI::CGage* pEN_;
	CInteriorCounter counter_,counter2_;
	CInteriorCounter y_;
	int nY_;
};

} // namespace Demo end
} // namespace BMW end