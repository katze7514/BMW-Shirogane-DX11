#include "stdafx.h"

#include "CCode_ctrl_item.h"

namespace BMW{
namespace Scene{
namespace Code{

void CCode_ctrl_item::OnAction(Task::CTaskContext* pContext)
{
	// まずは、増減数
	int nCalc = pContext->top();
	pContext->pop();
	// アイテムID
	int nItem = pContext->top();
	pContext->pop();

	map<int, int>& mapItem = pContext->getApp()->getExec().getItemMap();

	map<int, int>::iterator it = mapItem.find(nItem);
	if(it==mapItem.end())
	{// 見つからなかったら、新規に追加する
		if(nCalc>0)
			mapItem.insert(pair<int,int>(nItem,nCalc));
	}
	else
	{// あったら、増減させる
		int nValue = it->second + nCalc;
		// とりあえず、消しちゃう
		mapItem.erase(it);
		if(nValue>0)
		{// 個数が残ってれば、改めて追加
			mapItem.insert(pair<int,int>(nItem,nValue));
		}
	}
}

} // namespace Code end
} // namespace Scene end
} // namespace BMW end