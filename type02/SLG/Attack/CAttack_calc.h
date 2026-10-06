/*
	katze 05/05/07
	í“¬ŒnŒvZ
*/
#pragma once

#include "../../Misc/CRandLottery.h"

namespace BMW{
namespace SLG{
class CSLGContext;
class CDataCharaSLG;

namespace Attack{

class CAttack_calc
{/**
	í“¬ŒnŒvZ
 */
public:
	// ŒvZ–½’†—¦ŒvZ
	// ‘M”­“®‚È‚ç-1
	// •K’†”­“®‚È‚ç-2‚ª•Ô‚é
	enum eHit{
		HIT=-2,
		AVOID,
	};
	static int calcHit(CDataCharaSLG& attack, const Weapon::CDataWeaponBattle* pAttack,
					   CDataCharaSLG& def,
					   int nDist,
					   int nHeight,
					   CSLGContext& p,
					   bool bBackUp=false);
	static int calcSkillHit(CDataCharaSLG& attack, CDataCharaSLG& def, CSLGContext& p);
	static int calcSkillAvoid(CDataCharaSLG& def, CDataCharaSLG& atk, CSLGContext& p, bool bT=false);

	static int calcOffHit(CDataCharaSLG& attack, CDataCharaSLG& def, CSLGContext& p, bool bBackup=false);

	// –½’†’Š‘IŠí
	static katzeSDK::Math::CRandLottery randLot_;

	enum eFlag{
		ATTACK,		// ’ÊíŒvZ
		COUNTER,	// ƒJƒEƒ“ƒ^[ŒvZ
		ATTACK_B,	// ‰‡ŒìUŒ‚ŒvZ
		DEF_B,		// ‰‡Œì–hŒäŒvZ
	};
	static int calcDamage(CDataCharaSLG& attack,const Weapon::CDataWeaponBattle* pAttack,
						  CDataCharaSLG& def,	const Weapon::CDataWeaponBattle* pDef, 
						  int nFlag, CSLGContext& p, int nDist);

	static int calcTough(CDataCharaSLG& def, CSLGContext& p);

	static int calcCT(CDataCharaSLG& attack,const Weapon::CDataWeaponBattle* pAttack,
					  CDataCharaSLG& def, CSLGContext& p);
	static int calcSkillCT(CDataCharaSLG& attack, CSLGContext& p);
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end