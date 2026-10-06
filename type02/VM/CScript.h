/*
	katze 05/04/19
	実行スクリプトコードリスト
*/
#pragma once

//#include "../Task/ITaskBase.h"

namespace BMW{
namespace VM{

class CScript : public Task::ITaskBase
{/**
	実行スクリプトコードリスト
	つまり、こいつが関数に対応する
 */
public:
	typedef vector<Task::ITaskBase*> code_vec;
	// デストラクタ
	virtual ~CScript(){ clearCode(); }

	// 操作
	void				addCode(Task::ITaskBase*);
	void				clearCode();
	Task::ITaskBase*	getCode(int nPc){ return vecCode_[nPc]; }
	code_vec::size_type	getCodeSize() const { return vecCode_.size(); }

protected:
	code_vec vecCode_;
};

} // namespace VM end
} // namespace BMW end