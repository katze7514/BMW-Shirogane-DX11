/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	get_chara
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
namespace Code{

class CCode_get_chara : public BMW::Rule::IRuleTask
{/**
	get_chara

	どのパラメタが対象なのかは、stateで代用
 */
public:
	enum eState{
		LV,
		PHASE,
		HP,
		HP_MAX,
		EN,
		EN_MAX,
		SP,
		SP_MAX,
		MENTAL,
		MAP,
		ACT,
		WAY,
	};
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
