/*
	katze 06/03/01
	武器養成
*/
#pragma once

#include "ITrainBase.h"

namespace BMW{
namespace Inter{
namespace Chara{

class CTrainWeapon : public ITrainBase
{/**
	武器養成
 */
public:
	enum eButton{
		DOWN,UP,OK,CHANGE,
	};
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// イベントハンドラ
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext*);

	// アクション
	void actionUpdateBP(Task::CTaskContext*);

private:
	// 養成段階
	int nTrain_;
	int nUp_;
	int nCost_;
	// 武器ごとの養成タイプ
	vector<int> vecWeaponType_;

	// ページの分割数
	int nDiv_;
	// 表示対象外の個数
	int nOut_;

	void createWeaponPanel(BMW::Chara::CDataCharaInter& chara, Task::CTaskContext& p);
	void updateWeaponPanel();
	// インターフェイス
	// GUI::CPanel* pPanel;
	GUI::CPanel*	pBar_;
	GUI::IPanel*	pWeapon_;
	GUI::CNum*		pPercent_;
	GUI::CNum*		pSum_;
	GUI::CNumCtrl*	pRemain_;
	GUI::CPanel*	pChange_;
	GUI::CNum*		pPage_;
};

} // namespace Chara end
} // namesapce Inter end
} // namespace BMW end