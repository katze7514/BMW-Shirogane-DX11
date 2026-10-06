/*
	katze 05/03/24
	マップの当たり判定を担うクラス
*/
#pragma once

#ifdef BMW_DEBUG
//#define BMW_DEBUG_MAP
#endif

namespace BMW{

namespace Task{
class CTaskContext;
} // naemsapce Task end

namespace SLG{
class COffsetRange;

namespace Map{

class CMapChipState : public GUI::CButton
{/**
	マップのあたり判定を担うクラス

	他にもグリッドの表示や、移動・攻撃範囲領域表示なども担う
	つまり、マップチップの状態を扱う
 */
public:
	// コンストラクタ
	CMapChipState():nMove_(-1),nDist_(-1),nAttack_(-1),nRealDist_(-1),nAtkHeight_(-1),nFieldAttack_(-1){ setRange(0,0,64,16); resetFieldToward(); }

	// 設定・取得
	int		getMove() const { return nMove_; }
	void	setMove(int nMove){ nMove_=nMove; }
	int		getDist() const { return nDist_; }
	void	setDist(int nDist){ nDist_=nDist; }
	int		getAttack() const { return nAttack_; }
	void	setAttack(int nAttack){ nAttack_=nAttack; }
	int		getAtkHeight(){ return nAtkHeight_; }
	void	setAtkHeight(int nHeight){ nAtkHeight_=nHeight; }
	int		getRealDist() const { return nRealDist_; }
	void	setRealDist(int nRealDist){ nRealDist_=nRealDist; }
	//bool	IsCoreAttack()const{ return bCoreAttack_; }
	//void	coreAttack(bool bCore){ bCoreAttack_=bCore; }

	int		getFieldToward(int nIndex) const { return anFieldToward_[nIndex]; }
	void	setFieldToward(int nFieldToward)
	{
		if(anFieldToward_[0]<-1)
			anFieldToward_[0]=nFieldToward;
		ef(anFieldToward_[1]<-1)
			anFieldToward_[1]=nFieldToward;
	}
	void	resetFieldToward()
	{
		anFieldToward_[0]=anFieldToward_[1]=-2;
	}
	int		getFieldAttack() const { return nFieldAttack_; }
	void	setFieldAttack(int nFieldAttack){ nFieldAttack_=nFieldAttack; }

	// タスク
	void Task(Task::CTaskContext*);
	void OnDraw(Task::CTaskContext*);

	// アクション
	void actionOverIn(Task::CTaskContext*);
	//void actionOverOut(Task::CTaskContext*);

	// 操作
	bool IsAtkRange();
	bool IsHeight();
	bool IsCore();
	bool IsField();

	// static
	static bool IsAction(){ return bAction_; }
	static void action(bool bAction){ bAction_=bAction; }
	static bool IsGrid(){ return bGrid_; }
	static void grid(bool bGrid){ bGrid_=bGrid;}
	static bool IsMove(){ return bMove_; }
	static void move(bool bMove){ bMove_=bMove;}
	static bool IsAttack(){ return bAttack_; }
	static void attack(bool bAttack){ bAttack_=bAttack;}

	static void setRangeData(Weapon::CDataWeaponBattle* pWeapon, int nToward=-1);

	static int	getMax(){ return nMax_; }
	static void	setMax(int nMax){ nMax_=nMax; }
	static int	getMin(){ return nMin_; }
	static void	setMin(int nMin){ nMin_=nMin; }
	static int	getCoreMax(){ return nCoreMax_; }
	static void	setCoreMax(int nCoreMax){ nCoreMax_=nCoreMax; }
	static int	getCoreMin(){ return nCoreMin_; }
	static void	setCoreMin(int nCoreMin){ nCoreMin_=nCoreMin; }
	static int	getHeight(){ return nHeight_; }
	static void	setHeight(int nHeight){ nHeight_=nHeight; }
	static int	getField(){ return nField_; }
	static void	setField(int nField){ nField_=nField; }
	//static int	getHeightMin(){ return nHeightMin_; }
	//static void	setHeightMin(int nHeight){ nHeightMin_=nHeight; }
	static bool IsMapValid(){ return bEnable_; }
	static void mapValid(bool bEnable){ bEnable_=bEnable; }
	static void	setGraphic(GUI::CGraphic* graphic, int nSprite){ sprite_[nSprite]=graphic; }
	static void deleteGraphic()
	{
		for(int i=0; i<5; ++i)
			DELETE_SAFE(sprite_[i]);
	}

private:
	// 作業領域
	int nMove_;		// 移動関連
	int nDist_;		// 距離関連
	int nAttack_;	// 攻撃関連
	int nRealDist_;	// 攻撃距離関連
	int nAtkHeight_;// 攻撃時の基準位置からの高さの差
	//bool bCoreAttack_; // 中心距離か否か

	// フィールドライン武器用
	int anFieldToward_[2]; // フィールド武器の時のこいつが属する方向ID
						   // 最大で二つの方向と被る可能性がある
	int nFieldAttack_; // フィールドライン攻撃距離
	

	// フラグは、全MapChipState共通
	// 動作フラグ
	static bool bAction_;
	// グリッドフラグ
	static bool bGrid_;
	// 移動範囲表示フラグ
	static bool bMove_;
	// 攻撃範囲表示フラグ
	static bool bAttack_;
	// 現在有効な表示範囲
	static int	nMax_;
	static int	nMin_;
	static int	nCoreMax_;
	static int	nCoreMin_;
	static int	nHeight_;
	static int	nField_; // 現在表示すべきフィールド方向ID
	//static int	nHeightMin_;
	static bool	bEnable_;
	// スプライト自体は、全MapChipState共通
	static GUI::CGraphic* sprite_[5];

#ifdef BMW_DEBUG_MAP
	bool bOver_;
#endif
};

} // namespace Map end
} // namespace SLG end
} // namespace BMW end