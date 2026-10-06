/*
	katze 06/01/20
	ゲージの定義データ
*/
#pragma once

#include "IDGuiDef.h"
#include "IDataGuiDef.h"

namespace BMW{
namespace GUI{

class CDataGuiDefGage : public IDataGuiDef
{/**
	ゲージ定義データ
 */
public:
	// コンストラクタ
	CDataGuiDefGage():bLeft_(true),nRemainX_(0),nRemainY_(0){ setKind(Def::GAGE); }

	// 設定・取得
	int		getCurrentGage()const{ return nCurrentGage_; }
	void	setCurrentGage(int nCurrentGage){ nCurrentGage_=nCurrentGage; }
	bool	IsLeft()const{ return bLeft_; }
	void	left(bool bLeft){ bLeft_=bLeft; }

	const string&	getRemain()const{ return sRemain_; }
	void			setRemain(const string& sID){ sRemain_=sID; }
	int				getRemainX()const{ return nRemainX_; }
	void			setRemainX(int nX){ nRemainX_=nX; }
	int				getRemainY()const{ return nRemainY_; }
	void			setRemainY(int nY){ nRemainY_=nY; }
	
protected:
	int		nCurrentGage_;		// ゲージ
	bool	bLeft_;				// 減る方向

	string sRemain_;		// これに対応するRemainGUI
	int nRemainX_,nRemainY_;
};

} // namespace GUI end
} // namespace BMW end