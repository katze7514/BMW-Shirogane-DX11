/*
	genereted by code_gen.rb
*/
#include "stdafx.h"

#include "CCode_cursol_enable.h"

namespace BMW{
namespace VM{
namespace Code{

void CCode_cursol_enable::OnAction(Task::CTaskContext* pContext)
{// カーソルの表示・非表示
	pContext->getInput()->cursolVisible(IsVisible());
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end
