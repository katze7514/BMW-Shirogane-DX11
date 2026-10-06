#include "stdafx.h"

#include "../../SLG/IDRule.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CSLGDef.h"

#include "CCaosCreate.h"
#include "CFactorySLG_C_11.h"

namespace BMW{
namespace SLG{
namespace C_11{

smart_ptr<Task::ITaskList> CFactorySLG_C_11::createTaskList(int nID)
{
	if(nID>=0
	&&(nID<Rule::USER_DEF
	|| nID==nCaos_))
	{// API
		return mapApi_[nID];
	}
	else
	{// USER_DEF以降は文字通りユーザー定義スクリプト
		return createTaskListUser(nID);
	}
}

void CFactorySLG_C_11::setScript(CSLGContext* pContext)
{// C_11用
	// 通常のルーチン生成
	CSubroutineFactorySLG::setScript(pContext);

	// イベントのIDを取得
	// スクリプトで設定しておく
	nCaos_ = pContext->getSLGDef().getScriptID("CAOS_CREATE");
	// 混沌生成
	CCaosCreate* pCaos = new CCaosCreate();
	pCaos->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(nCaos_,smart_ptr<Task::ITaskList>(pCaos)));
}

} // namespace C_11 end
} // namespace SLG end
} // namespace BMW end
