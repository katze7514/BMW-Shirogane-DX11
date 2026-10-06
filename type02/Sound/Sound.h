/*
	katze 05/05/20
	サウンドインクルード
*/
#pragma once

//////////////////////////////////
// 100～0% から dB に変換する
__inline int percentVol2dB(int nVol)
{/**
	DirectSoundのボリュームはdBだった・・・。
	DSBVOLUME_MIN(-10000) = -100dB
	つまり、100倍されてる。
	んで、会社で使ってる式を使うことにする（爆）
 */
	if(nVol >= 10)
	{// 100～10 -> 0～-4500 
		return -4500 + 4500*(nVol-10)/90;
	}
	else if(nVol > 0)
	{// 10～0 -> -4500～-9000
		return -9000 + 4500*nVol/10;
	}
	else
	{// 0 の時は無音
		return DSBVOLUME_MIN;
	}	
}

//////////////////////////////////
// dB から 100～0% に変換する
__inline int dB2percentVol(int ndB)
{
	if(ndB >= -4500)
	{// 0～-4500 -> 100～10
		return 10 + ((ndB+4500)*90)/4500 ;
	}
	else if(ndB > -9000)
	{// -4500～-9000 -> 10～0
		return ((ndB+9000)*10)/4500;
	}
	else
	{// DSBVOLUME_MIN の時は無音
		return 0;
	}	
}

// サウンド系ヘッダインクルード
#include "CSoundDB.h"
#include "CSeDB.h"
#include "CBgm.h"
#include "IDSound.h"
#include "ConstSound.h"

