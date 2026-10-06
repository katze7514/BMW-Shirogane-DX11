/*
	katze 06/04/16
	勝利条件に関する判定
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondVictory : public ISlgCond
{/*
	勝利条件に関する判定
 */
public:
	enum eType
	{
		VICTORY,
		LOSE,
		EXPERT,
	};
	// コンストラクタ
	CCondVictory(int nType=VICTORY):nType_(nType){}
	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }

private:
	int nType_;
};

} // namespace SLG end
} // namespace BMW end