/*
	katze 05/02/20
	テキストクラス
*/
#pragma once

#include "../Task/ITaskBase.h"

namespace BMW{

namespace Task{
class CTaskContext;
} // namespace Task end

namespace GUI{
// using宣言
using namespace Task;

class CText : public CTaskBase
{/**
	テキストを表示するクラス
	まぁ、単なるCTextPlaneのラッパーだがｗ
 */
public:
	// フォント設定に使うフォントID
	enum eFontID{
		FONT_GOTHIC,	// MS ゴシック
		FONT_P_GOTHIC,  // MS Pゴシック
		FONT_MINCHO,	// MS 明朝
		FONT_P_MINCHO,	// MS P明朝
	};
	// デストラクタ
	CText();
	virtual ~CText(){}

	// タスク
	virtual void Task(CTaskContext*);
	virtual void OnDraw(CTaskContext*);

	// 設定・取得
	CTextFastPlane& getTextPlane(){ return text_; }

	// サイズの取得
	virtual void	getSize(LONG& lWidth, LONG& lHeight){ int nX,nY; text_.GetFont()->GetSize(nX,nY); lWidth=nX; lHeight=nY; }
	void			getDrawSize(LONG& lWidth, LONG& lHeight){ getSize(lWidth,lHeight); }
	
	// 操作
	/// 表示する文字列の設定・取得
	void			setText(const string& s){ text_.GetFont()->SetText(s); }
	const string&	getText() const { return text_.getFont().getText(); }
	string&			getText(){ return text_.GetFont()->getText(); }

	virtual void	UpdateText();
	virtual void	UpdateTextA();
	virtual void	UpdateTextAA();
	void			calcOffset(); // 寄せ位置からオフセットを計算
	
	/// フォント設定を少々
	void setFont(int nFontID);
	void setSize(int nPoint){ text_.GetFont()->SetSize(nPoint); }
	void setColor(COLORREF rgb){ text_.GetFont()->SetColor(rgb); }
	void setSide(int nSide){ nSide_=nSide; }

	// もっと細かい指定が必要な場合
	CFont& getFontConf(){ return *text_.GetFont(); }
	const CFont& getFontConf()const{ return text_.getFont(); }
	CFont* getFont(){ return text_.GetFont(); }

protected:
	CTextFastPlane  text_;
	int				nSide_;	// 寄せ位置:GUI::Textに定義
	int				nOffX_; // 寄せに対応するオフセット
};

} // namespace GUI end
} // namespace BMW end