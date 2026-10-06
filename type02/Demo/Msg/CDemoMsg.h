/*
	katze 05/07/05
	デモ用メッセージ構造体
*/
#pragma once

namespace BMW{
namespace Demo{

class CDemoMsg
{/**
	デモ用メッセージ
 */
public:
	// コンストラクタ
	CDemoMsg():nSide_(0),nChara_(-1),nFace_(0),bMask_(false){}

	// 設定
	int				getSide()const{ return nSide_; }
	void			setSide(int nSide){ nSide_=nSide; }
	int				getChara()const{ return nChara_; }
	void			setChara(int nChara){ nChara_=nChara; }
	int				getFace()const{ return nFace_; }
	void			setFace(int nFace){ nFace_=nFace; }
	const string&	getMsg()const{ return sMsg_; }
	void			setMsg(const string& sMsg){ sMsg_=sMsg; }
	bool			IsMask()const{ return bMask_; }
	void			mask(bool bMask){ bMask_=bMask; }

private:
	int		nSide_;
	int		nChara_;
	int		nFace_;
	string	sMsg_;
	bool	bMask_;
};

} // namespace Demo end
} // namespace BMW end