/*
	katze 06/01/20
	各種ウィジットデータ
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDefWidget.h"

namespace BMW{
namespace GUI{

class CDataGuiDefName : public IDataGuiDefWidget
{/**
	キャラ名ウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefName():nToward_(0){ setKind(Widget::NAME); }

	// 設定・取得
	int		getToward()const{ return nToward_; }
	void	setToward(int nToward){ nToward_=nToward; }

private:
	int nToward_;
};

class CDataGuiDefFace : public IDataGuiDefWidget
{/**
	顔ウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefFace():nToward_(0),bBattle_(false){ setKind(Widget::FACE); }

	// 設定・取得
	int		getToward()const{ return nToward_; }
	void	setToward(int nToward){ nToward_=nToward; }
	bool	IsBattle()const{ return bBattle_; }
	void	battle(bool bBattle){ bBattle_=bBattle; }

private:
	int		nToward_; // 左ならtrue
	bool	bBattle_; // 戦闘用ならtrue
};

class CDataGuiDefSymbol : public IDataGuiDefWidget
{/**
	シンボルウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefSymbol(){ setKind(Widget::SYMBOL); }

	// 設定・取得
	int	getSymbolID()const{ return nSymbolID_; }
	void setSymbolID(int nSymbolID){ nSymbolID_=nSymbolID; }

private:
	int nSymbolID_;
};

class CDataGuiDefWidgetButton : public IDataGuiDefWidget
{/**
	ボタンウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefWidgetButton():nAct_(Button::NORMAL){ setKind(Widget::BUTTON); }

	// 設定・取得
	int				getAct()const{ return nAct_; }
	void			setAct(int nAct){ nAct_=nAct; }
	const string&	getButtonID()const{ return sButtonID_; }
	void			setButtonID(const string& sButtonID){ sButtonID_=sButtonID; }
	int				getSymbolID()const{ return nSymbolID_; }
	void			setSymbolID(int nSymbolID){ nSymbolID_=nSymbolID; sButtonID_.clear(); }

private:
	int		nAct_;
	string	sButtonID_;
	int		nSymbolID_;
};

class CDataGuiDefText : public IDataGuiDefWidget
{/**
	テキストウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefText():nType_(Text::NORMAL),nFont_(CText::FONT_MINCHO),nSize_(14),nSide_(Text::LEFT),color_(RGB(63,57,54)){ setKind(Widget::TEXT); }

	// 設定・取得
	int				getType()const{ return nType_; }
	void			setType(int nType){ nType_=nType; }
	const string&	getText()const{ return sText_; }
	void			setText(const string& sText){ sText_=sText; }
	int				getFont()const{ return nFont_; }
	void			setFont(int nFont){ nFont_=nFont; }
	int				getSize()const{ return nSize_; }
	void			setSize(int nSize){ nSize_=nSize; }
	int				getSide()const{ return nSide_; }
	void			setSide(int nSide){ nSide_=nSide; }
	const COLORREF&	getColor()const{ return color_; }
	void			setColor(COLORREF color){ color_=color; }
	void			setColorRGB(const string& color){ string s = '"'+color+'"'; LPCSTR lp = s.c_str(); CStringScanner::GetStrColor(lp,color_); }

	// 操作
	bool IsDynamic()const{ return sText_.empty(); }

private:
	int			nType_; // ↓の判定は、こいつが負になってるってことで
	string		sText_; // nameが使われた時はこいつがID
	int			nFont_;
	int			nSize_;
	int			nSide_;
	COLORREF	color_;
};

class CDataGuiDefWidgetGraphic : public IDataGuiDefWidget
{/**
	グラフィックウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefWidgetGraphic(){ setKind(Widget::GRAPHIC); }

	// 設定・取得
	const string&	getGraphicID()const{ return sGraphicID_; }
	void			setGraphicID(const string& sGraphicID){ sGraphicID_=sGraphicID; }

private:
	string sGraphicID_;
};


class CDataGuiDefNum : public IDataGuiDefWidget
{/**
	数字ウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefNum(){ setKind(Widget::NUM); }

	// 設定・取得
	list<int>&	getNumList(){ return listNum_; }
	void		setNumID(int nNumID){ listNum_.push_back(nNumID); }

private:
	list<int> listNum_;
};

class CDataGuiDefWidgetRemain : public IDataGuiDefWidget
{/**
	ウィジットとしてのパネルウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefWidgetRemain():nTurn_(0){ setKind(Widget::REMAIN); }

	const string&	getRemainID()const{ return sRemainID_; }
	void			setRemainID(const string& sRemainID){ sRemainID_=sRemainID; }

	int				getTurn()const{ return nTurn_; }
	void			setTurn(int nTurn){ nTurn_=nTurn; }

private:
	string	sRemainID_;
	int		nTurn_;
};

class CDataGuiDefWidgetGage : public IDataGuiDefWidget
{/**
	ゲージウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefWidgetGage():nLeft_(-1){ setKind(Widget::GAGE); }

	const string&	getGageID()const{ return sGageID_; }
	void			setGageID(const string& sGageID){ sGageID_=sGageID; }
	int		IsLeft()const{ return nLeft_; }
	void	left(int nLeft){ nLeft_=nLeft; }

private:
	string	sGageID_;
	// 未定義だったら-1
	// 定義されてたら、大本のを上書きする
	int		nLeft_;
};

class CDataGuiDefWidgetPanel : public IDataGuiDefWidget
{/**
	ウィジットとしてのパネルウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefWidgetPanel(){ setKind(Widget::PANEL); }

	const string&	getPanelID()const{ return sPanelID_; }
	void			setPanelID(const string& sPanelID){ sPanelID_=sPanelID; }

private:
	string sPanelID_;
};

class CDataGuiDefObj : public IDataGuiDefWidget
{/**
	OBJウィジット
 */
public:
	// コンストラクタ
	CDataGuiDefObj():nType_(0){}

	// 設定・取得
	int		getType()const{ return nType_; }
	void	setType(int nType){ nType_=nType; }

private:
	int nType_;
};

} // namespace GUI end
} // namespace BMW end