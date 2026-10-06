#include "stdafx.h"

#include "IDInter.h"

#include "CSelect.h"
#include "CChara.h"

#include "CInterFactory.h"

namespace BMW{
namespace Inter{

smart_ptr<Task::ITaskList> CInterFactory::createTaskList(int nID)
{
	Task::ITaskList* pList=NULL;

	switch(nID)
	{
	case Scene::SELECT: pList = new Select::CSelect();	break;
	case Scene::CHARA:	pList = new Chara::CChara();	break;
	default: break;
	}

	return smart_ptr<Task::ITaskList>(pList);
}

} // namespace Inter end
} // namespace BMW end