/*
	katze 05/06/28
	update 06/02/13
	update 06/05/23 イベントスクリプト対応
	イベント
*/
#pragma once

#include "../../ADV/CMsgBoard.h"
#include "../Effect/CEffectInfo.h"

namespace BMW{

namespace Unit{
class CYesNoUnit;
} // namespace Unit end

//namespace ADV{
//class CMsgBoard;
//} // namespace ADV end

namespace SLG{
class CStatusCharaVeryEasy;

namespace Phase{
class CPhaseBall;
} // namespace Phase end

namespace Event{

class CEvent : public Task::CTaskList
{/**
	イベント
 */
public:
	typedef map<int, Effect::CEffectInfo*> effect_map;

	enum ePriority{
		EVENT,
		VERY_EASY_STATUS_L=100,
		VERY_EASY_STATUS_R,
		MSG,
		YES_NO,
		TURN,
	};
	// デストラクタ
	~CEvent();
	// タスク
	void OnInit(CSLGContext*);

	// 取得
	Task::CTaskCtrl<ADV::CMsgBoard>&	getBoardCtrl(){ return msgCtrl_; }
	CStatusCharaVeryEasy&				getStatus(int nSide){ return *pStatusChara_[nSide]; }
	Phase::CPhaseBall&					getTurnBall(){ return *pTurnBall_; }
	Phase::CPhaseBall*					getTurnBallPtr(){ return pTurnBall_; }
	Unit::CYesNoUnit&					getYesNoUnit(){ return *pUnit_;}

	// 有効化とか
	void	validBoard(bool bValid);
	void	validStatus(bool bValid, int nSide);
	void	validYesNo(bool bValid);

	// エフェクトマップ操作
	Effect::CEffectInfo*	getEffect(int nID);
	void					addEffect(int nNo, Task::ITaskBase* pEffect, Task::ITaskList* pList);
	void					delEffect(int nNo);

private:
	// MSG_BOARD
	Task::CTaskCtrl<ADV::CMsgBoard> msgCtrl_;
	// 超簡易ステータス
	CStatusCharaVeryEasy*	pStatusChara_[2];
	// ターンボール
	Phase::CPhaseBall*		pTurnBall_;
	// YES/NO
	Unit::CYesNoUnit*		pUnit_;

	// エフェクト実体化マップ
	effect_map	mapEffect_;
};

} // namespace Event end
} // namespace SLG end
} // namespace BMW end