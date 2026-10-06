#include "stdafx.h"

#include "ConstChara.h"

#include "CStatusAbility.h"
#include "CDataCharaBase.h"
#include "CDataCharaBattle.h"
#include "CDataCharaInter.h"
#include "CDataCharaTrain.h"

namespace BMW{
namespace Chara{
/////////////////////////////////////////////////
// クリア
/////////////////////////////////////////////////
void CDataCharaTrain::clear()
{
	nID_=-1;
	nLv_=1;
	nExp_=EXP_MAX;
	nKill_=0;
	nPenalty_=-1;
	nFP_=-1;
	fund_.reset();
	battle_.reset();
	nWeapon_=0;
	listSkill_.clear();
	listItem_.clear();
}

/////////////////////////////////////////////////
// 養成データ適用
/////////////////////////////////////////////////
void CDataCharaTrain::apply(CDataCharaBattle* battle)
{// 戦闘用
	applyBase(battle);
	
	// 養成段階にあわせて成長させる
	// 基礎
	// 加算するだけ
	battle->setStrength(	battle->getMaxStrength()	+	getStrength());
	battle->setMagic(		battle->getMaxMagic()		+	getMagic());
	battle->setHit(			battle->getMaxHit()			+	getHit());
	battle->setAvoid(		battle->getMaxAvoid()		+	getAvoid());
	battle->setDefence(		battle->getMaxDefence()		+	getDefence());
	battle->setSkill(		battle->getMaxSkill()		+	getSkill());
	battle->setSP(battle->getMaxSP());

	// 戦闘
	battle->setHP(battle->getMaxHP() + calcPer(battle->getMaxHP(), Const::HP_TRAINING_VALUE, getHP()));
	battle->setEN(battle->getMaxEN() + calcPer(battle->getMaxEN(), Const::EN_TRAINING_VALUE, getEN()));
	battle->setTough(battle->getMaxTough() + calcPer(battle->getMaxTough(), Const::TOUGH_TRAINING_VALUE, getTough()));
	battle->setQuick(battle->getMaxQuick() + calcPer(battle->getMaxQuick(), Const::QUICK_TRAINING_VALUE, getQuick()));

	// アイテム
	beginItem();
	while(!endItem()) battle->addItem(*nextItem());

	// 固有能力
	for(skill_list::iterator it=listTalent_.begin(); it!=listTalent_.end(); ++it)
		battle->addTalent(*it);
}

void CDataCharaTrain::applySkill(CDataCharaBattle* battle)
{
	// 技能
	beginSkill();
	while(!endSkill())	battle->addSkill(*nextSkill(),false);
}

void CDataCharaTrain::apply(CDataCharaInter* inter)
{// インターミッション用
	applyBase(inter);
	// 基礎
	// オフセットにコピー
	//inter->setFundTrain(getFund());
	//inter->setBattleTrain(getBattle());
	//inter->setWeaponTrain(getWeapon());
	// 技能
	//beginSkill();
	//while(!endSkill())	inter->addSkillAcqu(*nextSkill());
}

void CDataCharaTrain::applyBase(CDataCharaBase* base)
{// 共通部分
	base->setLv(getLv());
	base->setExp(getExp());
	base->setKill(getKill());
	if(getPenalty()>=0) base->setPena(getPenalty());
	if(getFP()>=0) base->setFP(getFP());

	// アイテム
//	beginItem();
//	while(!endItem()) base->addItem(*nextItem());
}

/////////////////////////////////////////////////
// 養成データ反映
/////////////////////////////////////////////////
void CDataCharaTrain::back(const CDataCharaBattle* battle)
{// 戦闘データのフィードバック
	// Lv Exp Kill
	setLv(battle->getLv());
	setExp(battle->getExp());
	setKill(battle->getKill());
	// アイテム
	listItem_.clear();
	battle->beginItem();
	while(!battle->endItem()) addItem(*battle->nextItem());
}

void CDataCharaTrain::back(const CDataCharaInter* inter)
{// 養成データのフィードバック
	setFund(inter->getFundTrain());
	setBattle(inter->getBattleTrain());
	setWeapon(inter->getWeaponTrain());
	// 技能
	listSkill_.clear();
	inter->beginSkillAcqu();
	while(!inter->endSkillAcqu()) addSkill(*inter->nextSkillAcqu());
	// アイテム
	listItem_.clear();
	inter->beginItem();
	while(!inter->endItem()) addItem(*inter->nextItem());
}

/////////////////////////////////////////////////
// データ上書き
/////////////////////////////////////////////////
void CDataCharaTrain::over(CDataCharaBattle& battle)
{
	setLv(battle.getLv());
	setExp(battle.getExp());
	setKill(battle.getKill());
	// アイテムは、バトルの方に設定されている
	listItem_.clear();
}

/////////////////////////////////////////////////
// シリアライズ
/////////////////////////////////////////////////
void CDataCharaTrain::Serialize(ISerialize& s)
{
	// キャラID
	s << nID_;

	// 養成データ
	s << nLv_ << nExp_ << nKill_ << fund_ << battle_ << nWeapon_;

	int nSize;
	if(s.IsStoring())
	{// save
		// 後天技能
		// サイズをsave
		nSize = (int)listSkill_.size();
		s << nSize;
		for(it_s=listSkill_.begin(); it_s!=listSkill_.end(); ++it_s)
			s << *it_s;

		// 保持アイテム
		// サイズをsave
		nSize = (int)listItem_.size();
		s << nSize;
		for(it_i=listItem_.begin(); it_i!=listItem_.end(); ++it_i)
			s << *it_i;
	}
	else
	{// load
		int i;
		CStatusAbility ability;
		// 後天技能
		// リストをクリア
		listSkill_.clear();
		// サイズを取得
		s << nSize;
		for(i=0; i<nSize; i++)
		{
			s << ability;
			addSkill(ability);
		}

		// 保持アイテム
		// リストをクリア
		listItem_.clear();
		// サイズを取得
		s << nSize;
		for(i=0; i<nSize; i++)
		{
			s << ability;
			addItem(ability);
		}
	}
}

} // namespace Chara end
} // namespace BMW end