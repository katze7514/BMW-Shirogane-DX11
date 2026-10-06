#include "stdafx.h"

#include "../../Spirit/Spirit/CSpirit_Acc.h"
#include "../../Spirit/Spirit/CSpirit_Jump.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/COffsetMove.h"


#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"

#include "CMove_range.h"

namespace BMW{
namespace SLG{
namespace Move{

void CMove_range::OnInit(Task::CTaskContext* pContext)
{
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);

	// 移動範囲のクリア
	//p->clearMove();

	// 不変値の設定
	nPhase_ = p->getCtrlCharaData()->getPhase();
}

void CMove_range::OnAction(Task::CTaskContext* pContext)
{
	// 計算
	int nIndex = p->getCtrlCharaData()->getIndex();
	p->getIndexSet().insert(nIndex);

	calcMove(p->getMapChip(nIndex), actionAbility(p->getCtrlCharaData()));

	// 計算が終わったら、returnする
	getTaskListCtrl()->returnTaskList();
}

int CMove_range::actionAbility(CDataCharaSLG* pData)
{// 精神とか技能の効果適用
	COffsetMove move;
	Spirit::CSpiritDB& sp = p->getApp()->getSpirit();
	// 移動系
	move.setMove(pData->getBattle().getMove());

	// 加速してたら+3
	if(pData->getBattle().IsSpirit(Chara::CValidSpirit::ACC))
		sp.getDataCast<Spirit::CSpirit_Acc>(Spirit::ACC)->applyOffset(move);
	// 移動状態変化だったら移動力半分
	if(pData->getBattle().IsCond(Chara::CValidCond::MOVE))
		move.setMove(move.getMove()/2);

	// 最低でも1
	if(move.getMove()<0) move.setMove(1);

	// Jump系
	nJump_	= calcAbilityJump(pData,p);

	return move.getMove();
}

/////////////////////////////////
// 技能
/////////////////////////////////
int CMove_range::calcAbilityJump(CDataCharaSLG* pChara, CSLGContext* p)
{
	COffsetMove move;
	Ability::CAbilityDB& ab = p->getApp()->getAbility();
	Spirit::CSpiritDB& sp = p->getApp()->getSpirit();

	int nJump = pChara->getBattle().getJump();
	// 飛行してたら、好きなだけ飛んでいけ！
	if(pChara->getBattle().IsTalent(Ability::FLY))
	{// 飛行持ち
		if(ab.enable(*pChara,0,*p,Ability::FLY))
			// 発動中！
			nJump=INT_MAX;
	}
	ef(pChara->getBattle().IsTalent(Ability::FLOAT))
	{// 浮揚持ち
		if(ab.enable(*pChara,0,*p,Ability::FLOAT))
			// 発動中！
			nJump=INT_MAX;
	}
	ef(pChara->getBattle().IsSpirit(Chara::CValidSpirit::JUMP))
	{// 跳躍してたら、Jump+4
		sp.getDataCast<Spirit::CSpirit_Jump>(Spirit::JUMP)->applyOffset(move);
		nJump+=move.getJump();
	}
	return nJump;
}

//////////////////////////////////////////////////
// 計算
//////////////////////////////////////////////////
void CMove_range::calcMove(Map::CMapChip* pMap, int nMove)
{
	if(pMap==NULL) return;
	
	// 移動力が無くなったら終了
	if(nMove<0) return;

	// マップデータを取得
	int nMapMove = pMap->getMapChipState()->getMove();
	// 検索されてないか、もしくは残り歩数が大きかったら、上書き
	if( nMapMove < 0 || nMapMove < nMove) pMap->getMapChipState()->setMove(nMove);
	// そうで無かったら、検索済みなのでリターン
	else return;

	// 上へいけるか
	Map::CMapChip* pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::TOP));
	switch(IsMove(pOn,pMap->getMapInfo().getHeight(),nMove))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcMove(pOn,nMove - pOn->getMapInfo().getMove());
	break;
	default: break;
	}

	// 左へいけるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::LEFT));
	switch(IsMove(pOn,pMap->getMapInfo().getHeight(),nMove))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcMove(pOn,nMove - pOn->getMapInfo().getMove());
	break;
	default: break;
	}

	// 下へいけるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::BOTTOM));
	switch(IsMove(pOn,pMap->getMapInfo().getHeight(),nMove))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
		calcMove(pOn,nMove - pOn->getMapInfo().getMove());
	break;
	default: break;
	}

	// 右へいけるか
	pOn = p->getMapChip(pMap->getMapInfo().getOnMap(Way::RIGHT));
	switch(IsMove(pOn,pMap->getMapInfo().getHeight(),nMove))
	{
	case ENABLE:
		p->getIndexSet().insert(pOn->getIndex());
	case THROUGH:
		calcMove(pOn,nMove - pOn->getMapInfo().getMove());
	break;
	default: break;
	}
}

int CMove_range::IsMove(Map::CMapChip* pMap, int nCurrentHeight, int nMove)
{
	if(pMap==NULL) return NOTENABLE;
	// 単純に移動可能か？
	if(pMap->getMapInfo().getMove() > nMove 
	|| abs(pMap->getMapInfo().getHeight() - nCurrentHeight) > nJump_)
		return NOTENABLE; // できない・・・

	// マップの上に何かある？
	ITaskBase* pBase = pMap->getTask(Map::CMapChip::CHARA);
	if(pBase!=NULL)
	{// キャラがいる
		int nID = static_cast<Map::CMapChipChara2*>(pBase)->getID();
		if(nPhase_==p->getCharaData(nID)->getPhase())
		{// 仲間だったら通り抜けは可
			return ENABLE;
		}
		else
		{// 仲間じゃなかったら、ダメ
			return NOTENABLE;
		}
	}
	else
	{// 移動可能
		return ENABLE;
	}
}

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
