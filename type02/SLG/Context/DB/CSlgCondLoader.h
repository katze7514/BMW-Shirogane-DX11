/*
	katze 06/04/09
	SLG条件ローダ
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGDef;

class CSlgCondLoader
{/*
	SLG条件ローダ
 */
public:
	// ロード
	ISlgCond* createCond(const string& sData);

	// アクセッサ
	void setDef(CSLGDef* pDef){ pDef_ = pDef; }

private:
	CSLGDef* pDef_;
};

} // namespace SLG end
} // namespace BMW end