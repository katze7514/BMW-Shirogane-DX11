/*
	katze 06/06/25
	T_20用サブルーチンファクトリ
*/
#pragma once

#include "../../SLG/VM/CSubroutineFactorySLG.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace T_20{

class CFactorySLG_T_20: public CSubroutineFactorySLG
{/*
	T_20用サブルーチンファクトリ
*/
public:
	smart_ptr<Task::ITaskList> createTaskList(int nID);
	void setScript(CSLGContext*);

private:
	int nArcher_;
};

} // namespace T_20 end
} // namespace SLG end
} // namespace BMW end
