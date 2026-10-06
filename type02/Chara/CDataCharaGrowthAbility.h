/*
	katze 05/02/28
	キャラのスキル系成長データ
*/
#pragma once

#include "CStatusGrowthAbility.h"

namespace BMW{
namespace Chara{

class CDataCharaBase;

class CDataCharaGrowthAbility
{/**
	キャラの成長データ
	ようは、精神・技能のLvによる成長データ
	固有は成長しないから、固有なんだよｗ
 */
public:
	// 設定・取得
	const CStatusGrowthAbility& getSpirit(int nSlot) const { return spirit_[nSlot]; }
	void						setSpirit(const CStatusGrowthAbility& growth,int nSlot){ spirit_[nSlot]=growth; }

	// 操作
	// 技能リスト
	skill_g_list::iterator	beginSkill() const
							{ 
								CDataCharaGrowthAbility* pA = const_cast<CDataCharaGrowthAbility*>(this);
								pA->it_s=pA->listSkill_.begin();
								return pA->it_s;
							}
	bool					endSkill() const
							{ 
								CDataCharaGrowthAbility* pA = const_cast<CDataCharaGrowthAbility*>(this);
								return pA->it_s==pA->listSkill_.end();
							}
	skill_g_list::iterator	nextSkill() const
							{ 
								CDataCharaGrowthAbility* pA = const_cast<CDataCharaGrowthAbility*>(this);
								return pA->it_s++; 
							}
	void					addSkill(const CStatusGrowthAbility& skill)
							{// Lv順に並べておく 
								for(it_s=listSkill_.begin(); it_s!=listSkill_.end(); ++it_s)
									if(it_s->getLv() > skill.getLv()) break;

								listSkill_.insert(it_s,skill);
							}
	bool					IsValid()const{ return spirit_[0].getAbilityID()>=0 || !listSkill_.empty(); }
	
	// データ適用
	void					apply(CDataCharaBase* base, int nLv=0);

private:
	// 精神成長データリスト
	// つっても、精神は6個までなので配列
	CStatusGrowthAbility spirit_[6];
	// 技能成長データリスト
	skill_g_list listSkill_;
	skill_g_list::iterator it_s;
};

} // namespace Chara end
} // namespace BMW end