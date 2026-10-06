#include "stdafx.h"

#include "CDemoMsgCond.h"

namespace BMW{
namespace Demo{

int CDemoMsgCond::getMsgID(int nHP, int nChara, Task::CTaskContext* pContext)
{// —^‚¦‚ç‚ê‚½ğŒ‚É‡‚í‚¹‚ÄMsgID‚ğæ“¾
	if(nHP<0) nHP=0;
	int nRatio = CApp::rand_.Get(100)+1;
	int nRatio2 = CApp::rand_.Get(100)+1;
	cond_list::iterator it;
	for(it=listCond_.begin(); it!=listCond_.end(); ++it)
	{
		// TYPE•Ê‚É”»’è
		switch((*it)->getType())
		{
		case CDemoMsgCondBase::HP:
		// HPc—Ê
			//CDbg().Out("HP %d %d", (*it)->getCond(), nHP);
			if((*it)->getCond()<=nHP && (*it)->getRand()>=nRatio)
				return (*it)->getMsgID();
		break;

		case CDemoMsgCondBase::CHARA:
		// ƒLƒƒƒ‰ID
			if(pContext->getApp()->getChara().IsChild(nChara,(*it)->getCond())
			&& (*it)->getRand()>=nRatio)
				return (*it)->getMsgID();
		break;

		default: // Šm—¦
		{
			//CDbg().Out("RANDOM %d %d", (*it)->getCond(), nRatio);
			if((*it)->getCond()>=nRatio2 && (*it)->getRand()>=nRatio) 
				return (*it)->getMsgID();
		}
		break;
		}
	}

	return -1;
}

} // namespace Demo end
} // namespace BMW end