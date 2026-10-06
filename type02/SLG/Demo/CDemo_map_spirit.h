/*
	katze 05/07/20
	update 06/03/17
	マップデモ for 精神
	というか、戦闘以外を一括して管理
*/
#pragma once

#include "CDemo_map_base.h"

namespace BMW{

namespace Demo{
class CDemoMovieClip;
} // namespace Demo end

namespace SLG{
namespace Demo{

class CDemo_map_spirit : public CDemo_map_base
{/**
	マップデモ for 精神
 */
public:
	enum eState{
		NORMAL,
		INIT,	// 初期化フレームはスルー
		EFFECT,	// エフェクト
		WAIT,	// ちょっと待って
		END,	// 終了
	};
	enum eSpirit{
		FIREBALL,
		SPIRIT,
		AVOID,
		TOUGH,
		DEFENCE,
		CONCENT,
		HIT,
		ACC,
		JUMP,
		AWAKE,
		GUTS,
		VERYGUTS,
		TRUST,
		FRIEND,
		SUPPLY,
		HOPE,
		POWER,
		ENCOURAGE,
		SNIPE,
		DIRECT,
		CHARGE,
		SPY,
		WEAK,
		EASYON,
		MIRACLE,
		FORTUNE,
		EFFORT,
		FAITH,
		PROVO,
		SPIRIT_END,
	};
	// コンストラクタ・デストラクタ
	CDemo_map_spirit():pChara_(NULL),nFrame_(0){}
	~CDemo_map_spirit();

	// タスク
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 操作
	void setSpiritEffect(int nID, CDataCharaSLG* pTarget);
	void setItemEffect(int nID, CDataCharaSLG* pTarget);
	void setCureEffect(int nNum, CDataCharaSLG* pTarget);
	void setRefillEffect(int nNum, CDataCharaSLG* pTarget);

private:
	// キャラ
	BMW::Demo::CDemoMovieClip*	pChara_;
	// エフェクト
	// ↓実行中のエフェクト
	Effect::CEffectMovieClip*	pEffect_;
	// 精神エフェクト
	Effect::CEffectMovieClip*	pSpiritEffect_[SPIRIT_END];
	// 数字
	GUI::CNumCtrl*				pNum_;

	// デモ表示状況
	bool	bNum_;
	// 待ちフレーム
	int		nFrame_;
};

} // namespace Demo end
} // namespace SLG end
} // namespace BMW end