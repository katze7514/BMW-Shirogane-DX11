#include "stdafx.h"

#include "CDataCharaBase.h"
#include "CDataCharaGrowthAbility.h"

namespace BMW{
namespace Chara{

void CDataCharaGrowthAbility::apply(CDataCharaBase* base, int nLv)
{// Lvに合わせて技能・精神を取得する
	// 精神
	int i;
	for(i=0; i<6; i++)
	{
		// Lv順に並んでいるので、覚えるLvが
		// 現在値を超えてたら終了
		if(spirit_[i].getLv() > base->getLv()) break;
		// 適用済みLv以下だったら、次へ
		if(spirit_[i].getLv() <= nLv) continue;
		// そうじゃなければ、追加
		base->setSpirit(spirit_[i].getAbility(),i);
	}

	// 技能
	CValidSkill valid;
	skill_g_list::iterator it;
	beginSkill();
	while(!endSkill())
	{
		it=nextSkill();
		// 養成できる最大Lvを計算
		valid.decValid(it->getAbilityID());
		// Lv順に並んでいるので、覚えるLvが現在値を超えてなく
		// 適用済みLv以上だったら、追加
		// ただし、同じ技能IDがあったら上書きされる
		if(it->getLv() <= base->getLv()
		&& it->getLv() > nLv)
			base->addSkill(it->getAbility());

	}
	// 養成できる最大Lvを設定
	base->setSkillValid(valid);
}

} // namespace Chara end
} // namespace BMW end