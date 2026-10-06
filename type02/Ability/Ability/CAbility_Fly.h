/*
	katze 06/03/14
	FLY
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Fly : public IDataAbility
{/**
	飛行
*/
public:
	// コンストラクタ
	CAbility_Fly(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

