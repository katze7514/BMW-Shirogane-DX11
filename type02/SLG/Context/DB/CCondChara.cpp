#include "stdafx.h"

#include "../CSLGContext.h"
#include "../CDataCharaSLG.h"

#include "CCondChara.h"

namespace BMW{
namespace SLG{

bool CCondChara::judg(CSLGContext* p)
{
#ifdef BMW_DEBUG
	CDbg().Out("CHARA %d %d %d",getChara(), getType(), getValue());
#endif
	CDataCharaSLG* pChara;

	switch(getChara())
	{
	case TARGET:
		pChara=p->getTargetCharaData();
	break;

	case CTRL:
		pChara=p->getCtrlCharaData();
	break;

	default:
		pChara=p->getCharaData(getChara());
	break;
	}

	switch(getType())
	{
	case HP:	return pChara==NULL
					|| ((double)getValue()/100.0 >= (double)pChara->getBattle().getHP()/(double)pChara->getBattle().getMaxHP());
	case EXIST: return pChara!=NULL && pChara->IsExist();
	case INDEX:	return pChara!=NULL && getValue()==pChara->getState().getIndex();
	case CHARA: return pChara!=NULL && getValue()==pChara->getBattle().getID();
	case DAMAGE:return pChara==NULL || getValue()<=pChara->getBattle().getBattleOffset().getHP();
	case ID:	return pChara!=NULL && getValue()==pChara->getID();
	default:	return false;
	}
}

} // namespace SLG end
} // namespace BMW end