#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CItem_apply.h"

namespace BMW{
namespace SLG{
namespace Item{

void CItem_apply::OnInit(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// 使用キャラを取得
	CDataCharaSLG* pChara = p->getCtrlCharaData();
	// 使用するアイテムIDを取得
	int nAttr = p->getTargetAbility();
	int nID = pChara->getBattle().hasItemAttr(nAttr);
	
	// アイテムを使用
	// つまり、アイテムを削除する
	pChara->getBattle().delItem(nAttr);

	// 全体数からも減らす
	p->getApp()->getExec().decItem(nID);

	// アイテムを使用
	p->getApp()->getItem().use(*pChara, nAttr, *p, nID);

	// 終了
	getTaskListCtrl()->returnTaskList();
}

} // namespace Item end
} // namespace SLG end
} // namespace BMW end