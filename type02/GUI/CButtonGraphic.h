/*
	katze 05/02/20
	グラフィックボタンクラス
*/
#pragma once

#include "../Draw/CSpriteInfo.h"
#include "CButton.h"

namespace BMW{
namespace GUI{
// using宣言
using Draw::CSpriteInfo;

class CButtonGraphic : public CButton
{/**
	グラフィックを表示能力を持つボタン
 */
public:
	// デストラクタ
	virtual ~CButtonGraphic(){}

	// タスク
	virtual void OnDraw(CTaskContext*);

	// 設定・取得
	const CSpriteInfo&	getSpriteInfo(int nState) const { return sprite_[nState]; }
	void				setSpriteInfo(const CSpriteInfo& info,int nState){ sprite_[nState]=info; }

	// 操作
	void getSize(LONG& lWidth,LONG& lHeight) const
	{ lWidth=sprite_[getState()].getWidth(); lHeight=sprite_[getState()].getHeight(); }
	void getDrawSize(LONG& lWidth,LONG& lHeight) const { getSize(lWidth,lHeight); }

protected:
	// ボタングラフィック
	// 0:NORMAL 1:OVER 2:PRESS
	CSpriteInfo sprite_[3];
};

} // namespace GUI end
} // namespace BMW end