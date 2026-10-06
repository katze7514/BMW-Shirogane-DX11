/*
	katze 05/05/05
	Update 06/01/17
	戦闘状態クラス
*/
#pragma once

namespace BMW{
namespace SLG{
class CDataCharaSLG;

class CBattleStateBase
{/**
	状態のベース
 */
public:
	// コンストラクタ
	CBattleStateBase():pChara_(NULL),nWeaponID_(-1),nHit_(0){}

	// 設定・取得
	CDataCharaSLG*	getCharaData() const { return pChara_; }
	void			setCharaData(CDataCharaSLG* pChara){ pChara_=pChara; }
	int		getWeaponID() const { return nWeaponID_; }
	void	setWeaponID(int nID){ nWeaponID_=nID; }
	int		getHit() const { return nHit_; }
	void	setHit(int nHit){ nHit_=nHit; }
	int		getOffHit() const { return nOffHit_; }
	void	setOffHit(int nOffHit){ nOffHit_=nOffHit; }

	// 操作
	void	clearData(){ pChara_=NULL; nWeaponID_=-1; nHit_=0; nOffHit_=0; }

private:
	CDataCharaSLG*	pChara_;	// キャラSLGデータ
	int				nWeaponID_; // 武器SLG ID(負の時は回避とかそういう感じ)
	int				nHit_;		// 計算命中率
	int				nOffHit_;	// 絶対命中補正
};

class CBattleState
{/**
	戦闘状態を管理するクラス

	つまり、attack_ruleの前半で構築されるのはこれ
	で、これから、CDataBattleが作られ、
	デモやルールに送られていく
 */
public:
	enum eState{
		ATTACK,
		COUNTER,
		ATTACK_BACK,
		COUNTER_BACK,
	};

	// 設定・取得
	const CBattleStateBase& getState(int nState) const { return state_[nState]; }
	CBattleStateBase&		getState(int nState) { return state_[nState]; }
	CBattleStateBase*		getStatePtr(int nState){ return &state_[nState]; }

	CDataCharaSLG*	getCharaData(int nState) const { return state_[nState].getCharaData(); }
	void			setCharaData(CDataCharaSLG* pChara, int nState){ state_[nState].setCharaData(pChara); }
	int				getWeaponID(int nState) const { return state_[nState].getWeaponID(); }
	void			setWeaponID(int nID, int nState){ state_[nState].setWeaponID(nID); }
	int				getHit(int nState) const { return state_[nState].getHit(); }
	void			setHit(int nHit, int nState){ state_[nState].setHit(nHit); }
	int				getOffHit(int nState) const { return state_[nState].getOffHit(); }
	void			setOffHit(int nOffHit, int nState){ state_[nState].setOffHit(nOffHit); }

	// 操作
	void	clearData()
	{
		state_[0].clearData();
		state_[1].clearData();
		state_[2].clearData();
		state_[3].clearData();
	}

private:
	CBattleStateBase state_[4];
};

} // namespace SLG end
} // namespace BMW end