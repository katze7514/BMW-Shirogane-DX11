/*
	katze 06/02/19
	ボタンの定義データ
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDef.h"

namespace BMW{
namespace GUI{

class CDataGuiDefButton : public IDataGuiDef
{/**
	ボタン定義データ
 */
public:
	// コンストラクタ
	CDataGuiDefButton(){ setKind(Def::BUTTON); }

	// 設定・取得
	int				getSymbolID()const{ return nSymbolID_; }
	void			setSymbolID(int nID){ nSymbolID_=nID; }
	const string&	getPopUp()const{ return sPopUp_; }
	void			setPopUp(const string& sPopUp){ sPopUp_=sPopUp; }
	
protected:
	int nSymbolID_;
	string sPopUp_;
};

class CDataGuiDefTextGui : public IDataGuiDef
{/**
	テキスト定義データ
 */
public:
	// コンストラクタ
	CDataGuiDefTextGui():nFont_(CText::FONT_MINCHO),nSize_(14),nSide_(Text::LEFT),color_(RGB(0,0,0)),nType_(Text::POPUP){ setKind(Def::TEXT); }

	// 設定・取得
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
	int				getType()const{ return nType_; }
	void			setType(int nType){ nType_=nType; }
	const string&	getPopUp()const{ return sPopUp_; }
	void			setPopUp(const string& sPopUp){ sPopUp_=sPopUp; }
	
protected:
	string		sText_;
	int			nFont_;
	int			nSize_;
	int			nSide_;
	COLORREF	color_;
	int			nType_;
	string		sPopUp_;
};

class CDataGuiDefGraphic : public IDataGuiDef
{/**
	グラフィック定義データ
 */
public:
	// コンストラクタ
	CDataGuiDefGraphic(){ setKind(Def::GRAPHIC); }

	// 設定・取得
	int				getSymbolID()const{ return nSymbolID_; }
	void			setSymbolID(int nID){ nSymbolID_=nID; }
	const string&	getPopUp()const{ return sPopUp_; }
	void			setPopUp(const string& sPopUp){ sPopUp_=sPopUp; }
	
protected:
	int nSymbolID_;
	string sPopUp_;
};

} // namespace GUI end
} // namespace BMW end