/*
	katze 05/06/24
	PUSH状態を維持し、もう一回押されるとキャンセルされるボタン
*/
#pragma once

namespace BMW{
namespace GUI{

class CButtonKeepSymbol : public CButtonSymbol
{/**
	PUSH状態を維持し、もう一回押されるとキャンセルされるボタン
 */
public:
	// デストラクタ
	virtual ~CButtonKeepSymbol(){}

	// タスク
	virtual void Task(Task::CTaskContext*);
	virtual void OnAction(Task::CTaskContext*);

	// アクション
	virtual void actionRelease(Task::CTaskContext*);
	virtual void actionCancel(Task::CTaskContext*);

	// 描画状態を取得
	int getDrawState();
};

} // namespace GUI end
} // namespace BMW end