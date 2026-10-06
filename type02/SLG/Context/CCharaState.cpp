#include "stdafx.h"

#include "CCharaState.h"

namespace BMW{
namespace SLG{

void CCharaState::Serialize(ISerialize& s)
{
	s << nIndex_ << nAct_ << nWay_ << bApper_ << bPinch_;

	for(int i=MOVE; i<=PERS; i++)
		s << baValid_[i];
}

} // namespace SLG end
} // namespace BMW end