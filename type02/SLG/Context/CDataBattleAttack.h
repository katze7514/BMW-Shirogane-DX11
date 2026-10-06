/*
	katze 05/05/04
	攻撃側戦闘データクラス
*/
#pragma once

#include "CDataBattleAbility.h"

namespace BMW{

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{

class CDataBattleAttack : public CDataBattleAbility
{/**
	攻撃側戦闘データクラス

	これがapplyなどで利用される
 */
public:
	// コンストラクタ
	CDataBattleAttack():nEn_(0),nDamage_(0),nDamageEN_(0),bCT_(false),bDeath_(false){}

	// 設定・取得
	smart_ptr<Weapon::CDataWeaponBattle>&	getWeaponData(){ return pWeapon_; }
	void									setWeaponData(const smart_ptr<Weapon::CDataWeaponBattle>& weapon){ pWeapon_ = weapon; }

	int		getDamage() const { return nDamage_; }
	void	setDamage(int nDamage){ nDamage_=nDamage; }
	int		getDamageEN() const { return nDamageEN_; }
	void	setDamageEN(int nDamageEN){ nDamageEN_=nDamageEN; }
	int		getEN() const { return nEn_; }
	void	setEN(int nEN){ nEn_=nEN; }
	bool	IsCT() const { return bCT_; }
	void	ct(bool bCT){ bCT_=bCT; }
	bool	IsDeath() const { return bDeath_; }
	void	death(bool bDeath){ bDeath_=bDeath; }

	// 操作
	void	clearData(){ CDataBattleAbility::clearData(); nEn_=0; nDamage_=0; nDamageEN_=0; bCT_=false; bDeath_=false; }

protected:
	// データ
	smart_ptr<Weapon::CDataWeaponBattle>	pWeapon_;

	// 演算結果
	int				nDamage_;	// 相手に与えるダメージ
	int				nDamageEN_;	// 相手に与えるEN
	int				nEn_;		// 消費EN
	bool			bCT_;		// CTが発生するかどうか
	bool			bDeath_;	// この攻撃で相手を倒すのか？
};

} // namespace SLG end
} // namespace BMW end