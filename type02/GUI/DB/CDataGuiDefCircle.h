/*
	katze 06/02/03
	サークルメニューの定義データ
*/
#pragma once

#include "CDataGuiDefPanel.h"

namespace BMW{
namespace GUI{

class CDataGuiDefCircle : public CDataGuiDefPanel
{/**
	サークルメニューの定義データ
 */
public:
	// コンストラクタ
	CDataGuiDefCircle():nR_(1),nIntro_(10),nExit_(5){ setKind(Def::CIRCLE); }

	// 設定・取得
	int		getR()const{ return nR_; }
	void	setR(int nR){ nR_=nR; }
	int		getIntro()const{ return nIntro_; }
	void	setIntro(int nIntro){ nIntro_=nIntro; }
	int		getExit()const{ return nExit_; }
	void	setExit(int nExit){ nExit_=nExit; }

private:
	int nR_; // 半径
	int nIntro_,nExit_;
};

} // namespace GUI end
} // namespace BMW end