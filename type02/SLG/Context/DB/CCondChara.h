/*
	katze 06/04/06
	キャラデータ判定
*/
#pragma once

#include "ISlgCond.h"

namespace BMW{
namespace SLG{

class CCondChara : public ISlgCond
{/**
	キャラデータ判定
 */
public:
	enum eChara
	{
		TARGET=-2,
		CTRL,
	};

	enum eType
	{
		HP,		// 残りHP
		EXIST,	// マップ上にいるか？
		INDEX,	// マップ上の位置
		CHARA,	// キャラID
		DAMAGE,	// 現在くらっているダメージ量
		ID,		// SLG ID
	};


	// 判定
	bool judg(CSLGContext* p);

	// アクセッサ
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }
	int		getChara()const{ return nChara_; }
	void	setChara(int nChara){ nChara_=nChara; }
	int		getValue()const{ return nValue_; }
	void	setValue(int nValue){ nValue_=nValue; }

private:
	int nType_;		// 判別タイプ
	int nChara_;	// 対象キャラID
	int nValue_;	// 判定値
};

} // namespace SLG end
} // namespace BMW end