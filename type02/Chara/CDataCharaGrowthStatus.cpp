#include "stdafx.h"

#include "CDataCharaBattle.h"
#include "CDataCharaInter.h"
#include "CDataCharaGrowthStatus.h"

namespace BMW{
namespace Chara{

void CDataCharaGrowthStatus::apply(CDataCharaBattle* battle, int nLv)
{// ステータス成長
	status_g_list::iterator it;
	beginStatus();
	while(!endStatus())
	{
		it=nextStatus();
		// Lv順に並んでいるので、覚えるLvが
		// 現在値を超えてたら終了
		if(it->getLv() > battle->getLv()) break;
		// 適用済みLv以下だったら、次へ
		if(it->getLv() <= nLv) continue;
		// 加算
		if(battle->getStrength()<CStatusFund::FUND_MAX)	battle->setStrength(	battle->getMaxStrength()	+	it->getStrength());
		if(battle->getMagic()<CStatusFund::FUND_MAX)	battle->setMagic(		battle->getMaxMagic()		+	it->getMagic());
		if(battle->getHit()<CStatusFund::FUND_MAX)		battle->setHit(			battle->getMaxHit()			+	it->getHit());
		if(battle->getAvoid()<CStatusFund::FUND_MAX)	battle->setAvoid(		battle->getMaxAvoid()		+	it->getAvoid());
		if(battle->getDefence()<CStatusFund::FUND_MAX)	battle->setDefence(		battle->getMaxDefence()		+	it->getDefence());
		if(battle->getSkill()<CStatusFund::FUND_MAX)	battle->setSkill(		battle->getMaxSkill()		+	it->getSkill());
		if(battle->getSP()<CStatusFund::FUND_MAX)		battle->setSP(			battle->getMaxSP()			+	it->getSP());
	}
}

void CDataCharaGrowthStatus::apply(CDataCharaInter* inter, int nLv)
{// ステータス成長
	status_g_list::iterator it;
	beginStatus();
	while(!endStatus())
	{
		it=nextStatus();
		// Lv順に並んでいるので、覚えるLvが
		// 現在値を超えてたら終了
		if(it->getLv() > inter->getLv()) break;
		// 適用済みLv以下だったら、次へ
		if(it->getLv() <= nLv) continue;
		// 加算
		inter->setStrength(	inter->getSourceStrength()	+	it->getStrength());
		inter->setMagic(	inter->getSourceMagic()		+	it->getMagic());
		inter->setHit(		inter->getSourceHit()		+	it->getHit());
		inter->setAvoid(	inter->getSourceAvoid()		+	it->getAvoid());
		inter->setDefence(	inter->getSourceDefence()	+	it->getDefence());
		inter->setSkill(	inter->getSourceSkill()		+	it->getSkill());
		inter->setSP(		inter->getMaxSP()			+	it->getSP());
	}
}

} // namespace Chara end
} // namespace BMW end