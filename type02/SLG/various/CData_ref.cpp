#include "stdafx.h"

#include "../../Chara/CDataCharaTrain.h"

#include "../../Hero/IDHero.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Action/IAction.h"

#include "CData_ref.h"

namespace BMW{
namespace SLG{
namespace Data{

void CData_ref::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Save::CExecData& save = p->getApp()->getExec();

	Chara::CDataCharaTrain* pTrain;
	CDataCharaSLG* pChara;
	CSLGContext::chara_map& Map = p->getCharaMap();
	CSLGContext::chara_map::iterator it;
	for(it=Map.begin(); it!=Map.end(); ++it)
	{// キャラマップを回して、味方フェーズのやつをセーブデータに反映
	 // NonPlayerキャラはスルー
		pChara = it->second;
		if(pChara==NULL) continue;

		if(pChara->getPhase()==Phase::PLAYER
		&& !pChara->getAction()->IsNonPlayer())
		{// 養成データ取得
			pTrain = save.getTrainData(pChara->getCharaID());
			// 反映
			pTrain->back(pChara->getBattlePtr());
		}
	}
	// 主人公のLvを取得
	pChara = p->getCharaData(p->getApp()->getExec().getHero()==Hero::Target::HARUNA?"HARUNA":"TAKUMI");
	// 主人公のLvを反映
	if(pChara!=NULL) save.setLv(pChara->getBattle().getLv());

	// BP反映
	save.setBP(p->getBP());
	// FP反映
	save.setFP(p->getFP());

	// 熟練度関係
	if(p->top()==Victory::VICTORY)
	{// VITORYフラグ
		// 熟練度取ってたら1取れてなかったら0
		if(save.getExpert() < p->getExpert())
			save.setFlag("VICTORY",1);
		else
			save.setFlag("VICTORY",0);
		// 熟練度反映
		save.setExpert(p->getExpert());
	}

	// 総ターン数追加
	save.calcTurn(p->getTurn());
	// 取得アイテム
	for(list<int>::iterator it=p->getGetItemList().begin(); it!=p->getGetItemList().end(); ++it)
		save.incItem(*it);
}

} // namespace Data end
} // namespace SLG end
} // namespace BMW end