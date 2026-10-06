#include "stdafx.h"

#include "../CDataCharaBase.h"
#include "CDataCharaData.h"

namespace BMW{
namespace Chara{

void CDataCharaData::applyInit(CDataCharaBase* base,bool bCont)
{
	if(!pParent_.isNull())
	{// こいつは差分データだぜ！
#ifdef BMW_DEBUG
		//CDbg().Out("ApplySub %d",pParent_->getInit().getID());
#endif
		pParent_->applyInit(base,bCont);
		applyInitSub(base,bCont);
	}
	else
	{// 一番元になるデータだぜ！
		init_.apply(base,bCont);
	}
}

void CDataCharaData::applyInitSub(CDataCharaBase* base, bool bCont)
{
	init_.applySub(base,bCont);
}

void CDataCharaData::applyAbility(CDataCharaBase* base, int nLv)
{
	if(!pParent_.isNull() && !ability_.IsValid())
	{// 親が居て、自分のGrowthデータ無効なら親のを使う
		pParent_->applyAbility(base,nLv);
	}
	else
	{// そうじゃなかったら、自分のを適用
		ability_.apply(base,nLv);
	}
}

} // namespace Chara end
} // namespace BMW end
