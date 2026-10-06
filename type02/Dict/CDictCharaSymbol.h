/**
	katze 08/08/19
	シンボルビューア
*/
#pragma once

namespace BMW{

namespace Movie{
class CSymbolDB;
} // namespace Movie end

namespace Dict{

class CDictCharaSymbol : public Task::CTaskList
{
public:
	enum ePriority{
		SYMBOL_DEFAULT,
		SYMBOL_DEFENCE,
		SYMBOL_DAMAGE,

		CHIP_T,
		CHIP_L,
		CHIP_B,
		CHIP_R,
		CHIP_AT,
		CHIP_AL,
		CHIP_AB,
		CHIP_AR,
		CHIP_PINCH_T,
		CHIP_PINCH_L,
		CHIP_PINCH_B,
		CHIP_PINCH_R,
		CHIP_PINCH_AT,
		CHIP_PINCH_AL,
		CHIP_PINCH_AB,
		CHIP_PINCH_AR,

		CHIP_JUMP1_T,
		CHIP_JUMP2_T,
		CHIP_JUMP3_T,
		CHIP_JUMP1_L,
		CHIP_JUMP2_L,
		CHIP_JUMP3_L,
		CHIP_JUMP1_R,
		CHIP_JUMP2_R,
		CHIP_JUMP3_R,
		CHIP_JUMP1_B,
		CHIP_JUMP2_B,
		CHIP_JUMP3_B,

		CHIP_WALK_T,
		CHIP_WALK_L,
		CHIP_WALK_B,
		CHIP_WALK_R,
		CHIP_ATTACK,
		CHIP_ITEM,
		CHIP_DEFENCE,
		CHIP_DAMAGE,
	};

	// コンストラクタ・デストラクタ
	CDictCharaSymbol();
	~CDictCharaSymbol();

	// タスク
	void Task(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnReset(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

private:
	// 現在、表示してるキャラデータ
	CDictCharaItem* pCurrentItem_;
	// 背景
	CFastPlane* pBack_;

	// コマを一個追加する
	void addChip(const string& sID, int nPri, int& nX, int& nY, int& nMaxHeight);
};

} // namespace Dict end
} // namespace BMW end
