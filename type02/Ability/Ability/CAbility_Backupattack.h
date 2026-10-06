/*
	katze 06/03/14
	BACKUPATTACK
*/
#pragma once

#include "../IDataAbility.h"

namespace BMW{
namespace Ability{

class CAbility_Backupattack : public IDataAbility
{/**
	援護攻撃
*/
public:
	// コンストラクタ
	CAbility_Backupattack(const string& sGuiDefID):IDataAbility(sGuiDefID){}
	
	// 操作
	// この技能を獲得するのに必要なFP
	int	getGetFP(int nAttr=0)const;
	
	// SLGデータ補正値計算
	// 個別対応
	// 使用可能かのチェック
	bool			enable(const SLG::CDataCharaSLG& slg, int nAttr, SLG::CSLGContext& p){ return true; }
};

} // namespace Ability end
} // namespace BMW end

