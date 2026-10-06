/*
	katze 06/07/25
	攻撃可能ルートの計算Ver.2
*/
#pragma once

namespace BMW{
namespace SLG{
class COffsetRange;

namespace Map{
class CMapChip;
} // namespace Map end

class CSLGContext;
namespace Attack{

class CAttack_road2 : public Task::ITaskList
{/**
	攻撃ルートの計算Ver.2
	Move領域を使って計算。
	対象までの距離を取得
 */
public:
	// タスク
	void OnReset(Task::CTaskContext*);
	void OnInit(Task::CTaskContext*);
	void OnAction(Task::CTaskContext*);

	// 計算用
	void calcAttack(Map::CMapChip* pMap, int nAttack, int nOff, int nDist, int nToward);
	enum eReturn
	{
		NOTENABLE,		// 攻撃不可
		THROUGH,		// 通り抜けは可
		ENABLE,			// 攻撃可能
		ENABLE_CHARA,	// キャラがいるが攻撃可
	};
	int IsAttack(Map::CMapChip* pMap);
	void setIndex(Map::CMapChip* pMap, int nAttack, int nDist);
	// 設定
	void	setSLGContext(CSLGContext* p_){ p=p_; }
	int		getPhase()const{ return nPhase_; }
	void	setPhase(int nPhase){ nPhase_=nPhase; }
	int		getIndex()const{ return nIndex_; }
	void	setIndex(int nIndex){ nIndex_=nIndex; }
	int		getMin()const{ return nMin_; }
	void	setMin(int nMin){ nMin_=nMin; }
	int		getMax()const{ return nMax_; }
	void	setMax(int nMax){ nMax_=nMax; }
	int		getReach()const{ return nReach_; }
	void	setReach(int nReach){ nReach_=nReach; }
	int		getHeight()const{ return nHeight_; }
	void	setHeight(int nHeight){ nHeight_=nHeight; }

	// アクション
	void actionAbility(CDataCharaSLG* pChara);

private:
	CSLGContext*	p;
	int nPhase_;
	int nIndex_;
	int nMax_;
	int nMin_;
	int nReach_;	// 武器の到達度
	int nHeight_;	// この武器を使うキャラいる高さ
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end