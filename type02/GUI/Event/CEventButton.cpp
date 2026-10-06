#include "stdafx.h"

#include "CEventButton.h"

namespace BMW{
namespace GUI{

bool IsOverIn(const smart_ptr<CEventButton>& pButton)
{
	return pButton->getState()==CButton::OVER_IN;
}

bool IsOverOut(const smart_ptr<CEventButton>& pButton)
{
	return pButton->getState()==CButton::OVER_OUT;
}

bool IsPress(const smart_ptr<CEventButton>& pButton)
{
	return pButton->getState()==CButton::PRESS;
}

bool IsRelease(const smart_ptr<CEventButton>& pButton)
{
	return pButton->getState()==CButton::RELEASE;
}

bool IsCancel(const smart_ptr<CEventButton>& pButton)
{
	return pButton->getState()==CButton::CANCEL;
}

} // namespace GUI end
} // namespace BMW end