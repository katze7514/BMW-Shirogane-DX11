#include "stdafx.h"

#include "CDataGuiDefPanel.h"

namespace BMW{
namespace GUI{

CDataGuiDefPanel::~CDataGuiDefPanel()
{
	widget_list::iterator it;
	for(it=listWidget_.begin(); it!=listWidget_.end(); it++)
		DELETE_SAFE(*it);

	listWidget_.clear();
}

} // namespace GUI end
} // namespace BMW end