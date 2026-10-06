/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CCode_chara2slg.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_chara2slg::OnAction(Task::CTaskContext* pContext)
{
	// スタックトップに変換前のキャラIDが、積まれている
	int nChara = pContext->top();
	pContext->pop();
	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 検索結果をpushする
	 p->push(p->searchSlg(nChara));
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
