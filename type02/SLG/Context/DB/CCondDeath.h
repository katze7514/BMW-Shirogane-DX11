/*
	katze 06/04/06
	戦闘不能系判定
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondDeath : public ISlgCond
{/*
	戦闘不能系判定
	全体とか、誰か一人とかって感じなので特殊化
*/
public:
	enum eType
	{
		ALL,
		ONE,
		ALL_ALIVE,
		ONE_ALIVE,
	};
	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getPhase()const{ return nPhase_; }
	void	setPhase(int nPhase){ nPhase_=nPhase; }

private:
	int nType_;
	int nPhase_;
};

class CCondDeathLive : public ISlgCond
{/*
	戦闘不能系判定
	全体とか、誰か一人とかって感じなので特殊化
	生きててもOKなキャラを指定できる
*/
public:
	enum eType
	{
		ALL,
		ONE,
		ALL_ALIVE,
		ONE_ALIVE,
	};
	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getPhase()const{ return nPhase_; }
	void	setPhase(int nPhase){ nPhase_=nPhase; }
	set<int>&	getSetChara(){ return setChara_; }
	void		setSetChara(int nChara){ setChara_.insert(nChara); }

private:
	int nPhase_;
	set<int> setChara_;
};

} // namespace SLG end
} // namespace BMW end