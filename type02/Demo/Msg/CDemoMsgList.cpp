#include "stdafx.h"

#include "CDemoMsg.h"
#include "CDemoMsgList.h"

namespace BMW{
namespace Demo{

CDemoMsgList::~CDemoMsgList()
{
	vector<CDemoMsg*>::iterator it;
	for(it=vecMsg_.begin(); it!=vecMsg_.end(); it++)
		DELETE_SAFE(*it);

	vecMsg_.clear();
}

} // namespace Demo end
} // namespace BMW end