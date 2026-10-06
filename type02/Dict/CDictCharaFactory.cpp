#include "stdafx.h"

#include "CDictCharaScene.h"
#include "CDictCharaSelect.h"
#include "CDictCharaView.h"

#ifdef BMW_DEBUG
#include "CDictCharaFace.h"
#include "CDictCharaSymbol.h"
#endif

#include "CDictCharaFactory.h"

namespace BMW{
namespace Dict{

smart_ptr<Task::ITaskList> CDictCharaFactory::createTaskList(int nID)
{
	Task::ITaskList* pList=NULL;

	switch(nID)
	{
	case CDictCharaScene::SELECT:	pList = new CDictCharaSelect();	break;
	case CDictCharaScene::VIEW:		pList = new CDictCharaView();	break;
#ifdef BMW_DEBUG
	case CDictCharaScene::FACE:		pList = new CDictCharaFace();	break;
	case CDictCharaScene::SYMBOL:	pList = new CDictCharaSymbol();	break;
#endif
	default: break;
	}

	return smart_ptr<Task::ITaskList>(pList);
}

} // namesapce Dict end
} // namespace BMW end