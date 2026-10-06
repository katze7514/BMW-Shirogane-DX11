#include "stdafx.h"

#include "../IDSLG.h"

#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"

#include "CVictory_check.h"

namespace BMW{
namespace SLG{
namespace Victory{

bool CVictory_check::IsDeath(int nID, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(nID);
	if(pChara==NULL) return true;
	return !pChara->IsExist();
}

bool CVictory_check::IsDeath(const string& sID, CSLGContext& context)
{
	return IsDeath(context.getSLGDef().getSlgID(sID),context);
}

bool CVictory_check::IsDeathPhase(int nPhase, CSLGContext& context)
{// nPhaseのキャラ群が全滅してるか？
	
	switch(nPhase)
	{
	case Phase::ENEMY:	return IsDeathList(context.getEnemyPhaseList(),context);
	case Phase::NEUTRAL:return IsDeathList(context.getNeutralPhaseList(),context);
	default:			return IsDeathList(context.getPlayerPhaseList(),context);
	}
}


bool CVictory_check::IsDeathPhaseOne(int nPhase, CSLGContext& context)
{// nPhaseのキャラが一人でも死んでるか？
	switch(nPhase)
	{
	case Phase::ENEMY:	return IsDeathList(context.getEnemyPhaseList(),context,false);
	case Phase::NEUTRAL:return IsDeathList(context.getNeutralPhaseList(),context,false);
	default:			return !context.getDeathSet().empty() 
							/*|| IsDeathList(context.getPlayerPhaseList(),context,false)*/;
	}
}

bool CVictory_check::IsDeathList(list<int>& List, CSLGContext& context, bool bAll)
{// IsDeathPhaseの下請け
	int nNull=0;
	list<int>::iterator it;
	CDataCharaSLG* pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = context.getCharaData(*it);
		if(pChara==NULL){ ++nNull; continue; }
		if(bAll)
		{// 全滅してるとtrue
			if(pChara->IsExist()) return false;
		}
		else
		{// 一人でも死んでるとtrue
			if(!pChara->IsExist()) return true;
		}
	}

	// 一人でもで、全部NULLだった時はtrueにする
	return bAll || (int)List.size()==nNull;
}

bool CVictory_check::IsDeathPhase(int nPhase, CSLGContext& context, set<int>& setChara)
{
	list<int>& List = context.getPhaseList(nPhase);
	list<int>::iterator it;
	CDataCharaSLG* pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = context.getCharaData(*it);
		if(pChara==NULL) continue;
		// 一人でも生きてたらfalse
		// setCharaに入ってるやつは生きててOK
		if(setChara.find(*it)==setChara.end()
		&& pChara->IsExist()) return false;
	}

	// 全部NULLだった時はtrueにする
	return true;
}

bool CVictory_check::IsAlive(int nID, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(nID);
	if(pChara==NULL) return false;
	return pChara->IsExist();
}

bool CVictory_check::IsAlive(const string& sID, CSLGContext& context)
{
	return IsAlive(context.getSLGDef().getSlgID(sID),context);
}

bool CVictory_check::IsAlivePhase(int nPhase, CSLGContext& context)
{// nPhaseのキャラ群が全員生きてるか？
	
	switch(nPhase)
	{
	case Phase::ENEMY:	return IsAliveList(context.getEnemyPhaseList(),context);
	case Phase::NEUTRAL:return IsAliveList(context.getNeutralPhaseList(),context);
	default:			return context.getDeathSet().empty();
	}
}


bool CVictory_check::IsAlivePhaseOne(int nPhase, CSLGContext& context)
{// nPhaseのキャラが一人でも生きているか？
	switch(nPhase)
	{
	case Phase::ENEMY:	return IsAliveList(context.getEnemyPhaseList(),context,false);
	case Phase::NEUTRAL:return IsAliveList(context.getNeutralPhaseList(),context,false);
	default:			return IsAliveList(context.getPlayerPhaseList(),context,false);
	}
}

bool CVictory_check::IsAliveList(list<int>& List, CSLGContext& context, bool bAll)
{// IsAlivePhaseの下請け
	int nNull=0;
	list<int>::iterator it;
	CDataCharaSLG* pChara;
	for(it=List.begin(); it!=List.end(); ++it)
	{
		pChara = context.getCharaData(*it);
		if(pChara==NULL){ ++nNull; continue; }
		if(bAll)
		{// 一人でも死んでると
			if(!pChara->IsExist()) return false;
		}
		else
		{// 一人でも生きてれば
			if(pChara->IsExist()) return true;
		}
	}

	// 一人でもで、全部NULLだった時はfalseにする
	return bAll;
}

} // namespace Victory end
} // namespace SLG end
} // namespace BMW end