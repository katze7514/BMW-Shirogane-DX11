/*
	genereted by code_gen_slg.rb
*/
#include "stdafx.h"

#include "../Map/CMapChipState.h"
#include "CMove_view.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_view::OnAction(Task::CTaskContext* pContext)
{
	Map::CMapChipState::move(IsVisible());
}

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
