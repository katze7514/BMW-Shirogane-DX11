#include "stdafx.h"

#include "../../GUI/DB/CGuiDefDB.h"

#include "../IDataAbility.h"
#include "../Ability.h"

#include "CAbilityDB.h"

namespace BMW{
namespace Ability{

CAbilityDB::CAbilityDB()
{
	pGuiDef_ = new GUI::CGuiDefDB();
}

CAbilityDB::~CAbilityDB()
{
	DELETE_SAFE(pGuiDef_);

	ability_map::iterator it;
	for(it=mapAbility_.begin(); it!=mapAbility_.end(); it++)
		DELETE_SAFE(it->second);

	mapAbility_.clear();
}

void CAbilityDB::setAbilityDB(const string& sFile)
{
	pGuiDef_->setGuiDef(sFile);

	// SPECTER
	addData(new CAbility_Specter("SPECTER"),SPECTER);

	// MAGICIAN
	addData(new CAbility_Magician("MAGICIAN"),MAGICIAN);

	// VAMPIRE
	addData(new CAbility_Vampire("VAMPIRE"),VAMPIRE);

	// FUNDPOWER
	addData(new CAbility_Fundpower("FUNDPOWER"),FUNDPOWER);

	// COUNTER
	addData(new CAbility_Counter("COUNTER"),COUNTER);

	// BACKUPATTACK
	addData(new CAbility_Backupattack("BACKUPATTACK"),BACKUPATTACK);

	// BACKUPDEFENCE
	addData(new CAbility_Backupdefence("BACKUPDEFENCE"),BACKUPDEFENCE);

	// LINKAGEATTACK
	addData(new CAbility_Linkageattack("LINKAGEATTACK"),LINKAGEATTACK);

	// ATTACKER
	addData(new CAbility_Attacker("ATTACKER"),ATTACKER);

	// REVENGE
	addData(new CAbility_Revenge("REVENGE"),REVENGE);

	// GUARD
	addData(new CAbility_Guard("GUARD"),GUARD);

	// BREAKLINE
	addData(new CAbility_Breakline("BREAKLINE"),BREAKLINE);

	// HITAWAY
	addData(new CAbility_Hitaway("HITAWAY"),HITAWAY);

	// MAGICSAVE
	addData(new CAbility_Magicsave("MAGICSAVE"),MAGICSAVE);

	// BATTLESPIRIT
	addData(new CAbility_Battlespirit("BATTLESPIRIT"),BATTLESPIRIT);

	// FIGHTUP
	addData(new CAbility_Fightup("FIGHTUP"),FIGHTUP);

	// RITHM
	addData(new CAbility_Rithm("RITHM"),RITHM);

	// AGAINST
	addData(new CAbility_Against("AGAINST"),AGAINST);

	// SPUP
	addData(new CAbility_Spup("SPUP"),SPUP);

	// SPRECOVER
	addData(new CAbility_Sprecover("SPRECOVER"),SPRECOVER);

	// CONCENT
	addData(new CAbility_Concent("CONCENT"),CONCENT);

	// LUCKY
	addData(new CAbility_Lucky("LUCKY"),LUCKY);

	// MOVE_UP
	addData(new CAbility_Moveup("MOVE_UP"),MOVE_UP);

	// BALLETSAVE
	addData(new CAbility_Balletsave("BALLETSAVE"),BALLETSAVE);

	// IKIYOYO
	addData(new CAbility_Ikiyoyo("IKIYOYO"),IKIYOYO);

	// MAGICBARRIER_A
	addData(new CAbility_Magicbarrier_a("MAGICBARRIER_A"),MAGICBARRIER_A);

	// MAGICBARRIER_B
	addData(new CAbility_Magicbarrier_b("MAGICBARRIER_B"),MAGICBARRIER_B);

	// MAGICBARRIER_C
	addData(new CAbility_Magicbarrier_c("MAGICBARRIER_C"),MAGICBARRIER_C);

	// MAGICRELEASE
	addData(new CAbility_Magicrelease("MAGICRELEASE"),MAGICRELEASE);

	// AVALON
	addData(new CAbility_Avalon("AVALON"),AVALON);

	// ARROWBLESS
	addData(new CAbility_Arrowbless("ARROWBLESS"),ARROWBLESS);

	// BATTLEFOLLOW
	addData(new CAbility_Battlefollow("BATTLEFOLLOW"),BATTLEFOLLOW);

	// ROAIAS
	addData(new CAbility_Roaias("ROAIAS"),ROAIAS);

	// SUPERPOWER
	addData(new CAbility_Superpower("SUPERPOWER"),SUPERPOWER);

	// DEMON
	addData(new CAbility_Demon("DEMON"),DEMON);

	// TWELVECROSS
	addData(new CAbility_Twelvecross("TWELVECROSS"),TWELVECROSS);

	// DEATH
	addData(new CAbility_Death("DEATH"),DEATH);

	// ORIGIN
	addData(new CAbility_Origin("ORIGIN"),ORIGIN);

	// MADRED
	addData(new CAbility_Madred("MADRED"),MADRED);

	// SYNC
	//addData(new CAbility_Sync("SYNC"),SYNC);

	// DIVISION
	addData(new CAbility_Division("DIVISION"),DIVISION);

	// GENUINECUT
	addData(new CAbility_Death_True("DEATH_TRUE"),DEATH_TRUE);

	// SCAEPGOAT
	addData(new CAbility_Scaepgoat("SCAPEGOAT"),SCAPEGOAT);

	// FUTUREEYE
	addData(new CAbility_Futureeye("FUTUREEYE"),FUTUREEYE);

	// FLY
	addData(new CAbility_Fly("FLY"),FLY);

	// FLOAT
	addData(new CAbility_Float("FLOAT"),FLOAT);

	// HP_RECOVER_S
	addData(new CAbility_Hp_recover_s("HP_RECOVER_S"),HP_RECOVER_S);

	// HP_RECOVER_M
	addData(new CAbility_Hp_recover_m("HP_RECOVER_M"),HP_RECOVER_M);

	// HP_RECOVER_L
	addData(new CAbility_Hp_recover_l("HP_RECOVER_L"),HP_RECOVER_L);

	// EN_RECOVER_S
	addData(new CAbility_En_recover_s("EN_RECOVER_S"),EN_RECOVER_S);

	// EN_RECOVER_M
	addData(new CAbility_En_recover_m("EN_RECOVER_M"),EN_RECOVER_M);

	// EN_RECOVER_L
	addData(new CAbility_En_recover_l("EN_RECOVER_L"),EN_RECOVER_L);

	// ALTER_EGO
	addData(new CAbility_AlterEgo("ALTER_EGO"),ALTER_EGO);

	// COND_IGNORE
	addData(new CAbility_CondIgnore("COND_IGNORE"),COND_IGNORE);

	// LOLI
	addData(new CAbility_Loli("LOLI"),LOLI);

	// DEKA
	addData(new CAbility_Deka("DEKA"),DEKA);

	// DEATH_EX
	addData(new CAbility_Death_Ex("DEATH_EX"),DEATH_EX);

	// FUTOU
	addData(new CAbility_Futou("FUTOU"),FUTOU);

	// FIELD_IGNORE
	addData(new CAbility_Field_Ignore("FIELD_IGNORE"),FIELD_IGNORE);

	// KEHAI
	addData(new CAbility_Kehai("KEHAI_SHADAN"),KEHAI_SHADAN);

	// FUTSUNO
	addData(new CAbility_Futsuno("FUTSUNO"),FUTSUNO);

	// OPEN_GET
	addData(new CAbility_OpenGet("OPEN_GET"),OPEN_GET);

	// CHALICE_CONECT
	addData(new CAbility_ChaliceConect("CHALICE_CONECT"),CHALICE_CONECT);

	// MUDAI_SHIELD
	addData(new CAbility_MudaiShield("MUDAI_SHIELD"),MUDAI_SHIELD);

	// HIYOKURENRI
	addData(new CAbility_Hiyokurenri("HIYOKURENRI"),HIYOKURENRI);

	// MEKA_BARRIAR
	addData(new CAbility_MekaBarriar("MEKA_BARRIAR"),MEKA_BARRIAR);

	// TRUE_DIVISION
	addData(new CAbility_Division_True("TRUE_DIVISION"),TRUE_DIVISION);

	// TWICE_ACTION
	addData(new CAbility_TwiceAction("TWICE_ACTION"),TWICE_ACTION);

	// FUTUREEYE_SECOND
	addData(new CAbility_Futureeye_Second("FUTUREEYE_SECOND"),FUTUREEYE_SECOND);

	// COLA_BARRIAR
	addData(new CAbility_Colabarriar("COLA_BARRIAR"),COLA_BARRIAR);

	// LAND_IGNORE
	addData(new CAbility_Land_Ignore("LAND_IGNORE"),LAND_IGNORE);

	// TWICE_ATTACK
	addData(new CAbility_TwiceAttack("TWICE_ATTACK"),TWICE_ATTACK);

	// TWICE_ATTACK_ENEMY
	addData(new CAbility_TwiceAttack_enemy("TWICE_ATTACK_ENEMY"),TWICE_ATTACK_ENEMY);

	// MEKA_BARRIAR_WEAK
	addData(new CAbility_MekaBarriarWeak("MEKA_BARRIAR_WEAK"),MEKA_BARRIAR_WEAK);
}

int CAbilityDB::getGetFP(int nID, int nAttr)const
{
	CAbilityDB* db = const_cast<CAbilityDB*>(this);
	IDataAbility* pData = db->getData(nID);
	return pData!=NULL ? pData->getGetFP(nAttr) : 0;
}

int CAbilityDB::getEN(int nID, int nAttr)const
{
	CAbilityDB* db = const_cast<CAbilityDB*>(this);
	IDataAbility* pData = db->getData(nID);
	return pData!=NULL ? pData->getEN(nAttr) : 0;
}

////////////////////////////////////////////////
// ステータス系
////////////////////////////////////////////////
void CAbilityDB::applyStatus(SLG::CDataCharaSLG& slg, int nAttr, int nID)
{
	getData(nID)->applyStatus(slg,nAttr);
}
	
void CAbilityDB::backStatus(SLG::CDataCharaSLG& slg, int nAttr, int nID)
{
	getData(nID)->backStatus(slg,nAttr);
}

void CAbilityDB::applyStatus(Chara::CDataCharaInter& inter, int nAttr, int nID)
{
	getData(nID)->applyStatus(inter,nAttr);
}

void CAbilityDB::backStatus(Chara::CDataCharaInter& inter, int nAttr, int nID)
{
	getData(nID)->backStatus(inter,nAttr);
}

void CAbilityDB::applyWeapon(Weapon::CDataWeaponBattle& weapon,int nAttr, int nID)
{
	getData(nID)->applyWeapon(weapon,nAttr);
}

///////////////////////////////////////////////////////////////////
// 使用系
///////////////////////////////////////////////////////////////////
bool CAbilityDB::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID)
{
	return getData(nID)->enable(slg,nAttr,p);
}

bool CAbilityDB::enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID)
{
	return getData(nID)->enableTarget(slg,nAttr,p);
}

bool CAbilityDB::enable(const SLG::CDataCharaSLG& base, const SLG::CDataCharaSLG& target, int nAttr, SLG::CSLGContext& p, int nID)
{
	return getData(nID)->enable(base,target,nAttr,p);
}

void CAbilityDB::use(SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p, int nID)
{
	getData(nID)->use(slg,nAttr,p);
}

///////////////////////////////////////////////////////////////////
// GuiDefDB
///////////////////////////////////////////////////////////////////
const string& CAbilityDB::getGuiDefID(int nID)
{
	return getData(nID)->getGuiDefID();
}

void CAbilityDB::setGraphicHolder(GUI::CGraphicPopUp* pGraphic, int nID)
{
	if(nID<0) return;
	pGuiDef_->setGraphicHolder(pGraphic, getGuiDefID(nID)+"_G");
}

void CAbilityDB::setTextPopUpHolder(GUI::CTextPopUp* pText, int nID)
{
	if(nID<0) return;
	pGuiDef_->setTextPopUpHolder(pText, getGuiDefID(nID)+"_T");
}

void CAbilityDB::setButtonHolder(GUI::CButtonSymbol* pButton, int nID, const GUI::CButton::ButtonEvent& fun, int nValue)
{
	if(nID<0) return;
	pGuiDef_->setButtonHolder(pButton, getGuiDefID(nID)+"_B", fun, nValue);
}

void CAbilityDB::setButtonHolder(GUI::CButtonSymbol* pButton, int nID)
{
	if(nID<0) return;
	pGuiDef_->setButtonHolder(pButton, getGuiDefID(nID)+"_B");
}

} // namespace Ability end
} // namespace BMW end