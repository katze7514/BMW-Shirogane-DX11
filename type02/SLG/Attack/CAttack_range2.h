/*
	katze 06/06/09
	攻撃可能範囲の計算Ver.2
*/
#pragma once

namespace BMW{

namespace Weapon{
class CDataWeaponBattle;
} // namespace Weapon end

namespace SLG{
class COffsetRange;

namespace Map{
class CMapChip;
} // namespace Map end

class CSLGContext;
namespace Attack{

class CAttack_range2 : public Task::ITaskList
{/**
	攻撃可能範囲の計算Ver.2

	全射程計算
	すべての計算状況が、コンテキスト内のrangeWeaponに設定される
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

	// フィールド計算
	void calcField(Map::CMapChip* pMapChip);
	// フィールド円系範囲計算
	void calcFieldCircle(Map::CMapChip* pMapChip, int nAttack, int nToward, int nMax);
	int IsFieldAttack(Map::CMapChip* pMap);
	void setFieldIndex(Map::CMapChip* pMap, int nAttack);
	// フィールドライン武器の範囲計算
	void calcFieldLine(Map::CMapChip* pMapChip);
	void calcFieldLineToward(Map::CMapChip* pMapChip, int nToward, int nMin, int nMax);
	void calcFieldLineTowardLine(Map::CMapChip* pMapChip, int nToward, int nMax, int nWidth);
	
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
	void actionAbility(CDataCharaSLG* pChara, bool bP=false);

	// 射程計算
	static void calcRange(CDataCharaSLG* pChara, Weapon::CDataWeaponBattle* pWeapon, Task::CTaskContext* pContext, COffsetRange& range);
	static void calcAllRange(CDataCharaSLG* pChara, CSLGContext* p);

private:
	CSLGContext* p;
	int nPhase_;
	int nIndex_;
	int nMax_;
	int nMin_;
	int nReach_;	// 武器の到達度
	int nHeight_;	// この武器を使うキャラいる高さ

	// フィールド武器
	Weapon::CDataWeaponBattle* pFieldWeapon_;
};

} // namespace Attack end
} // namespace SLG end
} // namespace BMW end