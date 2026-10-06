/*
	katze 05/03/26
	SLG上でのキャラ状態を表現するクラス
*/ 
#pragma once

namespace BMW{
namespace SLG{

class CCharaState : public IArchive
{/**
	SLG上でのキャラ状態を表現するクラス
 */
public:
	enum eValid{
		MOVE,
		ATTACK,
		SPIRIT,
		ITEM,
		CURE,
		REFILL,
		PERS,
	};
	// コンストラクタ
	CCharaState():nIndex_(-1),nAct_(0),nWay_(0),bApper_(false),bPinch_(false){ resetValid(); }

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int		getIndex() const { return nIndex_; }
	void	setIndex(int nIndex){ nIndex_=nIndex; }
	int		getAct() const { return nAct_; }
	void	setAct(int nAct){ nAct_=nAct; }
	int		getWay() const { return nWay_; }
	void	setWay(int nWay){ nWay_=nWay; }
	bool	IsApper()const{ return bApper_; }
	void	apper(bool bApper){ bApper_=bApper; }
	bool	IsPinch()const{ return bPinch_; }
	void	pinch(bool bPinch){ bPinch_=bPinch; }
	bool	IsValid(int nID)const{ return baValid_[nID]; }
	void	valid(bool bValid, int nID){ baValid_[nID]=bValid; }

	// 操作
	void	resetValid()
	{
		for(int i=MOVE; i<=PERS; i++)
			baValid_[i]=true;
	}

private:
	int	nIndex_;// 現在いるMapIndex
	int nAct_;	// 行動(Act::eAct準拠)
	int nWay_;	// 向いてる方向

	bool bApper_; // いわゆる偵察云々
	bool bPinch_; // ピンチ？

	// キャラメニューの有効フラグ
	// イベント的動作で使うと思われ
	bool baValid_[PERS+1];
};

} // namespace SLG end
} // namespace BMW end