/*
	katze 05/07/07
	ペナルティのパネル
*/
#pragma once

#include "../../Scene/GUI/CTextAndNum.h"

namespace BMW{
namespace SLG{
namespace Penalty{

class CPenalty_panel : public Task::ITaskList
{/**
	ペナルティのパネル
 */
public:
	// コンストラクタ
	CPenalty_panel();
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// 設定
	void setChara(int nPos, const string& sName, int nPenalty);

private:
	GUI::CTextAndNum text_[12];

	void callTaskDraw(Task::CTaskContext*);
};

} // namespace Penalty end
} // namespace SLG end
} // namespace BMW end