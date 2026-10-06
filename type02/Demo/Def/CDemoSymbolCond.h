/*
	katze 06/03/24
	デモシンボル条件
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoSymbolCond
{/**
	デモシンボル条件
 */
public:
	// コンストラクタ
	CDemoSymbolCond():nRatio_(100){}

	// アクセッサ
	int	 getRatio()const{ return nRatio_; }
	void setRatio(int nRatio){ nRatio_=nRatio; }
	int	 getSymbolID()const{ return nID_; }
	void setSymbolID(int nID){ nID_=nID; }

private:
	// 確率
	int nRatio_;
	// シンボルID
	int nID_;
};

} // namespace Demo end
} // namespace BMW end