#include "stdafx.h"

#include "../../SLG/Context/CDataBattle.h"

#include "../IDDemo.h"
#include "../CDemoContext.h"
#include "../CDemoScene.h"
#include "../GUI/CDemoEasyStatus.h"
#include "../GUI/CDemoDamage.h"

#include "CCode_gage.h"

namespace BMW{
namespace Demo{
namespace Code{

namespace{
__inline void setGageDown(int nSide, int nValue, int nStatus, CDemoContext* p, int nValue2=-1)
{
	// ステータス
	smart_ptr<CDemoEasyStatus>& pGage = p->getEasyStatus(nSide);
	pGage->action(p->getValue(nValue),nStatus, p->getValue(nValue2));

	if(nStatus==CDemoEasyStatus::HP_DOWN
	|| nStatus==CDemoEasyStatus::HPEN_DOWN)// ダメージ
		p->getDamage()->action(nSide, p->getValue(nValue));
}

} // namespace end

void CCode_gage::OnAction(Task::CTaskContext* pContext)
{
	CDemoContext* p = static_cast<CDemoContext*>(pContext);

	switch(getKind())
	{
	case ATTACK_CHANGE:
		p->getDemoScene()->setStatus(CDemoScene::ATTACK);
	break;

	case ATTACK_HP:
		if(!p->getValue(Flag::BACK_DEF))
			p->getDemoScene()->calcSub(p->getValue(Flag::ATTACK_HP), CDemoScene::COUNTER, CDemoScene::HP);
		setGageDown(1-p->getBattleData()->getSide(), Flag::ATTACK_HP, CDemoEasyStatus::HPEN_DOWN, p, Flag::ATTACK_EN);
	break;

	case ATTACK_WEAPON_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::ATTACK_WEAPON_EN), CDemoScene::ATTACK, CDemoScene::EN);
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_WEAPON_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case ATTACK_SKILL_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::ATTACK_SKILL_EN), CDemoScene::ATTACK, CDemoScene::EN);
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_SKILL_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case ATTACK_SKILL_DEF_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::ATTACK_SKILL_DEF_EN), CDemoScene::ATTACK, CDemoScene::EN);
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_SKILL_DEF_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case COUNTER_CHANGE:
		p->getDemoScene()->setStatus(CDemoScene::COUNTER);
	break;

	case COUNTER_HP:
		p->getDemoScene()->calcSub(p->getValue(Flag::COUNTER_HP), CDemoScene::ATTACK, CDemoScene::HP);
		setGageDown(p->getBattleData()->getSide(), Flag::COUNTER_HP, CDemoEasyStatus::HPEN_DOWN, p, Flag::COUNTER_EN);
	break;

	case COUNTER_WEAPON_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::COUNTER_WEAPON_EN), CDemoScene::COUNTER, CDemoScene::EN);
		setGageDown(1-p->getBattleData()->getSide(), Flag::COUNTER_WEAPON_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case COUNTER_SKILL_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::COUNTER_SKILL_EN), CDemoScene::COUNTER, CDemoScene::EN);
		setGageDown(1-p->getBattleData()->getSide(), Flag::COUNTER_SKILL_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case COUNTER_SKILL_DEF_EN:
		p->getDemoScene()->calcSub(p->getValue(Flag::COUNTER_SKILL_DEF_EN), CDemoScene::COUNTER, CDemoScene::EN);
		setGageDown(1-p->getBattleData()->getSide(), Flag::COUNTER_SKILL_DEF_EN, CDemoEasyStatus::EN_DOWN,p);
	break;
		
	case ATTACK_BACK_CHANGE:
		p->getDemoScene()->setStatus(CDemoScene::ATTACK_BACK);
	break;

	case ATTACK_BACK_HP:
		setGageDown(1-p->getBattleData()->getSide(), Flag::ATTACK_BACK_HP, CDemoEasyStatus::HP_DOWN, p);
	break;

	case ATTACK_BACK_WEAPON_EN:
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_BACK_WEAPON_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case ATTACK_BACK_SKILL_EN:
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_BACK_SKILL_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case ATTACK_BACK_SKILL_DEF_EN:
		setGageDown(p->getBattleData()->getSide(), Flag::ATTACK_BACK_SKILL_DEF_EN, CDemoEasyStatus::EN_DOWN,p);
	break;

	case COUNTER_BACK_CHANGE:
		p->getDemoScene()->setStatus(CDemoScene::COUNTER_BACK);
	break;

	case COUNTER_BACK_SKILL_DEF_EN:
		setGageDown(1-p->getBattleData()->getSide(), Flag::COUNTER_BACK_SKILL_DEF_EN, CDemoEasyStatus::EN_DOWN,p);
	break;
	
	case INTRO:
		p->getEasyStatus(0)->action(0,CDemoEasyStatus::INTRO);
		p->getEasyStatus(1)->action(0,CDemoEasyStatus::INTRO);
	break;

	case EXIT:
		p->getEasyStatus(0)->action(0,CDemoEasyStatus::EXIT);
		p->getEasyStatus(1)->action(0,CDemoEasyStatus::EXIT);
	break;
	}
}

} // namespace Code end
} // namespace Demo end
} // namespace BMW end