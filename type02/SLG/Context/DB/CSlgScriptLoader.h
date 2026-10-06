/*
	katze 06/04/19
	イベントスクリプトローダ
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGDef;

class CSlgScriptLoader
{
public:
	VM::CScript* createScript(const string& sData);

	// アクセッサ
	void setSLGContext(CSLGContext* p){ p_=p; }
	void setDef(CSLGDef* pDef){ pDef_ = pDef; }

private:
	CSLGContext* p_;
	CSLGDef* pDef_;
};

} // namespace SLG end
} // namespace BMW end