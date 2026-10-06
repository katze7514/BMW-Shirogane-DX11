/*
	katze 05/05/18
	背景DBの要素
*/
#pragma once

namespace BMW{
namespace ADV{

class CDataBack
{/**
	背景DBの要素
 */
public:
	// コンストラクタ
	CDataBack():nSpriteID_(-1){}

	// 設定・取得
	int				getSpriteID() const { return nSpriteID_; }
	void			setSpriteID(int nSpriteID){ nSpriteID_=nSpriteID; }
	const string&	getName() const { return sName_; }
	void			setName(const string& sName){ sName_=sName; }

private:
	// スプライトID
	int nSpriteID_;
	// 名前
	string sName_;
};

} // namespace ADV end
} // namesapce BMW end