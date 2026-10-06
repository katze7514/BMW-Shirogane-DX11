/*
	katze 05/05/17
	エフェクトDB
*/
#pragma once

#include "../../Movie/DB/CSymbolDB.h"

namespace BMW{

namespace Demo{
class CDemoMovieClip;
} // namespace Demo end

namespace SLG{
namespace Effect{
class CEffectMovieClip;

class CEffectDB : public Movie::CSymbolDB
{/**
	エフェクトDB

	ようは、取り出す部分に応じて、CDemoMovieClipとCMovieClipが
	切り分けられる
 */
public:
	// デストラクタ
	virtual ~CEffectDB(){}
	// エフェクトシンボルを取り出す
	CEffectMovieClip* createEffect(int nSymbol);
	CEffectMovieClip* createEffect(const string& sSymbol);

	// デモシンボルとして取り出す
	BMW::Demo::CDemoMovieClip* createDemoMovieClip(int nSymbol);
	BMW::Demo::CDemoMovieClip* createDemoMovieClip(const string& sSymbol);
};

} // namespace Effect end
} // namespace SLG end
} // namespace BMW end