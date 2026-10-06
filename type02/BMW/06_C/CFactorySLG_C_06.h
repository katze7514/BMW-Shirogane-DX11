/*
	katze 05/07/14
	C_06用サブルーチンファクトリ
*/
#pragma once

#include "../../SLG/VM/CSubroutineFactorySLG.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace C_06{

class CFactorySLG_C_06: public CSubroutineFactorySLG
{/*
	C_06用サブルーチンファクトリ
*/
public:
	smart_ptr<Task::ITaskList> createTaskListUser(int nID);
	void setScript(CSLGContext*);

private:
	int nTrap_;
};

} // namespace C_06 end
} // namespace SLG end
} // namespace BMW end
