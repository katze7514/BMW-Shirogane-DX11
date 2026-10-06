#include "stdafx.h"

#include "../Chara/CDataCharaTrain.h"

#include "../SLG/IDSLG.h"
#include "../SLG/Context/CSLGContext.h"
#include "../SLG/Context/CDataCharaSLG.h"

#include "IDWeapon.h"
#include "Weapon.h"

#include "CalcWeapon.h"

namespace BMW{
namespace Weapon{

CDataWeaponBattle* createWeapon(int nID)
{
	switch(nID)
	{// 種別別生成
	case Kind::FIGHT_COLLAB:
	case Kind::MAGIC_COLLAB:
		return new CDataWeaponBattleCollab();

	case Kind::STATUS:
		return new CDataWeaponBattleStatus();

	case Kind::FIGHT_COND:
	case Kind::MAGIC_COND:
		return new CDataWeaponBattleCond();

	case Kind::CURE:
		return new CDataWeaponBattleCure();

	case Kind::REFILL:
		return new CDataWeaponBattleRefill();

	default: // 通常
		return new CDataWeaponBattle();
	}
}

void setWeaponData(Weapon::CDataWeaponBattle* pBattle, SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, bool bCont)
{
	// 弾数
	// 弾数増強持ってる？
	if(chara.getBattle().hasSkill(Ability::BALLETSAVE)>=0)
	// 持ってたら1.5倍（切り上げ）
		pBattle->setBallet((int)ceil(((double)pBattle->getBallet()*3.0)/2.0));
	
	// コンテニュー時のデータ生成だったら、必要無し
	if(!bCont && pBattle->getBallet()>0)
		pBattle->setBalletRest(pBattle->getBallet());

	// 魔力節約持ってる？
	if(chara.getBattle().hasSkill(Ability::MAGICSAVE)>=0)
	{// 持ってたら消費EN20%カット
		pBattle->setEN((pBattle->getEN()*40)/50);
	}

	if(pBattle->getKind()!=Weapon::Kind::STATUS
	&& pBattle->getKind()!=Weapon::Kind::CURE
	&& pBattle->getKind()!=Weapon::Kind::REFILL)
	{// ステータス武器には適用意味無し
		// 武器養成
		int nTrain = chara.getTrain();
		int nWeapon; // 武器養成段階
		if(nTrain<0)
		{// 負だったらセーブデータから
			nWeapon =
				(context.getApp()->getExec().getTrainData(chara.getBattle().getID()))->getWeapon();
		}
		else
		{// 正だったら、一時養成データから
			nWeapon = context.getTrain(nTrain).getWeapon();
		}

		// 周回だったらここで足しておく
		if((context.getApp()->getExec().getFlag("HANDOVER",1) || context.getApp()->getExec().getFlag("HANDOVER",2)) && chara.getPhase()==SLG::Phase::ENEMY)
		{
			int nEnemyTrain=0;
			context.getApp()->getExec().getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nEnemyTrain);
			nWeapon += nEnemyTrain;
		}

		// 養成適用
		int nValue=0;
		for(int i=1; i<=nWeapon; i++)
			nValue += Weapon::Const::WEAPON_TRAINING_VALUE[pBattle->getTrainingType()][i];

		pBattle->setAttack(pBattle->getAttack()+nValue);
	}
	
	// アイテム効果の適用
	if(pBattle->getKind()!=Weapon::Kind::CURE
	&& pBattle->getKind()!=Weapon::Kind::REFILL)
	{
		Item::CItemDB& db = context.getApp()->getItem();
		Chara::CDataCharaBattle& battle = chara.getBattle();
		battle.beginItem();
		while(!battle.endItem())
		{
			Chara::CStatusAbility& item = *battle.nextItem();
			if(db.IsWeapon(item.getID()))
				db.applyWeapon(*pBattle, item.getAttr(), item.getID());
		}
	}
}

void setWeaponData(CDataWeaponBattleCollab* pBattle, SLG::CDataCharaSLG& chara, SLG::CSLGContext& context, bool bCont)
{// 合体攻撃時は、養成の適用の仕方が変わるのだ！
	// 弾数
	// 弾数増強持ってる？
	if(chara.getBattle().hasSkill(Ability::BALLETSAVE)>=0)
	// 持ってたら1.5倍（切り上げ）
		pBattle->setBallet((int)ceil(((double)pBattle->getBallet()*3.0)/2.0));
	
	// コンテニュー時のデータ生成だったら、必要無し
	if(!bCont && pBattle->getBallet()>0)
		pBattle->setBalletRest(pBattle->getBallet());

	// 魔力節約持ってる？
	if(chara.getBattle().hasSkill(Ability::MAGICSAVE)>=0)
	{// 持ってたら消費EN20%カット
		pBattle->setEN((pBattle->getEN()*40)/50);
	}
	
	// まずは、全キャラの養成値平均を取得
	float fWeapon=0;
	int nTrain;
	int nSize=0;
	set<int>& setID = pBattle->getSlgIDSet();
	set<int>::iterator it;
	SLG::CDataCharaSLG* pChara;
	for(it=setID.begin(); it!=setID.end(); ++it)
	{	// 対象キャラデータ取得
		++nSize;
		pChara = context.getCharaData(*it);

		if(pChara==NULL)
		{// って、なぜかいなくなってるし！！
			// ロード済みリストから消す
			setID.erase(it);
			// んで、どうせ使えないからここでリターン
			return;
		}

		nTrain = pChara->getTrain();
		if(nTrain<0)
		{// 負だったらセーブデータから
			fWeapon +=
				(context.getApp()->getExec().getTrainData(pChara->getBattle().getID()))->getWeapon();
		}
		else
		{// 正だったら、一時養成データから
			fWeapon += context.getTrain(nTrain).getWeapon();
		}

		// 周回だったらここで足しておく
		if((context.getApp()->getExec().getFlag("HANDOVER",1) || context.getApp()->getExec().getFlag("HANDOVER",2)) && chara.getPhase()==SLG::Phase::ENEMY)
		{
			int nEnemyTrain=0;
			context.getApp()->getExec().getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nEnemyTrain);
			fWeapon += nEnemyTrain;
		}
	}

	fWeapon /= nSize;
	int nWeapon = ::floor(fWeapon);
	// 養成値計算
	int nValue=0;
	for(int i=1; i<=nWeapon; ++i)
		nValue += Weapon::Const::WEAPON_TRAINING_VALUE[pBattle->getTrainingType()][i];

	// 端数は次の養成段階値に掛け算
	fWeapon = fWeapon - nWeapon;
	nValue += Weapon::Const::WEAPON_TRAINING_VALUE[pBattle->getTrainingType()][nWeapon+1] * fWeapon;

	// 養成！
	pBattle->setAttack(pBattle->getAttack()+nValue);

	// アイテム効果の適用
	Item::CItemDB& db = context.getApp()->getItem();
	Chara::CDataCharaBattle& battle = chara.getBattle();
	battle.beginItem();
	while(!battle.endItem())
	{
		Chara::CStatusAbility& item = *battle.nextItem();
		if(db.IsWeapon(item.getID()))
			db.applyWeapon(*pBattle, item.getAttr(), item.getID());
	}
}

int getIconID(int nKind, GUI::CPanelCtrl* pCtrl)
{
	switch(nKind)
	{
	case Kind::FIGHT:		return pCtrl->getID("KAKUTOU1");
	case Kind::MAGIC:		return pCtrl->getID("MAJUTSU1"); 
	case Kind::STATUS:		return pCtrl->getID("UP");
	case Kind::FIGHT_COND:
	case Kind::MAGIC_COND:	return pCtrl->getID("JOUTAI");
	case Kind::CURE:		return pCtrl->getID("CURE");	
	case Kind::REFILL:		return pCtrl->getID("REFILL");	
	default:				return pCtrl->getID("GATTAI"); 
	}
}

int getAttrID(bool bP, bool bM, bool bT, bool bF, GUI::CPanelCtrl* pCtrl)
{
	if(bP&&bM&&bF)	return pCtrl->getID("FPM");
	ef(bP&&bM)		return pCtrl->getID("PM");
	ef(bP&&bT)		return pCtrl->getID("PT");
	ef(bP&&bF)		return pCtrl->getID("FP");
	ef(bP)			return pCtrl->getID("P");
	if(bM&&bF)		return pCtrl->getID("FM");
	ef(bM)			return pCtrl->getID("M");
	if(bT&&bF)		return pCtrl->getID("FT");
	ef(bT)			return pCtrl->getID("T");
	ef(bF)			return pCtrl->getID("F");
	else			return -1;
}

} // namespace Weapon end
} // namespace BMW end