/*
	katze 05/05/20
	サウンドDB
*/
#pragma once

namespace BMW{
namespace Sound{

class CSoundDB
{/**
	サウンドDB

	役割的には、CPlaneに対するCSpriteDBみたいなもの
	つまり、こいつはCSoundに対するもの
 */
public:
	// 設定・取得
	void	setSoundFactory(const smart_ptr<ISoundFactory>& factory){ sound_.SetSoundFactory(factory); }
	void	setSoundDB(const string& sFile, const string& sFileID){ sound_.Set(sFile); soundID_.readMapFile(sFileID); }

	CSoundLoader* getSoundLoader(){ return &sound_; }

	smart_ptr<ISound> getSound(int nID)
	{
		return sound_.GetSound(nID);
	}
	smart_ptr<ISound> getSound(const string& sID)
	{
		return sound_.GetSound(soundID_.getValue(sID));
	}

	int		getID(const string& sID) const { return soundID_.getValue(sID); }

private:
	CSoundLoader				sound_;
	katzeSDK::Misc::CStringMap	soundID_;
};

} // namespace Sound end
} // namespace BMW end