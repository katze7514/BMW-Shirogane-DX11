/*
	katze 08/06/17
	FUTUREEYE
	未来視・弐
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Futureeye_Second : public IDataAbility
{/**
	未来視・弐
*/
public:
	// コンストラクタ
	CAbility_Futureeye_Second(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// この技能を使用するのに必要なEN
	int	getEN(int nAttr=0)const{ return 5; }
	
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

