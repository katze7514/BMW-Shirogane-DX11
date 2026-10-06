/*
	katze 05/05/21
	コード生成用設定クラス
*/
#pragma once

namespace BMW{
namespace Sound{
namespace Code{

class CCmdSound
{/**
	Sound

	BGM/SE兼用
 */
public:
	// コンストラクタ
	CCmdSound():nCtrl_(-1),nFade_(0){}
	// 設定・取得
	int				getCtrl()const{ return nCtrl_; }
	void			setCtrl(int nCtrl){ nCtrl_=nCtrl; }
	const string&	getBgm()const{ return sBgm_; }
	void			setBgm(const string& sBgm){ sBgm_=sBgm; }
	int				getFade()const{ return nFade_; }
	void			setFade(int nFade){ nFade_=nFade; }

private:
	int		nCtrl_;
	string	sBgm_;
	int		nFade_;
};


} // namespace Code end
} // namespace Sound end
} // namespace BMW end