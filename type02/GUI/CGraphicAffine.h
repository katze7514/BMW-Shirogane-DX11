/*
	katze 09/01/10
	正しくaffine変換を行うグラフィック
*/
#pragma once

#include "CGraphic.h"

namespace BMW{
namespace GUI{

class CGraphicAffine : public CGraphic
{/**
	正しくaffine変換を行うグラフィック	
 */
public:
	virtual ~CGraphicAffine(){}
	virtual void OnDraw(Task::CTaskContext*);
};

} // namespace GUI end
} // namespace BMW end