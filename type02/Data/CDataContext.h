/*
	katze 05/06/25
	データコンテキスト
*/
#pragma once

#include "../Save/CExecDataHead.h"

#include "IDData.h"

namespace BMW{
namespace Data{

class CDataContext : public Task::CTaskContext
{/**
	データコンテキスト
 */
public:
	typedef map<int, Save::CExecDataHead*> head_map;

	// コンストラクタ・デストラクタ
	CDataContext();
	~CDataContext();

	// ヘッダ操作
	Save::CExecDataHead*	getHead(int nID)
	{ 
		head_map::iterator it = mapHead_.find(nID);
		if(it==mapHead_.end()) return NULL;
		return it->second;
	}
	void					setHead(int nID, Save::CExecDataHead* head)
							{
								mapHead_.insert(pair<int,Save::CExecDataHead*>(nID,head));
							}
	void					clearHead();
	Save::CExecDataHead*	getTargetHead()
	{ 
		head_map::iterator it = mapHead_.find(getValue(Flag::TARGET_DATA));
		if(it==mapHead_.end()) return NULL;
		return it->second;
	}

private:
	// ヘッダマップ
	head_map mapHead_;
};

} // namespace Data end
} // namespace BMW end