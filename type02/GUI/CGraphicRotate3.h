/*
	katze 06/07/09
	回転できるグラフィック Ver.
	オフセット回転
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicRotate3 : public CGraphic
{/**
	回転できるグラフィック Ver.3
 */
public:
	virtual ~CGraphicRotate3(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end