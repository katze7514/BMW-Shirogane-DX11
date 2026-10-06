/*
	katze 05/05/20
	BGM操作用サウンドクラス
*/
#pragma once

#include "IDSound.h"

namespace BMW{
namespace Sound{
class CSoundDB;

class CBgm
{/**
	BGM操作用サウンドクラス
 */
public:
	// コンストラクタ
	CBgm():nState_(Ctrl::STOP),nID_(-1){}
	CBgm(const smart_ptr<CSoundDB>& db):nState_(Ctrl::STOP),nID_(-1),soundDB_(db){}
	// フェードを実現するために、毎フレーム呼び出す
	void OnSound();

	// 設定・取得
	int		getState()const{ return nState_; }
	int		getBgmID()const{ return nID_; }
	CSound& getBgmSound(){ return sound_; }
	void	setSoundDB(const smart_ptr<CSoundDB>& db){ soundDB_=db; }

	// 操作
	void	Play();
	void	RePlay();
	void	Stop();
	void	Pause();
	void	FadeIn(int nFrame);
	void	FadeOut(int nFrame);

	void	change(int nID);
	void	change(const string& sID);

	// 音量調整
	void		setVolume(int nVolume);
	static int	getBgmVolume(){ return nBgmVolume_; }
	static void	setBgmVolume(int nVolume){ nBgmVolume_=nVolume; }

private:
	// 再生状態
	int	nState_;
	// 現在再生中のBGMID
	int nID_;
	// BGMの実体
	CSound				sound_;
	// BGMの音量
	static int			nBgmVolume_;
	// フェードの時に使う内分カウンタ
	CInteriorCounter	counter_;
	// サウンド生成を担うDB
	smart_ptr<CSoundDB> soundDB_;

	// 状態設定
	void setState(int nState){ nState_=nState; }
};

} // namespace Sound end
} // namespace BMW end