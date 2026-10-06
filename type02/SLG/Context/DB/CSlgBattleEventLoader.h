/*
	katze 06/04/19
	戦闘イベントスクリプトローダ
*/
#pragma once

namespace BMW{
namespace SLG{
class CSLGDef;

namespace Event{
class CBattleEventData;
} // namespace Event end

class CSlgBattleEventLoader
{
public:
	Event::CBattleEventData* createBattleEvent(const string& sData);

	// アクセッサ
	void setDef(CSLGDef* pDef){ pDef_ = pDef; }

private:
	CSLGDef* pDef_;
};

} // namespace SLG end
} // namespace BMW end