/*
	katze 05/02/19
	描画の基本情報を扱うクラス
*/
#pragma once

namespace BMW{
namespace Draw{

class CDrawInfo
{/**
	描画の基本情報を扱う

	Movieではこいつを元にトゥーインをする
	つまり、CMotionDataに当たる
 */
public:
	// 各パラメタのスケール定数みたいなもの
	enum eDrawInfoScale
	{
		SCALE_ALPHA=255,
		SCALE_RATE=1<<8,
		SCALE_ANGLE=511,
	};
	// コンストラクタ・デストラクタ
	CDrawInfo():lX_(0),lY_(0),lAlpha_(SCALE_ALPHA),lWidth_(SCALE_RATE),lHeight_(SCALE_RATE),lAngle_(0){}
	CDrawInfo(LONG lL):lX_(lL),lY_(lL),lAlpha_(lL),lWidth_(lL),lHeight_(lL),lAngle_(lL){}
	CDrawInfo(LONG lX, LONG lY, LONG lAlpha, LONG lWidth, LONG lHeight, LONG lAngle)
		:lX_(lX),lY_(lY),lAlpha_(lAlpha),lWidth_(lWidth),lHeight_(lHeight),lAngle_(lAngle){}

	// 設定・取得
	LONG getX() const { return lX_; }
	void setX(LONG lX){ lX_=lX; }
	LONG getY() const { return lY_; }
	void setY(LONG lY){ lY_=lY; }
	LONG getAlpha() const { return lAlpha_; }
	void setAlpha(LONG lAlpha){ lAlpha_=lAlpha; }
	LONG getWidth() const { return lWidth_; }
	void setWidth(LONG lWidth){ lWidth_=lWidth; }
	LONG getHeight() const { return lHeight_; }
	void setHeight(LONG lHeight){ lHeight_=lHeight; }
	LONG getAngle() const { return lAngle_; }
	void setAngle(LONG lAngle){ lAngle_=lAngle; }

	// 操作
	void check() const
	{// 上限下限のチェック
		CDrawInfo* pInfo = const_cast<CDrawInfo*>(this);
		if(getAlpha()>255)	pInfo->setAlpha(255);
		ef(getAlpha()<0)	pInfo->setAlpha(0);

		if(getWidth()<0)	pInfo->setWidth(0);
		if(getHeight()<0)	pInfo->setHeight(0);

		if(getAngle()<0)	pInfo->setAngle(0);
		if(getAngle()>511)	pInfo->setAngle(511);
	}
	const Draw::CDrawInfo calcAbsolute(const Draw::CDrawInfo& child)const
	{// 渡ってきたやつと自身は相対の関係にあるので、絶対値に変換する
		return Draw::CDrawInfo(getX() + child.getX(),
							   getY() + child.getY(),
							   getAlpha() * child.getAlpha() / SCALE_ALPHA,
							   roundRShift(getWidth() * child.getWidth(),8),
							   roundRShift(getHeight() * child.getHeight(),8),
							  (getAngle() + child.getAngle()) % SCALE_ANGLE
							  );
	}
	const Draw::CDrawInfo calcAbsoluteBorn(const Draw::CDrawInfo& child)const
	{// 渡ってきたやつと自身は相対の関係にあるので、絶対値に変換する
		return Draw::CDrawInfo(getX() + roundRShift(getWidth() * (gSinTable.Cos(getAngle(),child.getX())-gSinTable.Sin(getAngle(),child.getY())),8),
							   getY() + roundRShift(getHeight() * (gSinTable.Cos(getAngle(),child.getY())+gSinTable.Sin(getAngle(),child.getX())),8),
							   getAlpha() * child.getAlpha() / SCALE_ALPHA,
							   roundRShift(getWidth() * child.getWidth(),8),
							   roundRShift(getHeight() * child.getHeight(),8),
							   (getAngle() + child.getAngle()) % SCALE_ANGLE
							   );
	}

	// 演算
	bool operator==(const CDrawInfo& rhs) const
	{ return lX_==rhs.lX_&&lY_==rhs.lY_&&lAlpha_==rhs.lAlpha_&&lWidth_==rhs.lWidth_&&lHeight_==rhs.lHeight_&&lAngle_==rhs.lAngle_; }

	const CDrawInfo operator<<(int n)
	{// nビット左シフト
		return CDrawInfo(lX_<<n, lY_<<n, lAlpha_<<n, lWidth_<<n, lHeight_<<n, lAngle_<<n);
	}

	void bitShiftL(int n)
	{// 破壊的nビット左シフト
		lX_=lX_<<n;
		lY_=lY_<<n;
		lAlpha_=lAlpha_<<n;
		lWidth_=lWidth_<<n;
		lHeight_=lHeight_<<n;
		lAngle_=lAngle_<<n;
	}

	const CDrawInfo operator>>(int n)
	{// nビット右シフト
		return CDrawInfo(
							roundRShift(lX_,n), roundRShift(lY_,n), 
							roundRShift(lAlpha_,n), 
							roundRShift(lWidth_,n), roundRShift(lHeight_,n), 
							roundRShift(lAngle_,n)
						);
	}

	void bitShiftR(int n)
	{// 破壊的nビット右シフト
		lX_=roundRShift(lX_,n);
		lY_=roundRShift(lY_,n);
		lAlpha_=roundRShift(lAlpha_,n);
		lWidth_=roundRShift(lWidth_,n);
		lHeight_=roundRShift(lHeight_,n);
		lAngle_=roundRShift(lAngle_,n);
	}

private:
	LONG lX_,lY_; // 描画位置
	LONG lAlpha_; // α値(0～255)
	LONG lWidth_,lHeight_; // 幅,高さ(1<<8 スケール)
	LONG lAngle_; // 回転角度(0～511)
};


//////////////////////////////////////////////
// 各種演算子
//////////////////////////////////////////////
__inline const CDrawInfo operator+(const CDrawInfo& lhs, const CDrawInfo& rhs)
{
	return CDrawInfo(
						lhs.getX()		+	rhs.getX(),
						lhs.getY()		+	rhs.getY(),
						lhs.getAlpha()	+	rhs.getAlpha(),
						lhs.getWidth()	+	rhs.getWidth(),
						lhs.getHeight()	+	rhs.getHeight(),
						lhs.getAngle()	+	rhs.getAngle()
					);

}

__inline const CDrawInfo operator-(const CDrawInfo& lhs, const CDrawInfo& rhs)
{
	return CDrawInfo(
						lhs.getX()		-	rhs.getX(),
						lhs.getY()		-	rhs.getY(),
						lhs.getAlpha()	-	rhs.getAlpha(),
						lhs.getWidth()	-	rhs.getWidth(),
						lhs.getHeight()	-	rhs.getHeight(),
						lhs.getAngle()	-	rhs.getAngle()
					);
}

__inline const CDrawInfo operator*(const CDrawInfo& lhs, const CDrawInfo& rhs)
{
	return CDrawInfo(
						lhs.getX()		*	rhs.getX(),
						lhs.getY()		*	rhs.getY(),
						lhs.getAlpha()	*	rhs.getAlpha(),
						lhs.getWidth()	*	rhs.getWidth(),
						lhs.getHeight()	*	rhs.getHeight(),
						lhs.getAngle()	*	rhs.getAngle()
					);
}

__inline const CDrawInfo operator/(const CDrawInfo& lhs, const CDrawInfo& rhs)
{
	return CDrawInfo(
						lhs.getX()		/	rhs.getX(),
						lhs.getY()		/	rhs.getY(),
						lhs.getAlpha()	/	rhs.getAlpha(),
						lhs.getWidth()	/	rhs.getWidth(),
						lhs.getHeight()	/	rhs.getHeight(),
						lhs.getAngle()	/	rhs.getAngle()
					);
}

} // namespace Draw end
} // namespace BMW end