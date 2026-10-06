#include "stdafx.h"

#include "../../SLG/IDRule.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CSLGDef.h"

#include "CEvent_trap.h"

#include "CFactorySLG_C_06.h"

namespace BMW{
namespace SLG{
namespace C_06{

smart_ptr<Task::ITaskList> CFactorySLG_C_06::createTaskListUser(int nID)
{
	if(nID==nTrap_)
	{// API
		return mapApi_[nID];
	}
	else
	{// USER_DEF以降は文字通りユーザー定義スクリプト
		return CSubroutineFactorySLG::createTaskListUser(nID);
	}
}

void CFactorySLG_C_06::setScript(CSLGContext* pContext)
{// C_06用
	// 通常のルーチン生成
	CSubroutineFactorySLG::setScript(pContext);

	// イベントのIDを取得
	// スクリプトで設定しておく
	nTrap_ = pContext->getSLGDef().getScriptID("CASTER_TRAP");
	// 魔法トラップ
	CEvent_trap* pTrap = new CEvent_trap();
	pTrap->OnReset(pContext);
	mapApi_.insert(pair<int, smart_ptr<Task::ITaskList> >(getSlgDef()->getScriptID("CASTER_TRAP"),smart_ptr<Task::ITaskList>(pTrap)));
}

} // namespace C_06 end
} // namespace SLG end
} // namespace BMW end
