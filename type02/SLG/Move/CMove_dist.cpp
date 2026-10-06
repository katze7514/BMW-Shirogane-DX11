#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CMove_dist.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_dist::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
}

void CMove_dist::OnAction(Task::CTaskContext* pContext)
{
	// 計算する最大移動力
	nMaxMove_ = p->top();
	p->pop();
	// ジャンプ
	nJump_ = p->top();
	p->pop();
	// エンドインデックス
	nEnd_ = p->top();
	p->pop();
	// スタートインデックス
	int nStart = p->top();
	p->pop();
	// 距離初期化
	nDist_=-1;

	// 計算
	calcDist(p->getMapChip(nStart),0);

	// 計算終了したら、スタックに結果を積む
	p->push(nDist_);

	getTaskListCtrl()->returnTaskList();
}

void CMove_dist::setResult(int nDist)
{
	if(nDist_<0 || nDist_>nDist) nDist_=nDist;
}
///////////////////////////////////////////////////////
// 計算
///////////////////////////////////////////////////////
void CMove_dist::calcDist(Map::CMapChip* pMap,int nDist)
{
	// 移動力が最大値を越えたら終了
	if(nMaxMove_>=0 && nDist>nMaxMove_) return;
	// 走査するマップがなくても終了
	if(pMap==NULL) return;

	// マップデータを取得
	int nMapDist = pMap->getMapChipState()->getDist();
	if(nMapDist>=0)
	{
		if(nDist>=nMapDist)
		{// 現在のDistがマップに設定されてるDistより大きかったら
		 // これ以上の探索は無意味なので、終了
			//CDbg().Out("CalcDist %d %d %d %d",getEndIndex(),pMap->getIndex(),pMap->getMapChipState()->getDist(),nDist);
			if(pMap->getIndex()==getEndIndex())
			{// ただ、この場所が目的の場所かもしれない 
				setResult(pMap->getMapChipState()->getDist());
			}
			return;
		}
		else
		{// ここまで来た距離が小さかったら、上書き
			pMap->getMapChipState()->setDist(nDist);
		}
	}
	else
	{// 検索されてない上書き
		pMap->getMapChipState()->setDist(nDist);
	}

	// 目的地に到達
	if(pMap->getIndex()==getEndIndex()){ setResult(pMap->getMapChipState()->getDist()); return; }

	// 上へ行けるか
	Map::CMapChip* pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::TOP));
	switch(IsDist(pOn,pMap->getMapInfo().getHeight()))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcDist(pOn,nDist + pOn->getMapInfo().getMove());
	break;
	default: break;
	}

	// 左へいけるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::LEFT));
	switch(IsDist(pOn,pMap->getMapInfo().getHeight()))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcDist(pOn,nDist + pOn->getMapInfo().getMove());
	break;
	default: break;
	}

	// 下へ行けるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::BOTTOM));
	switch(IsDist(pOn,pMap->getMapInfo().getHeight()))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcDist(pOn,nDist + pOn->getMapInfo().getMove());
	break;
	default: break;
	}
	
	// 右へいけるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::RIGHT));
	switch(IsDist(pOn,pMap->getMapInfo().getHeight()))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcDist(pOn,nDist + pOn->getMapInfo().getMove());
	break;
	default: break;
	}
}

int CMove_dist::IsDist(Map::CMapChip* pMap, int nCurrentHeight)
{
	if(pMap==NULL) return NOTENABLE;
	// 単純に移動可能か？
	if(abs(pMap->getMapInfo().getHeight() - nCurrentHeight) > nJump_)
		return NOTENABLE; // できない・・・

	// 目的の場所だったらOKにする
	if(pMap->getIndex()==getEndIndex()) return ENABLE;

	// マップの上に何かある？
	ITaskBase* pBase = pMap->getTask(Map::CMapChip::CHARA);
	if(pBase!=NULL)
	{// キャラがいる
		int nID = static_cast<Map::CMapChipChara2*>(pBase)->getID();
		if(nPhase_==p->getCharaData(nID)->getPhase())
		{// 仲間だったら通り抜けは可
			return ENABLE;
		}
		else
		{// 仲間じゃなかったら、ダメ
			return NOTENABLE;
		}
	}
	else
	{// 移動可能
		return ENABLE;
	}
}

} // namespace Move end
} // namespace SLG end
} // namespace BMW end