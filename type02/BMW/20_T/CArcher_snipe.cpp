#include "stdafx.h"

#include "../../SLG/Context/CSLGContext.h"
#include "../../SLG/Context/CSLGDef.h"
#include "../../SLG/Context/CDataCharaSLG.h"
#include "../../SLG/Map/CMap.h"
#include "../../SLG/Map/CMapChip.h"
#include "../../SLG/Map/CMapChipChara2.h"
#include "../../SLG/Effect/CEffectMovieClip.h"
#include "../../SLG/Event/CEvent.h"
#include "../../SLG/Phase/CPhaseBall.h"

#include "CArcher_snipe.h"

namespace BMW{
namespace SLG{
namespace T_20{

CArcher_snipe::~CArcher_snipe()
{
	DELETE_SAFE(pEffect_);
}


void CArcher_snipe::Task(Task::CTaskContext* pContext)
{
	pEffect_->Task(pContext);

	if(pContext->IsAction()) OnAction(pContext);
}

void CArcher_snipe::OnReset(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// エフェクトの取得
	// とりあえずは、爆発
	pEffect_ = p->getSLGDef().getEffect().createEffect("ARCHER_SNIPE");
	// フェーズボール消し
	p->getEvent()->getTurnBall().intro(false);
}

void CArcher_snipe::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 狙撃するキャラを選択
	list<int>& phaseList = p->getPlayerPhaseList();
	CDataCharaSLG* pChara;

	int nTarget;
	list<int>::iterator it;
	int i;
	do{
		nTarget = CApp::rand_.Get((int)phaseList.size());
		i = 0;
		for(it=phaseList.begin(); i!=nTarget; ++it, ++i);
		pChara = p->getCharaData(*it);
	}
	while(pChara==NULL
		|| !pChara->IsExist());// そのキャラが無効だったら選び直し

	// いたら、ダメージ
	// とりあえず、残HPの2割ダメージで
	Chara::CDataCharaBattle& battle = pChara->getBattle();
	battle.calcHP(battle.getHP()/20);
	// 10は残る
	if(battle.getHP()<=10) battle.calcHP(-battle.getMaxHP()+10);

	// エフェクト箇所を画面の中心に
	p->getMap()->scrollIndex(pChara->getIndex());

	// エフェクト設定
	pEffect_->valid(true);
	pEffect_->visible(true);
	pEffect_->OnReset(p);

	// アニメ中のスキップはフラグ次第
	p->getApp()->animeSkip();
}

void CArcher_snipe::OnAction(Task::CTaskContext* pContext)
{
	if(pEffect_->IsEnd())
	{// エフェクト終了したらリターン
		getTaskListCtrl()->returnTaskList();
		// マップはフレームスキップ
		pContext->getApp()->skip(true);
		static_cast<CSLGContext*>(pContext)->getEvent()->getTurnBall().intro(true);
	}
}

} // namespace T_20 end
} // namespace SLG end
} // namespace BMW end