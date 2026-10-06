#include "stdafx.h"

#include "../../SLG/IDRule.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CSLGDef.h"

#include "CArcher_snipe.h"
#include "CFactorySLG_T_20.h"

namespace BMW{
namespace SLG{
namespace T_20{

smart_ptr<Task::ITaskList> CFactorySLG_T_20::createTaskList(int nID)
{
	if(nID>=0
	&&(nID<Rule::USER_DEF
	|| nID==nArcher_))
	{// API
		return mapApi_[nID];
	}
	else
	{// USER_DEF以降は文字通りユーザー定義スクリプト
		return createTaskListUser(nID);
	}
}

void CFactorySLG_T_20::setScript(CSLGContext* pContext)
{// T_20用
	// 通常のルーチン生成
	CSubroutineFactorySLG::setScript(pContext);

	// アーチャーの狙撃イベントのIDを取得
	// スクリプトで設定しておく
	nArcher_ = pContext->getSLGDef().getScriptID("ARCHER_SNIPE");
	// アーチャーの狙撃イベント
	CArcher_snipe* pArcher = new CArcher_snipe();
	pArcher->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(nArcher_,smart_ptr<Task::ITaskList>(pArcher)));
}

} // namespace T_20 end
} // namespace SLG end
} // namespace BMW end
