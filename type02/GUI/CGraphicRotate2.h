/*
	katze 06/07/09
	回転できるグラフィック Ver.2
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicRotate2 : public CGraphic
{/**
	回転できるグラフィック Ver.2
 */
public:
	virtual ~CGraphicRotate2(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end