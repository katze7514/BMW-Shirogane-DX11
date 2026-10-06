/*
	katze 06/01/30
	円運動を担うモーション
*/
#pragma once

#include "IMotion.h"

namespace BMW{
namespace Movie{

class CMotionCircle : public IMotion
{/**
	円運動を担うモーション
	つまり、動くのは角度
 */
public:
	typedef katzeSDK::Misc::CFixedNum CFixedNum;
	// コンストラクタ
	CMotionCircle():nX_(0),nY_(0),nR_(1){}

	// 設定・取得
	int		getCoreX()const{ return nX_; }
	void	setCoreX(int nX){ nX_=nX; }
	int		getCoreY()const{ return nY_; }
	void	setCoreY(int nY){ nY_=nY; }
	int		getR()const{ return nR_; }
	void	setR(int nR){ nR_=nR; }

	void	setStartAngle(LONG lAngle){ lStartAngle_=lAngle; }
	void	setEndAngle(LONG lAngle){ lEndAngle_=lAngle; }
	void	setCurrentAngle(LONG lAngle){ lCurrentAngle_=lAngle; }
	
	// 動作
	bool inc();
	bool dec();
	bool IsStart(){ return lStartAngle_==lCurrentAngle_; }
	bool IsEnd(){ return lEndAngle_==lCurrentAngle_; }
	void reset(bool bEnd=false)
	{
		IMotion::reset();
		if(!bEnd)
		{// 初期
			lCurrentAngle_=lStartAngle_;
		}
		else
		{// 終了
			lCurrentAngle_=lEndAngle_;
		}
		calc();
	}
	void swap()
	{
		lCurrentAngle_ = lStartAngle_;
		lStartAngle_ = lEndAngle_;
		lEndAngle_ = lCurrentAngle_;
	}

protected:
	// 角度0～511
	LONG		lStartAngle_;	// 初期角度
	LONG		lEndAngle_;		// 終了角度
	int			nR_;			// 半径
	int			nX_,nY_;		// 中心座標

	LONG		lCurrentAngle_;	// 現在角度

	// 角度計算
	void edging();

	// 座標計算
	void calc();
};

} // namespace Movie end
} // namespace BMW end