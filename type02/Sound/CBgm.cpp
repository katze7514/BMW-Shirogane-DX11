#include "stdafx.h"

#include "CBgm.h"

namespace BMW{
namespace Sound{
// static宣言
int	CBgm::nBgmVolume_;

void CBgm::OnSound()
{
	switch(getState())
	{
	case Ctrl::FADE_IN:
		++counter_;
		// Fadeカウンタは0～100%なのでdBに変換
		sound_->SetVolume(percentVol2dB(counter_));
		if(counter_.IsEnd()) setState(Ctrl::PLAY);
	break;

	case Ctrl::FADE_OUT:
		counter_++;
		// Fadeカウンタは0～100%なのでdBに変換
		sound_->SetVolume(percentVol2dB(counter_));
		if(counter_.IsEnd()) Stop();
	break;

	default: break;
	}
}

void CBgm::Play()
{
	setState(Ctrl::PLAY);
	sound_->SetVolume(getBgmVolume());
	sound_->Play();
}

void CBgm::RePlay()
{
	setState(Ctrl::PLAY);
	sound_->SetVolume(getBgmVolume());
	sound_->Replay();
}

void CBgm::Stop()
{
	setState(Ctrl::STOP);
	sound_->Stop();
	nID_=-1;
}

void CBgm::Pause()
{
	setState(Ctrl::PAUSE);
	sound_->Pause();
}

void CBgm::FadeIn(int nFrame)
{
	// Fadeカウンタは、0～100%で変化
	counter_.Set(0,dB2percentVol(getBgmVolume()),nFrame);
	sound_->SetVolume(DSBVOLUME_MIN);
	sound_->Play();
	setState(Ctrl::FADE_IN);
}

void CBgm::FadeOut(int nFrame)
{
	// Fadeカウンタは、0～100%で変化
	counter_.Set(dB2percentVol(getBgmVolume()),0,nFrame);
	sound_->SetVolume(getBgmVolume());
	setState(Ctrl::FADE_OUT);
}

void CBgm::change(int nID)
{// 現在のBGMを停止する
	if(getState()!=Ctrl::STOP) Stop();
	// BGMを入れ替える
	sound_ = soundDB_->getSoundLoader()->GetSound(nID);
	sound_->SetVolume(getBgmVolume());
	sound_->SetLoopPlay(true);
	nID_=nID;
}

void CBgm::change(const string& sID)
{
	change(soundDB_->getID(sID));
}

void CBgm::setVolume(int nVolume)
{
	if(getState()!=Ctrl::STOP) sound_->SetVolume(nVolume);
	setBgmVolume(nVolume);
}

} // namespace Sound end
} // namespace BMW end