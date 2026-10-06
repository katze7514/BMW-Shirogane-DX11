#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CAttack_road.h"

namespace BMW{
namespace SLG{
namespace Attack{

void CAttack_road::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Map::CMapChip *pCurrent, *pTarget, *pAttack;
	const smart_ptr<Map::CMap>& pMap = p->getMap();
	pCurrent = p->getTargetMapChip();
	int nEnd = p->getCtrlCharaData()->getIndex();
	int nHeight = pMap->getMapChip(nEnd)->getMapInfo().getHeight();
	int nCurrentAttack,nTargetAttack,nAttack;
	int nMin=0,nMax=0;

	while(pCurrent!=NULL && pCurrent->getIndex()!=nEnd)
	{// 自分のAttack値より、小さいAttack値を持つMapChipを探す
	 // なお、基本的に差が最大値を通るように移動する。つまり、最短距離
		nAttack=INT_MAX; pAttack=NULL;
		nCurrentAttack = pCurrent->getMapChipState()->getAttack();
		for(int i=Way::TOP; i<=Way::RIGHT; ++i)
		{
			pTarget=pMap->getMapChip(pCurrent->getMapInfo().getOnMap(i));
			if(pTarget==NULL){ continue; }
			nTargetAttack = pTarget->getMapChipState()->getAttack();
			if(nTargetAttack>=0 && nCurrentAttack > nTargetAttack)
			{// まずは、今の場所と対象位置の残り移動数を比較
				if(nTargetAttack < nAttack)
				{// また、現在移動予定方向と比較
				 // 今検索してる方が、より差が大きかったらそれに変更
					nAttack = nTargetAttack;
					pAttack = pTarget;
				}
			}
		}
		if(pAttack!=NULL)
		{// 攻撃方向が決まっていたら高さの差比較
			int n = pAttack->getMapInfo().getHeight() - nHeight;
			if(n < nMin) nMin=n;
			if(n > nMax) nMax=n;
		}
		// 次の検索へ
		pCurrent=pAttack;
	}

	// 最小・最大の順
	p->push(nMin);
	p->push(nMax);
	// 計算が終了したら、戻る
//	getTaskListCtrl()->returnTaskList();
}

} // namespace Attack end
} // namespace SLG end
} // mamespace BMW end