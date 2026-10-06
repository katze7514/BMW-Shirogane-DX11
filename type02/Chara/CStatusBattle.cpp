#include "stdafx.h"

#include "CStatusBattle.h"

namespace BMW{
namespace Chara{

void CStatusBattle::Serialize(ISerialize& s)
{
	s << nHP_ << nEN_ << nTough_ << nQuick_ << nMove_ << nJump_;
}

} // namespace Chara end
} // namespace BMW end