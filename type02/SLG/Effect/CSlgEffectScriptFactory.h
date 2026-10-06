/*
	katze 06/05/24
	エフェクトスクリプト生成
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGContext;

namespace Effect{
struct CCmdLoad;
struct CCmdAddSymbol;
struct CCmdNo;
struct CCmdCtrl;
struct CCmdMapScroll;
struct CCmdMapChip;

class CSlgEffectScriptFactory
{
public:
	static void createLoad(CCmdLoad& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createAddEvent(CCmdAddSymbol& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createAddMap(CCmdAddSymbol& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createDel(int nNo, VM::CScript* pScript);
	static void createWait(int nNo, VM::CScript* pScript);
	static void createCtrl(CCmdCtrl& cmd, VM::CScript* pScript);
	static void createMapScroll(CCmdMapScroll& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createBack(CCmdLoad& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createMapChip(CCmdMapChip& cmd, CSLGContext* p, VM::CScript* pScript);
	static void createSkip(int nSkip, VM::CScript* pScript);
	static void createHelp(int nHelp, VM::CScript* pScript);
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end