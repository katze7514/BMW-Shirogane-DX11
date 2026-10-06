#include "stdafx.h"

#include "CSlgCondList.h"

namespace BMW{
namespace SLG{

CSlgCondList::~CSlgCondList()
{
	for_each(listCond_.begin(), listCond_.end(), DeleteObj());
	listCond_.clear();
}

bool CSlgCondList::judg(CSLGContext* p)
{
	cond_list::iterator it;
	for(it=listCond_.begin(); it!=listCond_.end(); it++)
		if(!(*it)->judg(p)) return false;

	return true;
}

bool CSlgCondListOr::judg(CSLGContext* p)
{
	cond_list::iterator it;
	for(it=listCond_.begin(); it!=listCond_.end(); it++)
		if((*it)->judg(p)) return true;

	return false;
}


///////////////////////////////////
// Not

CSlgCondNot::~CSlgCondNot()
{
	DELETE_SAFE(pCond_);
}

bool CSlgCondNot::judg(CSLGContext* p)
{
	return !(pCond_->judg(p));
}

} // namespace SLG end
} // namespace BMW end