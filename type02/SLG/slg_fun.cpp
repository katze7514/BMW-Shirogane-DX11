#include "stdafx.h"

#include "../Chara/CValidCond.h"
#include "../Chara/CValidSpirit.h"
#include "../Scene/GUI/CCircleMenu.h"
#include "../Scene/GUI/CCircleMenuButton.h"

#include "Context/CSLGContext.h"

#include "slg_fun.h"

namespace BMW{
namespace SLG{
//////////////////////////////////////////////
// ÉJÅ[É\Éãà⁄ìÆ
//////////////////////////////////////////////
void moveCursol(Task::CTaskContext* p)
{
	moveCursol(static_cast<CSLGContext*>(p));
}

void moveCursol(CSLGContext* p)
{
	int nX,nY;
	p->setCirclePos(Pos::CHIP,nX,nY);
	p->getInput()->setCursolPos(nX,nY);
}

///////////////////////////////////////////////////////////////
// ÉXÉeÅ[É^ÉXê›íË
///////////////////////////////////////////////////////////////
void setJotai(GUI::CPanel* pPanel, const Chara::CValidCond& cond, bool bApper)
{// èÛë‘ïœâªê›íË
	pPanel->visibleAll(false);
	// ÇΩÇæÇµÅAîwåiÇ™Ç†ÇÈÇ»ÇÁÇªÇÍÇÕï\é¶
	if(pPanel->getWidget("BACK")!=NULL)
		pPanel->getWidget("BACK")->visible(true);

	using Chara::CValidCond;
	if(bApper)
	{
		// çsìÆ
		Task::ITaskBase* pBase = pPanel->getWidget("ACTION");
		pBase->valid(cond.IsValid(CValidCond::ACTION));
		pBase->visible(cond.IsValid(CValidCond::ACTION));
		// à⁄ìÆ
		pBase = pPanel->getWidget("MOVE");
		pBase->valid(cond.IsValid(CValidCond::MOVE));
		pBase->visible(cond.IsValid(CValidCond::MOVE));
		// ñhå‰
		pBase = pPanel->getWidget("TOUGH");
		pBase->valid(cond.IsValid(CValidCond::DEFENCE));
		pBase->visible(cond.IsValid(CValidCond::DEFENCE));
		// ñΩíÜ
		pBase = pPanel->getWidget("HIT");
		pBase->valid(cond.IsValid(CValidCond::HIT));
		pBase->visible(cond.IsValid(CValidCond::HIT));
		// âÒî
		pBase = pPanel->getWidget("AVOID");
		pBase->valid(cond.IsValid(CValidCond::AVOID));
		pBase->visible(cond.IsValid(CValidCond::AVOID));
	}
}

void setEngo(GUI::CPanel* pPanel, int nAttack, int nDef, bool bApper)
{// âáåÏ
	pPanel->visibleAll(false);
	pPanel->getWidget("BACK")->visible(true);
	if(bApper)
	{
		// çUåÇ
		for(int i=0; i<nAttack; i++)
			pPanel->getWidget(Misc::linkStrAndNum("ENGO_ATK",i+1))->visible(true);

		// ñhå‰
		for(int i=0; i<nDef; i++)
			pPanel->getWidget(Misc::linkStrAndNum("ENGO_DEF",i+1))->visible(true);
	}
}

namespace{
__inline void setSpiritGraphic(GUI::CPanel* pPanel, int nID, Spirit::CSpiritDB& db, int& nPos)
{
	GUI::CGraphicPopUp* pGraphic = pPanel->getWidgetCast<GUI::CGraphicPopUp>(Misc::linkStrAndNum("ICON",nPos++));
	db.setGraphicHolder(pGraphic, nID);
	pGraphic->valid(true);
	pGraphic->visible(true);
}
} // namespace end
void setSpirits(GUI::CPanel* pPanel, const Chara::CValidSpirit& spirit, Spirit::CSpiritDB& db)
{// ê∏ê_
	pPanel->validAll(false);
	pPanel->visibleAll(false);
	using Chara::CValidSpirit;
	int nPos=1;
	// îMåå
	if(spirit.IsValid(CValidSpirit::FIREBALL))
		setSpiritGraphic(pPanel, Spirit::FIREBALL, db, nPos);
	// ç∞
	if(spirit.IsValid(CValidSpirit::SPIRIT))
		setSpiritGraphic(pPanel, Spirit::SPIRIT, db, nPos);
	// Ç–ÇÁÇﬂÇ´
	if(spirit.IsValid(CValidSpirit::AVOID))
		setSpiritGraphic(pPanel, Spirit::AVOID, db, nPos);
	// ïsã¸
	if(spirit.IsValid(CValidSpirit::TOUGH))
		setSpiritGraphic(pPanel, Spirit::TOUGH, db, nPos);
	// ìSï«
	if(spirit.IsValid(CValidSpirit::DEFENCE))
		setSpiritGraphic(pPanel, Spirit::DEFENCE, db, nPos);
	// èWíÜ
	if(spirit.IsValid(CValidSpirit::CONCENT))
		setSpiritGraphic(pPanel, Spirit::CONCENT, db, nPos);
	// ïKíÜ
	if(spirit.IsValid(CValidSpirit::HIT))
		setSpiritGraphic(pPanel, Spirit::HIT, db, nPos);
	// â¡ë¨
	if(spirit.IsValid(CValidSpirit::ACC))
		setSpiritGraphic(pPanel, Spirit::ACC, db, nPos);
	// íµñÙ
	if(spirit.IsValid(CValidSpirit::JUMP))
		setSpiritGraphic(pPanel, Spirit::JUMP, db, nPos);
	// äoê¡
	if(spirit.IsValid(CValidSpirit::AWAKE))
		setSpiritGraphic(pPanel, Spirit::AWAKE, db, nPos);
	// ÇƒÇ©Ç∞ÇÒ
	if(spirit.IsValid(CValidSpirit::EASYON))
		setSpiritGraphic(pPanel, Spirit::EASYON, db, nPos);
	// ë_åÇ
	if(spirit.IsValid(CValidSpirit::SNIPE))
		setSpiritGraphic(pPanel, Spirit::SNIPE, db, nPos);
	// íºåÇ
	if(spirit.IsValid(CValidSpirit::DIRECT))
		setSpiritGraphic(pPanel, Spirit::DIRECT, db, nPos);
    // ìÀåÇ
	if(spirit.IsValid(CValidSpirit::CHARGE))
		setSpiritGraphic(pPanel, Spirit::CHARGE, db, nPos);
	// çKâ^
	if(spirit.IsValid(CValidSpirit::FORTUNE))
		setSpiritGraphic(pPanel, Spirit::FORTUNE, db, nPos);
    // ìwóÕ
	if(spirit.IsValid(CValidSpirit::EFFORT))
		setSpiritGraphic(pPanel, Spirit::EFFORT, db, nPos);
	// êMîO
	if(spirit.IsValid(CValidSpirit::FAITH))
		setSpiritGraphic(pPanel, Spirit::FAITH, db, nPos);
	// íßî≠
	if(spirit.IsValid(CValidSpirit::PROVO))
		setSpiritGraphic(pPanel, Spirit::PROVO, db, nPos);
}

} // namespace SLG end
} // namespace BMW end