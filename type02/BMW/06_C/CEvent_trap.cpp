#include "stdafx.h"

#include "../../Hero/IDHero.h"
#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CSLGDef.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Effect/CEffectMovieClip.h"
#include "../../SLG/Map/CMap.h"
#include "../../SLG/Map/CMapChip.h"

#include "CEvent_trap.h"

namespace BMW{
namespace SLG{
namespace C_06{

CEvent_trap::~CEvent_trap()
{
	for(int i=0; i<7; ++i)
		DELETE_SAFE(pEffect_[i]);
}

void CEvent_trap::OnReset(Task::CTaskContext* pContext)
{// エフェクトをSLGDefからゲット
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	for(int i=0; i<7; i++)
	{
		pEffect_[i]=p->getSLGDef().getEffect().createEffect("TRAP");
		pEffect_[i]->remove(true);
		pEffect_[i]->setX(32);
		pEffect_[i]->setY(16);
	}
}

void CEvent_trap::OnInit(Task::CTaskContext* pContext)
{// 味方のチップにエフェクトを仕掛ける
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	list<int>& List = p->getPlayerPhaseList();
	list<int>::iterator it;

	nPos_=0;
	Map::CMapChip*	pMap;
	CDataCharaSLG* pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = p->getCharaData(*it);
		if(pChara==NULL || !pChara->IsExist()) continue;
		pMap = p->getMapChip(pChara->getIndex());
		if(pMap==NULL) continue;
		pMap->addTask(pEffect_[nPos_],Map::CMapChip::EFFECT);
		// エフェクトを仕掛ける
		pEffect_[nPos_++]->OnReset(p);
	}
	setState(WAIT);

	// アニメ中のスキップはフラグ次第
	p->getApp()->animeSkip();

	// 主人公キャラを中心に
	int nChara = Chara::Const::charaID_.getValue(p->getApp()->getExec().getHero()==Hero::Target::TAKUMI 
												 ? "PLAYER_TAKUMI"
												 : "PLAYER_HARUNA");
	p->getMap()->scrollIndex(p->getCharaData(nChara)->getIndex());
}

void CEvent_trap::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case WAIT:
	{
		int i;
		for(i=0; i<nPos_; ++i)
		{
			if(!pEffect_[i]->IsEnd())
				break;
		}
		if(i>=nPos_)
		{// エフェクト終了
			pContext->getApp()->skip(true);
			setState(END);
		}
	}
	break;

	default:
		getTaskListCtrl()->returnTaskList();
	break;
	}
}


} // namespace C_06 end
} // namespace SLG end
} // namespace BMW end