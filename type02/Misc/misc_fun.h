/*
	katze 06/02/19
	‘S‘Ì‚Å—Ç‚­g‚¢‚»‚¤‚ÈŠÖ”ŒQ
*/
#pragma once

namespace BMW{
namespace Misc{

__inline string linkStrAndNum(const string& str, int nNum)
{
	return str + CStringScanner::NumToString(nNum);
}

} // naemspace Misc end
} // namespace BMW end