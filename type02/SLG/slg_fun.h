/**
	katze 06/02/16
	設定関数とか
*/
#pragma once

namespace BMW{

namespace Chara{
class CValidCond;
class CValidSpirit;
} // namespace Chara end

namespace Spirit{
class CSpiritDB;
} // namespace Spirit end

namespace SLG{
//////////////////////////////////////////////
// カーソル移動
//////////////////////////////////////////////
void moveCursol(Task::CTaskContext* p);
void moveCursol(CSLGContext* p);

//////////////////////////////////////////////
// インターフェイス設定
//////////////////////////////////////////////
// 状態変化設定
void setJotai(GUI::CPanel* pPanel, const Chara::CValidCond& cond, bool bApper=true);
// 援護
void setEngo(GUI::CPanel* pPanel, int nAttack, int nDef, bool bApper=true);
// 精神
void setSpirits(GUI::CPanel* pPanel, const Chara::CValidSpirit& spirit, Spirit::CSpiritDB& db);
} // namespace SLG end
} // namespace BMW end