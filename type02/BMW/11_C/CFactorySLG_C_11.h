/*
	katze 06/06/17
	C_11用サブルーチンファクトリ
*/
#pragma once

#include "../../SLG/VM/CSubroutineFactorySLG.h"

namespace BMW{
namespace SLG{
class CSLGContext;

namespace C_11{

class CFactorySLG_C_11: public CSubroutineFactorySLG
{/*
	C_11用サブルーチンファクトリ
*/
public:
	smart_ptr<Task::ITaskList> createTaskList(int nID);
	void setScript(CSLGContext*);

private:
	int nCaos_;
};

} // namespace C_11 end
} // namespace SLG end
} // namespace BMW end
