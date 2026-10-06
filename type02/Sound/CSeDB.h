/*
	katze 05/05/20
	SE用のDB
*/
#pragma once

namespace BMW{
namespace Sound{

class CSeDB
{/**
	SE用のDB

	ようは、文字列ID管理可能版
 */
public:
	// 設定・取得
	CSELoader*	getSeLoader(){ return &loader_; }
	void		setSeDB(const string& sFile, const string& sFileID);

	//	ID版
	void		OnPlay(){ loader_.OnPlay(); }
	void		PlayN(int nNo){ if(getSoundSE(nNo)){ loader_.PlayN(nNo); } }
	void		Play(int nNo){ if(getSoundSE(nNo)){ loader_.Play(nNo); } }
	void		PlayLN(int nNo){ if(getSoundSE(nNo)){ loader_.PlayLN(nNo); }}
	void		PlayL(int nNo){ if(getSoundSE(nNo)){ loader_.PlayL(nNo); }}
	void		PlayT(int nNo,int nTimes,int nInterval=0){ if(getSoundSE(nNo)){ loader_.PlayT(nNo,nTimes,nInterval); } }
	void		Stop(int nNo){ loader_.Stop(nNo); }
	void		Reset(){ loader_.Reset(); }
	bool		IsPlay(int nNo){ return getSoundSE(nNo) ? loader_.IsPlay(nNo) : true; }
	smart_ptr<ISound>	GetSound(int nNo){ return loader_.GetSound(nNo); }

	// 文字列ID版
	void		PlayN(const string& sNo);
	void		Play(const string& sNo);
	void		PlayLN(const string& sNo);
	void		PlayL(const string& sNo);
	void		PlayT(const string& sNo,int nTimes,int nInterval=0);
	void		Stop(const string& sNo);
	bool		IsPlay(const string& sNo);
	smart_ptr<ISound>	GetSound(const string& sNo);

	// 共通
	void		StopAll(){ loader_.StopAll(); }

	// 音量
	static	LONG	getSeVolume(){ return nSeVolume_; }
	static	void	setSeVolume(LONG nVolume){ nSeVolume_=nVolume; }

private:
	CSELoader					loader_;
	// SE音量
	static	LONG				nSeVolume_;

	// 音量を反映させるためにこいつを介す
	// 直呼び（GetSound）を呼び出すと特に反映されない
	bool		getSoundSE(int nNo);
};

} // namespace Sound end
} // namespace BMW end