/*
	katze 06/01/20
	GuiDefの設定データ基底クラス
*/
#pragma once

namespace BMW{
namespace GUI{

class IDataGuiDef
{/**
	GuiDefの設定データ基底クラス
 */
public:
	// デストラクタ
	virtual ~IDataGuiDef(){}

	// 設定・取得
	int		getKind()const{ return nKind_; }
	void	setKind(int nKind){ nKind_=nKind; }

	const string&	getID()const{ return sID_; }
	void			setID(const string& sID){ sID_=sID; }

protected:
	int		nKind_;
	string	sID_;
};

} // namespace GUI end
} // namespace BMW end