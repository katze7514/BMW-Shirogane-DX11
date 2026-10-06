/*
	katze 05/03/21
	サイズ変化のあるグラフィック描画
*/
#pragma once

#include "CGraphic.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Tasl end

namespace GUI{

class CGraphicSize : public CGraphic
{/**
	サイズ変化のあるグラフィック描画をするクラス
 */
public:
	// デストラクタ
	virtual ~CGraphicSize(){}

	// 描画
	virtual void OnDraw(Task::CTaskContext*);

	// 描画情報
	virtual void getDrawSize(LONG& lWidth,LONG& lHeight) const;
};

} // namespace GUI end
} // namespace BMW end