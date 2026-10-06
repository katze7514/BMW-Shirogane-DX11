/*
	katze 06/11/24
	なんでもできるグラフィック
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicMorph : public CGraphic
{/**
	なんでもできるグラフィック
 */
public:
	virtual ~CGraphicMorph(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end