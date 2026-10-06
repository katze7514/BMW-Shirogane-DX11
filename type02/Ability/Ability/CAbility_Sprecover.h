/*
	katze 06/03/14
	SPRECOVER
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Sprecover : public IDataAbility
{/**
	SP回復
*/
public:
	// コンストラクタ
	CAbility_Sprecover(const string& sGuiDefID):IDataAbility(sGuiDefID){}

	// 操作
	// この技能を獲得するのに必要なFP
	int	getGetFP(int nAttr=0)const{ return 100; }
	
	// 適用
	// ステータス適用
	void			applyStatus(SLG::CDataCharaSLG& slg, int nAttr);
	void			backStatus(SLG::CDataCharaSLG& slg, int nAttr);
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

