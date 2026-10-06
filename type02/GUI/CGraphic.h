/*
	katze 05/02/19
	グラフィッククラス
*/
#pragma once

#include "../Draw/CSpriteInfo.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Tasl end

namespace GUI{
using namespace Task;
using Draw::CSpriteInfo;

class CGraphic : public CTaskBase
{/**
	グラフィックを一枚表示する

	表示方法は、テンプレートパラメータとして、
	policyクラスを受け取る。
 */
public:
	// デストラクタ
	virtual ~CGraphic(){}

	// タスク処理
	virtual void Task(CTaskContext*);
	virtual void OnDraw(CTaskContext*);

	// 設定・取得
	const CSpriteInfo&	getSpriteInfo() const { return sprite_; }
	void				setSpriteInfo(const CSpriteInfo& sprite){ sprite_=sprite; }

	// 操作
	void getSize(LONG& lWidth,LONG& lHeight) const
	{ lWidth=sprite_.getWidth(); lHeight=sprite_.getHeight(); }

	virtual void getDrawSize(LONG& lWidth,LONG& lHeight) const { getSize(lWidth,lHeight); }
	
protected:
	// スプライト情報
	CSpriteInfo sprite_;
};

} // namespace GUI end
} // namespace BMW end