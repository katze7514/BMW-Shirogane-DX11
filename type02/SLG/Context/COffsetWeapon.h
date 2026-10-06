/*
	katze 06/03/14
	武器補正値
*/
#pragma once

namespace BMW{
namespace SLG{

class COffsetRange
{/**
	武器補正値
 */
public:
	// コンストラクタ
	COffsetRange(){ reset(); }
	// 設定・取得
	int		getMax()const{ return nMax_; }
	void	setMax(int nMax){ nMax_=nMax; }
	void	calcMax(int nMax){ nMax_+=nMax; }
	int		getReach(bool bFlyOffset=false)const{ if(bFlyOffset){ return bFly_ ? 100000 : nReach_; }else{ return nReach_; } }
	void	setReach(int nReach){ nReach_=nReach; }
	void	calcReach(int nReach){ nReach_+=nReach; }
	bool	IsFly()const{ return bFly_; }
	void	fly(bool bFly){ bFly_=bFly; }
	
	// リセット
	void reset()
	{
		nMax_=nReach_=0;
		bFly_=false;
	}

private:
	int nMax_;		// 射程
	int nReach_;	// 到達度
	bool bFly_;		// 飛行・浮揚フラグ
};

class COffsetHit
{/**
	武器補正値
 */
public:
	// コンストラクタ
	COffsetHit(){ reset(); }
	// 設定・取得
	int		getHit()const{ return nHit_; }
	void	setHit(int nHit){ nHit_=nHit; }
	void	calcHit(int nHit){ nHit_+=nHit; }
	int		getHitOff()const{ return nHitOff_; }
	void	setHitOff(int nHitOff){ nHitOff_=nHitOff; }
	void	calcHitOff(int nHitOff){ nHitOff_+=nHitOff; }
	int		getAvoid()const{ return nAvoid_; }
	void	setAvoid(int nAvoid){ nAvoid_=nAvoid; }
	void	calcAvoid(int nAvoid){ nAvoid_+=nAvoid; }
	
	// リセット
	void reset()
	{
		nHit_=nHitOff_=nAvoid_=0;
	}

private:
	int nHit_;		// 命中
	int nHitOff_;	// 絶対命中
	int nAvoid_;	// 回避
};


} // namespace SLG end
} // namespace BMW end