/*
	katze 05/02/20
	テキストクラス
*/
#pragma once

#include "CText.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace GUI{
// using宣言
using namespace Task;

class CTextSize : public CText
{/**
	テキストを表示するクラス
	拡縮対応版
 */
public:
	// デストラクタ
	virtual ~CTextSize(){}

	// タスク
	virtual void OnDraw(CTaskContext*);

	// サイズの取得
	void	getSize(LONG& lWidth, LONG& lHeight){ lWidth=nSizeX_; lHeight=nSizeY_; }
	
	// 操作
	virtual void	UpdateText();
	virtual void	UpdateTextA();
	virtual void	UpdateTextAA();

protected:
	int nSizeX_;
	int nSizeY_;
};

} // namespace GUI end
} // namespace BMW end