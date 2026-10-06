#include "stdafx.h"

#include "../../Weapon/IDWeapon.h"
#include "../../Weapon/CDataWeaponBattle.h"

#include "../../Status/status_fun.h"
#include "../../Scene/IScene.h"
#include "../../Scene/GUI/CGraphicFace.h"

#include "../IDSLG.h"
#include "../Event/CEvent.h"
#include "../Phase/CPhaseBall.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"
#include "../Context/CDataBattle.h"
#include "../Action/IAction.h"

#include "CAttack_result.h"

namespace BMW{
namespace SLG{
namespace Attack{

namespace{
// Expのレベル差係数
const int EXP_LV[18]=
{
	400,370,340,310,280,250,220,190,160,130,
	100,
	70,50,35,20,10,5,2
};

enum eLvSub
{
	PLUS_10,
	PLUS_9,
	PLUS_8,
	PLUS_7,
	PLUS_6,
	PLUS_5,
	PLUS_4,
	PLUS_3,
	PLUS_2,
	PLUS_1,
	ZERO,
	MINUS_1,
	MINUS_2,
	MINUS_3,
	MINUS_4,
	MINUS_5,
	MINUS_6,
	MINUS_7,
};

__inline int LvSub(int nAttack, int nDef)
{// Lv差と係数添え字変換
	int nSub = nDef - nAttack;

	if(nSub>=10) return PLUS_10;
	ef(nSub==9) return PLUS_9;
	ef(nSub==8) return PLUS_8;
	ef(nSub==7) return PLUS_7;
	ef(nSub==6) return PLUS_6;
	ef(nSub==5) return PLUS_5;
	ef(nSub==4) return PLUS_4;
	ef(nSub==3) return PLUS_3;
	ef(nSub==2) return PLUS_2;
	ef(nSub==1) return PLUS_1;
	ef(nSub==0) return ZERO;
	ef(nSub==-1) return MINUS_1;
	ef(nSub==-2) return MINUS_2;
	ef(nSub==-3) return MINUS_3;
	ef(nSub==-4) return MINUS_4;
	ef(nSub==-5) return MINUS_5;
	ef(nSub==-6) return MINUS_6;
	else return MINUS_7;
}

} // namespace end

void CAttack_result::OnReset(Task::CTaskContext* pContext)
{// 受け入れ口を作っておく
	// OK
	BMW::Rule::CRuleOK* pOK = new BMW::Rule::CRuleOK();
	pOK->setValue(CLICK);
	addTask(pOK,OK_T);

	// CANCEL
	BMW::Rule::CRuleCancel* pCancel = new BMW::Rule::CRuleCancel();
	pCancel->setValue(CLICK);
	addTask(pCancel,CANCEL_T);

	// インターフェイス取得
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// 戦闘結果
	pResult_ = db.createInterfaceCast<GUI::CPanel>("PANEL_RESULT");
	addTask(pResult_,RESULT);
	// アイテム部分は、とりあえず非動作に
	pResult_->getWidgetCast<GUI::CPanel>("GET_ITEM")->validAll(false);
	// ステータス
	pStatus_ = db.createInterfaceCast<GUI::CPanel>("PANEL_BASICSTATUS");
	pStatus_->setX(20);
	pStatus_->setY(190);
	addTask(pStatus_,STATUS);
	// LVアップ
	pLvUp_ = db.createInterfaceCast<GUI::CPanel>("LEVELUP_FACE");
	addTask(pLvUp_,LVUP);
}

void CAttack_result::OnInit(Task::CTaskContext* pContext)
{
	using BMW::GUI::CNum;

	setState(NORMAL);
	pContext->getInput()->guard(false);
	pContext->getInput()->cursolVisible(true);
	visible(true);

	// 取得アイテムのリセット
	listItem_.clear();
	// 各種データのリセット
	nExp_=0;
	nBP_=0;
	nFP_=0;
	nLv_=0;

#ifdef BMW_DEBUG
	// カウンタリセット
	nFrame_=0;
#endif

	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	if(p->getCtrlWeaponData()->IsF())
	{// MAP兵器
		CDataBattleMap& map = p->getBattleMap();
		CDataBattleMapAtk& atk = map.getMapAtk();
		CDataCharaSLG* pAtk = atk.getChara();
		// いなかったらスルー
		bool bPlayer = pAtk->getPhase()==Phase::PLAYER && pAtk->getState().getAct()!=Act::REMOVE;

		// 名前とかの設定
		// 味方側の攻撃のみ
		if(bPlayer)	setPanelChara(*pAtk,*p);

		// 全データ結果を回収
		CDataBattleMap::mapdef_list::iterator it;
		map.beginMapDef();
		while(!map.endMapDef())
		{
			it = map.nextMapDef();

			if(atk.getWeapon()->getKind()==Weapon::Kind::STATUS)
			{// ステータス武器
				if(bPlayer)	calcResult(atk,*it,*p);
			}
			else
			{
				// 敵の時はIsDeathだけが必要
				if(IsDeath(it->getChara(),*p)
				&& bPlayer
				&& pAtk->getPhase()!=it->getChara()->getPhase())
				{// 倒してたらアイテムゲット？
					// 撃墜数増やす
					pAtk->getBattle().incKill();
					getItem(it->getChara(),p);
				}
				// 味方を倒した時は、EXPとかは手に入らない
				if(bPlayer && pAtk->getPhase()!=it->getChara()->getPhase())
					calcResult(atk,*it,*p);
			}
		}

		if(bPlayer)
		{// やっぱり味方ん時だけ
			// 精神補正
			checkSpirit(atk.getChara()->getBattlePtr());

			// LVUP
			LvUP(atk.getChara(),*p);
			// レベルアップするならステータス生成
			if(nLv_>0) setPanelLvUp(*atk.getChara(),*p);
		}
		else // 敵だったら表示はしない
		{ setState(CLICK); visible(false); return; }
	}
	else
	{// MAP兵器じゃない
		smart_ptr<CDataBattle>& pBattle = p->getBattleData();

		// 攻撃側データ
		CDataBattleBase& attack			= pBattle->getBattleData(CDataBattle::ATTACK);
		// 援護攻撃データ
		CDataBattleBase& attackBack		= pBattle->getBattleData(CDataBattle::ATTACK_BACK);
		// 反撃側データ
		CDataBattleBase& counter		= pBattle->getBattleData(CDataBattle::COUNTER);
		// 援護防御
		CDataBattleBase& counterBack	= pBattle->getBattleData(CDataBattle::COUNTER_BACK);

		// 攻撃側がPlayerか？
		if(attack.getChara()->getPhase()==Phase::PLAYER)
		{// そうだったら、反撃側を倒したか、チェック
			// まず、こちらが倒されてないかをチェック
			// 離脱していてもスルーする
			// また、NPCでもスルー
			if(attack.getChara()->getState().getAct()==Act::REMOVE
			|| IsDeath(attack.getChara().getPointer(),*p)
			|| attack.getChara()->getAction()->IsNonPlayer())
			{ setState(CLICK); visible(false); return; }
		
			// 名前とかの設定
			setPanelChara(*attack.getChara(),*p);

			// 援護防御がおこなわれたか？
			if(counterBack.getDefence().getAction()!=Battle::NO)
			{// 行われた
				if(IsDeath(counterBack.getChara().getPointer(),*p))
				{// 倒してたら、アイテム持ってる？
					// 撃墜数を増やす
					attack.getChara()->getBattle().incKill();
					getItem(counterBack.getChara().getPointer(),p);
				}
				calcResult(attack,counterBack,*p,attack.getChara()->getBattle().getLv());
			}
			else
			{// 行われてない
				if(IsDeath(counter.getChara().getPointer(),*p))
				{// 倒してたら、アイテム持ってる？
					attack.getChara()->getBattle().incKill();
					getItem(counter.getChara().getPointer(),p);
				}
				calcResult(attack,counter,*p,attack.getChara()->getBattle().getLv());
			}
			// 援護攻撃が行われたか？
			if(attackBack.getDefence().getAction()!=Battle::NO)
			{
				// 援護攻撃が行われ、反撃側撃破された
				if(counterBack.getDefence().getAction()!=Battle::NO
				&& IsDeath(counter.getChara().getPointer(),*p))
				{// 撃墜数を増やす
					attack.getChara()->getBattle().incKill();
					getItem(counter.getChara().getPointer(),p);
				}
				calcResult(attackBack,counter,*p,attack.getChara()->getBattle().getLv());
			}

			// 精神補正
			checkSpirit(attack.getChara()->getBattlePtr());

			// LVUP
			LvUP(attack.getChara().getPointer(),*p);
			// レベルアップするならステータス生成
			if(nLv_>0) setPanelLvUp(*attack.getChara(),*p);
		}
		else
		{// 攻撃側を倒したか、チェック
			// まず、こちらが倒されてないかをチェック
			// もしくは、NPCキャラでもスルー
			if(IsDeath(counter.getChara().getPointer(),*p)
			|| counter.getChara()->getAction()->IsNonPlayer())
			{ setState(CLICK); visible(false); return; }

			// 名前とかの設定
			setPanelChara(*counter.getChara(),*p);
		
			// 援護防御がおこなわれたか？
			if(counterBack.getDefence().getAction()!=Battle::NO)
			{// 行われた
				IsDeath(counterBack.getChara().getPointer(),*p);
			}
			// 反撃したか？
			if(attack.getDefence().getAction()!=Battle::NO)
			{
				// ↓反撃時のこの判定は、CMenu_select_cpuで行われる
				if(IsDeath(attack.getChara().getPointer(),*p))
				{// アイテムを持ってるか？
					getItem(attack.getChara().getPointer(),p);
					counter.getChara()->getBattle().incKill();
				}
				calcResult(counter,attack,*p,counter.getChara()->getBattle().getLv());
			}

			// 精神補正
			checkSpirit(counter.getChara()->getBattlePtr());

			// LVUP
			LvUP(counter.getChara().getPointer(),*p);
			// レベルアップするならステータス生成
			if(nLv_>0) setPanelLvUp(*counter.getChara(),*p);
		}
	}

	// インターフェイス
	// まずは、レザルト画面から
	pResult_->visible(true);
	pStatus_->visible(false);
	pStatus_->valid(false);
	pLvUp_->visible(false);
	// 経験値とか取得
	GUI::CPanel* pPanel = pResult_->getWidgetCast<GUI::CPanel>("GET_EXPBPFP");
	// EXP
	CNum* pEXP = pPanel->getWidgetCast<GUI::CNum>("EXP");
	// BP
	CNum* pBP = pPanel->getWidgetCast<GUI::CNum>("BP");
	// FP
	CNum* pFP = pPanel->getWidgetCast<GUI::CNum>("FP");

	// 獲得EXP
	pEXP->setNum(nExp_);
	// 獲得BP
	p->setBP((BP_MAX>(p->getBP()+nBP_)) ? (p->getBP()+nBP_) : BP_MAX);
	pBP->setNum(nBP_);
	// 獲得FP
	p->setFP((FP_MAX>(p->getFP()+nFP_)) ? (p->getFP()+nFP_) : FP_MAX);
	pFP->setNum(nFP_);

	// ターンボールへ反映
	p->getEvent()->getTurnBall().update(p);

	// 獲得アイテム
	// とりあえず、非表示に
	pPanel = pResult_->getWidgetCast<GUI::CPanel>("GET_ITEM");
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	GUI::CGraphicPopUp* pIcon;
	Item::CItemDB& iDB = p->getApp()->getItem();
	// 獲得しただけ表示
	list<int>::iterator it;
	int nCount=1;
	for(it=listItem_.begin(); it!=listItem_.end(); it++)
	{
		pIcon = pPanel->getWidgetCast<GUI::CGraphicPopUp>(Misc::linkStrAndNum("ITEM",nCount++));
		iDB.setGraphicHolder(pIcon, *it);
		pIcon->valid(true);
		pIcon->visible(true);		
	}
}

void CAttack_result::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case CLICK:
		if(nLv_>0)
		{// 次はレベルアップ
			if(!pLvUp_->IsVisible())
			{
				// LVアップサウンド
				pContext->getBgmSound()->Pause();
				pContext->getApp()->getSeDB().PlayN("LV_UP");

				// 表示入れ替え
				pResult_->visible(false);
				pLvUp_->visible(true);
				pStatus_->visible(true);
				pStatus_->valid(true);
				setState(NORMAL);

			#ifdef BMW_DEBUG
				nFrame_=0;
			#endif

				// Lvアップ時はクイックロード無効化
				static_cast<CSLGContext*>(pContext)->quickLoad(false);
			}
			else
			{// レベルアップ表示終了
				if(!pContext->getApp()->getSeDB().IsPlay("LV_UP"))
				{
					pContext->getBgmSound()->RePlay();
					pContext->getInput()->guard(true);
					static_cast<CSLGContext*>(pContext)->quickLoad(true);
					getTaskListCtrl()->returnTaskList();
				}
			}
		}
		else
		{// 何もないなら、そのままリターン
			pContext->getApp()->getFoward()->clearPopUp();
			pContext->getInput()->guard(true);
			pContext->getInput()->cursolVisible(pContext->getValue(Flag::PHASE)==Phase::PLAYER);
			getTaskListCtrl()->returnTaskList();
		}
	break;

#ifdef BMW_DEBUG
	case NORMAL:
		// 3秒たったらCLICK扱い
		if(++nFrame_>=30) setState(CLICK);
	break;
#endif

	default: break;
	}
}


////////////////////////////////////////////////////////////
// 結果計算
////////////////////////////////////////////////////////////
bool CAttack_result::IsDeath(CDataCharaSLG* pChara, CSLGContext& p)
{
	if(pChara->getState().getAct()==Act::DEATH
	|| pChara->getState().getAct()==Act::DEATH_EVENT)
	{// 死んで、ますよ？
		// 死んだのがPlayerだったら、死亡リストに入れる
		// フェーズリストからはずす
		if(pChara->getPhase()==Phase::PLAYER)
		{
			p.getDeathSet().insert(pChara->getID());
			p.delPhase(pChara->getID(),Phase::PLAYER);
		}

		return true;
	}

	return false;
}

void CAttack_result::calcResult(CDataBattleMapAtk& atk, CDataBattleMapDef& def, CSLGContext& p)
{// MAP版
	int nExp=0;
	if(atk.getWeapon()->getKind()==Weapon::Kind::STATUS)
	{// 補給・ステータス武器だったら、ある意味固定EXP
		nExp = 100;
	}
	else
	{
		nExp = def.getChara()->getBattle().getExp();
		if(def.IsDeath()) // 撃墜したか？
		{// した
			nBP_ += def.getChara()->getBattle().getPena();
			nFP_ += def.getChara()->getBattle().getFP();
		}
		else
		{// してなかったら、EXPは10分の1
			nExp/=10;
		}
	}

	// Lv差係数を掛けておく
	// MAP兵器補正も入ってる
	// つまり、通常の二分の一しかEXPもらえない
	nExp_ += nExp
			* EXP_LV[LvSub(atk.getChara()->getBattle().getLv(),def.getChara()->getBattle().getLv())]
			/ 200;
}

void CAttack_result::calcResult(CDataBattleBase& attack, CDataBattleBase& def, CSLGContext& p, int nLv)
{
	int nExp;
	if(attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::CURE)
	{// 治癒武器だったら、ある意味固定EXP
		nExp = -(attack.getAttack().getDamage()/20);
	}
	ef(attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::REFILL
	|| attack.getAttack().getWeaponData()->getKind()==Weapon::Kind::STATUS)
	{// 補給・ステータス武器だったら、ある意味固定EXP
		nExp = 200;
	}
	else
	{
		nExp = def.getChara()->getBattle().getExp();
		// 撃墜したか？
		if(attack.getAttack().IsDeath())
		{// した
			int nBP = def.getChara()->getBattle().getPena();

			// NORMALだったらBP1.2倍
			if(p.getScenarioData()->getExpertRank(p.getApp()->getExec().getExpert())==Expert::NORMAL)
				nBP = (nBP*6)/5;

			nBP_ += nBP;
			nFP_ += def.getChara()->getBattle().getFP();
		}
		else
		{// してなかったら、EXPは10分の1
			nExp/=10;
		}
	}

	// Lv差係数を掛けておく
	nExp_ += nExp
			* EXP_LV[LvSub(nLv,def.getChara()->getBattle().getLv())]
			/ 100;
}

void CAttack_result::checkSpirit(Chara::CDataCharaBattle* pBattle)
{
	if(pBattle->IsSpirit(Chara::CValidSpirit::EFFORT))
	{// 努力が掛かっている
		nExp_=nExp_<<1;
		pBattle->spirit(false,Chara::CValidSpirit::EFFORT);
	}

	int nRatio=100;
	if(pBattle->IsSpirit(Chara::CValidSpirit::FORTUNE))
	{// 幸運が掛かっている
		nRatio=200;
		pBattle->spirit(false,Chara::CValidSpirit::FORTUNE);
	}
	else
	{
		// 強運持ちなら、BP1.2倍
		if(pBattle->hasSkill(Ability::LUCKY)>=0)
			nRatio=120;
		// エースなら、BP1.2倍
		if(pBattle->getKill()>=50)
			nRatio=(nRatio*6)/5;
	}
	nBP_=(nBP_*nRatio)/100;
}

void CAttack_result::getItem(CDataCharaSLG* pChara, CSLGContext* p)
{// 持ってるアイテムゲット～
	// アイテム～
	pChara->getBattle().beginItem();
	while(!pChara->getBattle().endItem())
	{// アイテムリストに突っ込む
		int nID = pChara->getBattle().nextItem()->getID();
		listItem_.push_back(nID);
		// ゲットアイテムリストへ投入
		p->addGetItem(nID);
	}
}

void CAttack_result::LvUP(CDataCharaSLG* pChara, CSLGContext& p)
{
	// Lv
	int nLv = pChara->getBattle().getLv();
	int nID = pChara->getBattle().getID();
	Chara::CDataCharaTrain* pTrain = p.getApp()->getExec().getTrainData(nID,false);

	// LvUP計算
	nLv_ = pChara->getBattlePtr()->calcExp(nExp_);
	// セーブデータへ反映
	pTrain->back(pChara->getBattlePtr());

	if(nLv_>0)
	{// LvUPしたら成長
		p.getApp()->getChara().setBattle(pChara->getBattlePtr(),
										nID,
										*pTrain,
										nLv);
	}
}

//////////////////////////////////////////////////////////////
// インターフェイス設定
//////////////////////////////////////////////////////////////
void CAttack_result::setPanelChara(CDataCharaSLG& chara, CSLGContext& p)
{
	// 戦闘結果
	GUI::CPanel* pPanel = pResult_->getWidgetCast<GUI::CPanel>("TITLE_CHARA");
	// 名前
	GUI::CText* pName = pPanel->getWidgetCast<GUI::CText>("CHARANAME");
	pName->setText(p.getApp()->getFaceMap().getName(chara.getBattle().getFaceID()));
	pName->UpdateTextAA();

	// Lv
	GUI::CNum* pLv = pPanel->getWidgetCast<GUI::CNum>("CHARALEVEL");
	pLv->setNum(chara.getBattle().getLv());

	// キャラ
	GUI::CGraphic* pChara = pPanel->getWidgetCast<GUI::CGraphic>("CHARA");
	chara.getMapSymbol()->setGraphic(pChara,"BEFORE_LEFT");
}

void CAttack_result::setPanelLvUp(CDataCharaSLG& chara, CSLGContext& p)
{
	// LVアップ
	// 顔
	pLvUp_->getWidgetCast<GUI::CGraphicFace>("FACE")->setFace(chara.getBattle().getFaceID(),"DEFAULT",&p);
	// 上がる前Lv
	//GUI::CText* pPreLv;
	///pPreLv = pLvUp_->getWidgetCast<GUI::CText>("PRELEVEL");
	//pPreLv->setText(CStringScanner::NumToString(chara.getBattle().getLv()-nLv_));
	//pPreLv->UpdateTextAA();
	pLvUp_->getWidgetCast<GUI::INum>("PRELEVEL")->setNum(chara.getBattle().getLv()-nLv_);
	// 今のLv
	pLvUp_->getWidgetCast<GUI::INum>("UPLEVEL")->setNum(chara.getBattle().getLv());
	
	// ステータス反映
	Status::setStatusBasic(pStatus_,chara,p);
}

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end