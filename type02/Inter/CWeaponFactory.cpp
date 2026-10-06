#include "stdafx.h"

#include "../Chara/CDataCharaInter.h"
#include "../Weapon/CDataWeaponBattle.h"

#include "CInterContext.h"

#include "CWeaponFactory.h"

namespace BMW{
namespace Inter{

CWeaponFactory::~CWeaponFactory()
{
	map<int,Weapon::CDataWeaponBattle*>::iterator it;
	for(it=mapWeapon_.begin(); it!=mapWeapon_.end(); it++)
		DELETE_SAFE(it->second);

	mapWeapon_.clear();
}

Weapon::CDataWeaponBattle* CWeaponFactory::createWeapon(int nID, BMW::Chara::CDataCharaInter* pChara, Task::CTaskContext& p)
{
	// マップにあるってことは設定済み
	map<int,Weapon::CDataWeaponBattle*>::iterator it = mapWeapon_.find(nID);
	if(it!=mapWeapon_.end()) return it->second;

	// マップになかったら新しく生成
	Weapon::CDataWeaponInit* pInit = const_cast<Weapon::CWeaponDB&>(p.getApp()->getWeapon()).getData(nID);
	if(pInit==NULL) return NULL;
	
	switch(pInit->getKind())
	{
	case Weapon::Kind::FIGHT_COLLAB:
	case Weapon::Kind::MAGIC_COLLAB:
	// 合体攻撃は表示しない
		++nOut_;
		return NULL;

	case Weapon::Kind::CURE:
	case Weapon::Kind::REFILL:
	case Weapon::Kind::STATUS:
		++nOut_;
	default:
	{
		Weapon::CDataWeaponBattle* pBattle;
		pBattle = new Weapon::CDataWeaponBattle();
		mapWeapon_.insert(pair<int,Weapon::CDataWeaponBattle*>(nID,pBattle));
		// IDの設定
		pBattle->setID(nID);
		// ステータスの設定
		pBattle->setStatus(*pInit);
		// 弾数があるなら、弾数を設定
		if(pInit->getBallet()>0)
		{
			// 弾数増強持ってる？
			if(pChara->hasSkill(Ability::BALLETSAVE)>=0)
			{// 持ってたら1.5倍（切り上げ）
				pBattle->setBallet((int)ceil(((double)pBattle->getBallet()*3.0)/2.0));
			}
			pBattle->setBalletRest(pBattle->getBallet());
		}

		// 魔力節約持ってる？
		if(pChara->hasSkill(Ability::MAGICSAVE)>=0)
		{// 持ってたら消費EN20%カット
			pBattle->setEN((pBattle->getEN()*40)/50);
		}

		// 武器養成
		// 養成適用
		int nValue=0;
		for(int i=1; i<=pChara->getWeaponTrain(); i++)
			nValue+=Weapon::Const::WEAPON_TRAINING_VALUE[pInit->getTrainingType()][i];

		pBattle->setAttack(pInit->getAttack()+nValue);

		// アイテム効果適用
		Item::CItemDB& db = p.getApp()->getItem();
		pChara->beginItem();
		while(!pChara->endItem())
		{
			Chara::CStatusAbility& item = *pChara->nextItem();
			if(db.IsWeapon(item.getID()))
				db.applyWeapon(*pBattle,item.getAttr(),item.getID());
		}

		// ランク
		if(pInit->getKind()==Weapon::Kind::FIGHT)
			strMap_.insert(pair<int, Weapon::CDataWeaponBattle*>(pBattle->getAttack(),pBattle));
		ef(pInit->getKind()==Weapon::Kind::MAGIC)
			mgcMap_.insert(pair<int, Weapon::CDataWeaponBattle*>(pBattle->getAttack(),pBattle));

		return pBattle;
	}
	}

	return NULL;
}

void CWeaponFactory::updateRank()
{
	// 攻撃力に合わせてRank計算
	map<int,Weapon::CDataWeaponBattle*>::iterator it;
	int i=0;
	// 格闘
	for(it=strMap_.begin(); it!=strMap_.end(); ++it,++i)
		it->second->setRank(i);
	// 魔術
	i=0;
	for(it=mgcMap_.begin(); it!=mgcMap_.end(); ++it,++i)
		it->second->setRank(i);
}

} // namespace Inter end
} // namespace BMW end