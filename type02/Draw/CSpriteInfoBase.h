/*
	katze 05/02/22
	DBなどで使用される
	CSpriteInfoを生成するための元になる
	データクラス
*/
#pragma once

namespace BMW{
namespace Draw{

class CSpriteInfoBase
{/**
	スプライト情報の元になるクラス
 */
public:
	// コンストラクタ
	CSpriteInfoBase():nGraphicID_(-1),nX_(0),nY_(0){ ::SetRect(&rcRect_,0,0,0,0); }
	
	// 設定・取得
	int			getGraphicID() const { return nGraphicID_; }
	void		setGraphicID(int nID){ nGraphicID_=nID; }
	const RECT& getRect() const { return rcRect_; }
	void		setRect(const RECT& rcRect){ rcRect_=rcRect; }
	void		setOffsetPos(const POINT& p){ nX_=p.x; nY_=p.y;}

	int			getX() const { return nX_; }
	int			getY() const { return nY_; }

private:
	int		nGraphicID_;	// 画像ID
	RECT	rcRect_;		// 矩形
	int		nX_,nY_;		// オフセット値
};

} // namespace Draw end
} // namespace BMW end