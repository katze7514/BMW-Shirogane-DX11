/*
	katze 05/02/28
	技能などの成長データ
*/
#pragma once

#include "CStatusAbility.h"

namespace BMW{
namespace Chara{

class CStatusGrowthAbility
{/**
	個々の成長データ
	取得するLvが追加されるだけだけどねｗ
 */
public:
	// コンストラクタ
	CStatusGrowthAbility():nLv_(INT_MAX){}

	// 設定・取得
	int	 getLv() const { return nLv_; }
	void setLv(int nLv){ nLv_=nLv; }
	const CStatusAbility&	getAbility(){ return ability_; }
	void					setAbility(const CStatusAbility& ability){ ability_=ability; }

	// 操作
	int  getAbilityID() const { return ability_.getID(); }
	void setAbilityID(int nID){ ability_.setID(nID); }
	int  getAbilityAttr() const { return ability_.getAttr(); }
	void setAbilityAttr(int nAttr){ ability_.setAttr(nAttr); }

private:
	// 取得Lv
	int				nLv_;
	// 取得するアビリティ
	CStatusAbility	ability_;
};

// 良く使うパターンのtypedef
typedef list<CStatusGrowthAbility> skill_g_list;

} // namespace Chara end
} // namespace BMW end