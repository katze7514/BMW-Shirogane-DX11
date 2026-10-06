/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	move_road
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
class CSLGContext;
namespace Move{

class CMove_road : public Task::ITaskList
{/**
	move_road
 */
public:
	// É^ÉXÉN
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	CSLGContext* p;
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
