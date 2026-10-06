#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "CSally_set_pers.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_set_pers::OnAction(Task::CTaskContext* pContext)
{
	// 設定対象の取得
	int nChara = pContext->getValue(Flag::TARGET_CHARA);
	// スタック
	int nSetChara = pContext->top();
	pContext->pop();

	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	CDataCharaSLG* pData = p->getCharaData(nChara);
	if(pData!=NULL) pData->addPers(nSetChara);
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end