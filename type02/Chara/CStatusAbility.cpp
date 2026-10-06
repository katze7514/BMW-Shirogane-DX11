#include "stdafx.h"

#include "CStatusAbility.h"

namespace BMW{
namespace Chara{

void CStatusAbility::Serialize(ISerialize& s)
{
	s << nID_ << nAttr_;
}

} // namespace Chara end
} // namespace BMW end