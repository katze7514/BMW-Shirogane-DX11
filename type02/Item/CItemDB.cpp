#include "stdafx.h"

#include "../GUI/DB/CGuiDefDB.h"

#include "IDItem.h"
#include "Item.h"
#include "CItemDB.h"

namespace BMW{
namespace Item{

void CItemDB::setAbilityDB(const string& sFile)
{
	pGuiDef_->setGuiDef(sFile);

	// MEDI
	addData(new CItem_Medi("MEDI"),MEDI);

	// CURRY
	addData(new CItem_Curry("CURRY"),CURRY);

	// SHIRO
	addData(new CItem_Shiro("SHIRO"),SHIRO);

	// MAGAZINE
	addData(new CItem_Magazine("MAGAZINE"),MAGAZINE);

	// MABO
	addData(new CItem_Mabo("MABO"),MABO);

	// OTSU
	addData(new CItem_Otsu("OTSU"),OTSU);

	// WOTSU
	addData(new CItem_Wotsu("WOTSU"),WOTSU);

	// NEKO
	addData(new CItem_Neko("NEKO"),NEKO);

	// HOUI
	addData(new CItem_Houi("HOUI"),HOUI);

	// BOOT
	addData(new CItem_Boot("BOOT"),BOOT);

	// KYUDO
	addData(new CItem_Kyudo("KYUDO"),KYUDO);

	// MAID
	addData(new CItem_Maid("MAID"),MAID);

	// MEGANE
	addData(new CItem_Megane("MEGANE"),MEGANE);

	// ROD
	addData(new CItem_Rod("ROD"),ROD);

	// GEM
	addData(new CItem_Gem("GEM"),GEM);

	// EARRING
	addData(new CItem_Earring("EARRING"),EARRING);

	// RIBBON
	addData(new CItem_Ribbon("RIBBON"),RIBBON);

	// TIGER
	addData(new CItem_Tiger("TIGER"),TIGER);

	// SABER
	addData(new CItem_Saber("SABER"),SABER);

	// LANCER
	addData(new CItem_Lancer("LANCER"),LANCER);

	// ARCHER
	addData(new CItem_Archer("ARCHER"),ARCHER);

	// RIDER
	addData(new CItem_Rider("RIDER"),RIDER);

	// CASTER
	addData(new CItem_Caster("CASTER"),CASTER);

	// ASSASSIN
	//addData(new CItem_Assassin("ASSASSIN"),ASSASSIN);

	// BERSERKER
	addData(new CItem_Berserker("BERSERKER"),BERSERKER);

	// TRUE_ASSASSIN
	addData(new CItem_True_assassin("TRUE_ASSASSIN"),TRUE_ASSASSIN);

	// GIRU
	addData(new CItem_Giru("GIRU"),GIRU);

	// AVENGER
	addData(new CItem_Avenger("AVENGER"),AVENGER);

	// A_IJO
	addData(new CItem_Agonist("AGONIST"),AGONIST);
}

bool CItemDB::IsUse(int nID)
{
	switch(nID)
	{
	case MEDI:
	case CURRY:
	case SHIRO:
	case MAGAZINE:
	case MABO:
		return true;

	default: return false;
	}
}

bool CItemDB::IsStatus(int nID)
{
	switch(nID)
	{
	case OTSU:
	case WOTSU:
	case NEKO:
	case HOUI:
	case BOOT:
	case MAID:
	case GEM:
	case TIGER:
	case LANCER:
	case RIDER:
	case BERSERKER:
	case AGONIST:
		return true;

	default: return false;
	}
}

bool CItemDB::IsWeapon(int nID)
{
	switch(nID)
	{
	case ARCHER:
	case KYUDO:
	case MEGANE:
	case ROD:
	case TRUE_ASSASSIN:
	case AGONIST:
		return true;

	default: return false;
	}
}

} // namespace Item end
} // namespace BMW end