/*
	katze 06/02/02
	モーションの基底
*/
#pragma once

#include "../Draw/CDrawInfo.h"

namespace BMW{
namespace Movie{
// using宣言
using namespace Draw;

class IMotion
{/**
	モーションの基底
 */
public:
	// コンストラクタ・デストラクタ
	IMotion():lStep_(1),nEdging_(0),lCurrentStep_(0){}
	virtual ~IMotion(){}

	// 設定・取得
	LONG				getStep() const { return lStep_;}
	void				setStep(LONG lStep){ lStep_=lStep; }
	int					getEdging() const { return nEdging_; }
	void				setEdging(int nEdging){ nEdging_=nEdging; }

	const CDrawInfo&	getCurrent() const { return current_; }
	void				setCurrent(const CDrawInfo& current){ current_=current; }
	void				setCurrent(LONG lX=0, LONG lY=0, LONG lAlpha=255, LONG lWidth=256, LONG lHeight=256, LONG lAngle=0)
						{	
							current_.setX(lX); current_.setY(lY); 
							current_.setAlpha(lAlpha); 
							current_.setWidth(lWidth); current_.setHeight(lHeight); 
							current_.setAngle(lAngle); 
						}
	LONG				getCurrentStep() const { return lCurrentStep_; }
	void				setCurrentStep(LONG lStep) { lCurrentStep_=lStep; }

	// 動作
	virtual bool inc(){ return true; }	// 開始値から終了値へ進む。終了値に到達するとtrueを返す
	virtual bool dec(){ return true; }	// 終了値から開始値へ進む。開始値に到達するとtrueを返す
	virtual bool IsStart(){ return true; }
	virtual bool IsEnd(){ return true; }
	virtual void reset(bool bEnd=false){ bEnd ? lCurrentStep_=lStep_ : lCurrentStep_=0; }

	// 現在値系のヘルパー
	LONG getX() const { return current_.getX(); }
	void setX(LONG lX){ current_.setX(lX); }
	LONG getY() const { return current_.getY(); }
	void setY(LONG lY){ current_.setY(lY); }
	LONG getAlpha() const { return current_.getAlpha(); }
	void setAlpha(LONG lAlpha){ current_.setAlpha(lAlpha); }
	LONG getWidth() const { return current_.getWidth(); }
	void setWidth(LONG lWidth){ current_.setWidth(lWidth); }
	LONG getHeight() const { return current_.getHeight(); }
	void setHeight(LONG lHeight){ current_.setHeight(lHeight); }
	LONG getAngle() const { return current_.getAngle(); }
	void setAngle(LONG lAngle){ current_.setAngle(lAngle); }

	// あるとなにげに便利じゃね？
	// CDrawInfo info = CMotion motion; ができるし
	// CMotionは、CDrawInfoを動かすためのものなので、理屈としても問題と思われ
	operator const CDrawInfo& () const { return current_; }

protected:
	LONG		lStep_;		// ステップ数
	int			nEdging_;	// イージングパラメタ(-100～100)

	// イージング処理
	// 他の補間法を使う時は、こいつをオーバーライドする
	virtual void edging(){}

	// 現在の状況
	CDrawInfo	current_;
	LONG		lCurrentStep_;
};

} // namespace Movie end
} // namespace BMW end