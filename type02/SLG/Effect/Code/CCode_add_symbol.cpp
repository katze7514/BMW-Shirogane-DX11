#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Context/CDataCharaSLG.h"
#include "../../Context/CSLGDef.h"
#include "../../Map/CMap.h"
#include "../../Map/CMapChip.h"
#include "../../Event/CEvent.h"

#include "../CEffectInfo.h"
#include "CCode_add_symbol.h"

namespace BMW{
namespace SLG{
namespace Effect{

//////////////////////////////////////////////////
// イベントレイヤー対象
//////////////////////////////////////////////////
__inline static int getPriEvent(Event::CEvent* pEvent)
{
	int nPri=Event::CEvent::EVENT;	
	while(pEvent->getTask(nPri)!=NULL) nPri++;
	return nPri;
}

void CCode_add_event_symbol::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// No
	int nNo = p->top();
	p->pop();
	// 対象
	int nTarget = p->top();
	p->pop();
	
	Event::CEvent* pEvent = p->getEvent().getPointer();
	// シンボルを取得
	CEffectInfo* pInfo = pEvent->getEffect(nNo);
	pInfo->pEffect_->OnReset(pContext);
	// 投入
	pInfo->pList_=pEvent;
	pInfo->pList_->addTask(pInfo->pEffect_, getPriEvent(pEvent));

	int nX=0, nY=0;

	// キャラだったら、そいつかがいるINDEXを取得
	if(nTarget!=EVENT)
	{// 指定されたマップを親にする
		int nIndex;
		if(nTarget==EVENT_CHARA)
		{// 指定したキャラがいるマップ
			CDataCharaSLG* pChara = p->getCharaData(p->top());
			if(pChara==NULL)
			{// キャラがいなかったら
				// 後始末して
				p->pop();
				p->pop();
				p->pop();
				return; // 終了
			}
			
			nIndex = pChara->getIndex();
		}
		else
		{// マップ直接指定
			nIndex = p->top();
		}
		// 投入先マップチップ
		Map::CMapChip* pChip = p->getMapChip(nIndex);
		if(pChip==NULL)
		{// 存在しないマップだったら
			// 後始末して
			p->pop();
			p->pop();
			p->pop();
			return; // 終了
		}
		pInfo->pEffect_->setParent(smart_ptr<Task::ITaskBase>(pChip,false));
		p->pop();
	}

	nX += p->top();
	p->pop();

	nY += p->top();
	p->pop();

	// 座標設定
	pInfo->pEffect_->setX(nX);
	pInfo->pEffect_->setY(nY);
}

//////////////////////////////////////////////////
// マップレイヤー対象
//////////////////////////////////////////////////
__inline static int getPriMap(Map::CMapChip* pMap)
{
	int nPri=Map::CMapChip::EFFECT;	
	while(pMap->getTask(nPri)!=NULL) nPri++;
	return nPri;
}

void CCode_add_map_symbol::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// No
	int nNo = p->top();
	p->pop();
	// 対象
	int nTarget = p->top();
	p->pop();

	Event::CEvent* pEvent = p->getEvent().getPointer();
	// シンボルを取得
	CEffectInfo* pInfo = pEvent->getEffect(nNo);
	pInfo->pEffect_->OnReset(pContext);

	// index
	int nIndex = p->top();
	p->pop();

	// X
	int nX = p->top();
	p->pop();
	// Y
	int nY = p->top();
	p->pop();

	if(nTarget==MAP_CHARA)
	{// キャラのいるマップを取得
		CDataCharaSLG* pChara = p->getCharaData(nIndex);
		if(pChara==NULL) return;
		nIndex = pChara->getIndex();
	}
	// マップチップ取得
	Map::CMapChip* pChip = p->getMapChip(nIndex);
	if(pChip==NULL) return;
	// 座標設定
	pInfo->pEffect_->setX(nX);
	pInfo->pEffect_->setY(nY);
	// 投入
	pInfo->pList_=pChip;
	pInfo->pList_->addTask(pInfo->pEffect_, getPriMap(pChip));
}

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end