#include "stdafx.h"

#include "../IDSLG.h"
#include "../CSLGScene.h"
#include "../Context/CSLGContext.h"

#include "../various/CData_ref.h"
#include "CCode_slg_end.h"

namespace BMW{
namespace SLG{
namespace Code{

void CCode_slg_end::OnAction(Task::CTaskContext* pContext)
{
	if(getState()!=END)
	{
		if(getState()==SAVE) // SAVEフラグ足っていたらデータセーブ
			Data::CData_ref().OnAction(pContext);

		// 無条件でSLGを終了し、次へ行く
		pContext->push(Victory::VICTORY);
		pContext->getScene()->setState(CSLGScene::END);
		setState(END);
		// 入力系を止める
		pContext->getInput()->guard(true);
	}
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end