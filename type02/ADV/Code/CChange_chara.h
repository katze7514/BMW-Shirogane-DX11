/*
	katze 06/06/13
	キャラ養成データコピー
*/
#pragma once

namespace BMW{
namespace ADV{
namespace Code{

class CChange_chara : public BMW::Rule::IRuleTask
{/**
	キャラ養成データコピー
 */
public:
	void OnAction(Task::CTaskContext*);
};

} // namespace Code end
} // namespace ADV end
} // namespace BMW end