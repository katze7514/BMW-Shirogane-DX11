#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Context/CSLGDef.h"
#include "../../Context/CDataCharaSLG.h"
#include "../../Map/CMapChip.h"

#include "CCode_ctrl_map_chip.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_ctrl_map_chip::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nType = p->top();
	p->pop();

	switch(nType)
	{
	case CHARA: // キャラチップ対象
	{
		int nID = p->top();
		p->pop();

		int nValue = p->top();
		p->pop();

	#ifdef BMW_DEBUG
		CDbg().Out("CHARA_CHIP %d %d", nID, nValue);
	#endif

		CDataCharaSLG* pChara = p->getCharaData(nID);
		if(pChara!=NULL
		&& pChara->getIndex()>=0)
		{
			Map::CMapChip* pMap = p->getMapChip(pChara->getIndex());
			pMap->getTask(Map::CMapChip::CHARA)->visible(nValue);
		}
	}
	break;

	case MAP: // マップチップ対象
	{
		int nCtrl = p->top();
		p->pop();

		int nID = p->top();
		p->pop();

		int nIndex = p->top();
		p->pop();

		int nPriority = p->top();
		p->pop();

	#ifdef BMW_DEBUG
		CDbg().Out("MAP_CHIP %d %d %d %d", nCtrl, nID, nIndex, nPriority);
	#endif

		Map::CMapChip* pMap = p->getMapChip(nIndex);

		if(pMap!=NULL)
		{
			if(pMap->getTask(Map::CMapChip::OBJ)!=NULL)
			{
				Task::ITaskList* pList = static_cast<Task::ITaskList*>(pMap->getTask(Map::CMapChip::OBJ));
				switch(nCtrl)
				{
				case VISIBLE:
				{
					int nValue = p->top();
					p->pop();

					Task::ITaskBase* pCtrl = pList->getTask(nPriority);
					if(pCtrl!=NULL)	pCtrl->visible(nValue);
				}
				break;

				case ADD:
				{
					Task::ITaskBase* pTask = p->getSLGDef().getEffect().createSymbol(nID);
					if(pTask!=NULL) pList->addTask(pTask, nPriority);
				}
				break;

				case SWAP:
				{
					Task::ITaskBase* pTask = p->getSLGDef().getEffect().createSymbol(nID);
					if(pTask!=NULL)
					{
						Task::ITaskBase* pCtrl = pList->removeTask(nPriority);
						if(pCtrl!=NULL)
						{
							pTask->setX(pCtrl->getDrawInfo(false).getX());
							pTask->setY(pCtrl->getDrawInfo(false).getY());
							DELETE_SAFE(pCtrl);
						}
						pList->addTask(pTask,nPriority);
					}
				}
				break;

				default: break;
				}
			}
		}
	}
	break;

	default: break;
	}
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end