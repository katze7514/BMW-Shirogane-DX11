#include "stdafx.h"

#include "IDData.h"
#include "CDataContext.h"

namespace BMW{
namespace Data{

CDataContext::CDataContext()
{
	setValue(1,Flag::DATA_FLAG);
	setValue(-1,Flag::TARGET_DATA);
}

CDataContext::~CDataContext()
{
	clearHead();
}

void CDataContext::clearHead()
{
	head_map::iterator it;
	for(it=mapHead_.begin(); it!=mapHead_.end(); it++)
		DELETE_SAFE(it->second);

	mapHead_.clear();
}

} // namespace Data end
} // namespace BMW end