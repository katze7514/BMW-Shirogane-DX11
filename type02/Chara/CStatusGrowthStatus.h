/*
	katze 05/03/01
	ステータス成長
*/
#pragma once

#include "CStatusFund.h"

namespace BMW{
namespace Chara{

class CStatusGrowthStatus
{/**
	ステータス成長
 */
public:
	// コンストラクタ
	CStatusGrowthStatus():nLv_(INT_MAX){}

	// 設定・取得
	int					getLv() const { return nLv_; }
	void				setLv(int nLv){ nLv_=nLv; }
	const CStatusFund&	getFund() const { return fund_; }
	void				setFund(const CStatusFund& fund){ fund_=fund; }

	// 操作
	int					getStrength() const { return fund_.getStrength(); }
	void				setStrength(int nStrength){ fund_.setStrength(nStrength); }
	int					getMagic() const { return fund_.getMagic(); }
	void				setMagic(int nMagic){ fund_.setMagic(nMagic); }
	int					getHit() const { return fund_.getHit(); }
	void				setHit(int nHit){ fund_.setHit(nHit); }
	int					getAvoid() const { return fund_.getAvoid(); }
	void				setAvoid(int nAvoid){ fund_.setAvoid(nAvoid); }
	int					getDefence() const { return fund_.getDefence(); }
	void				setDefence(int nDefence){ fund_.setDefence(nDefence); }
	int					getSkill() const { return fund_.getSkill(); }
	void				setSkill(int nSkill){ fund_.setSkill(nSkill); }
	int					getSP() const { return fund_.getSP(); }
	void				setSP(int nSP){ fund_.setSP(nSP); }

private:
	// 適用するLv
	int nLv_;
	// 成長度合い
	CStatusFund fund_;
};

// 良く使うパターンのtypedef
typedef list<CStatusGrowthStatus> status_g_list;

} // namespace Chara end
} // namespace BMW end