/*
	katze 05/05/18
	ADVのVM
*/
#pragma once

namespace BMW{
namespace ADV{

class CAdvVM : public VM::CVM
{/**
	ADVのVM

	ADV VMの構造は、SLGとほぼ同様
	ただ、ADVの場合のAPIに相当するのは、選択ウインドウぐらい
	それ以外は、いわば、main関数を実行するだけ
 */
public:
	// デストラクタ
	virtual ~CAdvVM(){}

#ifdef BMW_DEBUG
	void jumpTaskList(int nID){ VM::CVM::jumpTaskList(nID); CDbg().Out("Jump %d",nID); }
	void callTaskList(int nID,bool bFast){ VM::CVM::callTaskList(nID,bFast); CDbg().Out("Call %d",nID); }
	void returnTaskList(){ VM::CVM::returnTaskList(); CDbg().Out("Return"); }
#endif

protected:
	void noExist(Task::CTaskContext*);
};

} // namespace ADV end
} // namespace BMW end