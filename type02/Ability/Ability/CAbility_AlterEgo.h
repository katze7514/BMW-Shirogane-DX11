/*
	katze 06/06/27
	分身
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_AlterEgo : public IDataAbility
{/**
	分身
*/
public:
	// コンストラクタ
	CAbility_AlterEgo(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// この技能を使用するのに必要なEN
	int	getEN(int nAttr=0)const{ return 5; }

	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
};

} // namespace Ability end
} // namespace BMW end

