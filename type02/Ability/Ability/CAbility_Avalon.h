/*
	katze 08/06/09
	AVALON
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Avalon : public IDataAbility
{/**
	全て遠き理想郷
*/
public:
	// コンストラクタ
	CAbility_Avalon(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	// この技能を使用するのに必要なEN
	int		getEN(int nAttr=0)const{ return 5; }
	// 使用可能かのチェック
	bool	enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);
	// 本当自分じゃない相手に使うものだけど代用
	bool	enableTarget(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p);

	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
};

} // namespace Ability end
} // namespace BMW end

