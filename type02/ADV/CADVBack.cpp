#include "stdafx.h"

//#include "../Scene/IDGui.h"

#include "../Scene/IScene.h"
#include "CADVBack.h"

namespace BMW{
namespace ADV{

CADVBack::~CADVBack()
{
	DELETE_SAFE(pBack_);
	DELETE_SAFE(pBackName_);
}

void CADVBack::Task(Task::CTaskContext* pContext)
{
	if(!pContext->IsAction())
	{
		if(IsVisible())
		{
			pBack_->Task(pContext);
			pBackName_->Task(pContext);
		}
	}
}

void CADVBack::OnInit(Task::CTaskContext* pContext)
{
	using GUI::CGraphic;

	// íËã`DB
	GUI::CGuiDefDB& db = pContext->getScene()->getGuiDefDB();

	// éÛÇØì¸ÇÍÇçÏÇÈÇæÇØ
	pBack_ = new CGraphic();
	pBack_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	// îwåiñºï\é¶
	pBackName_ = db.createInterfaceCast<GUI::CPanel>("PLACE");
	pBackName_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

GUI::CText* CADVBack::getBackName()
{
	return pBackName_->getWidgetCast<GUI::CText>("MSG_TEXT");
}

} // namespace ADV end
} // namespace BMW end