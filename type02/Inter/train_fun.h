/*
	katze 06/03/01
	—{¬ŠÖ”
*/
#pragma once

namespace BMW{
namespace Inter{
namespace Chara{

__inline void updateTrainBar(GUI::CPanel* pBar, int nSource, int nUp)
{
	pBar->visibleAll(false);
	GUI::CPanelCtrl* pLv;
	for(int i=1; i<=nSource; i++)
	{// Œ³X‚Ì•”•ª‚ÍÔ
		pLv = pBar->getWidgetCast<GUI::CPanelCtrl>(i-1);
		pLv->validWidget(1/*Ô*/);
		pLv->visible(true);
	}
	for(int i=nSource+1; i<=nSource+nUp; i++)
	{// ‚ ‚ª‚Á‚Ä‚é•”•ª‚Í—Î
		pLv = pBar->getWidgetCast<GUI::CPanelCtrl>(i-1);
		pLv->validWidget(0/*—Î*/);
		pLv->visible(true);
	}
}



} // namespace Chara end
} // namespace Inter end
} // namespace BMW end