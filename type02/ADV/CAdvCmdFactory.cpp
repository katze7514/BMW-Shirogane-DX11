#include "stdafx.h"

#include "../Face/DB/CFaceDB.h"
#include "../Sound/SoundCmd.h"

#include "IDADV.h"
#include "ConstAdv.h"

#include "Code/CCode_msg_state.h"
#include "Code/CCode_fade.h"
#include "Code/CCode_valid.h"
#include "Code/CChange_chara.h"
#include "Code/CCode_train.h"
#include "Code/CCode_train_all.h"

#include "CADVContext.h"

#include "AdvCmd.h"
#include "CAdvCmdFactory.h"

namespace BMW{
namespace ADV{

void CAdvCmdFactory::createMsg(int nSide, int nChara, int nFace, int nString, bool bMask, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush;
	// サイド
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSide);
	script->addCode(pPush);
	// キャラID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nChara);
	script->addCode(pPush);
	// 顔ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nFace);
	script->addCode(pPush);
	// 文字列ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nString);
	script->addCode(pPush);
	// マスクフラグ
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(bMask?1:0);
	script->addCode(pPush);
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::MSG);
	script->addCode(pCall);
}

void CAdvCmdFactory::createMsg(CCmdMsg& cmd, CADVContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("MSG %s",cmd.getText().c_str());
#endif
	int nChara	= Face::Const::faceID_.getValue(cmd.getChara());
	int	nFace	= context.getApp()->getFaceMap().getFaceDB(nChara)->getFaceID(cmd.getFace());
	LPSTR pStr = const_cast<LPSTR>(cmd.getText().c_str());
	CLineParser::ConvertCR(pStr);
	int nString	= context.setString(string(pStr));

	createMsg(cmd.getSide(),nChara,nFace,nString,cmd.IsMask(),script);
}

void CAdvCmdFactory::createBack(int nBack, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush;
	// 背景ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nBack);
	script->addCode(pPush);
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::BACK);
	script->addCode(pCall);
}

void CAdvCmdFactory::createBack(CCmdBack& cmd, CADVContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("BACK %s",cmd.getBack().c_str());
#endif
	createBack(Const::backID_.getValue(cmd.getBack()),script);
}

void CAdvCmdFactory::createMsgState(int nSide, int nCtrl, bool bFlag, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush;
	// サイドID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSide);
	script->addCode(pPush);
	// メッセージ状態変更
	Code::CCode_msg_state* pState = new Code::CCode_msg_state();
	pState->setState(nCtrl);
	pState->visible(bFlag);
	script->addCode(pState);
}

void CAdvCmdFactory::createMsgState(CCmdMsgState& cmd, CADVContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("STATE %d %d %d",cmd.getSide(),cmd.getCtrl(),cmd.IsFlag());
#endif
	createMsgState(cmd.getSide(),cmd.getCtrl(),cmd.IsFlag(),script);
}


void CAdvCmdFactory::createFade(int nCtrl, int nFrame, int nColor, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush;
	// 制御コード
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nCtrl);
	script->addCode(pPush);
	// フレーム
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nFrame);
	script->addCode(pPush);
	// 色
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nColor);
	script->addCode(pPush);
	// フェード
	Code::CCode_fade* pFade = new Code::CCode_fade();
	script->addCode(pFade);
}

void CAdvCmdFactory::createFade(CCmdFade& cmd, Task::CTaskContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("Fade %d %d %d",cmd.getCtrl(),cmd.getFrame(),cmd.getColor());
#endif
	createFade(cmd.getCtrl(),cmd.getFrame(),cmd.getColor(),script);
}

#include "../SLG/IDRule.h"
void CAdvCmdFactory::createSeWait(Sound::Code::CCmdSound& cmd, VM::CScript* script, bool bAdv)
{
	int nID = Sound::Const::seID_.getValue(cmd.getBgm());
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nID);
	script->addCode(pPush);
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(bAdv? Rule::WAIT_SE : BMW::SLG::Rule::WAIT_SE);
	script->addCode(pCall);
}

void CAdvCmdFactory::createValid(CCmdValid& cmd, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("Valid %d ",cmd.getCtrl());
#endif
	Code::CCode_valid* pValid = new Code::CCode_valid();
	pValid->setState(cmd.getCtrl());

	if(cmd.getCtrl()==Code::CCode_valid::CLEAR)
	{
		script->addCode(pValid);
	}
	else
	{
		VM::Code::CCode_ipush* pPush;
		// まずは-1を積んでおく
		pPush = new VM::Code::CCode_ipush();
		pPush->setState(-1);
		script->addCode(pPush);
		
		list<string>& List = cmd.getCharaList();
		list<string>::iterator it;
		for(it=List.begin(); it!=List.end(); it++)
		{
			pPush = new VM::Code::CCode_ipush();
			pPush->setState(Chara::Const::charaID_.getValue(*it));
			script->addCode(pPush);
		}
		script->addCode(pValid);
	}
}

void CAdvCmdFactory::createChangeChara(CCmdChangeChara& cmd, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(Chara::Const::charaID_.getValue(cmd.sTarget_));
	script->addCode(pPush);
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(Chara::Const::charaID_.getValue(cmd.sSource_));
	script->addCode(pPush);
	script->addCode(new Code::CChange_chara());
}

void CAdvCmdFactory::createTrain(CCmdTrain& cmd, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("Train %s %d",cmd.sTarget_.c_str(),cmd.nKind_);
#endif
	Code::CCode_train* pTrain = new Code::CCode_train();
	pTrain->setCharaID(Chara::Const::charaID_.getValue(cmd.sTarget_));
	pTrain->setKind(cmd.nKind_);
	if(cmd.nType_>=0) pTrain->setType(cmd.nType_);
	if(cmd.nKind_==Code::CCode_train::CHANGE
	|| cmd.nKind_==Code::CCode_train::COPY) 
		// キャラ変更の場合は、値が変更先ID
		pTrain->setValue(Chara::Const::charaID_.getValue(cmd.sValue_));
	else
		// それ以外は、単なる整数
		pTrain->setValue(cmd.nValue_);
	pTrain->setMax(cmd.nMax_);
	script->addCode(pTrain);
}

void CAdvCmdFactory::createTrainAll(CCmdTrainAll& cmd, VM::CScript* script)
{
	Code::CCode_train_all* pTrain = new Code::CCode_train_all();
	
	if(cmd.nCtrl_>=0) pTrain->setCtrl(cmd.nCtrl_);
	pTrain->setKind(cmd.nKind_);
	if(cmd.nType_>=0) pTrain->setType(cmd.nType_);
	pTrain->setValue(cmd.nValue_);
	pTrain->setMax(cmd.nMax_);

	// キャラIDの設定
	list<string>::iterator it;
	for(it=cmd.listTarget_.begin(); it!=cmd.listTarget_.end(); ++it)
		pTrain->addCharaID(Chara::Const::charaID_.getValue(*it));

	script->addCode(pTrain);
}

namespace{
__inline VM::Code::CCode_ipush* newPush(int n)
{
	return new VM::Code::CCode_ipush(n);
}
} // namespace end

void CAdvCmdFactory::createItemCtrl(CCmdItemCtrl& cmd, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("ITEM_ADD");
#endif
	// とりあえず、リスト通りに積む
	list<pair<int,int> >::iterator it=cmd.listItem_.begin();
	for(; it!=cmd.listItem_.end(); ++it)
	{// 数・IDの順に詰む
		script->addCode(newPush(it->second));
		script->addCode(newPush(it->first));
	}
	// 追加数を積む
	script->addCode(newPush(cmd.listItem_.size()));
	// 操作モードを積む
	script->addCode(newPush(cmd.nCtrl_));
	// そして、アイテム追加を呼び出す
	script->addCode(new VM::Code::CCode_call(Rule::ITEM_CTRL));
}

void CAdvCmdFactory::createScenario(int nID, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("SCENARIO %d",nID);
#endif
	// 操作タイプを積む
	script->addCode(newPush(nID));
	// そして、シナリオ呼び出し
	script->addCode(new VM::Code::CCode_call(Rule::SCENARIO));
}

void CAdvCmdFactory::createFrameWait(int nFrame, Task::CTaskContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("WAIT_FRAME %d", nFrame);
#endif
	// 待ちフレーム数
	script->addCode(newPush(nFrame));
	// フレーム待ちを呼び出す
	script->addCode(new VM::Code::CCode_call(Rule::WAIT_FRAME));
}

} // namespace ADV end
} // namespace BMW end