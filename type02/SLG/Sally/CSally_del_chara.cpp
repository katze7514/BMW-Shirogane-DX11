#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "CSally_del_chara.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_del_chara::OnAction(Task::CTaskContext* pContext)
{
	// 削除対象キャラ(SLG ID)は、TargetChara
	// SLGコンテキスト化
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// コンテキストのを呼び出す
	p->delCharaData(p->getTargetChara());
	// そして、戻る
	getTaskListCtrl()->returnTaskList();
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end