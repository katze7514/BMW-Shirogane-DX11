#include "stdafx.h"

#include "CItem_Avenger.h"

namespace BMW{
namespace Item{
/////////////////////////////////////////////
// Žg—p
/////////////////////////////////////////////
bool CItem_Avenger::enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p)
{
	// 50%‚ÌŠm—¦‚Å”­“®‚·‚é
	return CApp::rand_.Get(2)==0;
}

} // namespace Item end
} // namespace BMW end
