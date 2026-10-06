/*
	katze 05/05/21
	SLGでのエフェクト用ムービークリップ
*/
#pragma once

namespace BMW{
namespace SLG{
namespace Effect{

class CEffectMovieClip : public Movie::CMovieClip
{/**
	SLGでのエフェクト用ムービークリップ

	終了すると、removeTaskする
 */
public:
	CEffectMovieClip():bRemove_(false){}
	virtual ~CEffectMovieClip(){}
	// タスク
	void OnAction(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);

	// 設定・取得
	bool IsRemove()const{ return bRemove_; }
	void remove(bool bRemove){ bRemove_=bRemove; }

private:
	bool bRemove_; // リムーブフラグ
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end