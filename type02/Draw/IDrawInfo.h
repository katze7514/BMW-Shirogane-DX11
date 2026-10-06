/*
	katze 06/02/01
	描画の基本情報を扱う基底クラス
*/
#pragma once

namespace BMW{
namespace Draw{

class IDrawInfo
{/**
	描画情報の基底クラス
 */
public:
	enum eDrawInfoScale
	{// パラメタのスケール値
		SCALE_ALPHA=255,
		SCALE_RATE=1<<8,
		SCALE_ANGLE=511,
	};
	// 設定・取得
	virtual LONG getX() const=0;
	virtual void setX(LONG lX)=0;
	virtual LONG getY() const=0;
	virtual void setY(LONG lY)=0;
	virtual LONG getAlpha() const=0;
	virtual void setAlpha=0;
	virtual LONG getWidth() const=0;
	virtual void setWidth(LONG lWidth)=0;
	virtual LONG getHeight() const=0;
	virtual void setHeight(LONG lHeight)=0;
	virtual LONG getAngle() const=0;
	virtual void setAngle(LONG lAngle)=0;
};

} // namespace Draw end
} // namespace BMW end