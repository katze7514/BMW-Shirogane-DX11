/*
	katze 05/06/03
	回転できるグラフィック
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicRotate : public CGraphic
{/**
	回転できるグラフィック
 */
public:
	virtual ~CGraphicRotate(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end