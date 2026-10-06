#include "stdafx.h"

#include "../Context/CSLGContext.h"

#include "CMapChip.h"
#include "CMapChipChara.h"

namespace BMW{
namespace SLG{
namespace Map{

void CMapChipChara::OnInit(Task::CTaskContext* pContext)
{
}

void CMapChipChara::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case MOVE_TOP:
	case MOVE_LEFT:
	case MOVE_BOTTOM:
	case MOVE_RIGHT:
	{// 移動処理準備
		// こいつが属してるMapChipを取得
		Map::CMapChip* pChip = static_cast<Map::CMapChip*>(pContext->getTaskList());
		// 移動先のMapChipを取得
		pMove_ = static_cast<CSLGContext*>(pContext)->getMapChip(pChip->getMapInfo().getOnMap(getState()));
		// 所属マップチップを入れ替える
		pChip->removeMe();
		pMove_->addTask(this, Map::CMapChip::CHARA);
		// 移動開始
		setState(MOVE);
	}
	break;

	case MOVE: actionMove(pContext); break;
	default: break;
	}
}

void CMapChipChara::actionMove(Task::CTaskContext* pContext)
{// 終了したら、NORMALへ
	setState(NORMAL);
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end