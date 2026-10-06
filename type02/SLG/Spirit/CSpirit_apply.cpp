#include "stdafx.h"

#include "../../Spirit/IDSpirit.h"
#include "../../Spirit/Spirit/CSpirit_Supply.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CSpirit_apply.h"

namespace BMW{
namespace SLG{
namespace Spirit{

void CSpirit_apply::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 使用キャラを取得
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	// 使用する精神を取得
	const Chara::CStatusAbility& spirit = pChara->getBattle().getSpirit(p->getTargetAbility());
	// 対象キャラを取得
	CDataCharaSLG* pTarget = p->getTargetCharaData();

	// 精神を使用
	// つまり、SPを減らす
	int nAttr = spirit.getAttr();
	// 集中力持ちだったら2割減
	if(pChara->getBattle().hasSkill(Ability::CONCENT)>=0)
		nAttr=(nAttr*4)/5;

	pChara->getBattle().calcSP(nAttr);

	// 精神を適用
	if(spirit.getID()==BMW::Spirit::SUPPLY)
		p->getApp()->getSpirit().getDataCast<BMW::Spirit::CSpirit_Supply>(BMW::Spirit::SUPPLY)->applyStatus(*pTarget, spirit.getAttr(), *p);
	ef(spirit.getID()==BMW::Spirit::PROVO)
		p->getApp()->getSpirit().applyStatus(*pTarget, pChara->getID(), BMW::Spirit::PROVO);
	else
		p->getApp()->getSpirit().applyStatus(*pTarget, spirit.getAttr(), spirit.getID());

	// 終了
	getTaskListCtrl()->returnTaskList();
}

} // namespace Spirit end
} // namespace SLG end
} // namespace BMW end