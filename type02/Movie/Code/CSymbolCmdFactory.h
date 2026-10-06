/*
	katze 05/05/26
	シンボルコマンドファクトリ
*/
#pragma once

namespace BMW{
namespace Movie{

class CSymbolCmdFactory
{/**
	シンボルコマンドファクトリ
 */
public:
	static void createSeWait(int nSE, VM::CScript* script);
};

} // namespace Movie end
} // namespace BMW end