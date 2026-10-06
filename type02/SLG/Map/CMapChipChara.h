/*
	katze 05/03/24
	キャラコマ
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
namespace Map{
class CMapChip;

class CMapChipChara : public Task::CTaskListDraw
{/**
	マップ上のキャラの状態を示す
 */
public:
	enum eState{
		MOVE_TOP,
		MOVE_LEFT,
		MOVE_BOTTOM,
		MOVE_RIGHT,
		NORMAL,
		MOVE,
	};
	enum ePriority
	{// タスクプライオリティ
		CHARA,
		EFFECT,			// あったりなかったり
	};
	// コンストラクタ
	CMapChipChara(int nID=-1):nID_(nID){ setState(NORMAL); }
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 操作
	int		getID() const { return nID_; }
	void	setID(int nID){ nID_=nID; }

	// アクション
	void actionMove(Task::CTaskContext*);

private:
	// 操作対象のSLG ID
	int nID_;
	// 移動先のMapChip
	Map::CMapChip* pMove_;
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end