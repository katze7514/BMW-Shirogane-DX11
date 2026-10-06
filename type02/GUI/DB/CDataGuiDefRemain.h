/*	
	katze 06/02/04
	現在値 /　最大値
	というGUIの定義データ
*/
#pragma once

#include "IDataGuiDef.h"

namespace BMW{
namespace GUI{

class CDataGuiDefRemain : public IDataGuiDef
{/**
	現在値 /　最大値
	というGUIの定義データ
 */
public:
	enum ePos{
		SYMBOL,
		X,
		Y,
	};
	// コンストラクタ
	CDataGuiDefRemain():nTurn_(0)
	{ 
		setKind(Def::REMAIN);
		nMaxNum_[SYMBOL]=nSlash_[SYMBOL]=-1;
		nMaxNum_[X]=nMaxNum_[Y]=nSlash_[X]=nSlash_[Y]=0;
	}

	// 設定・取得
	int		getCurrentNumN()const{ return nCurrentNumN_; }
	void	setCurrentNumN(int nCurrentNumN){ nCurrentNumN_=nCurrentNumN; }
	int		getCurrentNumE()const{ return nCurrentNumE_; }
	void	setCurrentNumE(int nCurrentNumE){ nCurrentNumE_=nCurrentNumE; }
	int		getMaxNum(int nPos)const{ return nMaxNum_[nPos]; }
	void	setMaxNum(int nMaxNum, int nPos){ nMaxNum_[nPos]=nMaxNum; }
	int		getSlash(int nPos)const{ return nSlash_[nPos]; }
	void	setSlash(int nSlash,int nPos){ nSlash_[nPos]=nSlash; }

	int		getTurn()const{ return nTurn_; }
	void	setTurn(int nTurn){ nTurn_=nTurn; }

private:
	int nCurrentNumN_;
	int nCurrentNumE_;
	int nMaxNum_[3];
	int nSlash_[3];
	int nTurn_;
};

} // namespace GUI end
} // namespace BMW end