/*
	katze 06/03/01
	戦闘養成
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CTrainBattle : public ITrainBase
{/**
	戦闘養成
 */
public:
	enum eBattle{
		HP_DOWN,
		HP_UP,
		EN_DOWN,
		EN_UP,
		QUICK_DOWN,
		QUICK_UP,
		TOUGH_DOWN,
		TOUGH_UP,
		RESET,
		OK,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionUpdateBP(Task::CTaskContext*);

private:
	// 対象キャラデータ
	BMW::Chara::CDataCharaInter* pChara_;
	// 養成段階
	enum eFirst{
		HP,EN,QUICK,TOUGH,
	};
	enum eSecond{
		SOURCE,UP,BASE
	};
	int nTrain_[4][3];

	int getPer(int nKind);
	int getTrainBP(int nKind, int nSource, int nUp);
	void updateBar(GUI::CPanel* pPanel,int nFirst, int nSource, int nItem);
	void decTrain(int nTrain, GUI::CPanel* pPanel, int nSource, int nItem);
	void incTrain(int nTrain, GUI::CPanel* pPanel, int nSource, int nItem);
	// インターフェイス
	// GUI::CPanel* pPanel;
	GUI::CPanel*	pHP_;
	GUI::CPanel*	pEN_;
	GUI::CPanel*	pQuick_;
	GUI::CPanel*	pTough_;
	GUI::CNum*		pSum_;
	GUI::CNumCtrl*	pRemain_;
};

} // namespace Chara end
} // namespace Inter end
} // namespace BMW end