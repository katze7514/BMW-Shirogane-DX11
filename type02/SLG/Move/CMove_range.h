/*
	katze 05/04/26
	genereted by code_gen_slg.rb
	move_range
*/
#pragma once

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace SLG{
class CSLGContext;
class CDataCharaSLG;

namespace Map{
class CMapChip;
} // namespace Map end

namespace Move{

class CMove_range : public Task::ITaskList
{/**
	move_range
 */
public:
	// デストラクタ
	virtual ~CMove_range(){}
	// タスク
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 計算用
	void calcMove(Map::CMapChip* pMap, int nMove);
	// 指定したキャラが、指定したマップに移動できるか
	enum eReturn
	{
		NOTENABLE,	// 移動不可
		THROUGH,	// 通り抜けは可
		ENABLE,		// 移動可能
	};
	int IsMove(Map::CMapChip* pMap, int nCurrentHeight, int nMove);

	// 設定
	void setContext(CSLGContext* p_){ p=p_;}
	void setPhase(int nPhase){ nPhase_=nPhase; }

	// アクション
	int actionAbility(CDataCharaSLG* pData);

	// static
	static int calcAbilityJump(CDataCharaSLG* pChara, CSLGContext* p);

protected:
	// 計算中変わらないけど必要なデータ
	CSLGContext*	p;
	int nJump_;
	int nPhase_;
};

} // namespace Move end
} // namespace SLG end
} // namespace BMW end
