/*
	katze 05/04/10
	マップクロージャ
*/
#pragma once

#include "CMapChipInfo.h"

namespace BMW{
namespace SLG{
namespace Map{

struct mapinfo_closure : boost::spirit::closure<mapinfo_closure, CMapChipInfo>
{
	member1 val;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end