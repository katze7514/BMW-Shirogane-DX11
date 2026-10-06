/*
	katze 05/12/22
	キャラコマ Ver.2
*/
#pragma once

#include "../IDSLG.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
class CCharaState;

namespace Map{

class CMapChipChara2 : public Task::CTaskListDraw
{/**
	マップ上のキャラの状態を示す Ver.2

	チップアニメを導入する上での変更バージョン
 */
public:
	enum eState{
		WALK		= ChipMovie::WALK_TOP,		// 歩き
		STAND,									// 静止
		JUMP_READY	= ChipMovie::JUMP_READY_TOP,// ジャンプ準備
		JUMP_UP		= ChipMovie::JUMP_UP_TOP,	// ジャンプ上昇
		JUMP_DOWN	= ChipMovie::JUMP_DOWN_TOP,	// ジャンプ下降
	};
	enum ePriority
	{// タスクプライオリティ
		CHARA,
		EFFECT,			// あったりなかったり
	};
	// コンストラクタ
	CMapChipChara2(int nID=-1):nID_(nID){ setState(STAND); }
	// タスク
	//void Task(Task::CTaskContext*);
	//void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 操作
	int		getID() const { return nID_; }
	void	setID(int nID){ nID_=nID; }

	void	setCharaState(const smart_ptr<CCharaState>& pState){ pState_=pState; }

//	static bool IsMove();
//	static void move(bool bMove);

private:
	// 操作対象のSLG ID
	int nID_;
	// キャラ状態(CDataCharaSLGのを共有する)
	smart_ptr<CCharaState> pState_;
	// キャラの状態から、行動前か後かを選択
	int getStand();

//	static bool bMove_; // 誰かが移動中
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end