/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "CCode_slg2chara.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_slg2chara::OnAction(Task::CTaskContext* pContext)
{
	// スタックトップに変換前SLG IDがある
	int nSlg = pContext->top();
	pContext->pop();
	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 取得
	CDataCharaSLG* pData = p->getCharaData(nSlg);
	if(pData==NULL) // 無かったら、-1をpush
		p->push(-1);
	else
		p->push(pData->getCharaID());
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
