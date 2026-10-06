/*
	katze 05/04/26
	SLG用VM
*/
#pragma once

namespace BMW{
namespace SLG{

class CSlgVM : public VM::CVM
{/**
	SLG用VM

	ようは、noExitをオーバーライドするだけだが
 */
public:
	// デストラクタ
	virtual ~CSlgVM(){}
#ifdef BMW_DEBUG
	// 操作
	void jumpTaskList(int nID){ VM::CVM::jumpTaskList(nID); CDbg().Out("Jump %d",nID); }
	void callTaskList(int nID,bool bFast){ VM::CVM::callTaskList(nID,bFast); CDbg().Out("Call %d",nID); }
	void returnTaskList(){ VM::CVM::returnTaskList(); CDbg().Out("Return"); }
#endif

protected:
	void noExist(Task::CTaskContext*);
};

} // namespace SLG end
} // namespace BMW end