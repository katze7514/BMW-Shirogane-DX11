#include "stdafx.h"

#include "../../Context/CSLGContext.h"
#include "../../Context/CSLGDef.h"
#include "../../Event/CEvent.h"
#include "../CEffectMovieClip.h"

#include "CCode_load_symbol.h"

namespace BMW{
namespace SLG{
namespace Effect{

void CCode_load_symbol::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	int nNo = p->top();
	p->pop();
	int nSymbol = p->top();
	p->pop();
	int nEffect = p->top();
	p->pop();

#ifdef BMW_DEBUG
	CDbg().Out("LOAD %d %d", nNo, nSymbol);
#endif

	// エフェクトリストへ追加
	p->getEvent()->addEffect(nNo, 
							nEffect==NORMAL 
							? p->getSLGDef().getEffect().createSymbol(nSymbol)
							: p->getEffectDB().createEffect(nSymbol)
							,
							NULL);
}

} // namespace Effect end
} // namespace SLG end
} // namepsace BMW end