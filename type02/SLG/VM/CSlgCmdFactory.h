/*
	katze 05/05/06
	SLGでのコマンドファクトリ
*/
#pragma once

namespace BMW{

namespace VM{
class CScript;
} // namespace VM end

namespace SLG{

namespace Code{
class CCmdAddChara;
class CCmdAddCharaMap;
class CCmdDelCharaMap;
class CCmdMsg;
struct CCmdBattle;
struct CCmdChara;
class CCmdSally;
struct CCmdVicChange;
struct CCmdChangeChara;
} // namespace Code end

class CSLGContext;

class CSlgCmdFactory
{/**
	SLGでのコマンドファクトリ

	ま、デバッグ用のコマンド生成クラスかな
	これを利用して、SLGスクリプトからの生成を効率化
	する目的もあるけど
 */
public:
	static void createAddChara(int nSlg, int nTrain, int nChara, int Phase, int nID, list<int>& listParam, VM::CScript* script, int nTarget=-1);
	static void createAddChara(Code::CCmdAddChara& cmd, CSLGContext& context, VM::CScript* script);

	static void createSetWeapon(int nSlg, VM::CScript* script);
	static void createSetWeapon(const string& sSlg, CSLGContext& context, VM::CScript* script);

	static void createAddCharaMap(int nSlg, int nIndex, int Way, int nEffect, VM::CScript* script);
	static void createAddCharaMap(int nSlg, int nIndex, int Way, int nEffect, int nType, int nTarget, int nOn, VM::CScript* script);
	static void createAddCharaMap(Code::CCmdAddCharaMap& cmd, CSLGContext& context, VM::CScript* script);

	static void createDelChara(int nSlg, VM::CScript* script);
	static void createDelChara(const string& sSlg, CSLGContext& context, VM::CScript* script);

	static void createDelCharaMap(int nSlg, int nEffect, VM::CScript* script);
	static void createDelCharaMap(Code::CCmdDelCharaMap& cmd, CSLGContext& context, VM::CScript* script);

	static void createMsg(int nSide, int nChara, int nFace, int nString, bool bMask, VM::CScript* script, bool nSlg=true);
	static void createMsg(int nSide, int nSlg, int nChara, int nFace, int nString, bool bMask, VM::CScript* script);
	static void createMsg(Code::CCmdMsg& cmd, CSLGContext& context, VM::CScript* script);

	static void createMsgState(bool bVisible, VM::CScript* script);

	static void createChara(Code::CCmdChara& cmd, CSLGContext& p, VM::CScript* script);

	static void cerateEventBattle(Code::CCmdBattle& cmd, VM::CScript* script);
	static void cerateEventBattle(int nID, bool bDemo, int nSide, VM::CScript* script);

	static void createPhaseBall(bool b, VM::CScript* pScript);

	static void createSally(Code::CCmdSally& cmd, CSLGContext& context, VM::CScript* pScript);

	static void createVic(Code::CCmdVicChange& cmd, VM::CScript* pScript);

	static void createChangeChara(Code::CCmdChangeChara& cmd, CSLGContext& context, VM::CScript* pScript);

	static void createEnemyReset(VM::CScript* pScript);

	static void createWaitFrame(int nFrame, VM::CScript* pScript);
};

} // namespace SLG end
} // namespace BMW end