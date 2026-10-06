#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara2.h"

#include "../Effect/CEffectMovieClip.h"

#include "CSally_del_chara_map.h"

namespace BMW{
namespace SLG{
namespace Sally{

CSally_del_chara_map::~CSally_del_chara_map()
{
	DELETE_SAFE(pEffectCtrl_);
}

void CSally_del_chara_map::OnReset(Task::CTaskContext* pContext)
{
	pEffectCtrl_ = new Task::CTaskCtrl<Effect::CEffectMovieClip>();
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Effect::CEffectMovieClip* pEffect = p->getEffectDB().createEffect("BOM");
	pEffect->setX(32);
	pEffect->setY(16);
	pEffectCtrl_->addTask(pEffect,DEATH);
	naVisibleFrame[DEATH]=3;

	pEffect = p->getEffectDB().createEffect("EXIT");
	pEffect->setX(32);
	pEffect->setY(16);
	pEffectCtrl_->addTask(pEffect,REMOVE);
	naVisibleFrame[REMOVE]=2;

	pEffectCtrl_->setState(0);
}

void CSally_del_chara_map::OnInit(Task::CTaskContext* pContext)
{
	Effect::CEffectMovieClip* pEffect = pEffectCtrl_->getTask(DEATH);
	pEffect->OnReset(pContext);
	pEffect->valid(false);
	pEffect->visible(false);

	pEffect = pEffectCtrl_->getTask(REMOVE);
	pEffect->OnReset(pContext);
	pEffect->valid(false);
	pEffect->visible(false);

	delChara(pContext);
	// アニメ中のスキップはフラグ次第
	pContext->getApp()->animeSkip();
}

void CSally_del_chara_map::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EFFECT:
		if((pEffectCtrl_->getCurrentTask())->getTask(0)->getState()
			== naVisibleFrame[pEffectCtrl_->getState()])
		{// キャラスプライト非表示
			pSprite_->visible(false);
		}
		ef(pEffectCtrl_->getCurrentTask()->IsEnd())
		{
			setState(END);
		}
	break;

	case END:
		// マップチップから取り除く
		if(pMap_!=NULL) pMap_->killTask(Map::CMapChip::CHARA);
		// 削除後は対象が無効になる
		pContext->setValue(-1,Flag::TARGET_CHARA);
		pContext->setValue(-1,Flag::TARGET_MAP);

		getTaskListCtrl()->returnTaskList();

		// マップはフレームスキップ
		pContext->getApp()->skip(true);
	break;
	}
}

void CSally_del_chara_map::delChara(Task::CTaskContext* pContext)
{
	pMap_=NULL;
	// 削除エフェクトIDがスタックトップにある
	int nEffect = pContext->top();
	pContext->pop();

	// SLGコンテキスト化
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

#ifdef BMW_DEBUG
	CDbg().Out("DELMAP %d %d", p->getTargetChara(), nEffect);
#endif

	// 消す対象は、TargetCharaに
	CDataCharaSLG* pChara = p->getTargetCharaData();
	if(pChara==NULL){ setState(END); return; }

	// キャラがいるMapChipを取得
	pMap_ = p->getMapChip(pChara->getIndex());
	if(pMap_==NULL){ setState(END); return; }

	// マップキャラスプライト取得
	Map::CMapChipChara2* pChip = static_cast<Map::CMapChipChara2*>(pMap_->getTask(Map::CMapChip::CHARA));
	if(pChip==NULL){ setState(END); return; }
	pSprite_ = pChip->getTask(Map::CMapChipChara2::CHARA);

	// 退場箇所を画面の中心に
	p->getMap()->scrollIndex(pChara->getIndex());

	// エフェクト設定
	if(nEffect>=0)
	{
		pEffectCtrl_->setState(nEffect);
		Effect::CEffectMovieClip* pEffect = pEffectCtrl_->getTask(nEffect);
		pEffect->setParent(smart_ptr<Task::ITaskBase>(pChip,false));
		pEffect->valid(true);
		pEffect->visible(true);
		setState(EFFECT);
	}
	else
	{
		setState(END);
	}

	// マップ上に存在しなくなるので
	pChara->setIndex(-1);
}

void CSally_del_chara_map::callTaskAction(Task::CTaskContext* pContext)
{
	pEffectCtrl_->Task(pContext);
}

void CSally_del_chara_map::callTaskDraw(Task::CTaskContext* pContext)
{
	pEffectCtrl_->Task(pContext);
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end