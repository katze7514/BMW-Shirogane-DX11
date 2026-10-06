#include "stdafx.h"

//#include "../../mode.h"

//#include "../../Chara/CDataCharaBattle.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"
#include "../Context/CMapSymbolDB.h"

#include "../Action/IAction.h"
#include "../Action/CActionFactory.h"

#include "CSally_add_chara.h"

namespace BMW{
namespace SLG{
namespace Sally{

void CSally_add_chara::OnAction(Task::CTaskContext* pContext)
{
	// まずは、引数の確保
	list<int> listParam;
	// 思考ルーチンに渡すパラメタ数
	int nParamNum = pContext->top();
	pContext->pop();
	for(int i=0; i<nParamNum; ++i)
	{// 思考ルーチンのパラメタ
		// 逆順でつまれている
		listParam.push_front(pContext->top());
		pContext->pop();
	}
	// 思考ルーチンID
	int nCpu = pContext->top();
	pContext->pop();
	// 所属フェーズ
	int nPhase = pContext->top();
	pContext->pop();
	// 追加したいキャラID
	int nChara = pContext->top();
	pContext->pop();
	// 養成データID（-1の時はセーブデータから）
	int nTrain = pContext->top();
	pContext->pop();
	// SLG ID
	int nSlg = pContext->top();
	pContext->pop();

	// SLGコンテキストキャスト
	CSLGContext* p = static_cast<CSLGContext*>(pContext);
	if(p->getCharaData(nSlg)==NULL)
	{// すでにデータが存在していたら、スルー
		// SLGデータの生成
		CDataCharaSLG* pData = new CDataCharaSLG();
		// SLGデータ設定
		initCharaData(nChara, nSlg, nPhase, nTrain, nCpu, listParam, pData, p);
		// CSLGContextへの追加
		p->setCharaData(pData->getID(), pData);
		// フェーズは、↑で自動的に設定される
	}
	// そして、戻る
	getTaskListCtrl()->returnTaskList();
}

void CSally_add_chara::initCharaData(int nChara, int nSlg, int nPhase, int nTrain, int nCpu, list<int>& listParam,
									 CDataCharaSLG* pData, CSLGContext* p)
{
	// SLG IDの設定
	pData->setID(nSlg);
	// 所属フェーズ
	pData->setPhase(nPhase);

	// 戦闘データの設定
	if(nTrain<0)
	{// 負の場合は、養成データはセーブデータから引っ張って来る
		p->getApp()->getChara().setBattle(pData->getBattlePtr(), 
										  nChara, 
										  *(p->getApp()->getExec().getTrainData(nChara)));
	}
	else
	{// 正の場合は、養成データマップから引っ張る
		// ↓HANDOVERの都合上コピーを持ってくる
		Chara::CDataCharaTrain trainData = p->getTrain(nTrain);

		// 引継ぎプレイの場合、敵だったら設定されている養成段階を＋する
		// コピーを使ってるので養成段階を戻すのはいらない
		Save::CExecData& exec = p->getApp()->getExec();
		if((exec.getFlag("HANDOVER", 1) || exec.getFlag("HANDOVER", 2)) && pData->getPhase()==Phase::ENEMY)
		{
			int nEnemyTrain=0;
			exec.getFlag(Scene::Const::flagID_.getValue("ENEMY_TRAIN"),nEnemyTrain);
			trainData.calcTrainAll(nEnemyTrain);
		}

		// 養成適用
		p->getApp()->getChara().setBattle(pData->getBattlePtr(),
										  nChara,
										  trainData);		
		
		// PLAYERフェーズのキャラの時
		if(nPhase==Phase::PLAYER)
		{// 指定された養成データを初期養成データとして、セーブデータ内に生成する
		 // もし、実はすでにセーブデータ内に存在してたとしても、上書きする
			Chara::CDataCharaTrain* pTrain = exec.getTrainData(nChara);
			*pTrain = trainData;
			// ↑で養成IDが上書きされる故
			pTrain->setID(exec.getTrainID(nChara));
			// セーブデータ内に確保したので、以後はそれで動く
			nTrain=-1;
		}
	}
	// 養成IDの設定
	pData->setTrain(nTrain);

	// アイテムの効果適用
	Item::CItemDB& db = p->getApp()->getItem();
	Chara::CDataCharaBattle& battle = pData->getBattle();
	battle.beginItem();
	while(!battle.endItem())
	{
		Chara::CStatusAbility& item = *battle.nextItem();
		// また、アイテムによる能力UP等があればここで行う
		if(db.IsStatus(item.getID()))
			db.applyStatus(*pData, item.getAttr(), item.getID());
	}

	// 思考ルーチンファクトリ
	Action::CActionFactory factory_;
	// 思考ルーチン
	pData->setAction(factory_.createAction(nCpu,listParam));
	pData->getAction()->responseAbility(*pData, *p);
}

} // namespace Sally end
} // namespace SLG end
} // namespace BMW end