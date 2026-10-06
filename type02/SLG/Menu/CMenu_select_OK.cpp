#include "stdafx.h"

#include "../../mode.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"

#include "CMenu_select_player.h"
#include "CMenu_select_OK.h"

namespace BMW{
namespace SLG{
namespace Menu{

void CMenu_select_OK::OnAction(Task::CTaskContext* pContext)
{
	// 実動作
	if(pContext->getInput()->getInputState(Input::IInput::OK)==Input::IInput::RELEASE)
	{// ボタンが押されたっぽい
		if(pContext->getValue(Flag::TARGET_MAP)>=0)
		{// 確かに
			if(pContext->getValue(Flag::TARGET_CHARA)>=0)
			{// キャラがいたらキャラメニューへ
				CSLGContext* p = static_cast<CSLGContext*>(pContext);
			#ifdef CHARA_CTRL
				if(p->getTargetCharaData()->getState().getAct()!=Act::AFTER)
			#else
				if(p->getTargetCharaData()->getPhase()==Phase::PLAYER 
				&& p->getTargetCharaData()->getState().getAct()!=Act::AFTER)
			#endif
				{// 味方かつ行動前だったらキャラメニュー
					// 対象キャラを操作キャラとして設定
					p->setCtrlChara(p->getTargetChara());
					getParent()->setState(CMenu_select_player::CHARA);
				}
				else
				{// そうじゃなかったら、ステータス
					getParent()->setState(CMenu_select_player::STATUS);
				}
			}
			else
			{// いなかったらターンメニュー
				CSLGContext* p = static_cast<CSLGContext*>(pContext);
				p->setCirclePos(Pos::MOUSE);
				getParent()->setState(CMenu_select_player::TURN);
			}
		}
	}
}

} // namespace Menu end
} // namespace SLG end
} // namespace BMW end