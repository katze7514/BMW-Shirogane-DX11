#include "stdafx.h"

#include "../Action/IAction.h"
#include "../Action/CActionFactory.h"
#include "CSLGContext.h"
#include "CDataCharaSLG.h"

#include "CCode_chara.h"

namespace BMW{
namespace SLG{
namespace Code{

__inline int CCode_chara::getRatioValue(int nAbs)
{
	return getType()==ABS ? getValue() : nAbs*getValue()/100;
}

void CCode_chara::OnAction(Task::CTaskContext* pContext)
{
#ifdef BMW_DEBUG
	CDbg().Out("CHARA %d %d %d %d",nChara_,nKind_,nType_,*listValue_.begin());
#endif
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	if(getChara()<0)	allChara(p);
	else				oneChara(p);
	
}

void CCode_chara::applyHP(CDataCharaSLG* pChara)
{
	pChara->calcHP(getRatioValue(getType()==RATIO_MAX ? pChara->getBattle().getMaxHP() : pChara->getBattle().getHP()));
}

void CCode_chara::applyEN(CDataCharaSLG* pChara)
{
	pChara->getBattle().calcEN(getRatioValue(getType()==RATIO_MAX ? pChara->getBattle().getMaxEN() : pChara->getBattle().getEN()));
}

void CCode_chara::applySP(CDataCharaSLG* pChara)
{
	pChara->getBattle().calcSP(getRatioValue(getType()==RATIO_MAX ? pChara->getBattle().getMaxSP() : pChara->getBattle().getSP()));
}

void CCode_chara::applyMental(CDataCharaSLG* pChara)
{
	if(getType()==ABS)
		pChara->getBattle().setMental(getValue());
	else
		pChara->getBattle().calcMental(getValue());
}

void CCode_chara::applyAct(CDataCharaSLG* pChara)
{
#ifdef BMW_DEBUG
	CDbg().Out("SETACT %d",getValue());
#endif
	if(getValue()<0)
	{// 値が負の時はイベントリムーブ
		if(pChara->getState().getAct()==Act::DEATH)
			pChara->getState().setAct(Act::DEATH_EVENT);
		else
			pChara->getState().setAct(Act::REMOVE);
	}
	else
	{	pChara->getState().setAct(getValue());	}
}

void CCode_chara::applyValid(CDataCharaSLG* pChara)
{
	pChara->getState().valid(getValue(),getType());
}

void CCode_chara::applyAction(CDataCharaSLG* pChara)
{
	// 思考ルーチンの入れ替え
	delete pChara->getAction();
	Action::CActionFactory fct;
	pChara->setAction(fct.createAction(getType(),listValue_));
}

void CCode_chara::for_each(list<int>& phaseList, CSLGContext* p, void (CCode_chara::*apply)(CDataCharaSLG* pChara))
{
	list<int>::iterator it;
	CDataCharaSLG* pChara;
	for(it=phaseList.begin(); it!=phaseList.end(); ++it)
	{
		pChara = p->getCharaData(*it);
		if(pChara==NULL || !pChara->IsExist()) continue;
		(this->*apply)(pChara);
	}
}

void CCode_chara::allChara(CSLGContext* p)
{
	list<int>& phaseList = p->getPhaseList(getChara()+3);

	// Kindにあわせて処理分岐
	switch(getKind())
	{
	case HP: for_each(phaseList, p, &CCode_chara::applyHP); break;
	case EN: for_each(phaseList, p, &CCode_chara::applyEN); break;
	case SP: for_each(phaseList, p, &CCode_chara::applySP); break;
	case MENTAL: for_each(phaseList, p, &CCode_chara::applyMental); break;

	//case STATE: pChara->getBattle().cond(getValue(),getType()); break;

	case ACT: for_each(phaseList, p, &CCode_chara::applyAct); break;
	//case WAY: pChara->getState().setWay(getValue()); break;
	//case APPER: pChara->getState().apper(getValue()); break;
	case VALID:  for_each(phaseList, p, &CCode_chara::applyValid); break;

	case ACTION: for_each(phaseList, p, &CCode_chara::applyAction); break;

	default: break;
	}
}

void CCode_chara::oneChara(CSLGContext* p)
{
	// キャラデータ取得
	CDataCharaSLG* pChara = p->getCharaData(getChara());
	if(pChara==NULL) return;

	// Kindにあわせて処理分岐
	switch(getKind())
	{
	case HP: applyHP(pChara);	break;
	case EN: applyEN(pChara);	break;
	case SP: applySP(pChara);	break;
	case MENTAL: applyMental(pChara); break;

	//case STATE: pChara->getBattle().cond(getValue(),getType()); break;

	case ACT: applyAct(pChara);	break;
	case WAY: pChara->getState().setWay(getValue()); break;
	case APPER: pChara->getState().apper(getValue()); break;
	case VALID: applyValid(pChara); break;

	case ACTION: applyAction(pChara); break;

	case LOVE:	pChara->addPers(getValue()); break;

	default: break;
	}
}

} // namespace Code end
} // namepsace SLG end
} // namespace BMW end