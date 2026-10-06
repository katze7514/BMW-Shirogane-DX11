#include "stdafx.h"

#include "../../mode.h"

#include "../Context/CSLGContext.h"
#include "../Context/CDataCharaSLG.h"

#include "../Map/CMapChip.h"
#include "../Map/CMapChipState.h"
#include "../Map/CMapChipChara2.h"
#include "../Map/CMapChipCharaSprite2.h"

#include "CMove_exec2.h"

namespace BMW{
namespace SLG{
namespace Move{

CMove_exec2::~CMove_exec2()
{
	DELETE_SAFE(pCancel_);
	DELETE_SAFE(pOK_);
}

__inline static int getPri(Map::CMapChip* pMap)
{
	int nPri=0;	
	while(pMap->getTask(Map::CMapChip::CHARA + nPri)!=NULL) nPri++;
	return nPri;
}

void CMove_exec2::OnReset(Task::CTaskContext*)
{
	pCancel_ = new BMW::Rule::CRuleCancel(CANCEL);
	pCancel_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	pOK_ = new BMW::Rule::CRuleOK(OK);
	pOK_->setParent(smart_ptr<Task::ITaskBase>(this,false));
}

void CMove_exec2::OnInit(Task::CTaskContext* pContext)
{
	setState(EXEC);
	pContext->getInput()->guard(false);
	// カーソル非表示
	pContext->getInput()->cursolVisible(false);
	// コンテキスト変換
	p = static_cast<CSLGContext*>(pContext);
	// 操作対象キャラデータ
	pChara_ = p->getCtrlCharaData();
	// 移動対象キャラを取得
	pStartMap_ = p->getMapChip(pChara_->getIndex());
	pCharaChip_ = static_cast<Map::CMapChipChara2*>(pStartMap_->getTask(Map::CMapChip::CHARA));
	//pCharaChip_->setParent(smart_ptr<Task::ITaskBase>(this,false));
	// 現在いるマップの座標を初期値とする
	//motion_.setStart(pStartMap_->getDrawInfo());
	// 移動対象キャラスプライトを取得
	pCharaChipSprite_ = static_cast<Map::CMapChipCharaSprite2*>(pCharaChip_->getTask(Map::CMapChipChara2::CHARA));	
	//Map::CMapChipChara2::move(true);
	// オフセット初期化
	nPri_=0;
}

void CMove_exec2::OnAction(Task::CTaskContext* pContext)
{
	pCancel_->OnAction(pContext);
	pOK_->OnAction(pContext);

	using Draw::CDrawInfo;
	switch(getState())
	{
	case OK:	// OKされた
	case CANCEL: // キャンセルされた
	{
		pContext->getInput()->guard(true);
		// スタックを巻き上げ
		int nWay=-1;
		while(p->top()>=0){ nWay=p->top(); p->pop(); }
		// 負の値の一個前の値が最後のWay
		if(nWay>=0) pChara_->getState().setWay(nWay);
		// 最後の負の値の絶対値が、移動先Index
		// 但し、0の時はINT_MINになっている
		int nTarget = p->top()==INT_MIN ? 0 : abs(p->top());
		p->pop();
		// 現在のマップからremove
		bStart_ ? pStartMap_->removeTask(Map::CMapChip::CHARA + nPri_)
				: pEndMap_->removeTask(Map::CMapChip::CHARA + nPri_);
		// 対象のマップへ移動させてしまって終了
		Map::CMapChip* pTarget = p->getMapChip(nTarget);
		pTarget->addTask(pCharaChip_,Map::CMapChip::CHARA);
		pCharaChip_->setState(Map::CMapChipChara2::STAND);
		pCharaChip_->setX(0);
		pCharaChip_->setY(0);
		setState(END_WAIT);
		nFrame_=0;
	}
	break;

	case EXEC:
	{
		// まず、一個取り出す
		int nTarget = p->top();
		p->pop();

#ifdef BMW_DEBUG
		CDbg().Out("MOVE EXEC %d", nTarget);
#endif

		if(nTarget<0)
		{// 負だったら、移動終了
		 	// 静止状態に戻る
			pContext->getInput()->guard(true);
			pCharaChip_->setState(Map::CMapChipChara2::STAND);
			setState(END_WAIT);
			nFrame_=0;
		}
		else
		{// 取得できるのは方向なので、移動先を取得する
			// 移動アニメーションを取得
			// 方向から移動先取得
			pEndMap_ = p->getMapChip(pStartMap_->getMapInfo().getOnMap(nTarget));
			// 向きを変更
			pChara_->getState().setWay(nTarget);
	
			// モーション設定		
			// 高さと優先順位に合わせて、動きを合わせる
			nHeight_ = pEndMap_->getMapInfo().getHeight()-pStartMap_->getMapInfo().getHeight();
			nIndex_  = pStartMap_->getIndex() - pEndMap_->getIndex();
			CDrawInfo info1 = pStartMap_->getDrawInfo();
			CDrawInfo info2 = pEndMap_->getDrawInfo();
			bStart_=true;

			if(nHeight_==0)
			{// 高さが同じなら歩く

				// 優先度の高い方につく
				if(nIndex_ > 0)
				{
					motion_.setStart(0,0);
					motion_.setEnd(info2.getX()-info1.getX(),info2.getY()-info1.getY());
				}
				else
				{
					// CharaChipを入れ替える
					pStartMap_->removeTask(Map::CMapChip::CHARA + nPri_);
					nPri_ = getPri(pEndMap_);
					pEndMap_->addTask(pCharaChip_,Map::CMapChip::CHARA + nPri_);
					bStart_=false;
					motion_.setStart(info1.getX()-info2.getX(),info1.getY()-info2.getY());
					motion_.setEnd(0,0);
				}
				motion_.setStep(6);				
				motion_.setEdging(0);
				motion_.reset();

				// 再生動画選択
				pCharaChip_->setState(Map::CMapChipChara2::WALK);
				// 移動終了待ち
				setState(EXEC_WALK);
				motion_.inc();
				pCharaChip_->setDrawInfo(motion_);
			}
			else
			{	// ジャンプ
				// 再生動画選択
				pCharaChip_->setState(Map::CMapChipChara2::JUMP_READY);
				// 疑似放物線を描く
				// 前半の設定
				motion_.setStart(0,0);
				motion_.setCurrent(0,0);
				// 上昇準備
				motion_.setEnd((info2.getX()-info1.getX())/2, 
								nHeight_<0 ? -16 : info2.getY()-info1.getY()-16);
				motion_.setStep(nHeight_>0 ? 9 : 5);
				motion_.setEdging(100);
				motion_.reset();
				nFrame_=0;
				// ジャンプ構え～
				setState(EXEC_JUMP_READY_UP);
			}
		}
	}
	break;

	case EXEC_WALK:
		if(motion_.inc())
		{// モーションが戻ってれば移動終了
		 // 次の一歩へ
		 // 初期値を設定しなおしておく
			if(nIndex_ > 0)
			{// CharaChipを入れ替える
				pStartMap_->removeTask(Map::CMapChip::CHARA + nPri_);
				nPri_ = getPri(pEndMap_);
				pEndMap_->addTask(pCharaChip_,Map::CMapChip::CHARA + nPri_);
				bStart_=false;
				pCharaChip_->setX(0);
				pCharaChip_->setY(0);
			}
			pStartMap_=pEndMap_;
			setState(EXEC);
		}
		else
			pCharaChip_->setDrawInfo(motion_);
	break;

	case EXEC_JUMP_READY_UP:
		// ジャンプ構え
		if(++nFrame_>=2)
		{// 3フレ待ってじゃ～んぷ
			pCharaChip_->setState(Map::CMapChipChara2::JUMP_UP);
			setState(EXEC_JUMP_UP);
			motion_.inc();
			pCharaChip_->setDrawInfo(motion_);
		}
	break;

	case EXEC_JUMP_UP:
		// 上昇～
		if(motion_.inc())
		{// 上昇終わったら下降ー
			pCharaChip_->setState(Map::CMapChipChara2::JUMP_DOWN);

			CDrawInfo info1 = pStartMap_->getDrawInfo();
			CDrawInfo info2 = pEndMap_->getDrawInfo();
				
			// 後半の設定
			if(nIndex_>0 && nHeight_>0)
			{	
				motion_.setStart(motion_.getEnd());
				motion_.setEnd(info2.getX()-info1.getX(), info2.getY()-info1.getY());
				bStart_=true;
			}
			else
			{
				// CharaChipを入れ替える
				pStartMap_->removeTask(Map::CMapChip::CHARA + nPri_);
				nPri_ = getPri(pEndMap_);
				pEndMap_->addTask(pCharaChip_,Map::CMapChip::CHARA + nPri_);
				bStart_=false;
				motion_.setStart((info1.getX()-info2.getX())/2, nHeight_>0?-16:info1.getY()-info2.getY()-16);
				motion_.setEnd(0,0);
			}

			motion_.setEdging(-100);
			motion_.setCurrent(motion_.getStart());
			motion_.setCurrentStep(0);
			motion_.setStep(nHeight_>0 ? 5 : 9);
			//motion_.setStep(nHeight_>0 ? 3 : 4);
			setState(EXEC_JUMP_DOWN);
			motion_.inc();
		}
		
		pCharaChip_->setDrawInfo(motion_);
	break;

	case EXEC_JUMP_DOWN:
		if(motion_.inc())
		{// 下降終わったら着地ー
			pCharaChip_->setState(Map::CMapChipChara2::JUMP_READY);
			nFrame_=0;
			setState(EXEC_JUMP_READY_DOWN);
		}
		else
			pCharaChip_->setDrawInfo(motion_);
	break;

	case EXEC_JUMP_READY_DOWN:
		if(++nFrame_>=2)
		{// 3フレ経ったら終了
			if(nIndex_>0 && nHeight_>0)
			{// CharaChipを入れ替える
				pStartMap_->removeTask(Map::CMapChip::CHARA + nPri_);
				nPri_ = getPri(pEndMap_);
				pEndMap_->addTask(pCharaChip_,Map::CMapChip::CHARA + nPri_);
				bStart_=false;
				pCharaChip_->setX(0);
				pCharaChip_->setY(0);
			}

			pStartMap_=pEndMap_;
			setState(EXEC);
		}
	break;

	case END_WAIT:
		if(++nFrame_>=3) setState(END);
	break;

	case END:
	{
		// 移動を確定
		pChara_->setIndex(p->getTargetMap());
		//Map::CMapChipChara2::move(false);
		
		// 一撃離脱時の移動状態
		if(pChara_->getState().getAct()==Act::HIT_AWAY)
			pChara_->getState().setAct(Act::HIT_AWAY_MOVE);
		else
			pChara_->getState().setAct(Act::MOVE);
		// 移動が終わったので戻る
		getTaskListCtrl()->returnTaskList();
		// カーソル表示
		pContext->getInput()->cursolVisible(p->getPhase()==Phase::PLAYER);
	}
	break;

	default: break;
	}
}

/*void CMove_exec2::callTaskAction(CTaskContext* pContext)
{
//	pCharaChip_->Task(pContext);
}

void CMove_exec2::callTaskDraw(CTaskContext* pContext)
{// 描画用
//	setDrawInfo(motion_);
//	pCharaChip_->Task(pContext);
}
*/
} // namespace Move end
} // namespace SLG end
} // namespace BMW end
