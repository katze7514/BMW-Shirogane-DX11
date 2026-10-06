#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CCharaState.h"
#include "CMapChipCharaSprite2.h"

namespace BMW{
namespace SLG{
namespace Map{

CMapChipCharaSprite2::CMapChipCharaSprite2()
{
	// ƒ}ƒbƒv’†‰›
	setX(32);
	setY(16);
}

CMapChipCharaSprite2::~CMapChipCharaSprite2()
{
	for(int i=0; i<ChipMovie::END; i++)
		DELETE_SAFE(pChipMovie_[i]);
}

void CMapChipCharaSprite2::Task(Task::CTaskContext* pContext)
{
	if(pContext->IsAction())
	{
		if(IsValid())
			pChipMovie_[getState()]->Task(pContext);
	}
	else
	{
		if(IsVisible())
			pChipMovie_[getState()]->Task(pContext);
	}
}


void CMapChipCharaSprite2::getSize(LONG& lWidth, LONG& lHeight)
{
	pChipMovie_[getState()]->getSize(lWidth, lHeight);
}

void CMapChipCharaSprite2::getDrawSize(LONG& lWidth, LONG& lHeight)
{
	pChipMovie_[getState()]->getDrawSize(lWidth, lHeight);
}

} // namespace Map end
} // namespace SLG end
} // namespace BMW end