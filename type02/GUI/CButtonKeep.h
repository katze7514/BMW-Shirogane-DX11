/*
	katze 05/06/24
	PUSH状態を維持し、もう一回押されるとキャンセルされるボタン
*/
#pragma once

namespace BMW{
namespace GUI{

class CButtonKeep : public CButtonGraphic
{/**
	PUSH状態を維持し、もう一回押されるとキャンセルされるボタン
 */
public:
	// デストラクタ
	virtual ~CButtonKeep(){}

	// タスク
	virtual void OnAction(Task::CTaskContext*);
	virtual void OnDraw(Task::CTaskContext*);

	// アクション
	virtual void actionRelease(Task::CTaskContext*);
	virtual void actionCancel(Task::CTaskContext*);

	// 描画状態を取得
	int getDrawState()
	{
		switch(getState())
		{
		case OVER:	return 1;

		case PRESS:
		case PUSH:	return 2;

		default:	return 0;
		}
	}
};

} // namespace GUI end
} // namespace BMW end