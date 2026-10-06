#include "stdafx.h"

#include "../../mode.h"

#include "../IDSLG.h"
#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"
#include "../Action/IAction.h"

#include "../Map/CMap.h"
#include "../Map/CMapChip.h"
#include "../Map/CMapChipChara.h"
#include "../Map/CMapChipCharaSprite.h"

#include "../Effect/CEffectMovieClip.h"

#include "CSally_add_chara_map.h"

namespace BMW{
namespace SLG{
namespace Sally{

CSally_add_chara_map::CSally_add_chara_map()
{
	for(int i=0; i<END_EFFECT; ++i)
		pEffect_[i]=NULL;
}

CSally_add_chara_map::~CSally_add_chara_map()
{
	for(int i=0; i<END_EFFECT; ++i)
		DELETE_SAFE(pEffect_[i]);
}

void CSally_add_chara_map::OnReset(Task::CTaskContext* pContext)
{
	// 登場エフェクト
	// オンデマンドにロード
	naVisibleFrame[NORMAL]=2;
	naVisibleFrame[BOSS]=1;
	naVisibleFrame[WARAKIA]=2;
}

void CSally_add_chara_map::OnInit(Task::CTaskContext* pContext)
{
	addChara(pContext);
	// アニメ中のスキップはフラグ次第
	pContext->getApp()->animeSkip();
}

void CSally_add_chara_map::OnAction(Task::CTaskContext* pContext)
{
	switch(getState())
	{
	case EFFECT:
		// エフェクト状態を監視
		if(pEffect_[nEffect_]->getTask(0)->getState()==naVisibleFrame[nEffect_])
		{// キーフレームの三つ目でキャラを登場させる
			pSprite_->visible(true);
		}
		ef(pEffect_[nEffect_]->IsEnd())
		{// 終了したら、リターン
			setState(END);
		}
	break;

	case END:
		// 追加後は、対象が無効になる
		pContext->setValue(-1,Flag::TARGET_CHARA);
		pContext->setValue(-1,Flag::TARGET_MAP);
		getTaskListCtrl()->returnTaskList();
			
		pContext->getApp()->skip(true);
	break;

	default: break;
	}
}

void CSally_add_chara_map::callTaskAction(Task::CTaskContext* pContext)
{
	pEffect_[nEffect_]->Task(pContext);
}

void CSally_add_chara_map::callTaskDraw(Task::CTaskContext* pContext)
{
	pEffect_[nEffect_]->Task(pContext);
}

void CSally_add_chara_map::addChara(Task::CTaskContext* pContext)
{	
	// 登場エフェクト
	nEffect_ = pContext->top();
	pContext->pop();
	// 追加向き
	int nWay = pContext->top();
	pContext->pop();
	// 追加タイプに応じて、追加位置を分岐
	int nType = pContext->top();
	pContext->pop();

	// コンテキスト変換
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	// キャラデータの取得
	CDataCharaSLG* pChara = p->getTargetCharaData();
	// 存在しなかったらすぐにリターン
	if(pChara==NULL){ setState(END); return; }
	// 登場するので、PhaseStartアクションを引っかけとく
	pChara->actionPhaseStart(*p,true);

	// 追加位置の取得
	// 基本的にはTargetMapフラグに設定されている
	// 追加しようとしている位置に、キャラすでにいるかどうかをチェック
	// いたら、移動できる範囲でずらしていく
	Map::CMapChip* pChip;
	if(nType==0)
	{// 通常時
		pChip = p->getTargetMapChip();
	}
	else
	{// キャラターゲットが指定されている
		int nOn = p->top();
		p->pop();
		int nTarget = p->top();
		pChip = p->getMapChip(p->getCharaData(nTarget)->getIndex());
		if(nOn>=0)
		{// Onが設定されてたら、その分ずらす
			nOn=pChip->getMapInfo().getOnMap(nOn);
			if(nOn>=0) // ずらし先が存在していれば
				pChip = p->getMapChip(nOn);
		}
		p->pop();
	}
	int nIndex = getEnableAddMap(pChip, pChara, p);

	// 状態の設定
	pChara->getState().setIndex(nIndex);
	pChara->getState().setAct(Act::BEFORE);
	pChara->getState().setWay(nWay);
	// 味方の時、tureにしておく
#ifndef HP_DIV
	pChara->getState().apper(pChara->getPhase()==Phase::PLAYER);
#else
	pChara->getState().apper(true);
#endif
	// 敵だったら、EnemyResetフラグを立てておく
	if(pChara->getPhase()==Phase::ENEMY) p->setEnemyReset(1);
	// NPCだったら、NPCリストへ
	if(pChara->getPhase()==Phase::PLAYER 
	&& pChara->getAction()->IsNonPlayer())
		p->getNonPlayerPhaseList().push_back(pChara->getID());

	// マップ上デモの設定
	// すでに読み込まれたらスルー
	if(pChara->getMapSymbol().isNull()
	|| !pChara->getMapSymbol()->IsRead())
	{// それまでに、同じシンボルを持ってるやつがいたら共有
		int nID = p->searchMapSymbol(pChara->getBattle().getMapSymbolID(), pChara->getPhase());
		if(nID>=0)
			pChara->setMapSymbol(p->getCharaData(nID)->getMapSymbol());
		else
			pChara->createMapSymbol(pChara->getBattle().getMapSymbolID());
	}

	// キャラチップの設定
	setupCharaChip(pChara,nIndex,p);

	// エフェクトの再生設定
	if(nEffect_>=0)
	{// エフェクトありの時はこっち
		pSprite_->visible(false);
		if(pEffect_[nEffect_]==NULL)
		{// まだロードされてなかったらロード
			const string sLoadID[END_EFFECT] = {"ARIVE","ARIVE_BOSS","ARIVE_WARAKIA"};
			pEffect_[nEffect_]=p->getEffectDB().createEffect(sLoadID[nEffect_]);
			pEffect_[nEffect_]->setX(32);
			pEffect_[nEffect_]->setY(16);
		}
		// 登場エフェクトを仕掛けて置く
		pEffect_[nEffect_]->setParent(smart_ptr<Task::ITaskBase>(p->getMapChip(nIndex),false));
		pEffect_[nEffect_]->OnReset(pContext);
		pEffect_[nEffect_]->valid(true);
		pEffect_[nEffect_]->visible(true);
		setState(EFFECT);
	}
	else
	{// エフェクトが指定されてなければ、そのままリターン
		setState(END);
	}

	// 登場箇所を画面の中心に
	p->getMap()->scrollIndex(nIndex);

	// 登場したキャラとして設定
	p->getApp()->getGlobal().addChara(pChara->getCharaID());
}

int CSally_add_chara_map::getEnableAddMap(Map::CMapChip* pMap, CDataCharaSLG* pChara, CSLGContext* pContext)
{
	// 今のマップ上にキャラがいなければ、それでOK
	if(pMap->getTask(Map::CMapChip::CHARA)==NULL)
		return pMap->getIndex();
		
	// いたら、周りを探していく
	// 基本的には、周りを一通り見て、
	// 現在の高さと比べて一番低いところに出現させる
	list<int>	listMap;
	set<int>	setMap;

	// スタートは追加予定だったIndex
	listMap.push_back(pMap->getIndex());

	// 探索ループで使うやつ
	Map::CMapChip* pPos = NULL;
	Map::CMapChip* pCurrent;
	Map::CMapChip* pCheck;
	int nIndex;

	// 探索ループ
	while(!listMap.empty())
	{// 先頭の一個を取り出す
		nIndex = listMap.front();
		listMap.pop_front();
		pCurrent = pContext->getMapChip(nIndex);

		if(pCurrent==NULL) continue;

		// すでにチェック済みか？
		if(setMap.find(nIndex)!=setMap.end()) continue;

		// チェック済み集合へ
		setMap.insert(nIndex);
		
		for(int i=Way::TOP; i<=Way::RIGHT; i++)
		{// 周りを一回り
			//CDbg().Out(pCurrent->getMapInfo().getOnMap(i));
			pCheck = pContext->getMapChip(pCurrent->getMapInfo().getOnMap(i));
			if(pCheck==NULL) continue;
			
			if(pCheck->getTask(Map::CMapChip::CHARA)==NULL)
			{// 誰もいないかをチェック
				if(pPos!=NULL)
				{// すでに追加位置があるなら、
					if(pPos->getMapInfo().getHeight()>pCurrent->getMapInfo().getHeight())
						// 現在の方が高さが低かったらこれにする
						pPos = pCheck;
				}
				else
				{// 追加位置が無いなら、こいつを設定
					pPos=pCheck;
				}
			}
			if(pPos==NULL)
			{// まだ見つからないなら、
			 // これの周りに探索を広げる
				listMap.push_back(pCheck->getIndex());
			}
		}

		if(pPos!=NULL)
		{// 追加位置が見つかってたら、これで終了
			return pPos->getIndex();
		}
	}

	return -1;
}

void CSally_add_chara_map::setupCharaChip(CDataCharaSLG* pChara, int nIndex, CSLGContext* p)
{
	// マップチップキャラの生成
	Map::CMapChipChara* pChip = new Map::CMapChipChara();
	pChip->setID(p->getTargetChara());
	// マップへの追加
	p->getMapChip(nIndex)->addTask(pChip, Map::CMapChip::CHARA);

	// マップチップキャラスプライトの生成
	Map::CMapChipCharaSprite* pSprite = new Map::CMapChipCharaSprite();
	pSprite_ = pSprite;
	// マップキャラへの追加
	pChip->addTask(pSprite, Map::CMapChipChara::CHARA);
	// 状態の共有
	pSprite->setCharaState(smart_ptr<CCharaState>(pChara->getStatePtr(),false));
	// チップの設定
	smart_ptr<CMapSymbolDB>& pDB = pChara->getMapSymbol();
	// 静止状態
	pDB->setGraphic(pSprite->getGraphic(0,0),"BEFORE_TOP");
	pDB->setGraphic(pSprite->getGraphic(0,1),"BEFORE_LEFT");
	pDB->setGraphic(pSprite->getGraphic(0,2),"BEFORE_BOTTOM");
	pDB->setGraphic(pSprite->getGraphic(0,3),"BEFORE_RIGHT");
	pDB->setGraphic(pSprite->getGraphic(1,0),"AFTER_TOP");
	pDB->setGraphic(pSprite->getGraphic(1,1),"AFTER_LEFT");
	pDB->setGraphic(pSprite->getGraphic(1,2),"AFTER_BOTTOM");
	pDB->setGraphic(pSprite->getGraphic(1,3),"AFTER_RIGHT");
}

} // namespace Sally end
} // namesapce SLG end
} // namespace BMW end