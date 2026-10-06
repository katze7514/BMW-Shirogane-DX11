/*
	katze 05/05/05
	戦闘時に発動するAbilityフラグクラス
*/
#pragma once

namespace BMW{
namespace SLG{

class CValidAbility
{/**
	戦闘時に発動するAbilityフラグクラス
 */
public:
	// 設定・取得
	set<int>&	getAbilitySet(){ return setAbility_; }
	bool		IsAbility(int nID) const{ return setAbility_.find(nID)!=setAbility_.end(); }
	void		setAbility(int nID){ setAbility_.insert(nID); }
	void		delAbility(int nID){ setAbility_.erase(nID); }
	void		clearAbility(){ setAbility_.clear(); }

private:
	set<int> setAbility_;
};

} // namespace SLG end
} // namespace BMW end