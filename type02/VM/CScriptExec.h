/*
	katze 05/04/19
	実行スクリプト
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace VM{
class CScript;

class CScriptExec : public Task::ITaskList
{/**
	実行スクリプト
 */
public:
	// コンストラクタ・デストラクタ
	CScriptExec():nPc_(0),bKill_(false),bRemove_(false){}
	virtual ~CScriptExec(){}

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnInit(Task::CTaskContext*);
	virtual void OnReset(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);
	virtual void OnDraw(Task::CTaskContext*);

	// 設定・取得
	const smart_ptr<CScript>&	getScript() const { return pScript_; }
	void						setScript(const smart_ptr<CScript>& s);

	virtual void killMe(){ bKill_=true; }
	virtual void removeMe(){ bRemove_=true; }

protected:
	// プログラムカウンタ
	// stateで代用
	// 描画する時に使う
	int nPc_;
	// 実行コード
	smart_ptr<CScript> pScript_;
	// フレーム進行フラグ
	bool bKill_;
	// Pc進行フラグ
	bool bRemove_;

	virtual bool IsKill() const { return bKill_; }
	virtual bool IsRemove() const { return bRemove_; }
};

} // namespace VM end
} // namespace BMW end