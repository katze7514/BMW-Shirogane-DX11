/*
	katze 06/02/28
	基礎能力養成
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CTrainFnd : public ITrainBase
{/**
	基礎能力養成
 */
public:
	enum eButton{
		STR_DOWN,
		STR_UP,
		MGC_DOWN,
		MGC_UP,
		HIT_DOWN,
		HIT_UP,
		AVO_DOWN,
		AVO_UP,
		DEF_DOWN,
		DEF_UP,
		SKL_DOWN,
		SKL_UP,
		RESET,
		OK,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionUpdateFP(Task::CTaskContext*);

private:
	void initBar(GUI::CPanel* pPanel, int nValue);
	void updateBar(GUI::CPanel* pPanel, bool bUp);
	// インターフェイス
	//GUI::CPanel* pPanel_;
	// 各バー
	GUI::CPanel* pStr_;
	GUI::CPanel* pMgc_;
	GUI::CPanel* pHit_;
	GUI::CPanel* pAvo_;
	GUI::CPanel* pDef_;
	GUI::CPanel* pSkl_;
	// 計
	GUI::CNum*		pSum_;
	GUI::CNumCtrl*	pRemain_;
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end