/*
	katze 06/04/06
	フェーズに関する判定
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondPhase : public ISlgCond
{/*
	フェーズに関する判定
 */
public:
	enum ePhase{
		EQ,GT,LT,
	};
	// コンストラクタ
	CCondPhase():nCond_(EQ),nTurn_(0),nPhase_(-1){}

	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getCond()const{ return nCond_; }
	void	setCond(int nCond){ nCond_=nCond; }
	int		getTurn()const{ return nTurn_; }
	void	setTurn(int nTurn){ nTurn_=nTurn; }
	int		getPhase()const{ return nPhase_; }
	void	setPhase(int nPhase){ nPhase_=nPhase; }

private:
	int nCond_;
	int nTurn_;
	int nPhase_;

	bool IsTurn(int nTurn)
	{
		if(getTurn()<=0) return true;
		switch(getCond())
		{
		case GT: return getTurn()<=nTurn;
		case LT: return getTurn()>=nTurn;
		default: return getTurn()==nTurn;
		}
	}
	bool IsPhase(int nPhase)
	{
		return getPhase()<0 || getPhase()==nPhase;
	}
};

} // namespace SLG end
} // namespace BMW end