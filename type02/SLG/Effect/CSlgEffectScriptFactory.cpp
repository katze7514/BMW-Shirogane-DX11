#include "stdafx.h"

#include "../../Scene/Code/CCode_help.h"

#include "../IDRule.h"
#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Code/CCode_map_scroll.h"

#include "CCmdSlgEffect.h"

#include "Code/CCode_load_symbol.h"
#include "Code/CCode_add_symbol.h"
#include "Code/CCode_del_symbol.h"
#include "Code/CCode_ctrl_symbol.h"
#include "Code/CCode_back.h"
#include "Code/CCode_ctrl_map_chip.h"
#include "Code/CCode_skip.h"

#include "CSlgEffectScriptFactory.h"

namespace BMW{
namespace SLG{
namespace Effect{

namespace{
__inline VM::Code::CCode_ipush* createPush(int n)
{
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(n);
	return pPush;
}

} // namespace end

void CSlgEffectScriptFactory::createLoad(CCmdLoad& cmd, CSLGContext* p, VM::CScript* pScript)
{
#ifdef BMW_DEBUG
	CDbg().Out("E_LOAD %s %d",cmd.sSymbol_.c_str(),p->getSLGDef().getEffect().getID(cmd.sSymbol_));
#endif
	pScript->addCode(createPush(cmd.nEffect_));
	pScript->addCode(createPush(cmd.nEffect_==CCode_load_symbol::NORMAL
								? p->getSLGDef().getEffect().getID(cmd.sSymbol_)
								: p->getEffectDB().getID(cmd.sSymbol_)));
	pScript->addCode(createPush(cmd.nNo_));
	pScript->addCode(new CCode_load_symbol());
}

void CSlgEffectScriptFactory::createAddEvent(CCmdAddSymbol& cmd, CSLGContext* p, VM::CScript* pScript)
{
#ifdef BMW_DEBUG
	CDbg().Out("ADD_EVENT %d %d %d",cmd.nNo_,cmd.nX_,cmd.nY_);
#endif
	pScript->addCode(createPush(cmd.nY_));
	pScript->addCode(createPush(cmd.nX_));
	if(cmd.nTarget_==CCode_add_event_symbol::EVENT_MAP)
		pScript->addCode(createPush(cmd.nIndex_));
	ef(cmd.nTarget_==CCode_add_event_symbol::EVENT_CHARA)
		pScript->addCode(createPush(p->getSLGDef().getSlgID(cmd.sChara_)));

	pScript->addCode(createPush(cmd.nTarget_));
	pScript->addCode(createPush(cmd.nNo_));
	pScript->addCode(new CCode_add_event_symbol());
}

void CSlgEffectScriptFactory::createAddMap(CCmdAddSymbol& cmd, CSLGContext* p, VM::CScript* pScript)
{
#ifdef BMW_DEBUG
	CDbg().Out("ADD_MAP %d %d %d",cmd.nNo_,cmd.nX_,cmd.nY_);
#endif
	pScript->addCode(createPush(cmd.nY_));
	pScript->addCode(createPush(cmd.nX_));
	if(cmd.nTarget_==CCode_add_map_symbol::MAP_CHARA)
		pScript->addCode(createPush(p->getSLGDef().getSlgID(cmd.sChara_)));
	else
		pScript->addCode(createPush(cmd.nIndex_));

	pScript->addCode(createPush(cmd.nTarget_));
	pScript->addCode(createPush(cmd.nNo_));
	pScript->addCode(new CCode_add_map_symbol());
}

void CSlgEffectScriptFactory::createDel(int nNo, VM::CScript* pScript)
{
	pScript->addCode(createPush(nNo));
	pScript->addCode(new CCode_del_symbol());
}

void CSlgEffectScriptFactory::createWait(int nNo, VM::CScript* pScript)
{
	pScript->addCode(createPush(nNo));
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::WAIT_EFFECT);
	pScript->addCode(pCall);
}

void CSlgEffectScriptFactory::createCtrl(CCmdCtrl& cmd, VM::CScript* pScript)
{
	pScript->addCode(createPush(cmd.nValue_));
	pScript->addCode(createPush(cmd.nType_));
	pScript->addCode(createPush(cmd.nNo_));
	pScript->addCode(new CCode_ctrl_symbol());
}

void CSlgEffectScriptFactory::createMapScroll(CCmdMapScroll& cmd, CSLGContext* p, VM::CScript* pScript)
{
	if(cmd.nType_==Code::CCode_map_scroll::TWEEN)
	{// À•W
		pScript->addCode(createPush(cmd.nEdging_));
		pScript->addCode(createPush(cmd.nFrame_));
		pScript->addCode(createPush(cmd.nY_));
		pScript->addCode(createPush(cmd.nX_));
		pScript->addCode(new VM::Code::CCode_call(Rule::MAP_SCROLL));
	}
	else
	{
		if(cmd.nType_==Code::CCode_map_scroll::MAP)
			pScript->addCode(createPush(cmd.nIndex_));
		else
			pScript->addCode(createPush(p->getSLGDef().getSlgID(cmd.sChara_)));

		pScript->addCode(createPush(cmd.nType_));
		pScript->addCode(new Code::CCode_map_scroll());
	}
}

void CSlgEffectScriptFactory::createBack(CCmdLoad& cmd, CSLGContext* p, VM::CScript* pScript)
{
	pScript->addCode(createPush(p->getSLGDef().getEffect().getID(cmd.sSymbol_)));
	pScript->addCode(new CCode_back());
}

void CSlgEffectScriptFactory::createMapChip(CCmdMapChip& cmd, CSLGContext* p, VM::CScript* pScript)
{
	switch(cmd.nType_)
	{
	case CCode_ctrl_map_chip::CHARA:
		pScript->addCode(createPush(cmd.bVisible_));
		pScript->addCode(createPush(p->getSLGDef().getSlgID(cmd.sSlg_)));
	break;

	case CCode_ctrl_map_chip::MAP:
		if(CCode_ctrl_map_chip::VISIBLE==cmd.nCtrl_)
			pScript->addCode(createPush(cmd.bVisible_));

		pScript->addCode(createPush(cmd.nPriority_));
		pScript->addCode(createPush(cmd.nIndex_));

	#ifdef BMW_DEBUG
		CDbg().Out("MAPCHIP %d %s %d %d %d", cmd.nCtrl_, cmd.sSlg_.c_str(), p->getSLGDef().getEffect().getID(cmd.sSlg_), cmd.nIndex_, cmd.nPriority_);
	#endif

		pScript->addCode(createPush(p->getSLGDef().getEffect().getID(cmd.sSlg_)));
		pScript->addCode(createPush(cmd.nCtrl_));
	break;

	default: break;
	}

	pScript->addCode(createPush(cmd.nType_));
	pScript->addCode(new CCode_ctrl_map_chip());
}

void CSlgEffectScriptFactory::createSkip(int nSkip, VM::CScript* pScript)
{
	pScript->addCode(createPush(nSkip));
	pScript->addCode(new CCode_skip());
}

void CSlgEffectScriptFactory::createHelp(int nHelp, VM::CScript* pScript)
{
	pScript->addCode(createPush(nHelp));
	pScript->addCode(new Scene::Code::CCode_help());
}

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end