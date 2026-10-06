#include "stdafx.h"

#include "CStatusFund.h"

namespace BMW{
namespace Chara{

void CStatusFund::Serialize(ISerialize& s)
{
	s << nStrength_ << nMagic_ << nHit_ << nAvoid_ << nDefence_ << nSkill_ << nSP_;
}

} // namespace Chara end
} // namespace BMW end