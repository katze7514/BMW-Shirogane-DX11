#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"

#include "CBattle_base.h"

namespace BMW{
namespace SLG{
namespace Battle{

bool CBattle_base::IsBattle(int nChara, CSLGContext& context)
{
	int nCtrl = context.getCtrlChara();
	int nTarget = context.getTargetChara();

	return nCtrl==nChara || nTarget==nChara;
}

bool CBattle_base::IsBattle(const string& sChara, CSLGContext& context)
{
	return IsBattle(context.getSLGDef().getSlgID(sChara), context);
}

bool CBattle_base::IsBattle(int nChara1, int nChara2, CSLGContext& context)
{
	return IsBattle(nChara1,context) && IsBattle(nChara2,context);
}

bool CBattle_base::IsBattle(const string& sChara1, const string& sChara2, CSLGContext& context)
{
	return IsBattle(context.getSLGDef().getSlgID(sChara1),
					context.getSLGDef().getSlgID(sChara2),
					context);
}

bool CBattle_base::IsHP(int nChara, int nRemain, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(nChara);
	if(pChara==NULL) return false;
	int nMaxHP = pChara->getBattle().getMaxHP();
	int nHP = pChara->getBattle().getHP();

	//CDbg().Out("HP %d %d %d",nHP,nMaxHP,nHP*100/nMaxHP);
	return nHP*100/nMaxHP <= nRemain;
}

bool CBattle_base::IsHP(const string& sChara, int nRemain, CSLGContext& context)
{
	return IsHP(context.getSLGDef().getSlgID(sChara),nRemain,context);
}

void CBattle_base::setRemove(const string& sChara, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(sChara);
	if(pChara==NULL) return;
	if(pChara->getState().getAct()==Act::DEATH)
		pChara->getState().setAct(Act::DEATH_EVENT);
	else
		pChara->getState().setAct(Act::REMOVE);
}

} // namespace Battle end
} // namespace SLG end
} // namespace BMW end

