/**
	クリアデータの引き継ぎ処理を行うシーン
	katze 08/08/27
*/
#pragma once

#include "../Scene/CScene.h"

namespace BMW{

namespace Unit{
class CYesNoUnit;
} // namespace Unit end

namespace Handover{

class CHandoverScene : public Scene::CScene<Task::CTaskContext>
{/**
	クリアデータの引き継ぎ処理を行うシーン
 */
public:
	enum eState{
		NORMAL,
		FADE,
		FADE_IN,
		FADE_END,
		KAKUNIN=-4,
		ENEMY_TRAIN,
		PANEL_START,
	};
	enum eButton{
		YES=-2,
		NO,
		// 0以降は養成段階そのもの
		TRAIN=0,
	};
	// コンストラクタ・デストラクタ
	CHandoverScene();
	~CHandoverScene();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);
	void OnComeBack(int nID, Task::CTaskContext*);

	// イベントハンドラ
	void eventFade(Task::CTaskContext*);
	void eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext);
	void eventCancel(Task::CTaskContext*);

private:
	// FADE終了後のjump先
	int nNextScene_,nHero_;
	// 選択した養成段階
	int nEnemyTrain_;
	// 現在、表示中のパネル
	GUI::CPanel* pCurrentPanel_;
	// GUI系
	GUI::CPanel* pKakunin_;
	GUI::CPanel* pEnemyTrain_;
	Unit::CYesNoUnit* pYesNo_;
	GUI::CGraphic*	pBlack_;
	GUI::CPanel* pNumButton_;

	// 引継ぎ処理
	void saveHandover();
	// パネル入れ替え
	void setNextPanel(int nNExt);
	// ボタン停止/解除
	void trainButtonPause(bool bPause);
};


} // namespace Handover end
} // namesapce BMW end
