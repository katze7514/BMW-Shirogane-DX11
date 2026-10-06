/*
	katze 05/03/29
	ルールリストを扱う基底クラス
*/
#pragma once

namespace BMW{
namespace Rule{

class IRuleList : public Task::CTaskListAfter
{/**
	ルールリストを扱う基底クラス
 */
public:
	// デストラクタ
	virtual ~IRuleList(){}

protected:
	// ルールリストチェンジ
	void ruleChange(int nRule, int nState=0)
	{

	}
};

} // namespace Rule end
} // namespace BMW end