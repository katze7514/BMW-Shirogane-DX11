/*
	katze 05/05/15
	描画座標が固定小数点(16bitシフト)
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicAccuracy : public CGraphic
{/**
	描画座標が固定小数点のグラフィック(16bitシフト)
 */
public:
	// デストラクタ
	virtual ~CGraphicAccuracy(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end
