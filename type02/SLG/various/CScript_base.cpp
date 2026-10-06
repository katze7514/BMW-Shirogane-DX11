#include "stdafx.h"

#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"

#include "CScript_base.h"

namespace BMW{
namespace SLG{
namespace Script{

int CScript_base::getFlag(int nFlag,Task::CTaskContext* pContext)
{
	return pContext->getValue(nFlag);
}

void CScript_base::setFlag(int nValue, int nFlag,Task::CTaskContext* pContext)
{
	return pContext->setValue(nValue,nFlag);
}

int CScript_base::getFlag(const string& sFlag,Task::CTaskContext* pContext)
{
	CSLGContext* p = getSlgContext(pContext);
	return p->getValue(p->getSLGDef().getFlagID(sFlag));
}

void CScript_base::setFlag(int nValue, const string& sFlag,Task::CTaskContext* pContext)
{
	CSLGContext* p = getSlgContext(pContext);
	return p->setValue(nValue, p->getSLGDef().getFlagID(sFlag));
}

int CScript_base::getFlag(const string& sFlag,CSLGContext* p)
{
	return p->getValue(p->getSLGDef().getFlagID(sFlag));
}

void CScript_base::setFlag(int nValue, const string& sFlag,CSLGContext* p)
{
	return p->setValue(nValue, p->getSLGDef().getFlagID(sFlag));
}

void CScript_base::callScript(const string& sID, Task::CTaskContext* pContext)
{
	//CDbg().Out("CallScript %s",sID.c_str());
	getTaskListCtrl()->callTaskList(getScriptID(sID,pContext),true);
}

int	CScript_base::getScriptID(const string& sID,Task::CTaskContext* pContext)
{
	return getSlgContext(pContext)->getSLGDef().getScriptID(sID);
}

int	CScript_base::getSlgID(const string& sID,Task::CTaskContext* pContext)
{
	return getSlgContext(pContext)->getSLGDef().getSlgID(sID);
}

CSLGContext* CScript_base::getSlgContext(Task::CTaskContext* pContext)
{
	return static_cast<CSLGContext*>(pContext);
}

bool CScript_base::IsAct(int nID, int nAct, CSLGContext& context)
{
	CDataCharaSLG* pChara = context.getCharaData(nID);
	if(pChara==NULL) return true;
	return pChara->getState().getAct()==nAct;
}

bool CScript_base::IsAct(const string& sID, int nAct, CSLGContext& context)
{
	return IsAct(context.getSLGDef().getSlgID(sID),nAct,context);
}

} // namespace Script end
} // namespace SLG end
} // namespace BMW end