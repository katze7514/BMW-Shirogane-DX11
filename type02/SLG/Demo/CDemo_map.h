/*
	katze 05/06/04
	update 06/03/17
	マップ上のデモ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Demo{

class CDemo_map : public Task::ITaskList
{/**
	マップ上のデモ
	構成的には、デモシーンと同様
 */
public:
	enum eState{
		NORMAL,
		END,
	};
	enum eKind{
		BATTLE,
		CURE,
		REFILL,
		SPIRIT,
		ITEM,
	};
	// コンストラクタ・デストラクタ
	CDemo_map();
	~CDemo_map();
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 取得
	GUI::CPanel*		getLeftPanel(){ return pLeft_; }
	GUI::CPanelCtrl*	getLeftHead(){ return pLeftHead_; }
	void				resetLeft();
	GUI::CPanel*		getRightPanel(){ return pRight_; }
	GUI::CPanelCtrl*	getRightHead(){ return pRightHead_; }
	void				resetRight();

	Task::ITaskBase*	getChip(int nSide){ return pChara_[nSide]; }
	Task::ITaskBase*	getSerif(int nSide){ return pEffect_[nSide]; }

private:
	// インターフェイス
	Task::CTaskCtrl<>	taskCtrl_;
	GUI::CPanel*		pPanel_;
	GUI::CPanel*		pLeft_;
	GUI::CPanelCtrl*	pLeftHead_;
	GUI::CPanel*		pRight_;
	GUI::CPanelCtrl*	pRightHead_;
	Task::ITaskBase*	pChara_[2];
	Task::ITaskBase*	pEffect_[2];

	void callTaskAction(Task::CTaskContext*);
	void callTaskDraw(Task::CTaskContext*);

	// ID
	int getDemoID(int nID);
};

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end