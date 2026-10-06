#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../Scene/GUI/CNumCtrl.h"

#include "../IDSLG.h"

#include "../Map/CMap.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "../Effect/CEffectMovieClip.h"

#include "CDemo_map.h"
#include "CDemo_map_battle.h"

namespace BMW{
namespace SLG{
namespace Demo{

CDemo_map_battle::~CDemo_map_battle()
{
	for(int i=0; i<EFFECT_END; ++i)
		DELETE_SAFE(pEffect_[i]);
}

void CDemo_map_battle::OnInit(Task::CTaskContext* pContext)
{
	CDemo_map_base::OnInit(pContext);

	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	battle = p->getBattleData();

	Effect::CEffectDB& db = p->getEffectDB();
	// この時に使うエフェクトを取得
	pEffect_[HIT]=db.createEffect("BASHI");
	pEffect_[AVOID_L]=db.createEffect("MISS_L");
	pEffect_[AVOID_R]=db.createEffect("MISS_R");
	pEffect_[DEF]=db.createEffect("DEF");
	pEffect_[CT]=db.createEffect("CRITY");
	pEffect_[BACKUP_ATK]=db.createEffect("ENGO_ATK");
	pEffect_[BACKUP_DEF]=db.createEffect("ENGO_DEF");
	pEffect_[BUNSHIN]=db.createEffect("BUNSHIN");
	pEffect_[COUNTER_E]=db.createEffect("COUNTER");
	pEffect_[BARRIER_L]=db.createEffect("BARRIER_L");
	pEffect_[BARRIER_R]=db.createEffect("BARRIER_R");
	pEffect_[STATUS_UP_L]=db.createEffect("STATUS_UP_L");
	pEffect_[STATUS_UP_R]=db.createEffect("STATUS_UP_R");
	pEffect_[DOKURO_L]=db.createEffect("CONDITION_L");
	pEffect_[DOKURO_R]=db.createEffect("CONDITION_R");
}

void CDemo_map_battle::OnReset(Task::CTaskContext* pContext)
{
	// 攻撃側のマップ兵器がどうかでエフェクト分岐
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// マップ兵器イベントは、デモOFFにしないこと
	// 敵キャラ設定の構文用意してないからｗ
	const smart_ptr<Weapon::CDataWeaponBattle>& pWeapon = p->getBattleData()->getBattleData(CDataBattle::ATTACK).getAttack().getWeaponData();
	if(pWeapon->IsF())
		initMap(pWeapon,p);
	else
		initNormal(p);
}

////////////////////////////////////////////////////
// ムービーデータ設定
////////////////////////////////////////////////////
////////////////////////////////////////////////////
// MAP武器モード設定
////////////////////////////////////////////////////
void CDemo_map_battle::initMap(const smart_ptr<Weapon::CDataWeaponBattle>& pWeapon, CSLGContext* p)
{
	// 現在の対象を取得する
	CDataBattleMap::mapdef_list::iterator mit = p->getBattleMap().currentMapDef();
	smart_ptr<CDataCharaSLG> pChara(mit->getChara(),false);
	// 対象位置にスクロール
	p->getMap()->scrollIndex(pChara->getIndex());
	// デモ対象によって出すパネルの切り替え
	bool bLeft = pChara->getPhase()!=Phase::PLAYER;
	pDemo_->getLeftPanel()->valid(bLeft);
	pDemo_->getLeftPanel()->visible(bLeft);
	pDemo_->getRightPanel()->valid(!bLeft);
	pDemo_->getRightPanel()->visible(!bLeft);
	if(!bLeft) pDemo_->getRightPanel()->getWidgetCast<GUI::CNumCtrl>("DAMAGE")->validNumGui(1/*赤*/);

	resetChara();
	// 戦闘デモを構築するでー
	// 一端、リストクリア
	listMovie_.clear();
	// まずは、登場
	listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CHANGE));
	// キャラデフォロード
	loadChara(ATTACK,CHARA_DEFAULT,pChara,bLeft);
	// ちょっとWAIT
	listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,WAIT_E,5));

	// CTエフェクト
	if(mit->IsCT()) listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CT));

	// 通常の行動
	DEMO_MOVIE movie;

	movie.bLeft_=bLeft;
	movie.nBattle_=ATTACK;
	
	// 防御行動によって再生ムービーが変化
	// 対MAP兵器は回避するか当たるかの二択
	if(pWeapon->getKind()==Weapon::Kind::STATUS)
	{// ステータス武器
		movie.nChara_=CHARA_DEFAULT;
		movie.nEffect_= movie.bLeft_ ? STATUS_UP_L : STATUS_UP_R;
	}
	ef(!mit->IsHit())
	{// 回避！
		movie.nChara_=CHARA_AVOID;
		if(mit->IsAlterEgo()) // 分身発動？
			movie.nEffect_=BUNSHIN;
		else
			movie.nEffect_=movie.bLeft_ ? AVOID_L : AVOID_R;
	}
	else
	{// 当たった！
		movie.nChara_=CHARA_DAMAGE;
		movie.nEffect_=HIT;

		// バリア発動してたら、エフェクト上書き
		if(mit->IsBarriar())
		{
			movie.nChara_=CHARA_DEF;
			movie.nEffect_=movie.bLeft_ ? BARRIER_L : BARRIER_R;
		}
	}

	// 最後にリストに追加
	listMovie_.push_back(movie);

	// ちょっと数字見せるためWAIT
	// 状態変化武器での攻撃だった？
	if((pWeapon->getKind()==Weapon::Kind::FIGHT_COND || pWeapon->getKind()==Weapon::Kind::MAGIC_COND)
	&& mit->getDamage()>0) // そうだったら、ドクロがつくでｗ
		listMovie_.push_back(DEMO_MOVIE(movie.bLeft_,movie.nBattle_,CHARA_DEFAULT,movie.bLeft_?DOKURO_L:DOKURO_R,20,mit->getDamage()));
	else
		listMovie_.push_back(DEMO_MOVIE(movie.bLeft_,movie.nBattle_,CHARA_DEFAULT,WAIT_E,10,mit->getDamage()));

	loadChara(ATTACK,movie.nChara_,pChara,bLeft);

	// スタート
	it=listMovie_.begin();
	actionNext(p);
}
////////////////////////////////////////////////////
// 通常武器モード設定
////////////////////////////////////////////////////
void CDemo_map_battle::initNormal(Task::CTaskContext* pContext)
{
	pDemo_->getLeftPanel()->valid(true);
	pDemo_->getLeftPanel()->visible(true);
	pDemo_->getRightPanel()->valid(true);
	pDemo_->getRightPanel()->visible(true);
	pDemo_->getRightPanel()->getWidgetCast<GUI::CNumCtrl>("DAMAGE")->validNumGui(1/*赤*/);

	resetChara();
	// 戦闘データ
	// 攻撃と反撃の設定
	CDataBattleBase& attack = battle->getBattleData(CDataBattle::ATTACK);
	CDataBattleBase& counter = battle->getBattleData(CDataBattle::COUNTER);
	CDataBattleBase& attackBack = battle->getBattleData(CDataBattle::ATTACK_BACK);
	CDataBattleBase& counterBack = battle->getBattleData(CDataBattle::COUNTER_BACK);

	// まずは、攻撃パターン判定
	// 反撃あり？
	bool bCounter = attack.getDefence().getAction()!=Battle::NO;
	// カウンター発動？
	bool bCounterA = bCounter ? counter.getAttack().IsAbility(Ability::COUNTER) : false;
	// 援護防御は？
	bool bBackDef = counterBack.getDefence().getAction()!=Battle::NO;
	// 援護攻撃は？
	bool bBackAtk = attackBack.getDefence().getAction()!=Battle::NO;
	// 攻撃側はどっち？
	bool bLeft = attack.getChara()->getPhase()!=Phase::PLAYER;

	// 戦闘デモを構築するでー
	// 一端、リストクリア
	listMovie_.clear();
	// まずは、両方登場
	listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CHANGE));
	listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_DEFAULT,CHANGE));
	// キャラデフォロード
	loadChara(ATTACK,CHARA_DEFAULT,attack.getChara(),bLeft);
	loadChara(COUNTER,CHARA_DEFAULT,counter.getChara(),!bLeft);
	// ちょっとWAIT
	listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,WAIT_E,5));

	// カウンター技能発動？
	if(!bCounterA)
	{// 発動してない
		// じゃ、攻撃
		// 前半
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_ATK));
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CHANGE));
		loadChara(ATTACK,CHARA_ATK,attack.getChara(),bLeft);

		// 後半
		setDefBack(bBackDef,bLeft,attack,counter,counterBack);
		
		// 反撃はある？
		if(bCounter
		&& (bBackDef || !attack.getAttack().IsDeath()))
			setCounter(bLeft,attack,counter);
	}
	else
	{// ある
		listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_DEFAULT,COUNTER_E));
		// じゃ、反撃側から
		setCounter(bLeft,attack,counter);

		// 攻撃側
		if(!counter.getAttack().IsDeath())
		{// 生きてれば
			// 前半
			listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_ATK));
			listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CHANGE));
			loadChara(ATTACK,CHARA_ATK,attack.getChara(),bLeft);

			// 後半
			setDefBack(bBackDef,bLeft,attack,counter,counterBack);
		}
	}

	// 援護攻撃ある？
	if((bBackDef || !attack.getAttack().IsDeath())
	&& !counter.getAttack().IsDeath()
	&& bBackAtk)
	{// あるじゃ、入れ替え
		//listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK_B,CHARA_DEFAULT,CHANGE));
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK_B,CHARA_DEFAULT,BACKUP_ATK));
		loadChara(ATTACK_B,CHARA_DEFAULT,attackBack.getChara(),bLeft);
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK_B,CHARA_ATK));
		loadChara(ATTACK_B,CHARA_ATK,attackBack.getChara(),bLeft);
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK_B,CHARA_DEFAULT,CHANGE));
		// で、攻撃を受ける
		int nChara = setAtkDefMovie(bLeft,
									ATTACK_B,COUNTER,
									attackBack.getAttack().getDamage(),
									attackBack.getAttack().IsCT(),
									attackBack.getDefence(),
									attackBack.getAttack().getWeaponData()->getKind());
		loadChara(COUNTER,nChara,counter.getChara(),!bLeft);
	}
	// スタート
	it=listMovie_.begin();
	actionNext(pContext);
}

void CDemo_map_battle::setCounter(bool bLeft, CDataBattleBase& attack, CDataBattleBase& counter)
{
	// 前半
	listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_ATK));
	loadChara(COUNTER,CHARA_ATK,counter.getChara(),!bLeft);
	listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_DEFAULT,CHANGE));
	// 後半
	int nChara =setAtkDefMovie(!bLeft,
							   COUNTER,ATTACK,
							   counter.getAttack().getDamage(),
							   counter.getAttack().IsCT(),
							   attack.getDefence(),
							   counter.getAttack().getWeaponData()->getKind());
	loadChara(ATTACK,nChara,attack.getChara(),bLeft);
}

void CDemo_map_battle::setDefBack(bool bBackDef, bool bLeft, CDataBattleBase& attack, CDataBattleBase& counter, CDataBattleBase& counterBack)
{
	// 援護防御ある？
	if(bBackDef)
	{// あるじゃ、入れ替え
		listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_DEFAULT,BACKUP_DEF));
		listMovie_.push_back(DEMO_MOVIE(bLeft,ATTACK,CHARA_DEFAULT,CHANGE));
		listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER_B,CHARA_DEFAULT,CHANGE));
		loadChara(COUNTER_B,CHARA_DEFAULT,counterBack.getChara(),!bLeft);
		// で、攻撃を受ける
		int nChara = setAtkDefMovie(bLeft,
									ATTACK,COUNTER_B,
									attack.getAttack().getDamage(),
									attack.getAttack().IsCT(),
									counterBack.getDefence(),
									attack.getAttack().getWeaponData()->getKind());
		loadChara(COUNTER_B,nChara,counterBack.getChara(),!bLeft);
		// 元に戻す
		listMovie_.push_back(DEMO_MOVIE(!bLeft,COUNTER,CHARA_DEFAULT,CHANGE));
	}
	else
	{// ない
		// 普通に攻撃を受ける
		int nChara = setAtkDefMovie(bLeft,
									ATTACK,COUNTER,
									attack.getAttack().getDamage(),
									attack.getAttack().IsCT(),
									counter.getDefence(),
									attack.getAttack().getWeaponData()->getKind());
		loadChara(COUNTER,nChara,counter.getChara(),!bLeft);
	}
}

int CDemo_map_battle::setAtkDefMovie(bool bLeft, int nFrom, int nTo, int nDamage, bool bCT, CDataBattleDefence& def, int nWeapon)
{
	if(nWeapon==Weapon::Kind::STATUS)
		nWeapon=STATUS;
	ef(nWeapon==Weapon::Kind::FIGHT_COND
	|| nWeapon==Weapon::Kind::MAGIC_COND)
		nWeapon=COND;
	else
		nWeapon=NORMAL_W;

	// CTエフェクト
	//listMovie_.push_back(DEMO_MOVIE(bLeft,nFrom,CHARA_DEFAULT,bCT?CT:CHANGE));
	if(bCT) listMovie_.push_back(DEMO_MOVIE(bLeft,nFrom,CHARA_DEFAULT,CT));

	// 通常の行動
	DEMO_MOVIE movie;

	movie.bLeft_=!bLeft;
	movie.nBattle_=nTo;

	// 防御行動によって再生ムービーが変化
	if(def.getAction()==Battle::AVOID)
	{// 回避！
		movie.nChara_=CHARA_AVOID;
		if(def.IsAlterEgo()) // 分身発動？
			movie.nEffect_=BUNSHIN;
		else
			movie.nEffect_=movie.bLeft_ ? AVOID_L : AVOID_R;
	}
	ef(def.getAction()==Battle::DEFENCE)
	{// 防御
		movie.nChara_=CHARA_DEF;
		movie.nEffect_=DEF;
	}
	else
	{// 当たった！
		if(nWeapon==STATUS)
		{// ステータスアップ
			movie.nChara_=CHARA_DEFAULT;
			movie.nEffect_= movie.bLeft_ ? STATUS_UP_L : STATUS_UP_R;
		}
		else
		{// ノーマル
			movie.nChara_=CHARA_DAMAGE;
			movie.nEffect_=HIT;
		}
	}

	// バリア発動してたら、エフェクト上書き
	if(def.IsBarriar())
		movie.nEffect_=movie.bLeft_ ? BARRIER_L : BARRIER_R;

	// 最後にリストに追加
	listMovie_.push_back(movie);

	// ちょっと数字見せるためWAIT
	 // 状態変化武器での攻撃だった？
	if(nWeapon==COND && nDamage>0) // そうだったら、ドクロがつくでｗ
		listMovie_.push_back(DEMO_MOVIE(movie.bLeft_,movie.nBattle_,CHARA_DEFAULT,movie.bLeft_?DOKURO_L:DOKURO_R,20,nDamage));
	else
		listMovie_.push_back(DEMO_MOVIE(movie.bLeft_,movie.nBattle_,CHARA_DEFAULT,WAIT_E,15,nDamage));
		

	return movie.nChara_;
}

////////////////////////////////////////////////////
// ムービー再生
////////////////////////////////////////////////////
void CDemo_map_battle::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EF: // エフェクト実行終了待ち
		if(pEffect_[it->nEffect_]->IsEnd())
		{
			++it;
			actionNext(pContext);
		}
	break;

	case EF_CHARA: // キャラ動作終了待ち
		if(pChara_[it->nBattle_][it->nChara_]->getState()==Movie::CMovieClip::END)
		{
			++it;
			actionNext(pContext);
		}
	break;

	case WAIT:
		if(it->nFrame_--<=0)
		{// ↑どうせ使い捨てだから、直接減算してしまう
			++it;
			actionNext(pContext);
		}
	break;
	
	default: break;
	}
}

////////////////////////////////////////////////////
// アクション
////////////////////////////////////////////////////
void CDemo_map_battle::actionNext(Task::CTaskContext* pContext)
{
GOTO:
	if(it==listMovie_.end()) // 終了ー
	{	// ホルダを戻して終了
		pDemo_->getLeftPanel()->valid(false);
		pDemo_->getLeftPanel()->visible(false);
		pDemo_->getRightPanel()->valid(false);
		pDemo_->getRightPanel()->visible(false);

		actionEnd(pContext);
		clearChara();
		setState(NORMAL);
	}
	else
	{// 使うエフェクトを設定・リセット
		#ifdef BMW_DEBUG
		CDbg().Out("DEMO %d %d %d %d",it->bLeft_,it->nBattle_,it->nChara_,it->nEffect_);
		#endif

		GUI::CPanel* pPanel = it->bLeft_ ? pDemo_->getLeftPanel() : pDemo_->getRightPanel();
		updateNum(pPanel);
		updateChara(pPanel,pContext);
		// エフェクト
		switch(it->nEffect_)
		{
		case CHANGE: // キャラチェンジは即座反応
			++it;
			goto GOTO;
		break;
		
		case WAIT_E:
			setState(WAIT);
		break;

		case EFFECT_END:
			setState(EF_CHARA);
		break;

		default:
			if(it->nChara_==CHARA_ATK
			|| it->nChara_==CHARA_AVOID)
				setState(EF_CHARA);
			else
				setState(EF);

			pEffect_[it->nEffect_]->OnReset(pContext);
			pPanel->removeWidget("SERIF");
			pPanel->addWidget(pEffect_[it->nEffect_],"SERIF");
			pEffect_[it->nEffect_]->setDrawInfo(pDemo_->getSerif(it->bLeft_?0:1)->getDrawInfo(false));
		break;
		}
	}
}

void CDemo_map_battle::updateChara(GUI::CPanel* pPanel, Task::CTaskContext* pContext)
{
	pChara_[it->nBattle_][it->nChara_]->OnReset(pContext);
	pPanel->removeWidget("CHIP");
	pPanel->addWidget(pChara_[it->nBattle_][it->nChara_],"CHIP");
	pChara_[it->nBattle_][it->nChara_]->setDrawInfo(pDemo_->getChip(it->bLeft_?0:1)->getDrawInfo(false));
	pPanel->getWidgetCast<GUI::CPanelCtrl>("HEADER")->validWidget(it->nBattle_);
}

void CDemo_map_battle::updateNum(GUI::CPanel* pPanel)
{
	GUI::INum* pNum = pPanel->getWidgetCast<GUI::INum>("DAMAGE");
	pNum->setNum(it->nDamage_);
	pNum->visible(it->nDamage_>=0);
}

void CDemo_map_battle::loadChara(int nBattle, int nChara, smart_ptr<CDataCharaSLG>& pChara, bool bLeft)
{
	// すでにロードされてたらいらぬ
	if(pChara_[nBattle][nChara]!=NULL) return;
	//pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr(bLeft?"BEFORE_BOTTOM":"BEFORE_LEFT");
	switch(nChara)
	{
	case CHARA_DEFAULT:
		pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr(bLeft?"BEFORE_BOTTOM":"BEFORE_LEFT");
	break;
	case CHARA_ATK:
		pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr("ATK");
	break;
	case CHARA_DEF:
		pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr("DEFENCE");
	break;
	case CHARA_AVOID:
		pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr(bLeft?"AVOID_L":"AVOID_R");
	break;
	case CHARA_DAMAGE:
		pChara_[nBattle][nChara]=pChara->getMapSymbol()->createSymbolStr("DAMAGE");
	break;
	}
}

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end