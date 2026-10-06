/*
	katze 05/03/24
	顔DBの要素
*/
#pragma once

namespace BMW{
namespace Face{

class CDataFace
{/**
	CFaceDBの要素
	ある一つの顔データ
 */
public:
	enum eToward
	{
		LEFT,
		RIGHT,
	};
	// コンストラクタ
	CDataFace(){ nFaceID_[LEFT]=nFaceID_[RIGHT]=0; }
	// 設定・取得
	int		getFaceID(int nToward) const { return nFaceID_[nToward]; }
	void	setFaceID(int nID, int nToward){ nFaceID_[nToward]=nID; }

private:
	// 0:左向き顔スプライトID
	// 1:右向き顔スプライトID
	int nFaceID_[2];
};

} // namespace Face end
} // namespace BMW end