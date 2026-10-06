/*
	katze 06/04/09
	条件付きcall
*/
#pragma once

namespace BMW{
namespace SLG{
class ISlgCond;

namespace Code{

class CCode_cond_call : public BMW::Rule::IRuleTask
{/**
	条件付きcall
 */
public:
	// デストラクタ
	~CCode_cond_call();

	// タスク
	void OnAction(Task::CTaskContext*);

	// アクセッサ
	ISlgCond*	getCond(){ return pCond_; }
	void		setCond(ISlgCond* pCond){ pCond_=pCond; }

private:
	ISlgCond* pCond_;
};

} // namespace Code end
} // namespace SLG end
} // namespace BMW end
