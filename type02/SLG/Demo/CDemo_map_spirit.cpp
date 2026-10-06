#include "stdafx.h"

#include "../../Spirit/IDSpirit.h"
#include "../../Item/IDItem.h"
#include "../../Scene/GUI/CNumCtrl.h"
#include "../../Demo/CDemoMovieClip.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "../Effect/CEffectMovieClip.h"

#include "CDemo_map.h"
#include "CDemo_map_spirit.h"

namespace BMW{
namespace SLG{
namespace Demo{

CDemo_map_spirit::~CDemo_map_spirit()
{
	for(int i=0; i<SPIRIT_END; ++i)
		DELETE_SAFE(pSpiritEffect_[i]);
}

void CDemo_map_spirit::OnInit(Task::CTaskContext* pContext)
{// タスクの生成
	CDemo_map_base::OnInit(pContext);

	// 数字の取得
	pNum_ = pDemo_->getRightPanel()->getWidgetCast<GUI::CNumCtrl>("DAMAGE");

	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	Effect::CEffectDB& db = p->getEffectDB();
	// 精神エフェクト取得
	pSpiritEffect_[FIREBALL] = db.createEffect("FIREBALL");
	pSpiritEffect_[FIREBALL]->remove(true);
	pSpiritEffect_[SPIRIT] = db.createEffect("SPIRIT");
	pSpiritEffect_[SPIRIT]->remove(true);
	pSpiritEffect_[AVOID] = db.createEffect("AVOID");
	pSpiritEffect_[AVOID]->remove(true);
	pSpiritEffect_[TOUGH] = db.createEffect("TOUGH");
	pSpiritEffect_[TOUGH]->remove(true);
	pSpiritEffect_[DEFENCE] = db.createEffect("DEFENCE");
	pSpiritEffect_[DEFENCE]->remove(true);
	pSpiritEffect_[CONCENT] = db.createEffect("CONCENT");
	pSpiritEffect_[CONCENT]->remove(true);
	pSpiritEffect_[HIT] = db.createEffect("HIT");
	pSpiritEffect_[HIT]->remove(true);
	pSpiritEffect_[ACC] = db.createEffect("ACC");
	pSpiritEffect_[ACC]->remove(true);
	pSpiritEffect_[JUMP] = db.createEffect("JUMP");
	pSpiritEffect_[JUMP]->remove(true);
	pSpiritEffect_[AWAKE] = db.createEffect("AWAKE");
	pSpiritEffect_[AWAKE]->remove(true);
	pSpiritEffect_[GUTS] = db.createEffect("GUTS");
	pSpiritEffect_[GUTS]->remove(true);
	pSpiritEffect_[VERYGUTS] = db.createEffect("VERYGUTS");
	pSpiritEffect_[VERYGUTS]->remove(true);
	pSpiritEffect_[TRUST] = db.createEffect("TRUST");
	pSpiritEffect_[TRUST]->remove(true);
	pSpiritEffect_[FRIEND] = db.createEffect("FRIEND");
	pSpiritEffect_[FRIEND]->remove(true);
	pSpiritEffect_[SUPPLY] = db.createEffect("SUPPLY");
	pSpiritEffect_[SUPPLY]->remove(true);
	pSpiritEffect_[HOPE] = db.createEffect("HOPE");
	pSpiritEffect_[HOPE]->remove(true);
	pSpiritEffect_[POWER] = db.createEffect("POWER");
	pSpiritEffect_[POWER]->remove(true);
	pSpiritEffect_[ENCOURAGE] = db.createEffect("ENCOURAGE");
	pSpiritEffect_[ENCOURAGE]->remove(true);
	pSpiritEffect_[SNIPE] = db.createEffect("SNIPE");
	pSpiritEffect_[SNIPE]->remove(true);
	pSpiritEffect_[DIRECT] = db.createEffect("DIRECT");
	pSpiritEffect_[DIRECT]->remove(true);
	pSpiritEffect_[CHARGE] = db.createEffect("CHARGE");
	pSpiritEffect_[CHARGE]->remove(true);
	pSpiritEffect_[SPY] = db.createEffect("SPY");
	pSpiritEffect_[SPY]->remove(true);
	pSpiritEffect_[WEAK] = db.createEffect("WEAK");
	pSpiritEffect_[WEAK]->remove(true);
	pSpiritEffect_[EASYON] = db.createEffect("EASYON");
	pSpiritEffect_[EASYON]->remove(true);
	pSpiritEffect_[MIRACLE] = db.createEffect("MIRACLE");
	pSpiritEffect_[MIRACLE]->remove(true);
	pSpiritEffect_[FORTUNE] = db.createEffect("FORTUNE");
	pSpiritEffect_[FORTUNE]->remove(true);
	pSpiritEffect_[EFFORT] = db.createEffect("EFFORT");
	pSpiritEffect_[EFFORT]->remove(true);
	pSpiritEffect_[FAITH] = db.createEffect("FAITH");
	pSpiritEffect_[FAITH]->remove(true);
	pSpiritEffect_[PROVO] = db.createEffect("PROVO");
	pSpiritEffect_[PROVO]->remove(true);
}

void CDemo_map_spirit::OnReset(Task::CTaskContext* pContext)
{// 状況設定
	pDemo_->getLeftPanel()->valid(false);
	pDemo_->getLeftPanel()->visible(false);
	pDemo_->getRightPanel()->valid(true);
	pDemo_->getRightPanel()->visible(true);
	GUI::CPanel* pPanel = pDemo_->getRightPanel();
	
	// 状況設定
	// キャラスプライトゲット
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	CDataCharaSLG* pTarget = p->getTargetCharaData();
	DELETE_SAFE(pChara_);
	if(p->top()!=CDemo_map::SPIRIT)
	{
		pChara_ = pTarget->getMapSymbol()->createSymbolStrCast<BMW::Demo::CDemoMovieClip>("ITEM");
		pChara_->setDrawInfo(pDemo_->getChip(1)->getDrawInfo(false));
		pPanel->removeWidget("CHIP");
		pPanel->addWidget(pChara_,"CHIP");
	}

	pNum_->visible(false);

	switch(p->top())
	{
	case CDemo_map::SPIRIT:
	{// 精神IDに合わせて、エフェクトや数字を表示するかを設定する
		pDemo_->getRightHead()->validWidget("SPIRIT");
		const Chara::CStatusAbility& spirit = p->getCtrlCharaData()->getBattle().getSpirit(p->getTargetAbility());
		setSpiritEffect(spirit.getID(),pTarget);

		if(p->getApp()->getSpirit().getRange(spirit.getID())!=Spirit::Range::ENEMY)
			pChara_ = pTarget->getMapSymbol()->createSymbolStrCast<BMW::Demo::CDemoMovieClip>("ITEM");
		else // 敵にかける時はこっち
			pChara_ = pTarget->getMapSymbol()->createSymbolStrCast<BMW::Demo::CDemoMovieClip>("ITEM_COUNTER");

		pChara_->setDrawInfo(pDemo_->getChip(1)->getDrawInfo(false));
		pPanel->removeWidget("CHIP");
		pPanel->addWidget(pChara_,"CHIP");
	}
	break;

	case CDemo_map::ITEM:
	{// アイテムIDに合わせて、エフェクトや数字を表示するかを設定する
		pDemo_->getRightHead()->validWidget("ITEM");
		int nItem = p->getCtrlCharaData()->getBattle().hasItemAttr(p->getTargetAbility());
		setItemEffect(nItem,pTarget);
	}
	break;

	case CDemo_map::CURE: // 治癒
		// 回復量はダメージとして設定されている
		pDemo_->getRightHead()->validWidget("CUR");
		setCureEffect(-((p->getBattleData()->getBattleData(CDataBattle::ATTACK)).getAttack().getDamage()),
					  pTarget);
	break;

	default: // 補給
		pDemo_->getRightHead()->validWidget("PIT");
		setRefillEffect(pTarget->getBattle().getMaxEN()-pTarget->getBattle().getEN(),pTarget);
	break;

	}
	p->pop();

	// デモにエフェクトをセット
	pEffect_->OnReset(pContext);
	pEffect_->setDrawInfo(pDemo_->getChip(1)->getDrawInfo(false));
	pPanel->addWidget(pEffect_,"SERIF");

	// 動作の初期設定
	nFrame_=0;
	setState(EFFECT);
}

void CDemo_map_spirit::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EFFECT:
		if(pEffect_->IsEnd())
		{// エフェクトが終わったら、数字表示
			pNum_->visible(bNum_);
			setState(WAIT);
		}
	break;

	case WAIT:
		if(nFrame_++>15)
		{ 
			// 全終了したら
			// 入れ替えたエフェクトとキャラを戻す
			pDemo_->getRightPanel()->valid(false);
			pDemo_->getRightPanel()->visible(false);
			actionEnd(pContext);
			setState(NORMAL);
		}
	break;

	default: break;
	}
}

//////////////////////////////////////////////
// 精神エフェクト設定
//////////////////////////////////////////////
namespace{
const int anSpirit_[Spirit::SPIRIT_END]=
{// 精神IDと精神エフェクトIDとのテーブル
	CDemo_map_spirit::FIREBALL,
	CDemo_map_spirit::SPIRIT,
	CDemo_map_spirit::AVOID,
	CDemo_map_spirit::TOUGH,
	CDemo_map_spirit::DEFENCE,
	CDemo_map_spirit::CONCENT,
	CDemo_map_spirit::HIT,
	CDemo_map_spirit::HIT, // 感応
	CDemo_map_spirit::ACC,
	CDemo_map_spirit::JUMP,
	CDemo_map_spirit::AWAKE,
	CDemo_map_spirit::AWAKE, // 再動
	CDemo_map_spirit::GUTS,
	CDemo_map_spirit::VERYGUTS,
	CDemo_map_spirit::TRUST,
	CDemo_map_spirit::FRIEND,
	CDemo_map_spirit::SUPPLY,
	CDemo_map_spirit::HOPE,
	CDemo_map_spirit::POWER,
	CDemo_map_spirit::ENCOURAGE,
	CDemo_map_spirit::SNIPE,
	CDemo_map_spirit::DIRECT,
	CDemo_map_spirit::CHARGE,
	CDemo_map_spirit::SPY,
	CDemo_map_spirit::WEAK,
	CDemo_map_spirit::EASYON,
	CDemo_map_spirit::MIRACLE,
	CDemo_map_spirit::FORTUNE,
	CDemo_map_spirit::FORTUNE, // 祝福
	CDemo_map_spirit::EFFORT,
	CDemo_map_spirit::EFFORT,	// 応援
	CDemo_map_spirit::FAITH,
	CDemo_map_spirit::FAITH,	// 祈り
	CDemo_map_spirit::PROVO,
};
} // namespace end

void CDemo_map_spirit::setSpiritEffect(int nID, CDataCharaSLG* pTarget)
{// 精神IDに応じて、使用するエフェクトなどを設定する
	pEffect_ = pSpiritEffect_[anSpirit_[nID]];

	switch(nID)
	{// 精神によっては数字を使う
	case Spirit::TRUST:
	case Spirit::GUTS:
	{
		bNum_=true;
		int nNum = (pTarget->getBattle().getMaxHP()*3)/10;
		int nNum2 = pTarget->getBattle().getMaxHP()-pTarget->getBattle().getHP();
		if(nNum>nNum2) nNum=nNum2;
		pNum_->setNum(nNum);
		pNum_->validNumGui(2/*緑*/);
	}
	break;

	case Spirit::VERYGUTS:
	case Spirit::FRIEND:
	{
		bNum_=true;
		int nNum = pTarget->getBattle().getMaxHP()-pTarget->getBattle().getHP();
		pNum_->setNum(nNum);
		pNum_->validNumGui(2/*緑*/);
	}
	break;

	case Spirit::POWER:
	case Spirit::ENCOURAGE:
		bNum_=true;
		pNum_->setNum(10);
		pNum_->validNumGui(2/*緑*/);
	break;
	
	case Spirit::SUPPLY:
	{
		bNum_=true;
		int nNum = pTarget->getBattle().getMaxEN()-pTarget->getBattle().getEN();
		pNum_->setNum(nNum);
		pNum_->validNumGui(2/*緑*/);
	}
	break;

	case Spirit::HOPE:
		bNum_=true;
		pNum_->setNum(50);
		pNum_->validNumGui(2/*緑*/);
	break;

	case Spirit::WEAK:
		bNum_=true;
		pNum_->validNumGui(1/*赤*/);
		pNum_->setNum(-10);
	break;

	default: bNum_=false; break;
	}
}

//////////////////////////////////////////////
// アイテムエフェクト設定
//////////////////////////////////////////////
namespace{
const int anItem_[5]=
{// アイテムIDとアイテムエフェクトIDとのテーブル
	CDemo_map_spirit::TRUST,	// MEDI
	CDemo_map_spirit::SUPPLY,	// カレー
	CDemo_map_spirit::HOPE,		// 士郎
	CDemo_map_spirit::SUPPLY,	// マガジン
	CDemo_map_spirit::TRUST,	// マーボー
};
} // namespace end

void CDemo_map_spirit::setItemEffect(int nID, CDataCharaSLG* pTarget)
{// アイテムIDに応じて、使用するエフェクトなどを設定する
	pEffect_ = pSpiritEffect_[anItem_[nID]];

	int nNum=0;
	switch(nID)
	{// 精神によっては数字を使う
	case Item::MEDI:
	case Item::MABO:
		bNum_=true;
		nNum = pTarget->getBattle().getMaxHP()-pTarget->getBattle().getHP();
	break;

	case Item::CURRY:
		bNum_=true;
		nNum = pTarget->getBattle().getMaxEN()-pTarget->getBattle().getEN();
	break;

	case Item::SHIRO:
		bNum_=true;
		nNum = 50;
	break;

	default: bNum_=false; break;
	}

	pNum_->setNum(nNum);
	pNum_->validNumGui(2/*緑*/);
}

//////////////////////////////////////////////
// 治癒エフェクト設定
//////////////////////////////////////////////
void CDemo_map_spirit::setCureEffect(int nNum, CDataCharaSLG* pTarget)
{
	// 治癒のエフェクトは信頼と同じ
	pEffect_ = pSpiritEffect_[TRUST];
	bNum_=true;
	pNum_->validNumGui(2/*緑*/);
	pNum_->setNum(nNum);
}

//////////////////////////////////////////////
// 補給エフェクト設定
//////////////////////////////////////////////
void CDemo_map_spirit::setRefillEffect(int nNum, CDataCharaSLG* pTarget)
{
	// 治癒のエフェクトは補給と同じ
	pEffect_ = pSpiritEffect_[SUPPLY];
	bNum_=true;
	pNum_->validNumGui(2/*緑*/);
	pNum_->setNum(nNum);
}

} // namespace Demo end
} // namesapce SLG end
} // namespace BMW end