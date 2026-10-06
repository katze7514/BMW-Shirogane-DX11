#include "stdafx.h"

#include "../../Face/DB/CFaceDB.h"

#include "../IDRule.h"
#include "../SlgApi.h"

#include "../Effect/Code/CCode_phase_ball_ctrl.h"

#include "../Context/DB/SlgCmd.h"
#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"

#include "../Action/IDAction.h"

#include "CSlgCmdFactory.h"

namespace BMW{
namespace SLG{

namespace{
__inline int getCharaCharaID(const string& sChara, CSLGContext& p)
{
	if(sChara=="PLAYER") return Code::CCode_chara::PLAYER;
	if(sChara=="ENEMY") return Code::CCode_chara::ENEMY;
	if(sChara=="NETUTORAL") return Code::CCode_chara::NEUTORAL;
	else return p.getSLGDef().getSlgID(sChara);
}

__inline VM::Code::CCode_ipush* createPush(int n)
{
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(n);
	return pPush;
}

} // namespace end


void CSlgCmdFactory::createAddChara(int nSlg, int nTrain, int nChara, int nPhase, int nID, list<int>& listParam, VM::CScript* script, int nTarget)
{
	VM::Code::CCode_ipush* pPush;
	// SLG ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 養成 ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nTrain);
	script->addCode(pPush);
	// キャラ ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nChara);
	script->addCode(pPush);
	// Phase ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nPhase);
	script->addCode(pPush);
	// 思考ルーチン ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nID);
	script->addCode(pPush);
	// 思考ルーチン パラメタ
	list<int>::iterator it;
	for(it=listParam.begin(); it!=listParam.end(); ++it)
	{
		pPush = new VM::Code::CCode_ipush();
		if(*it<0)
		{// 負の時はターゲットキャラ
			pPush->setState(nTarget);
		}
		else // 次の
		{	pPush->setState(*it);	}
		script->addCode(pPush);
	}
	// 思考ルーチンパラメタ数
	pPush = new VM::Code::CCode_ipush();
	pPush->setState((int)listParam.size());
	script->addCode(pPush);
	// キャラの追加
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::ADD_CHARA);
	script->addCode(pCall);
}

void CSlgCmdFactory::createAddChara(Code::CCmdAddChara& cmd, CSLGContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("ADD_CHARA %d %s %s", cmd.getID(), cmd.getSlg().c_str(), cmd.getChara().c_str());
#endif
	int nTarget=-1;
	if(!cmd.getActionInfo().getTargetChara().empty())
	{
		if(cmd.getActionInfo().getTargetChara()=="CHANGE")
			nTarget = Action::CHANGE;
		else
			nTarget = context.getSLGDef().getSlgID(cmd.getActionInfo().getTargetChara());
	}

	createAddChara(cmd.IsID() ? cmd.getID() : context.getSLGDef().getSlgID(cmd.getSlg()),
					cmd.getTrain(),
					Chara::Const::charaID_.getValue(cmd.getChara()),
					cmd.getPhase(),
					cmd.getActionInfo().getAction(),
					cmd.getActionInfo().getParam(),
					script,
					nTarget);
}

void CSlgCmdFactory::createSetWeapon(int nSlg, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("SET_WEAPON %d", nSlg);
#endif
	VM::Code::CCode_ipush* pPush;
	// 対象キャラに設定するID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 対象キャラ設定
	Code::CCode_set_target* pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::CHARA);
	script->addCode(pTarget);
	// 武器の追加
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::SETUP_WEAPON);
	script->addCode(pCall);
}

void CSlgCmdFactory::createSetWeapon(const string& sSlg, CSLGContext& context, VM::CScript* script)
{
	createSetWeapon(context.getSLGDef().getSlgID(sSlg),script);
}

void CSlgCmdFactory::createAddCharaMap(int nSlg, int nIndex, int nWay, int nEffect, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("ADD_MAP %d %d", nSlg, nIndex);
#endif
	// 対象キャラに設定するID
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 対象キャラ設定
	Code::CCode_set_target* pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::CHARA);
	script->addCode(pTarget);
	// 対象マップに設定するID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nIndex);
	script->addCode(pPush);
	// 対象マップ設定
	pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::MAP);
	script->addCode(pTarget);
	// 追加タイプ
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(0);
	script->addCode(pPush);
	// 追加向き
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nWay);
	script->addCode(pPush);
	// エフェクト
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nEffect);
	script->addCode(pPush);
	// マップへ追加
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::ADD_CHARA_MAP);
	script->addCode(pCall);
}

void CSlgCmdFactory::createAddCharaMap(int nSlg, int nIndex, int nWay, int nEffect, int nType, int nTarget, int nOn, VM::CScript* script)
{
	// 対象キャラに設定するID
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 対象キャラ設定
	Code::CCode_set_target* pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::CHARA);
	script->addCode(pTarget);
	// 対象マップに設定するID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nIndex);
	script->addCode(pPush);
	// 対象マップ設定
	pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::MAP);
	script->addCode(pTarget);
	if(nType==1)
	{// 対象キャラ付きならば
		pPush = new VM::Code::CCode_ipush();
		pPush->setState(nTarget);
		script->addCode(pPush);
		// そのキャラからみて、どこにおくか
		pPush = new VM::Code::CCode_ipush();
		pPush->setState(nOn);
		script->addCode(pPush);
	}
	// 追加タイプ
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nType);
	script->addCode(pPush);
	// 追加向き
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nWay);
	script->addCode(pPush);
	// エフェクト
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nEffect);
	script->addCode(pPush);
	// マップへ追加
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::ADD_CHARA_MAP);
	script->addCode(pCall);
}

void CSlgCmdFactory::createAddCharaMap(Code::CCmdAddCharaMap& cmd, CSLGContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("ADD_MAP %d %s", cmd.getID(), cmd.getSlg().c_str());
#endif
	createAddCharaMap(cmd.IsID() ? cmd.getID() : context.getSLGDef().getSlgID(cmd.getSlg()),
						cmd.getIndex(),
						cmd.getWay(),
						cmd.getEffect(),
						cmd.getType(),
						context.getSLGDef().getSlgID(cmd.getTarget()),
						cmd.getOn(),
						script);
}

void CSlgCmdFactory::createDelChara(int nSlg, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("DEL_CHARA %d", nSlg);
#endif

	// 対象キャラに設定するID
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 対象キャラ設定
	Code::CCode_set_target* pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::CHARA);
	script->addCode(pTarget);
	// データから削除
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::DEL_CHARA);
	script->addCode(pCall);
}

void CSlgCmdFactory::createDelChara(const string& sSlg, CSLGContext& context, VM::CScript* script)
{
	createDelChara(context.getSLGDef().getSlgID(sSlg),script);
}

void CSlgCmdFactory::createDelCharaMap(int nSlg, int nEffect,  VM::CScript* script)
{
	// 対象キャラに設定するID
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 対象キャラ設定
	Code::CCode_set_target* pTarget = new Code::CCode_set_target();
	pTarget->setState(Code::CCode_set_target::CHARA);
	script->addCode(pTarget);
	// エフェクトID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nEffect);
	script->addCode(pPush);
	// マップから削除
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::DEL_CHARA_MAP);
	script->addCode(pCall);
}

void CSlgCmdFactory::createDelCharaMap(Code::CCmdDelCharaMap& cmd, CSLGContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("DEL_MAP %d %s %d", cmd.getID(), cmd.getSlg().c_str(),cmd.getEffect());
#endif
	createDelCharaMap(cmd.IsID() ? cmd.getID() : context.getSLGDef().getSlgID(cmd.getSlg()),
						cmd.getEffect(),
						script);
}

void CSlgCmdFactory::createMsg(int nSide, int nChara, int nFace, int nString, bool bMask, VM::CScript* script, bool bSlg)
{
	VM::Code::CCode_ipush* pPush;
	// サイド
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSide);
	script->addCode(pPush);
	// キャラIDが、SLGなのかFaceなのか
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(bSlg?1:0);
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
	// MSG_BOARD
	Event::CEvent_Msg*	pMsg = new Event::CEvent_Msg();
	script->addCode(pMsg);
}

void CSlgCmdFactory::createMsg(int nSide, int nSlg, int nChara, int nFace, int nString, bool bMask, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush;
	// サイド
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSide);
	script->addCode(pPush);
	// SLG ID
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSlg);
	script->addCode(pPush);
	// 顔キャラID
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
	// MSG_BOARD
	Event::CEvent_Msg*	pMsg = new Event::CEvent_Msg();
	script->addCode(pMsg);
}

void CSlgCmdFactory::createMsg(Code::CCmdMsg& cmd, CSLGContext& context, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("MSG %s", cmd.getMsg().c_str());
#endif
	int nCharaFace;
	if(cmd.IsID()) // SLG IDそのもの
		nCharaFace = context.getCharaData(cmd.getID())->getBattle().getFaceID();
	ef(cmd.IsSlg())// SLG IDの文字列表現
		nCharaFace = context.getCharaData(cmd.getChara())->getBattle().getFaceID();
	else // FACE IDの文字列表現
		nCharaFace = Face::Const::faceID_.getValue(cmd.getChara());

	int	nFace		= context.getApp()->getFaceMap().getFaceDB(nCharaFace)->getFaceID(cmd.getFace());
	LPSTR pStr		= const_cast<LPSTR>(cmd.getMsg().c_str());
	CLineParser::ConvertCR(pStr);
	int nString		= context.setString(string(pStr));

	createMsg(cmd.getSide(),
				cmd.getSlg(),
				nCharaFace,
				nFace,
				nString,
				cmd.IsMask(),
				script);
}

void CSlgCmdFactory::createMsgState(bool bVisible, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("STATE %d", bVisible);
#endif
	Event::CEvent_Msg_state* pState = new Event::CEvent_Msg_state();
	pState->visible(bVisible);
	script->addCode(pState);
}

void CSlgCmdFactory::createChara(Code::CCmdChara& cmd, CSLGContext& p, VM::CScript* script)
{
#ifdef BMW_DEBUG
	CDbg().Out("CHARA %s %d %d", cmd.sChara_.c_str(), cmd.nKind_, cmd.nType_);
#endif
	Code::CCode_chara* pChara = new Code::CCode_chara();
	pChara->setChara(getCharaCharaID(cmd.sChara_,p));
	pChara->setKind(cmd.nKind_);
	pChara->setType(cmd.nKind_!=Code::CCode_chara::ACTION ? cmd.nType_ : cmd.action_.getAction());

	// valueの設定
	if(pChara->getKind()==Code::CCode_chara::LOVE)
	{// 告白時
		pChara->setValue(p.getSLGDef().getSlgID(cmd.action_.getTargetChara()));
	}
	// ↓ACTION設定時
	ef(!cmd.action_.getTargetChara().empty())
	{
		list<int> tmp;
		list<int>::iterator it;

		for(it=cmd.action_.getParam().begin(); it!=cmd.action_.getParam().end(); ++it)
		{
			if(*it<0)
				tmp.push_back(p.getSLGDef().getSlgID(cmd.action_.getTargetChara()));
			else
				tmp.push_back(*it);
		}

		pChara->setValueList(tmp);
	}
	else // それ以外
		pChara->setValueList(cmd.action_.getParam());

	script->addCode(pChara);
}

void CSlgCmdFactory::cerateEventBattle(Code::CCmdBattle& cmd, VM::CScript* script)
{
	cerateEventBattle(cmd.nID_,cmd.bDemo_,cmd.nSide_,script);
}

void CSlgCmdFactory::cerateEventBattle(int nID, bool bDemo, int nSide, VM::CScript* script)
{
	// データID
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nID);
	script->addCode(pPush);
	// デモフラグ
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(bDemo?1:0);
	script->addCode(pPush);
	// 攻撃サイド
	pPush = new VM::Code::CCode_ipush();
	pPush->setState(nSide);
	script->addCode(pPush);
	// イベント戦闘
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::EVENT_BATTLE);
	script->addCode(pCall);
}

void CSlgCmdFactory::createPhaseBall(bool b, VM::CScript* pScript)
{
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(b);
	pScript->addCode(pPush);
	pScript->addCode(new Effect::CCode_phase_ball_ctrl());
}

void CSlgCmdFactory::createSally(Code::CCmdSally& cmd, CSLGContext& context, VM::CScript* pScript)
{
	// まずは、キャラ
	pScript->addCode(createPush(-1));
	list<string>& listStr = cmd.getCharaList();
	list<string>::iterator sit;
	for(sit=listStr.begin(); sit!=listStr.end(); ++sit)
		pScript->addCode(createPush(context.getSLGDef().getSlgID(*sit)));

	// まずは、除外キャラ
	pScript->addCode(createPush(-1));
	listStr = cmd.getOutList();
	for(sit=listStr.begin(); sit!=listStr.end(); ++sit)
		pScript->addCode(createPush(context.getSLGDef().getSlgID(*sit)));

	// んで、マップから
	pScript->addCode(createPush(-1));
	list<int>& listInt = cmd.getIndexList();
	list<int>::iterator it;
	for(it=listInt.begin(); it!=listInt.end(); ++it)
		pScript->addCode(createPush(*it));
	// スクロールするIndex
	pScript->addCode(createPush(cmd.getIndex()));
	// 最大人数
	pScript->addCode(createPush(cmd.getMaxNum()));
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::SALLY_SELECT);
	pScript->addCode(pCall);
}

void CSlgCmdFactory::createVic(Code::CCmdVicChange& cmd, VM::CScript* pScript)
{
	// 表示フラグ
	pScript->addCode(createPush(cmd.nApper_));
	// 変更Expert
	pScript->addCode(createPush(cmd.nExpert_));
	// 変更Lose
	pScript->addCode(createPush(cmd.nLose_));
	// 変更Vic
	pScript->addCode(createPush(cmd.nVic_));
	// 変更Type
	pScript->addCode(createPush(0));
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::VICTORY_CHANGE);
	pScript->addCode(pCall);
}

void CSlgCmdFactory::createChangeChara(Code::CCmdChangeChara& cmd, CSLGContext& context, VM::CScript* pScript)
{
	// Offset
	pScript->addCode(createPush(cmd.nOffset_));
	// Flag
	pScript->addCode(createPush(cmd.nFlag_));
	// TrainID
	pScript->addCode(createPush(cmd.nTrain_));
	// CharaID
	pScript->addCode(createPush(Chara::Const::charaID_.getValue(cmd.sCharaID_)));
	// SLG ID
	// nIDが負だったらsIDを解決
	if(cmd.nID_<0) cmd.nID_=context.getSLGDef().getSlgID(cmd.sID_);
	pScript->addCode(createPush(cmd.nID_));
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::CHANGE_CHARA);
	pScript->addCode(pCall);
}

void CSlgCmdFactory::createEnemyReset(VM::CScript* pScript)
{
	pScript->addCode(createPush(1));
	pScript->addCode(new VM::Code::CCode_set_value(Flag::ENEMY_RESET));
}

void CSlgCmdFactory::createWaitFrame(int nFrame, VM::CScript* pScript)
{
	// 待ちフレーム
	pScript->addCode(createPush(nFrame));
	// Call
	VM::Code::CCode_call* pCall = new VM::Code::CCode_call();
	pCall->setState(Rule::WAIT_FRAME);
	pScript->addCode(pCall);

}

} // namespace SLG end
} // namespace BMW end