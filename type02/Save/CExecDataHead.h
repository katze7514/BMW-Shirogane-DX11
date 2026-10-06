/*
	katze 05/03/17
	Execセーブデータのヘッダ
*/
#pragma once

namespace BMW{
namespace Save{

class CExecDataHead : public IArchive
{/**
	データシーンで表示する部分
 */
public:
	// コンストラクタ
	CExecDataHead():nVer_(2),nClear_(0),nHero_(-1),nLv_(1),nStory_(0),nBP_(0),nFP_(0),nTurn_(0),nExpert_(0),nAce_(-1),nMode_(0){}

	// シリアライズ
	void Serialize(ISerialize& s);

	// 設定・取得
	int		getVer() const { return nVer_; }
	void	setVer(int nVer) { nVer_=nVer; }
	int		getClear() const { return nClear_; }
	void	setClear(int nClear){ nClear_=nClear; }
	int		getContinue() const { return nContinue_; }
	void	setContinue(int nContinue){ nContinue_=nContinue; }
	int		getHero() const { return nHero_; }
	void	setHero(int nHero){ nHero_=nHero; }
	int		getLv() const { return nLv_; }
	void	setLv(int nLv){ nLv_=nLv; }
	int		getStory() const { return nStory_; }
	void	setStory(int nStory){ nStory_=nStory; }
	int		getBP() const { return nBP_; }
	void	setBP(int nBP){ nBP_ = ( (nBP_>BP_MAX) ? BP_MAX : nBP); }
	int		getFP() const { return nFP_;}
	void	setFP(int nFP){ nFP_ = ( (nFP_>FP_MAX) ? FP_MAX : nFP); }
	int		getTurn() const { return nTurn_; }
	void	setTurn(int nTurn){ nTurn_=nTurn; }
	int		getExpert() const { return nExpert_; }
	void	setExpert(int nExpert){ nExpert_=nExpert; }
	int		getAce() const { return nAce_; }
	void	setAce(int nAce){ nAce_=nAce; }
	int		getMode() const { return nMode_; }
	void	setMode(int nMode){ nMode_=nMode; }

	// 操作
	void	clear()
			{
				nClear_=0;
				nContinue_=0;
				nHero_=-1;
				nLv_=1;
				nStory_=-1;
				nBP_=0;
				nFP_=0;
				nTurn_=0;
				nExpert_=0;
				nAce_=-1;
				nMode_=0;
			}

	void	clearHandover()
			{
				nContinue_=0;
				nHero_=-1;
				nLv_=1;
				nStory_=-1;
				nTurn_=0;
				nExpert_=0;
				nAce_=-1;
				nMode_=0;
			}
private:
	// セーブデータのバージョン
	int nVer_;
	// クリア回数
	int nClear_;
	// コンテニュー（not 中断）回数
	int nContinue_;
	// 選択した主人公のキャラID
	int nHero_;
	// ↑のLv
	int nLv_;
	// クリアした話ID
	int nStory_;
	// 保持BP
	int nBP_;
	// 保持FP
	int nFP_;
	// クリアターン数
	int nTurn_;
	// 熟練度
	int nExpert_;
	// ACEキャラのID
	int nAce_;
	// 現在の動作モード(Heavy/Hell など)
	int nMode_;
};

__inline void CExecDataHead::Serialize(ISerialize& s)
{
	s << nVer_ << nClear_ << nContinue_ << nHero_ << nLv_ << nStory_ << nBP_ << nFP_ << nTurn_ << nExpert_ << nAce_;
	if(nVer_>1)
	{
		s << nMode_;
	}
	ef(!s.IsStoring())
	{// バージョン1だったら、バージョン2になるように設定
		nVer_=2;
		nMode_=0;
	}
}

} // namespace Save end
} // namespace BMW end