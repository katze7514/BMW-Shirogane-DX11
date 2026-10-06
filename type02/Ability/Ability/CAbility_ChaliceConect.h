/*
	katze 08/05/24
	聖杯連結
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_ChaliceConect : public IDataAbility
{/**
	聖杯連結
*/
public:
	// コンストラクタ
	CAbility_ChaliceConect(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// この技能を使用するのに必要なEN
	int getEN(int nAttr=0)const{ return 10; }
	// 使用可能かのチェック
	bool enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);

	
	// 適用
	void			applyOffset(SLG::COffsetBattle& data, int nDamage);
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Ability end
} // namespace BMW end

