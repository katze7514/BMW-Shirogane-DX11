#include "stdafx.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "CCode_slg2valid.h"

namespace BMW{
namespace SLG{
namespace Code{

namespace{
__inline int getCharaID(const string& sID)
{
	return Chara::Const::charaID_.getValue(sID);
}
} // namespace end

void CCode_slg2valid::OnAction(Task::CTaskContext* pContext)
{
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// まず、VALIDリストをクリア
	BMW::Save::CExecData& data = p->getApp()->getExec();
	data.getValidSet().clear();
	// キャラマップを回して、現在の味方キャラをValidリストに突っ込む
	CSLGContext::chara_map& mapChara = p->getCharaMap();
	CSLGContext::chara_map::iterator it;
	for(it=mapChara.begin(); it!=mapChara.end(); ++it)
	{
		if(it->second->getPhase()==Phase::PLAYER)
		{// 味方だったらこいつのキャラIDをVALIDリストへ
			data.addValid(it->second->getCharaID());
		}
	}
}

} // namespace Code end
} // namespace SLG end
} // namespace BMW end