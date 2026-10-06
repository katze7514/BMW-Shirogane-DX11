/*
	katze 05/05/12
	デモ用ムービークリップ
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoMovieClip : public Movie::CMovieClip
{/**
	デモ用ムービークリップ

	ようは、ループしないで再生が終了したら（全レイヤーが全キーフレームを消費したら）、
	状態をENDにする
 */
public:
	// デストラクタ
	virtual ~CDemoMovieClip(){}
	// タスク
	void OnAction(Task::CTaskContext*);
};

} // namespace Demo end
} // namespace BMW end