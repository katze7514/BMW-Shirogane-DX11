/*
	katze 05/05/23
	update 06/03/26
	コード用ゲージ
*/
#pragma once

namespace BMW{
namespace Demo{
namespace Code{

class CCode_gage : public BMW::Rule::IRuleTask
{/**
	コード用ゲージ
 */
public:
	enum eKind{
		ATTACK_CHANGE,
		ATTACK_HP,
		ATTACK_WEAPON_EN,
		ATTACK_SKILL_EN,
		ATTACK_SKILL_DEF_EN,
		COUNTER_CHANGE,
		COUNTER_HP,
		COUNTER_WEAPON_EN,
		COUNTER_SKILL_EN,
		COUNTER_SKILL_DEF_EN,
		ATTACK_BACK_CHANGE,
		ATTACK_BACK_HP,
		ATTACK_BACK_WEAPON_EN,
		ATTACK_BACK_SKILL_EN,
		ATTACK_BACK_SKILL_DEF_EN,
		COUNTER_BACK_CHANGE,
		COUNTER_BACK_SKILL_DEF_EN,
		INTRO,
		EXIT,
	};
	// タスク
	void OnAction(Task::CTaskContext*);

	// 設定・取得
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }

private:
	int nKind_;
};

} // namespace Code end
} // namespace Demo end
} // namespace BMW end