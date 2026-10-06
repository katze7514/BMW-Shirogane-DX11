/*
	katze 05/03/28
	押されたら親に通知するだけのボタン
*/
#pragma once

#include "../GUI/CButtonGraphic.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace Rule{

class CRuleButton : public GUI::CButtonGraphic
{/**
	押されたら親の状態をRELEASEにする

	CRuleMenuButtonと組み合わせて使う
 */
public:
	// デストラクタ
	virtual ~CRuleButton(){}
	// アクション
	void actionRelease(Task::CTaskContext*);
};

} // namespace Rule end
} // namespace BMW end