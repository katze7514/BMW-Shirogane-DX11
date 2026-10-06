#include "stdafx.h"

#include "../Movie/DB/CSymbolDB.h"
#include "../Status/status_fun.h"
#include "../Scene/IScene.h"

#include "CInterContext.h"
#include "CInterChara.h"

namespace BMW{
namespace Inter{

CInterChara::~CInterChara()
{
	DELETE_SAFE(pChipSort_);
}

void CInterChara::OnInit(Task::CTaskContext* pContext)
{
	// シンボルの設定
	symbol_.setSymbol(data_.getMapSymbolID());
	// チップボタンの設定
	pChipSort_ = pContext->getScene()->getGuiDefDB().createInterfaceCast<GUI::CPanel>("CHIP_SORT");
	Status::setChipSort(pChipSort_,*this);
}

namespace{
__inline int getCharaID(const string& sID)
{
	return Chara::Const::charaID_.getValue(sID);
}

__inline void OnResetChara(const string& sID, CInterContext* p)
{
	CInterChara* pInter;
	pInter = p->getCharaData(Chara::Const::charaID_.getValue(sID));
	if(pInter!=NULL) pInter->OnReset(p,false);
}

} // naemespace end

void CInterChara::OnReset(CInterContext* pContext,bool bSecond)
{
	if(pChipSort_!=NULL)
		Status::setCharaSort(pChipSort_->getWidgetCast<GUI::CPanelCtrl>("SORT"),getID(),*getData());

	// 他に分身がいるか？
	if(bSecond)
	{// ↑のbSecondは別のOnResetから呼ばれたかどうかを識別する
	 // これが無いと、なんどもOnResetが呼ばれて、無限ループ状態になる
		if(data_.getID()==getCharaID("PLAYER_ARC_FTS"))
		{// こいつがファンタなら、あとエクリプスとアルクがいるかもしんね
			OnResetChara("PLAYER_ECLIPS",pContext);
			OnResetChara("PLAYER_ARCUEID",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_ECLIPS"))
		{// こいつがエクリプスなら、あとファンタとアルクがいるかもしんね
			OnResetChara("PLAYER_ARC_FTS",pContext);
			OnResetChara("PLAYER_ARCUEID",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_ARCUEID"))
		{// こいうがアルクなら、あとファンタとエクリプスがいるかもしんね
			OnResetChara("PLAYER_ARC_FTS",pContext);
			OnResetChara("PLAYER_ECLIPS",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_RIN_KALEIDO"))
		{// こいつが凛なら、あとカレイドルビーがいるかも
			OnResetChara("PLAYER_KALEIDO",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_KALEIDO"))
		{// こいつがカレイドルビーなら、あと凛がいるかもしんね
			OnResetChara("PLAYER_RIN_KALEIDO",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_KOHAKU"))
		{// こいつが琥珀なら、あとアンバーがいるかもしんね
			OnResetChara("PLAYER_AMBER",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_KOHAKU_2"))
		{// こいつが琥珀なら、あとアンバーがいるかもしんね
			OnResetChara("PLAYER_AMBER_2",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_AMBER"))
		{// こいつがアンバーなら、あと琥珀がいるかもしんね
			OnResetChara("PLAYER_KOHAKU",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_AMBER_2"))
		{// こいつがアンバーなら、あと琥珀がいるかもしんね
			OnResetChara("PLAYER_KOHAKU_2",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_SABER"))
		{// こいつがセイバーなら、あとリリィがいるかもしんね
			OnResetChara("PLAYER_LILY",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_SABER_EX"))
		{// こいつがセイバーなら、あとリリィがいるかもしんね
			OnResetChara("PLAYER_LILY_EX",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_SABER_AVALON"))
		{// こいつがセイバーなら、あとリリィがいるかもしんね
			OnResetChara("PLAYER_LILY_AVALON",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_LILY"))
		{// こいつがリリィなら、あとセイバーがいるかもしんね
			OnResetChara("PLAYER_SABER",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_LILY_EX"))
		{// こいつがリリィなら、あとセイバーがいるかもしんね
			OnResetChara("PLAYER_SABER_EX",pContext);
		}
		ef(data_.getID()==getCharaID("PLAYER_LILY_AVALON"))
		{// こいつがリリィなら、あとセイバーがいるかもしんね
			OnResetChara("PLAYER_SABER_AVALON",pContext);
		}
	}
}

} // namespace Inter end
} // namespace BMW end