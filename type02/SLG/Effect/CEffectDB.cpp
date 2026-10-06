#include "stdafx.h"

#include "../../Demo/CDemoMovieClip.h"
#include "CEffectMovieClip.h"
#include "CEffectDB.h"

namespace BMW{
namespace SLG{
namespace Effect{

CEffectMovieClip* CEffectDB::createEffect(int nSymbol)
{
	// エフェクトはEffectMovieClipでないといけない
	Movie::IDataSymbol* pData = getSymbolData(nSymbol);
	switch(pData->getKind())
	{
	case Movie::IDataSymbol::MOVIE_CLIP:
	{
		CEffectMovieClip* pClip = new CEffectMovieClip();
		setMovieClip(pClip, getSymbolData(nSymbol));
		return pClip;
	}

	default: return NULL;
	}
}

CEffectMovieClip* CEffectDB::createEffect(const string& sSymbol)
{
	return createEffect(symbolID_.getValue(sSymbol));
}

BMW::Demo::CDemoMovieClip* CEffectDB::createDemoMovieClip(int nSymbol)
{
	Movie::IDataSymbol* pData = getSymbolData(nSymbol);
	switch(pData->getKind())
	{
	case Movie::IDataSymbol::MOVIE_CLIP:
	{
		BMW::Demo::CDemoMovieClip* pClip = new BMW::Demo::CDemoMovieClip();
		setMovieClip(pClip, getSymbolData(nSymbol));
		return pClip;
	}

	default: return NULL;
	}
}

BMW::Demo::CDemoMovieClip* CEffectDB::createDemoMovieClip(const string& sSymbol)
{
	return createDemoMovieClip(symbolID_.getValue(sSymbol));
}

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end