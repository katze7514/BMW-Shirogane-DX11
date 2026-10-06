#include "stdafx.h"

#include "../../Hero/IDHero.h"

#include "../IDADV.h"
#include "CScenario_select.h"

namespace BMW{
namespace ADV{
namespace API{

void CScenario_select::OnAction(Task::CTaskContext* pContext)
{
	int nType= pContext->top();
	pContext->pop();

#ifdef BMW_DEBUG
	CDbg().Out("Scenario %d",nType);
#endif

	Save::CExecData& save = pContext->getApp()->getExec();
	BMW::Scenario::CScenarioDB& db = pContext->getApp()->getScenario();

	switch(nType)
	{
	case Scenario::NEXT:
		// シナリオセレクト
		// また、Nextが設定さてたらスルーする
		if(save.getNextScenario()<0) actionNext(save,db);
	break;

	case Scenario::MEKAHISUI:
	{// メカヒスイ仲間判定
		// 陽菜ルートのみ
		if(save.getHero()==Hero::Target::HARUNA)
		{
			// 翡翠・琥珀の合計撃墜数が45以上
			Chara::CDataCharaTrain* pHisui = save.getTrainData(Chara::Const::charaID_.getValue("PLAYER_HISUI_2"));
			Chara::CDataCharaTrain* pKohaku = save.getTrainData(Chara::Const::charaID_.getValue("PLAYER_KOHAKU_2"));

			if(pHisui!=NULL && pKohaku!=NULL // ってことはないはずだが念のため
			&& pHisui->getKill()+pKohaku->getKill()>=45)
			{// これなら仲間！
				save.setFlag("MECHHISUI_IN",1);
			}
		}
	}
	break;

	default: break;
	}

	getTaskListCtrl()->returnTaskList();
}

void CScenario_select::actionNext(Save::CExecData& save, BMW::Scenario::CScenarioDB& db)
{
	// 第一部だと、一話目だけ存在する
	if(save.getStory()==db.getScenarioID("01_C"))
	{
		// 主人公に応じて分岐
		if(save.getHero()==Hero::Target::TAKUMI)
		{ // 匠
			save.setNextScenario(db.getScenarioID("02_T"));
			save.getValidSet().clear();
			save.addValid(Chara::Const::charaID_.getValue("PLAYER_TAKUMI"));
		}
		else
		{ // 陽菜
			save.setNextScenario(db.getScenarioID("02_H"));
			save.getValidSet().clear();
			save.addValid(Chara::Const::charaID_.getValue("PLAYER_HARUNA"));
		}
	}
}

} // namespace API end
} // namespace ADV end
} // namespace BMW end