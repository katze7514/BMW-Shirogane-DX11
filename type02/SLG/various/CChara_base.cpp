#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara2.h"

#include "CChara_base.h"

namespace BMW{
namespace SLG{
namespace Chara{

int CChara_base::IsHere(int nIndex, CSLGContext& context)
{
	Map::CMapChip* pMap = context.getMapChip(nIndex);
	if(pMap==NULL) return -1;
	Task::ITaskBase* pBase = pMap->getTask(Map::CMapChip::CHARA);
	if(pBase==NULL) return -1;
	Map::CMapChipChara2* pChara = static_cast<Map::CMapChipChara2*>(pBase);
	return context.getCharaData(pChara->getID())->getPhase();
}

bool CChara_base::IsHere(int nID, int nIndex, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(nID);
	if(pChara==NULL) return false;
	return pChara->getIndex()==nIndex;
}

bool CChara_base::IsHere(const string& sID, int nIndex, CSLGContext& context)
{
	return IsHere(context.getSLGDef().getSlgID(sID),nIndex,context);
}

} // namespace Chara end
} // namespace SLG end
} // namespace BMW end