#include "stdafx.h"

#include "../../Movie/DB/CSymbolDB.h"
#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Scene/IScene.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Action/IAction.h"
#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CAttack_calc.h"
#include "CAttack_road.h"
#include "CAttack_action.h"

#include "CField_attack.h"

namespace BMW{
namespace SLG{
namespace Attack{

CField_attack::CField_attack():nWeaponID_(-1)
{
	pSymbol_ = new Movie::CSymbolDB();
}

CField_attack::~CField_attack()
{
	DELETE_SAFE(pPanel_);
	DELETE_SAFE(pCancel_);
	DELETE_SAFE(pSymbol_);
}

void CField_attack::OnReset(Task::CTaskContext* pContext)
{
	// キャンセル動作
	pCancel_ = new Rule::CRuleCancel(CANCEL);
	pCancel_->setParent(smart_ptr<ITaskBase>(this,false));

	// パネルロード
	pPanel_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("PANEL_MAPWEAPON");
	// オンオフ
	pOnOff_ = pPanel_->getWidgetCast<GUI::CPanelCtrl>("DEMO_ONOFF");
	// ハンドラ設定
	GUI::CButton::ButtonEvent fun(this, &CField_attack::eventButton);
	GUI::CButton::setButtonEvent(pPanel_->getWidgetCast<GUI::CButton>("BATTLESTART"),fun,GO);
	GUI::CButton::setButtonEvent(pOnOff_->getWidgetCast<GUI::CButton>("ON_BUTTON"),fun,ON);
	GUI::CButton::setButtonEvent(pOnOff_->getWidgetCast<GUI::CButton>("OFF_BUTTON"),fun,OFF);
}

void CField_attack::OnInit(Task::CTaskContext* pContext)
{
	// キャンセルはPLAYERフェーズで、かつ操作キャラがNPCでない時
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	pCancel_->valid(p->getPhase()==Phase::PLAYER && !(p->getCtrlCharaData()->getAction()->IsNonPlayer()));

	// インターフェイス設定
	pOnOff_->validWidget(pContext->getValue(Flag::DEMO) ? "ON_BUTTON" : "OFF_BUTTON");

	// 現在ロードしてるやつと違うなら再ロード
	if(nWeaponID_!=p->getCtrlWeapon())
	{
		nWeaponID_=p->getCtrlWeapon();
		pSymbol_->clearSymbol();
		pSymbol_->setSymbol(p->getCtrlWeaponData()->getFieldFile());
		pCutIn_=pSymbol_->createSymbolStrCast<Movie::CMovieClip>("CUTIN");
		// そして、再設定
		pPanel_->swapWidget(pCutIn_,"CUTIN");
	}
	// カットイン動作リセット
	pCutIn_->OnReset(pContext);
	setState(INTRO);
}

void CField_attack::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case INTRO:
		if(pCutIn_->getTask(0)->getState()==1)
		{// キーフレーム2に映ってたら、終了したら次へ
			pContext->getInput()->guard(false);
			pContext->getInput()->cursolVisible(true);
			setState(NORMAL);
		}
	break;

	case CANCEL:
		pContext->push(-1);
		pContext->getInput()->guard(true);
		pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
		getTaskListCtrl()->returnTaskList();
	break;

	case END:
		if(pCutIn_->IsEnd())
		{
			pContext->push(0);
			getTaskListCtrl()->returnTaskList();
		}
	break;

	default: break;
	}
}

void CField_attack::callTaskAction(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
	pCancel_->Task(pContext);
}

void CField_attack::callTaskDraw(Task::CTaskContext* pContext)
{
	pPanel_->Task(pContext);
}

void CField_attack::eventButton(const smart_ptr<GUI::CEventButton>& pButton, Task::CTaskContext* pContext)
{
	if(IsRelease(pButton))
	{// 何かボタン押された
		switch(pButton->getValue())
		{// 押されたボタンによって処理分岐
		case ON:
			// ONボタンが押された
			pContext->setValue(0,Flag::DEMO);
			pOnOff_->validWidget("OFF_BUTTON");
			pOnOff_->getValidWidget()->OnReset(pContext);
		break;
		
		case OFF:
			// OFFボタンが押された
			pContext->setValue(1,Flag::DEMO);
			pOnOff_->validWidget("ON_BUTTON");
			pOnOff_->getValidWidget()->OnReset(pContext);
		break;

		case GO:
			// 次はカーソルが非表示
			pContext->getInput()->cursolVisible(false);
			pContext->getInput()->guard(true);
			// 戦闘開始！
			actionGo(pContext);
			setState(END);
			// 退場動作
			pCutIn_->getTask(0)->setState(pCutIn_->getTask(0)->getState()+1);
		break;

		default: break;
		}
	}
}

void CField_attack::actionGo(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	CDataBattleMap& map = p->getBattleMap();
	// 攻撃側設定
	CDataBattleMapAtk& mapAtk = map.getMapAtk();
	mapAtk.clearData();
	CDataCharaSLG* pAtk = p->getCtrlCharaData();
	mapAtk.setChara(pAtk);
	Weapon::CDataWeaponBattle* pAtkWeapon = p->getCtrlWeaponData();
	mapAtk.setWeapon(pAtkWeapon);
		
	// 攻撃武器の消費ENを設定
	int nEN = pAtkWeapon->getEN();
	mapAtk.setEnWeapon(nEN);

	// 防御側データクリア
	map.clearMapDef();
	// 攻撃対象キャラリストを取得
	set<int> setChara;

	// フェーズ
	int nPhase = pAtkWeapon->IsFieldFriend() ? pAtk->getPhase() : -1;
	// 味方対象の武器だったらひっくり返す
	if(pAtkWeapon->getKind()==Weapon::Kind::STATUS)
	{
		if(nPhase==Phase::PLAYER) nPhase=Phase::ENEMY;
		ef(nPhase==Phase::ENEMY) nPhase=Phase::PLAYER;
	}

	p->getRangeFieldChara(pAtkWeapon->getField(),
						  nPhase,
						  pAtkWeapon->getMin(), 
						  pAtkWeapon->getMax(),
						  Map::CMapChipState::getField(),
						  setChara);
	// もう、Moveに確保している範囲はいらないのでクリア
	if(pAtkWeapon->getField()==Weapon::Field::THROW) p->clearMove();
	// 取得した対象に対して、攻撃計算
	set<int>::iterator it;
	for(it=setChara.begin(); it!=setChara.end(); ++it)
		if(pAtk->getID()!=*it) calcBattle(mapAtk, *it, map, *p);

	// 精神フラグ
	// 魂フラグを倒す
	if(pAtk->getBattle().IsSpirit(Chara::CValidSpirit::SPIRIT))
		pAtk->getBattle().spirit(false,Chara::CValidSpirit::SPIRIT);
	// 熱血フラグを倒す
	ef(pAtk->getBattle().IsSpirit(Chara::CValidSpirit::FIREBALL))
		pAtk->getBattle().spirit(false,Chara::CValidSpirit::FIREBALL);
	// 直撃フラグを倒す
	pAtk->getBattlePtr()->spirit(false,Chara::CValidSpirit::DIRECT);
	// てかげんのフラグを倒す
	pAtk->getBattle().spirit(false,Chara::CValidSpirit::EASYON);

	// 移動範囲クリア
	p->clearMove();

	// デモ再生用にデータ設定
	p->getBattleData()->clearBattleData();
	// 攻撃側の所属フェーズによって、どちら側なのかを選定
	p->getBattleData()->setSide(pAtk->getPhase()==Phase::PLAYER ? CDataBattle::RIGHT : CDataBattle::LEFT);
	// デモは前半だけなので、攻撃側だけ設定しておく
	CDataBattleBase& atk = p->getBattleData()->getBattleData(CDataBattle::ATTACK);
	atk.setChara(smart_ptr<CDataCharaSLG>(pAtk,false));
	atk.getAttack().setWeaponData(smart_ptr<Weapon::CDataWeaponBattle>(pAtkWeapon,false));
	atk.getAttack().setEN(nEN);
	// 防御側は先頭のやつをとりあえず設定しておく
	CDataBattleBase& def = p->getBattleData()->getBattleData(CDataBattle::COUNTER);
	def.setChara(smart_ptr<CDataCharaSLG>(map.beginMapDef()->getChara(),false));
	// 戦闘背景設定
	p->getBattleData()->setBack(p->getMap()->getDemoBack());
}

void CField_attack::calcBattle(CDataBattleMapAtk& atk, int nDef, CDataBattleMap& map, CSLGContext& p)
{
	// 攻撃キャラデータ
	CDataCharaSLG* pAtk = atk.getChara();
	Weapon::CDataWeaponBattle* pAtkWeapon = atk.getWeapon();
	pAtk->getState().apper(true);

	// 防御キャラデータ取得
	CDataBattleMapDef def;
	CDataCharaSLG* pDef = p.getCharaData(nDef);
	def.setChara(pDef);
	pDef->getState().apper(true);

	if(pAtkWeapon->getKind()==Weapon::Kind::STATUS)
	{// ステータス武器にはかならず当たる
		def.setDamage(0);
		def.hit(true);
	}
	ef(pDef->getBattle().IsTalent(Ability::FIELD_IGNORE))
	{// 防御側がフィールド武器無効化を持ってるなら、回避される
		def.setDamage(0);
		def.hit(false);
	}
	ef(pDef->getBattle().IsTalent(Ability::AVALON)
	&& p.getApp()->getAbility().enableTarget(*pDef,0,p,Ability::AVALON))
	{// アヴァロン第二段階目が発動してたら無効
	 // ifをわかりやすくするためにわけてある
		def.calcEnAbility(p.getApp()->getAbility().getEN(Ability::AVALON));
		def.setDamage(0);
		def.hit(false);
	}
	else
	{
		// 防御マップチップ
		Map::CMapChip* pChip = p.getMapChip(pDef->getIndex());
		Map::CMapChipState* pState = pChip->getMapChipState();
		// とりあえず、命中率計算
		int nHit = CAttack_calc::calcHit(*pAtk,pAtkWeapon,*pDef,
										 pState->getFieldAttack()
										 + (pAtkWeapon->getField()==Weapon::Field::THROW ? pState->getMove() : 0),
										 pState->getAtkHeight(),
										 p);
		if(nHit==CAttack_calc::HIT)		nHit=200;
		ef(nHit==CAttack_calc::AVOID)	nHit=0;
		else							nHit+=CAttack_calc::calcOffHit(*pAtk, *pDef, p);

		if(nHit<0) nHit=0;

	#ifdef BMW_DEBUG
		CDbg().Out("FielfHit %d",nHit);
	#endif

		// 命中判定
		bool bHit = (int)CApp::rand_.Get(100)+1<=nHit;
		//bool bHit = CAttack_calc::randLot_.lot(static_cast<unsigned int>(nHit));
		if(bHit && nHit<200)
		{// 当たるなら、分身だ！
			// 分身系Ability発動判定
			bHit = !CAttack_action::calcAlterEgo(*pDef,def,0,p,*pAtk,atk);
		#ifdef BMW_DEBUG
			CDbg().Out("AlterEgo %d", bHit);
		#endif
		}
		def.hit(bHit);

		if(bHit)
		{// 命中したので、ダメージ計算
			// THROWの時は、必ずCore距離になる
			calcDamage(atk, def, pAtkWeapon->getField()!=Weapon::Field::THROW ? pState->getFieldAttack() : pAtkWeapon->getCoreMin(), p);
		}

		// ひらめきフラグ倒し
		if(pDef->getBattle().IsSpirit(Chara::CValidSpirit::AVOID))
			pDef->getBattle().spirit(false,Chara::CValidSpirit::AVOID);
	}

	// リストへ追加
	map.addMapDef(def);
}

void CField_attack::calcDamage(CDataBattleMapAtk& atk, CDataBattleMapDef& def, int nDist, CSLGContext& p)
{
	int nDamage;
	// 攻撃サイド
	CDataCharaSLG* pAttackChara = atk.getChara();
	Weapon::CDataWeaponBattle* pAttackWeapon = atk.getWeapon();
	// 防御サイド
	CDataCharaSLG* pDefChara = def.getChara();

	// 通常攻撃
	if(pDefChara->getBattle().IsSpirit(Chara::CValidSpirit::TOUGH))
	{// 不屈
		// ダメージを10に
		nDamage=10;
		pDefChara->getBattle().spirit(false,Chara::CValidSpirit::TOUGH);
	}
	else
	{// 不屈無し		
	
		nDamage = CAttack_calc::calcDamage(*pAttackChara,pAttackWeapon,*pDefChara,NULL,CAttack_calc::ATTACK,p,nDist);
		// CTは発生したか？
		int nCT = CAttack_calc::calcCT(*pAttackChara,pAttackWeapon,*pDefChara,p);
		def.ct((int)CApp::rand_.Get(100)+1<=nCT);
		if(def.IsCT())
		{// CT発生したら、ダメージ1.25倍
			nDamage = (nDamage*125)/100;
		}
		
		// バリア系特殊技能持ち？
		// ただし、直撃があったり、合体攻撃の時は発動しない
		int nBarriar = 0;
		if(!pAttackChara->getBattle().IsSpirit(Chara::CValidSpirit::DIRECT))
			nBarriar = CAttack_action::calcBarriar(nDamage, *pDefChara, def, 0, pAttackWeapon->IsM(), pAttackWeapon->IsT(), p);

		// 最低ダメージは10
		// バリアで0になった場合は0でOK
		if(nDamage<10) nDamage= nBarriar!=0 ? 0 : 10;

		if(pDefChara->getBattle().getHP()<=nDamage)
		{// 与えるダメージが、防御側の現在値を越えたら、
			// てかげんされているか？
			bool bEasy=false;
			if(pAttackChara->getBattle().IsSpirit(Chara::CValidSpirit::EASYON))
			{// 技量を比べる
				bEasy = pAttackChara->getBattle().getSkill() >= pDefChara->getBattle().getSkill();
				if(bEasy) // 効果発動可能
					nDamage = pDefChara->getBattle().getHP()-10;
			}
			if(!bEasy)
			{// 死亡フラグを立てる
				if(pDefChara->getBattle().IsTalent(Ability::BATTLEFOLLOW)
				&& p.getApp()->getAbility().enable(*pDefChara,*pAttackChara,def.getEnAbility(),p,Ability::BATTLEFOLLOW))
				{// 戦闘続行もってるなら、発動するか判定
					// 発動した！
					// HP10で生き残る
					nDamage = pDefChara->getBattle().getHP()-10;

					def.setAbility(Ability::BATTLEFOLLOW);
					def.calcEnAbility(p.getApp()->getAbility().getEN(Ability::BATTLEFOLLOW));
				}
				ef(pDefChara->getBattle().IsTalent(Ability::SCAPEGOAT)
				&& p.getApp()->getAbility().enable(*pDefChara,def.getEnAbility(),p,Ability::SCAPEGOAT))
				{// 身代わりもってるなら、発動するか判定
					// 発動した！
					// ダメージ無効化
					nDamage = 0;
					def.setAbility(Ability::SCAPEGOAT);
					def.calcEnAbility(p.getApp()->getAbility().getEN(Ability::SCAPEGOAT));
				}
				else
				{	def.death(true);	}
			}
		}
	}
	// ダメージ設定
	def.setDamage(nDamage);
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end