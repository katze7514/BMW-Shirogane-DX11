/*
	katze 06/04/06
	フラグ判定系
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondFlag : public ISlgCond
{/*
	フラグ判定系
*/
public:
	enum eType{
		COND,
		COND_GLOBAL,
		SET,
		CALC,
		SET_GLOBAL,
		CALC_GLOBAL,
	};
	// コンストラクタ
	CCondFlag():nType_(COND),nValue_(1){}
	// 判定
	virtual bool judg(CSLGContext* p);

	// アクセッサ
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getFlag()const{ return nFlag_; }
	void	setFlag(int nFlag){ nFlag_=nFlag; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

private:
	int nType_;
	int nFlag_;
	int nValue_;
};

} // namespace SLG end
} // namespace BMW end