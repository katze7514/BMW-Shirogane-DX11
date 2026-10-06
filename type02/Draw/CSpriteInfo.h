/*
	katze 05/02/19
	スプライト情報を扱うクラス
*/
#pragma once

namespace BMW{
namespace Draw{

class CSpriteInfo
{/**
	スプライト管理の基本処理単位
	yaneでいうCSimpleSpriteに当たるクラス
 */
public:
	// コンストラクタ・デストラクタ
	CSpriteInfo():nX_(0),nY_(0){}
	virtual ~CSpriteInfo(){}
	
	// 設定・取得
	const CPlane&	getPlane() const { return plane_; }
	void			setPlane(const CPlane& plane){ plane_=plane; }
	const RECT&		getRect() const { return rcRect_; }
	void			setRect(const RECT& rect){ rcRect_=rect; }
	void			getOffsetPos(int& nX,int& nY) const { nX=nX_; nY=nY_; }
	void			setOffsetPos(int nX, int nY) { nX_=nX; nY_=nY; }

	// 操作
	void setRect(int nLeft, int nTop, int nRight, int nBottom)
	{ ::SetRect(&rcRect_,nLeft,nTop,nRight,nBottom); }
	int		getX() const { return nX_; }
	int		getY() const { return nY_; }
	LONG	getWidth() const { return rcRect_.right - rcRect_.left;}
	LONG	getHeight() const { return rcRect_.bottom - rcRect_.top;}

protected:
	CPlane	plane_;		// Plane:どっかのCPlaneLoaderから取得される
	RECT	rcRect_;	// 矩形
	int		nX_,nY_;	// オフセット値
};

} // namespace Draw end
} // namespace BMW end