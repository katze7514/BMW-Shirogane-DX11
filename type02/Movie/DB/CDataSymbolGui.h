/*
	katze 05/05/10
	update 06/01/16
	GUIシンボルの設定クラス群
*/
#pragma once

#include "IDataSymbol.h"

namespace BMW{
namespace Movie{

class CDataSymbolGui : public IDataSymbol
{/**
	GUIシンボル共通部分
 */
public:
	enum eGuiKind{
		SPRITE,
		SYMBOL,
	};
	// コンストラクタ・デストラクタ
	CDataSymbolGui(int nKind,int n,int nGuiKind=-1):IDataSymbol(nKind),nGuiKind_(nGuiKind){ vecID_.resize(n); }
	virtual ~CDataSymbolGui(){}

	// アクセッサ
	int		getGuiKind()const{ return nGuiKind_; }
	void	setGuiKind(int nKind){ nGuiKind_=nKind; }

	int		getID(int nIndex){ return vecID_[nIndex]; }
	void	setID(int nID, int nIndex){ vecID_[nIndex]=nID; }
	
protected:
	int nGuiKind_;
	vector<int> vecID_;
};

class CDataSymbolGraphic : public CDataSymbolGui
{/**
	グラフィックシンボル
 */
public:
	enum eGraphicKind{
		NORMAL,
		SIZE,
		ROTATE,
		ROTATE2,
		ROTATE3,
		MORPH,
		AFFINE,
	};

	// コンストラクタ・デストラクタ
	CDataSymbolGraphic():CDataSymbolGui(GRAPHIC,1,NORMAL){}
	virtual ~CDataSymbolGraphic(){}
};

class CDataSymbolButton : public CDataSymbolGui
{/**
	ButtonSymbolに対応する設定データ
 */
public:
	enum eButtonKind{
		NONE,
		SPRITE,
		SYMBOL
	};
	// コンストラクタ・デストラクタ
	CDataSymbolButton():CDataSymbolGui(BUTTON,3,SYMBOL){}
	virtual ~CDataSymbolButton(){}

	const RECT&	getRect()const{ return rect_; }
	void		setRect(const RECT& rect){ rect_=rect; }

protected:
	RECT rect_;	// 判定領域
};

class CDataSymbolNum : public CDataSymbolGui
{/**
	NumSymbolに対応する設定データ
 */
public:
	enum eNumKind{
		SPRITE,
		SYMBOL
	};
	// コンストラクタ・デストラクタ
	CDataSymbolNum():CDataSymbolGui(NUM,12,SPRITE){}
	virtual ~CDataSymbolNum(){}
};


} // namespace Movie end
} // namespace BMW end