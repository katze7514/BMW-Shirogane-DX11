#include "stdafx.h"

#include "../Movie/IDMovieCode.h"
#include "../Movie/DB/CDataSymbolMovieClip.h"
#include "../Movie/CScriptExecSymbol.h"

#include "GUI/CDemoMsgBoard.h"
#include "Msg/CDemoMsgList.h"

#include "Code/CCode_back_vel.h"
#include "Code/CCode_back_visible.h"
#include "Code/CCode_msg_demo.h"
#include "Code/CCode_gage.h"

#include "IDDemo.h"
#include "CDemoMovieClip.h"

#include "CDemoSymbolDB.h"

namespace BMW{
namespace Demo{

// staticêÈåæ
smart_ptr<CDemoMsgBoard> CDemoSymbolDB::pBoard_;

CDemoSymbolDB::CDemoSymbolDB()
{
	// ñhå‰ë§ÉVÉìÉ{ÉãÉfÅ[É^ìäâ∫
	symbolID_.writeMap("DEF_DEFAULT_MORPH",	DEF_DEFAULT_MORPH);
	symbolID_.writeMap("DEF_DEFENCE_MORPH",	DEF_DEFENCE_MORPH);
	symbolID_.writeMap("DEF_AVOID_MORPH",	DEF_AVOID_MORPH);
	symbolID_.writeMap("DEF_HIT_MORPH",		DEF_HIT_MORPH);
	symbolID_.writeMap("DEF_DEFAULT_ROTATE",DEF_DEFAULT_ROTATE);
	symbolID_.writeMap("DEF_DEFENCE_ROTATE",DEF_DEFENCE_ROTATE);
	symbolID_.writeMap("DEF_AVOID_ROTATE",	DEF_AVOID_ROTATE);
	symbolID_.writeMap("DEF_HIT_ROTATE",	DEF_HIT_ROTATE);
	symbolID_.writeMap("DEF_DEFAULT_SIZE",	DEF_DEFAULT_SIZE);
	symbolID_.writeMap("DEF_DEFENCE_SIZE",	DEF_DEFENCE_SIZE);
	symbolID_.writeMap("DEF_AVOID_SIZE",	DEF_AVOID_SIZE);
	symbolID_.writeMap("DEF_HIT_SIZE",		DEF_HIT_SIZE);
	symbolID_.writeMap("DEF_DEFAULT",		DEF_DEFAULT);
	symbolID_.writeMap("DEF_DEFENCE",		DEF_DEFENCE);
	symbolID_.writeMap("DEF_AVOID",			DEF_AVOID);
	symbolID_.writeMap("DEF_HIT",			DEF_HIT);
}

///////////////////////////////////////////
// ê∂ê¨
///////////////////////////////////////////
Task::ITaskBase* CDemoSymbolDB::createCode(int nID, Movie::CDataKeyFrame* pData)
{
	switch(nID)
	{
	case Movie::Code::BACK:
	{// îwåië¨ìxïœçXÉRÅ[Éhê∂ê¨
	 // DrawInfoÇÃXÇ™ïœçXÇ∑ÇÈë¨ìxID
		Code::CCode_back_vel* pBack = new Code::CCode_back_vel();
		pBack->setState(pData->getParam());
		return pBack;
	}

	case Movie::Code::BACK_VISIBLE:
	{// îwåiïœçXÉRÅ[Éhê∂ê¨
		Code::CCode_back_visible* pBack = new Code::CCode_back_visible();
		pBack->setState(pData->getParam());
		pBack->setType(pData->getParam2());
		return pBack;
	}

	case Movie::Code::MES:
	{// MsgBoardÇ÷ìoò^
		int nNo=-1;
		if(pMsgList_!=NULL)	nNo = pBoard_->addMsg(pMsgList_->getMsg(pData->getParam()));
		Code::CCode_msg_demo* pMes = new Code::CCode_msg_demo();
		pMes->setState(nNo);
		return pMes;
	}

	case Movie::Code::GAGE:
	{// ÉQÅ[ÉW
		Code::CCode_gage* pGage = new Code::CCode_gage();
		pGage->setKind(pData->getParam());
		return pGage;
	}
	}

	return CSymbolDB::createCode(nID, pData);
}

Task::ITaskBase* CDemoSymbolDB::createSymbol(int nSymbol)
{
	switch(nSymbol)
	{// ñhå‰ë§ÇÃÇégÇ§Ç»ÇÁÇ±Ç±Ç≈ó}Ç¶ÇÈ
		case DEF_DEFAULT_MORPH:		return pDef_->createSymbolStr("DEFAULT_MORPH");
		case DEF_DEFENCE_MORPH:		return pDef_->createSymbolStr("DEFENCE_MORPH");
		case DEF_AVOID_MORPH:		return pDef_->createSymbolStr("AVOID_MORPH");
		case DEF_HIT_MORPH:			return pDef_->createSymbolStr("HIT_MORPH");

		case DEF_DEFAULT_ROTATE:	return pDef_->createSymbolStr("DEFAULT_ROTATE");
		case DEF_DEFENCE_ROTATE:	return pDef_->createSymbolStr("DEFENCE_ROTATE");
		case DEF_AVOID_ROTATE:		return pDef_->createSymbolStr("AVOID_ROTATE");
		case DEF_HIT_ROTATE:		return pDef_->createSymbolStr("HIT_ROTATE");

		case DEF_DEFAULT_SIZE:		return pDef_->createSymbolStr("DEFAULT_SIZE");
		case DEF_DEFENCE_SIZE:		return pDef_->createSymbolStr("DEFENCE_SIZE");
		case DEF_AVOID_SIZE:		return pDef_->createSymbolStr("AVOID_SIZE");
		case DEF_HIT_SIZE:			return pDef_->createSymbolStr("HIT_SIZE");

		case DEF_DEFAULT:			return pDef_->createSymbolStr("DEFAULT");
		case DEF_DEFENCE:			return pDef_->createSymbolStr("DEFENCE");
		case DEF_AVOID:				return pDef_->createSymbolStr("AVOID");
		case DEF_HIT:				return pDef_->createSymbolStr("HIT");

		default:					return CSymbolDB::createSymbol(nSymbol);
	}

}

CDemoMovieClip* CDemoSymbolDB::createDemoSymbol(int nID)
{
	CDemoMovieClip* pClip = new CDemoMovieClip();
	setMovieClip(pClip, getSymbolData(nID));
	return pClip;
}

CDemoMovieClip* CDemoSymbolDB::createDemoSymbolStr(const string& sID)
{
	CDemoMovieClip* pClip = new CDemoMovieClip();
	setMovieClip(pClip, getSymbolData(symbolID_.getValue(sID)));
	return pClip;
}

} // namespace Demo end
} // namespace BMW end