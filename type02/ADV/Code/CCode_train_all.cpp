#include "stdafx.h"

#include "CCode_train_all.h"

namespace BMW{
namespace ADV{
namespace Code{

void CCode_train_all::OnAction(Task::CTaskContext* pContext)
{	
#ifdef BMW_DEBUG
	// 誤って、typeをCHANGE,COPYにしてしまった場合
	if(getType()==CCode_train::CHANGE||getType()==CCode_train::COPY)
	{
		CDbg().Out("TRAIN_ALLは、CHANGEorCOPYには対応しないよ");
		return;
	}
#endif
	
	Save::CExecData& save = pContext->getApp()->getExec();
	// 養成データ操作
	list<int>::iterator it;
	for(it=listCharaID_.begin(); it!=listCharaID_.end(); ++it)
	{// キャラリストをぶん回す
		// 差分適用の場合、validリストに入っていたらスキップ
		if(getCtrl()==SUB && save.IsValid(*it)) continue;
		CCode_train::ctrlTrain(*it,getKind(),getType(),getValue(),getMax(),pContext);
	}
}

} // namespace Code end
} // namespace ADV end
} // namespace BMW end