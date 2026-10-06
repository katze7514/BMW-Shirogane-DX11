#include "stdafx.h"

#include "../Scene/IDScene.h"
#include "../BMW/IDBMW.h"

#include "CTutorialScene.h"

namespace BMW{
namespace Tutorial{

void CTutorialScene::OnInit(Task::CTaskContext* pContext)
{// 読み込まれているシナリオデータに合わせて
 // シナリオ選択メニューリストを作る

	// シナリオデータに合わせてループ
/*	int nID=0;
	Scenario::CScenarioDB::scenario_map scenario = pContext->getApp()->getScenario().getScenarioDB();
	Scenario::CScenarioDB::scenario_map::iterator it;
	for(it=scenario.begin(); it!=scenario.end(); it++)
	{
	}*/
}

void CTutorialScene::OnAction(Task::CTaskContext* pContext)
{// とりあえずは、シナリオを直接呼び出す
}

} // namespace Tutorial end
} // namespace BMW end