/*
	katze 05/05/22
	シンボル用ScriptExec
*/
#pragma once

#include "../VM/CScriptExec.h"

namespace BMW{
namespace Movie{

class CScriptExecSymbol : public VM::CScriptExec
{/**
	シンボル用ScriptExec

	ようはループ実行する
 */
public:
	virtual ~CScriptExecSymbol(){}

	virtual void OnAction(Task::CTaskContext*);
};

} // namespace Movie end
} // namespace BMW end