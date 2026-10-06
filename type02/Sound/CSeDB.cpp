#include "stdafx.h"

#include "CSeDB.h"

namespace BMW{
namespace Sound{
// static宣言
LONG CSeDB::nSeVolume_;

void CSeDB::setSeDB(const string& sFile, const string& sFileID)
{
	loader_.Set(sFile);
	Const::seID_.readMapFile(sFileID);
}

bool CSeDB::getSoundSE(int nID)
{	// サウンドのロードと、ボリュームの設定
	smart_ptr<ISound> s = GetSound(nID);
	if(!s.isNull()){ s->SetVolume(getSeVolume()); return true; }
	return false;
}

void CSeDB::PlayN(const string& sNo)
{ 
	PlayN(Const::seID_.getValue(sNo)); 
}

void CSeDB::Play(const string& sNo)
{ 
	Play(Const::seID_.getValue(sNo)); 
}

void CSeDB::PlayLN(const string& sNo)
{ 
	PlayLN(Const::seID_.getValue(sNo)); 
}

void CSeDB::PlayL(const string& sNo)
{ 
	PlayL(Const::seID_.getValue(sNo)); 
}

void CSeDB::PlayT(const string& sNo,int nTimes,int nInterval)
{ 
	PlayT(Const::seID_.getValue(sNo),nTimes,nInterval); 
}

void CSeDB::Stop(const string& sNo)
{ 
	loader_.Stop(Const::seID_.getValue(sNo));
}

bool CSeDB::IsPlay(const string& sNo)
{ 
	return loader_.IsPlay(Const::seID_.getValue(sNo));
}

smart_ptr<ISound> CSeDB::GetSound(const string& sNo)
{ 
	return loader_.GetSound(Const::seID_.getValue(sNo));
}


} // namespace Sound end
} // namespace BMW end