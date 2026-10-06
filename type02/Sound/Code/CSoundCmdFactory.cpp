#include "stdafx.h"

#include "../SoundCode.h"
#include "../SoundCmd.h"
#include "CSoundCmdFactory.h"

namespace BMW{
namespace Sound{
namespace Code{

void CSoundCmdFactory::createBgm(int nCtrl, int nID, int nFade, VM::CScript* script)
{
	// ‘€ì‚É‚æ‚Á‚ÄAÏ‚Ý‚ª•Ï‚í‚é
	switch(nCtrl)
	{
	case Ctrl::PLAY:
	{
		VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
		pPush->setState(nID);
		script->addCode(pPush);
	}
	break;

	case Ctrl::FADE_OUT:
	{
		VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
		pPush->setState(nFade);
		script->addCode(pPush);
	}
	break;

	case Ctrl::FADE_IN:
	{
		VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
		pPush->setState(nID);
		script->addCode(pPush);
		pPush = new VM::Code::CCode_ipush();
		pPush->setState(nFade);
		script->addCode(pPush);
	}
	break;

	default: break;
	}
	CCode_bgm* pBgm = new CCode_bgm();
	pBgm->setState(nCtrl);
	script->addCode(pBgm);
}

void CSoundCmdFactory::createBgm(CCmdSound& cmd, Task::CTaskContext* context, VM::CScript* script)
{
	int nID = Sound::Const::bgmID_.getValue(cmd.getBgm());
	createBgm(cmd.getCtrl(),nID,cmd.getFade(),script);
}

void CSoundCmdFactory::createSe(int nCtrl, int nID, VM::CScript* script)
{
	VM::Code::CCode_ipush* pPush = new VM::Code::CCode_ipush();
	pPush->setState(nID);
	script->addCode(pPush);
	CCode_se* pSe = new CCode_se();
	pSe->setState(nCtrl);
	script->addCode(pSe);
}

void CSoundCmdFactory::createSe(CCmdSound& cmd, Task::CTaskContext* context, VM::CScript* script)
{
	int nID = Sound::Const::seID_.getValue(cmd.getBgm());
	createSe(cmd.getCtrl(),nID,script);
}

} // namespace Code end
} // namespace VM end
} // namespace BMW end