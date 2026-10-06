/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CMove_range.h"
#include "CMove_road.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_road::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
}

#pragma warning(disable:4701)

void CMove_road::OnAction(Task::CTaskContext* pContext)
{// 移動ルート計算
	const smart_ptr<Map::CMap>& pMap = p->getMap();

	// 移動しようとしてるキャラを取得
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	// ジャンプ値げっと
	// Move_rangeでも同じことやっている
	int nCharaJump = CMove_range::calcAbilityJump(pChara, p);
	
	// 各種前提条件設定
	Map::CMapChip *pCurrent, *pTarget, *pMove;

	pCurrent = p->getTargetMapChip();
	int nEnd = p->getCtrlCharaData()->getIndex();
	int nCurrentMove,nTargetMove,nMove,nCurrentHeight,nHeight,nWay,nStack;

	// 次のMOVE_EXECは、スタックトップから一個ずつ取り出し、
	// 移動していく。ちょうどこの計算と逆順になる
	// で、その終了として、一番下に移動先Indexを負にして積んでおく
	// 0が終了場所の場合は、INT_MINを積む
	p->push(pCurrent->getIndex()==0 ? INT_MIN : -pCurrent->getIndex());
	while(pCurrent!=NULL && pCurrent->getIndex()!=nEnd)
	{// 自分のMove値より、大きいMove値を持つMapChipを探す
	 // なお、基本的に差が最大値を通るように移動する。つまり、最短距離
	 // ただし、高さ最小の方が優先
		nMove=INT_MIN; nHeight=INT_MIN; pMove=NULL; nWay=-1;
		nCurrentMove = pCurrent->getMapChipState()->getMove();
		for(int i=Way::TOP; i<=Way::RIGHT; ++i)
		{
			pTarget=pMap->getMapChip(pCurrent->getMapInfo().getOnMap(i));
			if(pTarget==NULL){ continue; }
			nTargetMove = pTarget->getMapChipState()->getMove();
			if(nTargetMove > nCurrentMove)
			{// まずは、今の場所と対象位置の残り移動数を比較
				nCurrentHeight = abs(pTarget->getMapInfo().getHeight() - pCurrent->getMapInfo().getHeight());
				if(nCharaJump >= nCurrentHeight // とは言ってもJumpを越えてはいけない
				&& nTargetMove > nMove
				&& nCurrentHeight > nHeight)
				{// また、現在移動予定方向と比較
				 // 高さ最小の方が優先
				 // 今検索してる方が、より差が大きかったらそれに変更
					nHeight = nCurrentHeight;
					nMove = nTargetMove;
					pMove = pTarget;
					nWay=i;
				}
			}
		}
		// 移動方向をスタックに積む
		if(nWay!=-1)
		{
			if(nWay==Way::TOP){ nStack=Way::BOTTOM; }
			ef(nWay==Way::LEFT){ nStack=Way::RIGHT; }
			ef(nWay==Way::BOTTOM){ nStack=Way::TOP; }
			else/*(nWay==Way::RIGHT)*/{ nStack=Way::LEFT; }
			p->push(nStack);
		}
		// 次の検索へ
		pCurrent=pMove;
	}

	// 移動範囲のクリア
	//p->clearMove();
	// 計算が終了したら、戻る
	getTaskListCtrl()->returnTaskList();
}

#pragma warning(default:4701)

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
