/*
	katze 08/05/24
	オープンゲット
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_OpenGet : public IDataAbility
{/**
	オープンゲット
*/
public:
	// コンストラクタ
	CAbility_OpenGet(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// この技能を使用するのに必要なEN
	int	getEN(int nAttr=0)const{ return 5; }

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

