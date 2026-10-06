#include "stdafx.h"

#ifdef BMW_DEBUG

#include "../../Chara/CDataCharaTrain.h"
#include "../../Weapon/CDataWeaponBattle.h"
#include "../../Item/IDItem.h"

#include "../../SLG/IDSLG.h"
#include "../../SLG/Context/CDataBattle.h"
#include "../../SLG/Context/CDataCharaSLG.h"

#include "CDemoTestParser.h"

#include "../CTitleScene.h"

namespace BMW{
namespace Title{

void CTitleScene::setDemo(Task::CTaskContext* pContext)
{
	using namespace boost::spirit;
	using namespace phoenix;

	DemoTest::CDemoTestData data;
	// 構文解析文字列を取得
	CFile file;
	std::string s,p;
	file.Read("demo.xml");
	while(file.ReadLine(s)==0) p.append(s);
	file.Close();

	// 構文解析
	DemoTest::CDemoTestParser ps(data);
	Parser::skip_comment skip;
	parse_info<> r = 
	parse(p.c_str(), ps, skip);
	if(!r.full) CDbg().Out("%s 読み込み失敗！！",r.stop);

	smart_ptr<SLG::CDataBattle>& pBattle = pContext->getBattleData();
	pBattle->clearBattleData();
	pBattle->setSide(data.getSide());
	pBattle->setBack(data.getBack());
	pBattle->event(false);

	// 攻撃
	SLG::CDataBattleBase& attack = pBattle->getBattleData(SLG::CDataBattle::ATTACK);
	smart_ptr<SLG::CDataCharaSLG> pAttack(new SLG::CDataCharaSLG());
	pAttack->getStatePtr()->apper(true);
	Chara::CDataCharaTrain train;
	pContext->getApp()->getChara().setBattle(pAttack->getBattlePtr(), data.getAttackChara(), train);
	pAttack->setPhase(data.getAttackPhase());
	attack.setChara(pAttack);
	smart_ptr<Weapon::CDataWeaponBattle> pWeapon(new Weapon::CDataWeaponBattle());
	pWeapon->setStatus(
		*(const_cast<Weapon::CWeaponDB&>(pContext->getApp()->getWeapon()).getData(data.getAttackWeapon())
		));
	attack.getAttack().setWeaponData(pWeapon);
	attack.getAttack().setDamage(data.getAttackDamage());
	attack.getAttack().setEN(data.getAttackEN());
	//attack.getAttack().ct(true);

	// 反撃
	SLG::CDataBattleBase& def = pBattle->getBattleData(SLG::CDataBattle::COUNTER);
	smart_ptr<SLG::CDataCharaSLG> pCounter(new SLG::CDataCharaSLG());
	pCounter->getStatePtr()->apper(true);
	pContext->getApp()->getChara().setBattle(pCounter->getBattlePtr(), data.getDefChara(), train);
	pCounter->setPhase(data.getDefPhase());
	def.setChara(pCounter);
	def.getDefence().setAction(data.getDefAction());
	//def.getDefence().setAbility(Ability::ROAIAS);

	// 音楽
	if(data.IsBgm())
	{// 音ならすよ
		pContext->getBgmSound()->change(pAttack->getBattle().getBgmID()<0 ? pCounter->getBattle().getBgmID() : pAttack->getBattle().getBgmID());
		pContext->getBgmSound()->FadeIn(15);
	}
	else
	{
		pContext->getBgmSound()->FadeOut(30);
	}
	// se読み直し
	pContext->getApp()->clearSeCache();
}

} // namespace Title end
} // namespace BMW end

#endif