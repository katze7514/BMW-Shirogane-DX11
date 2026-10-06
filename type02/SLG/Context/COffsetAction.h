/*
	katze 06/02/07
	行動補正値
*/
#pragma once

namespace BMW{
namespace SLG{

class COffsetAction
{/**
	行動補正値
 */
public:
	// コンストラクタ
	COffsetAction(){ reset(); }

	// 操作
	bool IsSpirit(int nID)
	{
		return IsSet(nID,setSpirit_);
	}
	void addSpirit(int nID){ setSpirit_.insert(nID); }
	void delSpirit(int nID){ setSpirit_.erase(nID); }

	bool IsAbility(int nID)
	{
		return IsSet(nID,setAbility_);
	}
	void addAbility(int nID){ setAbility_.insert(nID); }
	void delAbility(int nID){ setAbility_.erase(nID); }

	// リセット
	virtual void reset()
	{
		setSpirit_.clear();
		setAbility_.clear();
	}

protected:
	set<int> setSpirit_;  // 補正に関係した精神ID
	set<int> setAbility_; // 補正に関係したアビリティID

	bool IsSet(int nID, set<int>& intSet)
	{
		return intSet.end()!=intSet.find(nID);
	}
};

} // namespace SLG end
} // namespace BMW end