#include "stdafx.h"

#include "../MovieCode.h"

#include "CSymbolCmdFactory.h"

namespace BMW{
namespace Movie{

void CSymbolCmdFactory::createSeWait(int nSE, VM::CScript* script)
{
	// まずは、stopコード
	Code::CCode_stop* pStop = new Code::CCode_stop();
	script->addCode(pStop);
	// SE Waitコード
	Code::CCode_se_wait* pWait = new Code::CCode_se_wait();
	pWait->setState(nSE);
	script->addCode(pWait);
}

} // namespace Movie end
} // namespace BMW end