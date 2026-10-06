/*
	katze 06/03/23
	戦闘デモ背景用DB
*/
#pragma once

#include "../../Movie/DB/CSymbolDB.h"
#include "../../Movie/DB/CDataSymbolGui.h"

#include "CBackKeyframe.h"

namespace BMW{
namespace Demo{

class CBackSymbolDB : public Movie::CSymbolDB
{/**
	戦闘デモ背景用DB
 */
public:
	// デストラクタ
	virtual ~CBackSymbolDB(){}

	GUI::CGraphic* createGraphic(Movie::IDataSymbol* pData)
	{// グラフィック設定
		Movie::CDataSymbolGraphic* pGraphicData = static_cast<Movie::CDataSymbolGraphic*>(pData);
		// グラフィックはすべて、こいつ
		GUI::CGraphicAccuracy* pGraphic = new GUI::CGraphicAccuracy();
		setGraphic(pGraphic,pGraphicData->getID(0));
		return pGraphic;
	}

	Movie::CKeyFrame* createKeyFrame(bool b){ return new CBackKeyFrame(); }
};

} // namespace Demo end
} // namespace BMW end