#include "stdafx.h"

#include "../CSLGScene.h"
#include "../Context/CSLGContext.h"
#include "../Context/CSLGDef.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"
#include "../Action/IAction.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara2.h"

#include "CSally_add_chara.h"
#include "CSally_change_chara.h"

namespace BMW{
namespace SLG{
namespace Sally{

struct CCharaBackUp
{// SLG部分のデータ
	int nSlgID_;
	int nPhase_;
	int nTrain_;
	int nMental_;
	CCharaState state_;
	list<int> listPers_;
	int nAction_;
	list<int> listAction_;
	int nLv;

	CCharaBackUp(CDataCharaSLG* pChara)
	{
		setBackUp(pChara);
	}
	void setBackUp(CDataCharaSLG* pChara)
	{
		nSlgID_= pChara->getID();
		nPhase_= pChara->getPhase();
		nTrain_= pChara->getTrain();
		nMental_=pChara->getBattle().getMental();
		state_ = pChara->getState();
		listPers_=pChara->getPersList();
		pChara->getAction()->getActionParam(nAction_,listAction_);
		nLv=pChara->getBattle().getLv();
	}
};

namespace{
__inline int getAct(int nAct, bool bPhase)
{
	if(nAct==Act::BEFORE)
		return nAct;
	else
		return bPhase ? Act::AFTER : Act::BEFORE;
}
} // namespace end

void CSally_change_chara::OnAction(Task::CTaskContext* pContext)
{/**
	スタックに
		SLG ID
		CHARA ID
		TRAIN No
		LV FLAG
		LV OFFSET
	と積まれてる。
 */

	int nSlgID = pContext->top();
	pContext->pop();
	int nCharaID = pContext->top();
	pContext->pop();
	int nTrain = pContext->top();
	pContext->pop();
	int nLvFlag = pContext->top();
	pContext->pop();
	int nLvOffset = pContext->top();
	pContext->pop(); 

	CSLGContext* p = static_cast<CSLGContext*>(pContext);

	// 引き継ぎ対象
	CDataCharaSLG* pChara = p->getCharaData(nSlgID);

	// データバックアップ
	CCharaBackUp back(pChara);
	back.state_.setAct(getAct(back.state_.getAct(), pContext->getValue(Flag::PHASE)==pChara->getPhase()));
	// キャラマップから削除
	p->delCharaData(pChara->getID());

	// 養成データ取得
	Chara::CDataCharaTrain* pTrain;
	if(nTrain<0)
	{// 養成IDが指定されてないなら、キャラデータのを使う
		if(pChara->getTrain()<0)
		{
			// まず、変更後のキャラ養成データが存在するかを確認
			// 先行して作られていたらそれを使う
			// 全滅プレイなどに対応
			pTrain = p->getApp()->getExec().getTrainData(nCharaID,false);

			if(pTrain==NULL)
			{// 無いなら、現在のからコピー
				// コピー元
				Chara::CDataCharaTrain* pTrainSource = p->getApp()->getExec().getTrainData(pChara->getCharaID());
				if(nTrain==-2) // TrainIDが-2だったらアイテムクリア
					pTrainSource->clearItem();
				// コピー先
				pTrain = p->getApp()->getExec().getTrainData(nCharaID);
				// データコピー
				*pTrain = *pTrainSource;
				// IDが上書きされてしまうので改めて設定
				pTrain->setID(Save::CExecData::getTrainID(nCharaID));

				// 元の養成データはいらなくなるので削除
				p->getApp()->getExec().delTrainData(pChara->getCharaID());
			}
		}
		else
		{	pTrain = &(p->getSLGDef().getTrain(pChara->getTrain()));	}
	}
	else
	{// 指定されてるなら、それを使う
		pTrain = &(p->getSLGDef().getTrain(nTrain));
		// 養成ID変更
		back.nTrain_ = nTrain;
		// 元のレベル保存
		back.nLv = pTrain->getLv();

		// LV補正あり？
		if(AVERAGE==nLvFlag)
		{// 味方の平均
			list<int>& listPhase = p->getPlayerPhaseList();
			int nAverage=0;
			for(list<int>::iterator it=listPhase.begin(); it!=listPhase.end(); ++it)
			{
				CDataCharaSLG* pChara = p->getCharaData(*it);
				if(pChara!=NULL && pChara->IsExist())
					nAverage += pChara->getBattle().getLv();
			}
			
			nAverage = nAverage / listPhase.size();
			
			// Lv書き換え
			if(nAverage!=0)
			{
				nAverage += nLvOffset;
				// 元のTrainのが最低レベル
				if(back.nLv > nAverage)	nAverage = back.nLv;
				pTrain->setLv(nAverage>LV_MAX ? LV_MAX : nAverage);
			}
		}
		ef(MAX==nLvFlag)
		{// 味方の最大
			list<int>& listPhase = p->getPlayerPhaseList();
			int nMax=0;
			for(list<int>::iterator it=listPhase.begin(); it!=listPhase.end(); ++it)
			{
				CDataCharaSLG* pChara = p->getCharaData(*it);
				if(pChara!=NULL && pChara->IsExist())
				{
					if(nMax < pChara->getBattle().getLv())
						nMax = pChara->getBattle().getLv();
				}
			}
			
			// Lv書き換え
			if(nMax!=0)
			{
				nMax += nLvOffset;
				// 元のTrainのが最低レベル
				if(back.nLv > nMax)	nMax = back.nLv;
				pTrain->setLv(nMax > LV_MAX ? LV_MAX : nMax);
			}
		}
	}


	// データ設定
	CDataCharaSLG* pCharaData = new CDataCharaSLG();
	CSally_add_chara::initCharaData(nCharaID, back.nSlgID_, back.nPhase_, back.nTrain_, back.nAction_, back.listAction_, pCharaData, p);
	// 元の状態に復元
	back.state_.pinch(false);
	pCharaData->setState(back.state_);
	pCharaData->setPersList(back.listPers_);
	pCharaData->getBattle().setMental(back.nMental_);
	// キャラマップに追加
	p->getCharaMap().insert(pair<int, CDataCharaSLG*>(pCharaData->getID(), pCharaData));
	// フェーズリストから外れてたら、改めて追加
	if(!p->searchPhase(pCharaData->getID(), pCharaData->getPhase()))
		p->getPhaseList(pCharaData->getPhase()).push_back(pCharaData->getID());
	// 登場するので、PhaseStartアクションを引っかけとく
	//pCharaData->actionPhaseStart(*p,true);
	// 援護回数は回復してしまう
	int nAttr = pCharaData->getBattle().hasSkill(Ability::BACKUPATTACK);
	if(nAttr!=-1)
		pCharaData->getBattle().setBackUpAttack(nAttr);

	nAttr = pCharaData->getBattle().hasSkill(Ability::BACKUPDEFENCE);
	if(nAttr!=-1)
		pCharaData->getBattle().setBackUpDefence(nAttr);

	// マップ上デモの設定
	// すでに読み込まれたらスルー
	if(pCharaData->getMapSymbol().isNull()
	|| !pCharaData->getMapSymbol()->IsRead())
	{// それまでに、同じシンボルを持ってるやつがいたら共有
		int nID = p->searchMapSymbol(pCharaData->getBattle().getMapSymbolID(), pCharaData->getPhase());
		if(nID>=0)
			pCharaData->setMapSymbol(p->getCharaData(nID)->getMapSymbol());
		else
			pCharaData->createMapSymbol(pCharaData->getBattle().getMapSymbolID());
	}

	
	if(pCharaData->getIndex()>=0)
	{// マップ上にいるなら
		// 現在のマップコマを削除
		delete p->getMapChip(back.state_.getIndex())->removeTask(Map::CMapChip::CHARA);
		// マップへ追加
		smart_ptr_static_cast<CSLGScene>(p->getScene())->addChara2(pCharaData->getID());
	}

	// 終了ー
	getTaskListCtrl()->returnTaskList();
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end