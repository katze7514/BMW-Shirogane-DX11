/*
	katze 06/04/01
	デモ背景用のムービークリップ
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoBackMovieClip : public Movie::CMovieClip
{/**
	デモ背景用のムービークリップ
	ようは、座標計算が16bit固定小数あつかい
 */
public:
	// デストラクタ
	virtual ~CDemoBackMovieClip(){}

	// 設定
	void setDrawInfo(const Draw::CDrawInfo& drawInfo){ drawInfo_=drawInfo; drawInfo_.setX(drawInfo_.getX()<<16); drawInfo_.setY(drawInfo_.getY()<<16); }
};

} // namespace Demo end
} // namespace BMW end5