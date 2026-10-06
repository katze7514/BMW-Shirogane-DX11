/*
	katze 05/03/08
	ステータス変化武器の変化値
*/
#pragma once

#include "../Chara/CStatusFund.h"

namespace BMW{
namespace Weapon{

class CStatusWeaponStatus
{/**
	ステータス変化武器のその変化量を表現するクラス
 */
public:
	// コンストラクタ
	CStatusWeaponStatus():nMental_(0){}

	// 設定・取得
	const Chara::CStatusFund&	getFund() const { return fund_; }
	void						setFund(const Chara::CStatusFund& fund){ fund_=fund; }
	int							getMental() const { return nMental_; }
	void						setMental(int nMental){ nMental_=nMental; }

	// 操作
	// 基礎ステ
	int							getStrength() const { return fund_.getStrength(); }
	void						setStrength(int nStrength){ fund_.setStrength(nStrength); }
	int							getMagic() const { return fund_.getMagic(); }
	void						setMagic(int nMagic){ fund_.setMagic(nMagic); }
	int							getHit() const { return fund_.getHit(); }
	void						setHit(int nHit){ fund_.setHit(nHit); }
	int							getAvoid() const { return fund_.getAvoid(); }
	void						setAvoid(int nAvoid){ fund_.setAvoid(nAvoid); }
	int							getDefence() const { return fund_.getDefence(); }
	void						setDefence(int nDefence){ fund_.setDefence(nDefence); }
	int							getSkill() const { return fund_.getSkill(); }
	void						setSkill(int nSkill){ fund_.setSkill(nSkill); }
	int							getSP() const { return fund_.getSP(); }
	void						setSP(int nSP){ fund_.setSP(nSP); }

private:
	Chara::CStatusFund	fund_;	 // 基礎ステ
	int					nMental_; // 気力
};

} // namespace Weapon end
} // namespace BMW end