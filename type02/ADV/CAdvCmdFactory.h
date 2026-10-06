/*
	katze 05/05/18
	ADVのコマンドファクトリ
*/
#pragma once

namespace BMW{

namespace Sound{
namespace Code{
class CCmdSound;
} // namespace Code end
} // namespace Sound end

namespace ADV{
class CADVContext;
class CCmdMsg;
class CCmdBack;
class CCmdMsgState;
class CCmdFade;
class CCmdValid;
struct CCmdChangeChara;
struct CCmdTrain;
struct CCmdTrainAll;
struct CCmdItemCtrl;

class CAdvCmdFactory
{/**
	ADVのコマンドファクトリ
 */
public:
	static void createMsg(int nSide, int nChara, int nFace, int nString, bool bMask, VM::CScript* script);
	static void createMsg(CCmdMsg& cmd, CADVContext& context, VM::CScript* script);
	static void createBack(int nBack, VM::CScript* script);
	static void createBack(CCmdBack& cmd, CADVContext& context, VM::CScript* script);
	static void createMsgState(int Side, int nCtrl, bool bFlag, VM::CScript* script);
	static void createMsgState(CCmdMsgState& cmd, CADVContext& context, VM::CScript* script);
	static void createFade(int nCtrl, int nFrame, int nColor, VM::CScript* script);
	static void createFade(CCmdFade& cmd, Task::CTaskContext& context, VM::CScript* script);
	static void createSeWait(Sound::Code::CCmdSound& cmd, VM::CScript* script,bool bAdv=true);
	static void createFrameWait(int nFrame, Task::CTaskContext& context, VM::CScript* script);

	static void createValid(CCmdValid& cmd, VM::CScript* script);
	static void createChangeChara(CCmdChangeChara& cmd, VM::CScript* script);
	static void createTrain(CCmdTrain& cmd, VM::CScript* script);
	static void createTrainAll(CCmdTrainAll& cmd, VM::CScript* script);
	static void createItemCtrl(CCmdItemCtrl& cmd, VM::CScript* script);
	static void createScenario(int nID, VM::CScript* script);
};

} // namespace ADV end
} // namespace BMW end