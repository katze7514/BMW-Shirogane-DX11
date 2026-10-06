/*
	katze 06/05/24
	エフェクトスクリプトローダ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CSlgEffectScriptLoader
{/**
	エフェクトスクリプトローダ
	受け取ったスクリプトに追記する形で行われる
 */
public:
	void createEffectScript(const string& sData, VM::CScript* pScript);

	// アクセッサ
	void setSLGContext(CSLGContext* p){ p_=p; }

private:
	CSLGContext* p_;
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end